
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hercs_prolog_sdk.h"
#include "prolog_mysql.h"

/*
g++ -o .... $(mysql_config --cflags) .....cpp $(mysql_config --libs)
*/

// do zrobienia:
// 1. nazwy atomow i textow w sql

#define FAIL {mysql_free_result (result); return false;}

class db_clause {
public:
	int id;
	int parameters;
	int body;
};

class db_element {
public:
	int id;
	int type;
	int reference;
	int reference_right;
	double value;
};

class mysql_term_reader : public PrologReader {
public:
	char * text;
	virtual void message (char * text) {}
	virtual int move_z (void) {
		if (* text == '\0') return -1;
		return * text++;
	}
	mysql_term_reader (PrologRoot * root, char * text) {
		this -> text = text;
		setRoot (root);
	}
};

class conn : public PrologNativeCode {
public:
	PrologRoot * root;
	bool fast;
	PrologAtom * atom;
	MYSQL * connection;
	MYSQL_ROW row;
	bool connected;
	PrologAtom * sql_atom;
	PrologAtom * adbcl_atom;
	PrologAtom * adbcl0_atom;
	PrologAtom * dbcl_atom;
	PrologAtom * dblist_atom;
	PrologAtom * dbdelcl_atom;
	PrologAtom * dbcg_atom;
	void collect_garbage_atoms (void) {
		if (fast) mysql_query (connection, "delete from atoms where (select count(*) from text_clauses where atom_id = atoms . id) < 1 and (select count(*) from elements where reference = atoms . id) < 1");
		else mysql_query (connection, "delete from atoms where (select count(*) from clauses where atom_id = atoms . id) < 1 and (select count(*) from elements where reference = atoms . id) < 1");
	}
	void collect_garbage_modules (void) {
		if (fast) mysql_query (connection, "delete from modules where (select count(*) from atoms where module_id = modules . id) < 1 and (select count(*) from elements where module_id = modules . id) < 1 and (select count(*) from text_clauses where module_id = modules . id) < 1");
		else mysql_query (connection, "delete from modules where (select count(*) from atoms where module_id = modules . id) < 1 and (select count(*) from elements where module_id = modules . id) < 1 and (select count(*) from clauses where module_id = modules . id) < 1");
	}
	void collect_garbage (void) {
		collect_garbage_atoms ();
		collect_garbage_modules ();
	}
	int last_insert_id (void) {
		if (mysql_query (connection, "select last_insert_id()") != 0) return -1;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return -1;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return -1;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return -1;}
		int ret = -1;
		if (row [0] != NULL) ret = atoi (row [0]);
		mysql_free_result (result);
		return ret;
	}
	PrologDirectory * find_atoms_module (PrologAtom * atom) {
		if (root == NULL) return NULL;
		PrologDirectory * dir = root -> root;
		while (dir != NULL) {
			PrologAtom * a = dir -> firstAtom;
			while (a != NULL) {
				if (a == atom) return dir;
				a = a -> next;
			}
			dir = dir -> next;
		}
		return NULL;
	}
	PrologAtom * find_or_create_atom (char * module_name, char * atom_name) {
		PrologDirectory * dir = root -> searchDirectory (module_name);
		if (dir == NULL) return NULL;
		PrologAtom * atom = dir -> searchAtom (atom_name);
		if (atom == NULL) atom = dir -> createAtom (atom_name);
		return atom;
	}
	int store_element (char * question, int module_id, int atom_id, int clause_id, PrologElement * head) {
		MYSQL_RES * result;
		MYSQL_ROW row;
		int ind, sub;
		switch (head -> type) {
		case 0: case 2: case 4: case 5: case 7: case 8: case 9:
			sprintf (question, "insert into elements (module_id, atom_id, clause_id, type, reference, value) values (%i, %i, %i, %i, %i, %f)", module_id, atom_id, clause_id, head -> type, (head -> type == 8 || head -> type == 2) ? head -> getInteger () : 0, head -> type == 9 ? head -> getDouble () : 0.0);
			if (mysql_query (connection, question) != 0) return -1;
			ind = last_insert_id ();
			return ind;
			break;
		case 1:
			ind = store_element (question, module_id, atom_id, clause_id, head -> getLeft ());
			sub = store_element (question, module_id, atom_id, clause_id, head -> getRight ());
			sprintf (question, "insert into elements (module_id, atom_id, clause_id, type, reference, reference_right) values (%i, %i, %i, %i, %i, %i)", module_id, atom_id, clause_id, head -> type, ind, sub);
			if (mysql_query (connection, question) != 0) return -1;
			return last_insert_id ();
			break;
		case 3:
			sprintf (question, "insert into elements (module_id, atom_id, clause_id, type, reference) values (%i, %i, %i, %i, %i)", module_id, atom_id, clause_id, head -> type, find_or_create_atom (question, head -> getAtom ()));
			if (mysql_query (connection, question) != 0) return -1;
			return last_insert_id ();
			break;
		case 6:
			sprintf (question, "insert into texts (module_id, atom_id, clause_id, text) values (%i, %i, %i, '%s')", module_id, atom_id, clause_id, head -> getText ());
			if (mysql_query (connection, question) != 0) return -1;
			sprintf (question, "insert into elements (module_id, atom_id, clause_id, type, reference) values (%i, %i, %i, %i, %i)", module_id, atom_id, clause_id, head -> type, last_insert_id ());
			if (mysql_query (connection, question) != 0) return -1;
			return last_insert_id ();
			break;
		default: break;
		}
		return -1;
	}
	int create_module (char * question, char * module_name) {
		sprintf (question, "insert into modules (module) values ('%s')", module_name);
		if (mysql_query (connection, question) != 0) return -1;
		return last_insert_id ();
	}
	bool find_clause (char * question, int module_id, int atom_id, int ordering, db_clause * clause) {
		if (fast) {
			sprintf (question, "select id, text from text_clauses where module_id = %i and atom_id = %i and ordering = %i", module_id, atom_id, ordering);
			if (mysql_query (connection, question) != 0) return false;
			MYSQL_RES * text_result = mysql_use_result (connection);
			if (text_result != NULL) {
				if (mysql_num_fields (text_result) > 1) {
					MYSQL_ROW text_row;
					if ((text_row = mysql_fetch_row (text_result)) != 0) {
						sprintf (question, "%s", text_row [1]);
					}
				}
				mysql_free_result (text_result);
			}
			return true;
		}
		sprintf (question, "select id, parameters_id, body_id from clauses where module_id = %i and atom_id = %i and ordering = %i", module_id, atom_id, ordering);
		if (mysql_query (connection, question) != 0) return false;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return false;
		if (mysql_num_fields (result) < 3) {mysql_free_result (result); return false;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return false;}
		clause -> id = 0; clause -> parameters = 0; clause -> body = 0;
		if (row [0] != NULL) clause -> id = atoi (row [0]);
		if (row [1] != NULL) clause -> parameters = atoi (row [1]);
		if (row [2] != NULL) clause -> body = atoi (row [2]);
		mysql_free_result (result);
		return true;
	}
	bool find_element (char * question, int element_id, db_element * element) {
		sprintf (question, "select type, reference, reference_right, value from elements where id = %i", element_id);
		if (mysql_query (connection, question) != 0) return false;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return false;
		if (mysql_num_fields (result) < 4) {mysql_free_result (result); return false;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return false;}
		element -> id = element_id; element -> type = 0; element -> reference = 0; element -> reference_right = 0; element -> value = 0.0;
		if (row [0] != NULL) element -> type = atoi (row [0]);
		if (row [1] != NULL) element -> reference = atoi (row [1]);
		if (row [2] != NULL) element -> reference_right = atoi (row [2]);
		if (row [3] != NULL) element -> value = atof (row [3]);
		mysql_free_result (result);
		return true;
	}
	bool find_atom (char * question, int atom_id) {
		sprintf (question, "select atom from atoms where id = %i", atom_id);
		if (mysql_query (connection, question) != 0) return false;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return false;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return false;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return false;}
		question [0] = '\0';
		if (row [0] != NULL) area_cat (question, row [0]);
		mysql_free_result (result);
		return true;
	}
	PrologAtom * find_or_create_atom (char * question, int atom_id) {
		sprintf (question, "select module, atom from atoms, modules where atoms . module_id = modules . id and atoms . id = %i", atom_id);
		if (mysql_query (connection, question) != 0) return NULL;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return NULL;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return NULL;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return NULL;}
		if (row [0] == NULL || row [1] == NULL) {mysql_free_result (result); return NULL;}
		PrologDirectory * dir = root -> searchDirectory (row [0]);
		if (dir == NULL) {dir = root -> createDirectory (row [0]); root -> close ();}
		if (dir == NULL) {mysql_free_result (result); return NULL;}
		PrologAtom * atom = dir -> searchAtom (row [1]);
		if (atom == NULL) atom = dir -> createAtom (row [1]);
		mysql_free_result (result);
		return atom;
	}
	bool find_text (char * question, int text_id) {
		sprintf (question, "select text from texts where id = %i", text_id);
		if (mysql_query (connection, question) != 0) return false;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return false;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return false;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return false;}
		question [0] = '\0';
		if (row [0] != NULL) area_cat (question, row [0]);
		mysql_free_result (result);
		return true;
	}
	bool find_element (char * question, char * module_name, PrologElement * head, int element_id) {
		db_element el; el . type = 0;
		if (! find_element (question, element_id, & el)) return false;
		PrologAtom * atom;
		switch (el . type) {
		case 0: head -> setEarth (); return true;
		case 1:
			head -> setPair ();
			find_element (question, module_name, head -> getLeft (), el . reference);
			find_element (question, module_name, head -> getRight (), el . reference_right);
			return true;
		case 2: head -> setVar (el . reference); return true;
		case 3:
			atom = find_or_create_atom (question, el . reference);
			if (atom == NULL) return false;
			head -> setAtom (atom);
			return true;
		case 4: head -> setSlash (); return true;
		case 5: head -> setFail (); return true;
		case 6:
			if (! find_text (question, el . reference)) return false;
			head -> setText (question);
			return true;
		case 7: head -> setHead (NULL); return true;
		case 8: head -> setInteger (el . reference); return true;
		case 9: head -> setDouble (el . value); return true;
		default: return false;
		}
		return true;
	}
	int find_module_id (char * question, char * module_name) {
		sprintf (question, "select id from modules where module = '%s'", module_name);
		if (mysql_query (connection, question) != 0) return -1;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return -1;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return -1;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return -1;}
		if (row [0] == NULL) {mysql_free_result (result); return -1;}
		int ret = atoi (row [0]);
		mysql_free_result (result);
		return ret;
	}
	int find_or_create_module (char * question, char * module_name) {
		int ind = find_module_id (question, module_name);
		if (ind < 0) return create_module (question, module_name);
		return ind;
	}
	int create_atom (char * question, int module_id, char * atom_name) {
		sprintf (question, "insert into atoms (module_id, atom) values (%i, '%s')", module_id, atom_name);
		if (mysql_query (connection, question) != 0) return -1;
		return last_insert_id ();
	}
	int find_atom_id (char * question, int module_id, char * atom_name) {
		sprintf (question, "select id from atoms where module_id = %i and atom = '%s'", module_id, atom_name);
		if (mysql_query (connection, question) != 0) return -1;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return -1;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return -1;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return -1;}
		if (row [0] == NULL) {mysql_free_result (result); return -1;}
		int ret = atoi (row [0]);
		mysql_free_result (result);
		return ret;
	}
	int find_or_create_atom (char * question, PrologAtom * atom) {
		PrologDirectory * dir = find_atoms_module (atom);
		if (dir == NULL) return -1;
		int module_id = find_or_create_module (question, dir -> name ());
		int ind = find_atom_id (question, module_id, atom -> name ());
		if (ind < 0) return create_atom (question, module_id, atom -> name ());
		return ind;
	}
	bool read_clause (PrologElement * parameters, char * module_name, char * atom_name, int ordering) {
		AREA area;
		int module_id = find_module_id (area, module_name);
		if (module_id < 0) return false;
		int atom_id = find_atom_id (area, module_id, atom_name);
		if (atom_id < 0) return false;
		PrologAtom * atom = find_or_create_atom (module_name, atom_name);
		if (atom == NULL) return false;
		db_clause clause;
		if (! find_clause (area, module_id, atom_id, ordering, & clause)) return false;
		if (fast) {
			mysql_term_reader trd (root, area);
			PrologElement * trdx = trd . readElement ();
			if (trdx != 0) {
				root -> getValue (trdx, area, 0);
				parameters -> setPair (trdx, root -> earth ());
				return true;
			}
			return false;
		}
		parameters -> setPair ();
		parameters = parameters -> getLeft ();
		parameters -> setPair ();
		PrologElement * head = parameters -> getLeft ();
		head -> setPair ();
		head -> getLeft () -> setAtom (atom);
		head = head -> getRight ();
		if (! find_element (area, module_name, head, clause . parameters)) return false;
//		head -> setInteger (clause . parameters);
		head = parameters -> getRight ();
		if (! find_element (area, module_name, head, clause . body)) return false;
//		head -> setInteger (clause . body);
		return true;
	}
	bool read_number_of_clauses (PrologElement * parameters, char * module_name, char * atom_name) {
		AREA area;
		int module_id = find_module_id (area, module_name);
		if (module_id < 0) {parameters -> setInteger (0); return true;}
		int atom_id = find_atom_id (area, module_id, atom_name);
		if (atom_id < 0) {parameters -> setInteger (0); return true;}
		if (fast) sprintf (area, "select 1+max(ordering) from text_clauses where module_id = %i and atom_id = %i", module_id, atom_id);
		else sprintf (area, "select 1+max(ordering) from clauses where module_id = %i and atom_id = %i", module_id, atom_id);
		if (mysql_query (connection, area) != 0) return false;
		MYSQL_RES * result = mysql_use_result (connection);
		if (result == NULL) return false;
		if (mysql_num_fields (result) < 1) {mysql_free_result (result); return false;}
		MYSQL_ROW row;
		if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return false;}
		if (row [0] == NULL) {parameters -> setInteger (0); mysql_free_result (result); return true;}
		int ret = atoi (row [0]);
		mysql_free_result (result);
		parameters -> setInteger (ret);
		return true;
	}
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		if (! a -> isAtom ()) return false;
		parameters = parameters -> getRight ();
		PrologAtom * control_atom = a -> getAtom ();
		AREA question;
		if (control_atom == sql_atom) {
			if (! parameters -> isPair ()) return false;
			PrologElement * q = parameters -> getLeft ();
			if (! q -> isText ()) return false;
			parameters = parameters -> getRight ();
			if (mysql_query (connection, q -> getText ()) != 0) return false;
			MYSQL_RES * result = mysql_use_result (connection);
			if (result == NULL) return true;
			MYSQL_ROW row;
			int num_fields = mysql_num_fields (result);
			parameters -> setEarth ();
			while ((row = mysql_fetch_row (result)) != 0) {
				parameters -> setPair ();
				PrologElement * head = parameters -> getLeft ();
				for (int ind = 0; ind < num_fields; ind++) {
					head -> setPair ();
					if (row [ind] != NULL) {
						head -> getLeft () -> setText (row [ind]);
					}
					head = head -> getRight ();
				}
				parameters = parameters -> getRight ();
			}
			mysql_free_result (result);
			return true;
		}
		if (control_atom == dbcl_atom) {
			if (! parameters -> isPair ()) return false;
			PrologElement * e = parameters -> getLeft ();
			parameters = parameters -> getRight ();
			if (e -> isText ()) {
				if (! parameters -> isPair ()) return false;
				PrologElement * atom = parameters -> getLeft ();
				if (! atom -> isText ()) return false;
				parameters = parameters -> getRight ();
				if (parameters -> isVar ()) return read_number_of_clauses (parameters, e -> getText (), atom -> getText ());
				if (! parameters -> isPair ()) return false;
				PrologElement * ordering = parameters -> getLeft ();
				if (ordering -> isVar ()) return read_number_of_clauses (ordering, e -> getText (), atom -> getText ());
				if (! ordering -> isInteger ()) return false;
				parameters = parameters -> getRight ();
				if (parameters -> isEarth ()) return read_number_of_clauses (ordering, e -> getText (), atom -> getText ());
				return read_clause (parameters, e -> getText (), atom -> getText (), ordering -> getInteger ());
			}
			if (e -> isAtom ()) {
				PrologDirectory * dir = find_atoms_module (e -> getAtom ());
				if (dir == NULL) return false;
				if (parameters -> isVar ()) return read_number_of_clauses (parameters, dir -> name (), e -> getAtom () -> name ());
				if (! parameters -> isPair ()) return false;
				PrologElement * ordering = parameters -> getLeft ();
				if (ordering -> isVar ()) return read_number_of_clauses (ordering, dir -> name (), e -> getAtom () -> name ());
				if (! ordering -> isInteger ()) return false;
				parameters = parameters -> getRight ();
				if (parameters -> isEarth ()) return read_number_of_clauses (ordering, dir -> name (), e -> getAtom () -> name ());
				return read_clause (parameters, dir -> name (), e -> getAtom () -> name (), ordering -> getInteger ());
			}
			return false;
		}
		if (control_atom == adbcl_atom || control_atom == adbcl0_atom) {
			if (! parameters -> isPair ()) return false;
			parameters = parameters -> getLeft ();
			AREA drop;
			if (fast) root -> getValue (parameters, drop, 0);
			if (! parameters -> isPair ()) return false;
			PrologElement * head = parameters -> getLeft ();
			if (! head -> isPair ()) return false;
			PrologElement * atom = head -> getLeft ();
			if (! atom -> isAtom ()) return false;
			MYSQL_RES * result;
			MYSQL_ROW row;
			PrologDirectory * dir = find_atoms_module (atom -> getAtom ());
			// get the module_id and atom_id
			int module_id = find_or_create_module (question, dir -> name ());
			int atom_id = find_or_create_atom (question, atom -> getAtom ());
			// now get the clause ordering
			int ordering = 0;
			if (control_atom == adbcl_atom) {
				if (fast) sprintf (question, "select max(ordering) + 1 from text_clauses where module_id = %i and atom_id = %i", module_id, atom_id);
				else sprintf (question, "select max(ordering) + 1 from clauses where module_id = %i and atom_id = %i", module_id, atom_id);
				if (mysql_query (connection, question) != 0) return false;
				result = mysql_use_result (connection);
				if (result == NULL) return false;
				if (mysql_num_fields (result) < 1) FAIL
				if ((row = mysql_fetch_row (result)) == 0) FAIL
				if (row [0] != NULL) ordering = atoi (row [0]);
				mysql_free_result (result);
			} else {
				if (fast) sprintf (question, "update text_clauses set ordering = ordering + 1 where atom_id = %i", atom_id);
				else sprintf (question, "update clauses set ordering = ordering + 1 where atom_id = %i", atom_id);
				if (mysql_query (connection, question) != 0) return false;
			}
			// insert clause and get clause id
			if (fast) {
				sprintf (question, "insert into text_clauses (module_id, atom_id, ordering, text) values (%i, %i, %i, '%s')", module_id, atom_id, ordering, drop);
				if (mysql_query (connection, question) != 0) {printf ("failed at text clause\n");}
				return true;
			}
			sprintf (question, "insert into clauses (module_id, atom_id, ordering, parameters_id, body_id) values (%i, %i, %i, 0, 0)", module_id, atom_id, ordering);
			if (mysql_query (connection, question) != 0) return false;
			int clause_id = last_insert_id();
			// now insert elements
			int head_id = store_element (question, module_id, atom_id, clause_id, head -> getRight ());
			int body_id = store_element (question, module_id, atom_id, clause_id, parameters -> getRight ());
			sprintf (question, "update clauses set parameters_id = %i, body_id = %i where id = %i", head_id, body_id, clause_id);
			if (mysql_query (connection, question) != 0) return false;
			return true;
		}
		if (control_atom == dblist_atom) {
			AREA area;
			if (parameters -> isVar ()) {
				sprintf (area, "select module from modules");
				if (mysql_query (connection, area) != 0) return false;
				MYSQL_RES * result = mysql_use_result (connection);
				if (result == NULL) return false;
				if (mysql_num_fields (result) < 1) {mysql_free_result (result); return false;}
				parameters -> setEarth ();
				MYSQL_ROW row;
				while ((row = mysql_fetch_row (result)) != 0) {
					parameters -> setPair ();
					if (row [0] != NULL) parameters -> getLeft () -> setText (row [0]);
					else parameters -> getLeft () -> setEarth ();
					parameters = parameters -> getRight ();
				}
				mysql_free_result (result);
				return true;
			}
			if (parameters -> isPair ()) {
				PrologElement * directory = parameters -> getLeft ();
				if (! directory -> isText ()) return false;
				parameters = parameters -> getRight ();
				sprintf (area, "select id from modules where module = '%s'", directory -> getText ());
				if (mysql_query (connection, area) != 0) return false;
				MYSQL_RES * result = mysql_use_result (connection);
				if (result == NULL) return false;
				if (mysql_num_fields (result) < 1) {mysql_free_result (result); return false;}
				MYSQL_ROW row;
				if ((row = mysql_fetch_row (result)) == 0) {mysql_free_result (result); return false;}
				if (row [0] == NULL) {mysql_free_result (result); return false;}
				int module_id = atoi (row [0]);
				mysql_free_result (result);
				PrologDirectory * dir = root -> searchDirectory (directory -> getText ());
				if (dir == NULL) {dir = root -> createDirectory (directory -> getText ()); root -> close ();}
				if (dir == NULL) return false;
				sprintf (area, "select atom from atoms where module_id = %i", module_id);
				if (mysql_query (connection, area) != 0) return false;
				result = mysql_use_result (connection);
				if (result == NULL) return false;
				if (mysql_num_fields (result) < 1) {mysql_free_result (result); return false;}
				parameters -> setEarth ();
				while ((row = mysql_fetch_row (result)) != 0) {
					parameters -> setPair ();
					if (row [0] != NULL) {
						PrologAtom * atom = dir -> searchAtom (row [0]);
						if (atom == NULL) atom = dir -> createAtom (row [0]);
						parameters -> getLeft () -> setAtom (atom);
					} else parameters -> getLeft () -> setEarth ();
					parameters = parameters -> getRight ();
				}
				mysql_free_result (result);
				return true;
			}
		}
		if (control_atom == dbdelcl_atom) {
			if (! parameters -> isPair ()) return false;
			PrologElement * atom = parameters -> getLeft ();
			if (! atom -> isAtom ()) return false;
			parameters = parameters -> getRight ();
			PrologDirectory * dir = find_atoms_module (atom -> getAtom ());
			if (dir == NULL) return false;
			int module_id = find_module_id (question, dir -> name ());
			if (module_id < 0) return false;
			int atom_id = find_atom_id (question, module_id, atom -> getAtom () -> name ());
			if (atom_id < 0) return false;
			if (parameters -> isEarth ()) {
				sprintf (question, "delete from elements where atom_id = %i", atom_id);
				if (mysql_query (connection, question) != 0) return false;
				sprintf (question, "delete from texts where atom_id = %i", atom_id);
				if (mysql_query (connection, question) != 0) return false;
				if (fast) sprintf (question, "delete from text_clauses where atom_id = %i", atom_id);
				else sprintf (question, "delete from clauses where atom_id = %i", atom_id);
				if (mysql_query (connection, question) != 0) return false;
				collect_garbage ();
				return true;
			}
			if (! parameters -> isPair ()) return false;
			PrologElement * ordering = parameters -> getLeft ();
			if (! ordering -> isInteger ()) return false;
			if (fast) sprintf (question, "delete from elements where atom_id = %i and clause_id = (select id from text_clauses where atom_id = %i and ordering = %i)", atom_id, atom_id, ordering -> getInteger ());
			else sprintf (question, "delete from elements where atom_id = %i and clause_id = (select id from clauses where atom_id = %i and ordering = %i)", atom_id, atom_id, ordering -> getInteger ());
			if (mysql_query (connection, question) != 0) return false;
			if (fast) sprintf (question, "delete from texts where atom_id = %i and clause_id = (select id from text_clauses where atom_id = %i and ordering = %i)", atom_id, atom_id, ordering -> getInteger ());
			else sprintf (question, "delete from texts where atom_id = %i and clause_id = (select id from clauses where atom_id = %i and ordering = %i)", atom_id, atom_id, ordering -> getInteger ());
			if (mysql_query (connection, question) != 0) return false;
			if (fast) sprintf (question, "delete from text_clauses where atom_id = %i and ordering = %i", atom_id, ordering -> getInteger ());
			else sprintf (question, "delete from clauses where atom_id = %i and ordering = %i", atom_id, ordering -> getInteger ());
			if (mysql_query (connection, question) != 0) return false;
			if (fast) sprintf (question, "update text_clauses set ordering = ordering - 1 where atom_id = %i and ordering > %i", atom_id, ordering -> getInteger ());
			else sprintf (question, "update clauses set ordering = ordering - 1 where atom_id = %i and ordering > %i", atom_id, ordering -> getInteger ());
			if (mysql_query (connection, question) != 0) return false;
			collect_garbage ();
			return true;
		}
		if (control_atom == dbcg_atom) {collect_garbage (); return true;}
		return false;
	}
	conn (PrologRoot * root, bool fast, PrologAtom * atom, char * server, char * user, char * password, char * database) {
		this -> root = root;
		this -> fast = fast;
		this -> atom = atom;
		//this -> select_atom = this -> insert_atom = this -> delete_atom = this -> update_atom = 0;
		this -> sql_atom = this -> adbcl_atom = this -> adbcl0_atom = this -> dbcl_atom = this -> dblist_atom = this -> dbdelcl_atom = this -> dbcg_atom = 0;
		PrologDirectory * dir = root -> searchDirectory ("sql");
		if (dir != NULL) {
			this -> sql_atom = dir -> searchAtom ("sql");
			this -> adbcl_atom = dir -> searchAtom ("adbcl");
			this -> adbcl0_atom = dir -> searchAtom ("adbcl0");
			this -> dbcl_atom = dir -> searchAtom ("dbcl");
			this -> dblist_atom = dir -> searchAtom ("dblist");
			this -> dbdelcl_atom = dir -> searchAtom ("dbdelcl");
			this -> dbcg_atom = dir -> searchAtom ("dbcg");
		}
		connection = mysql_init (0);
		this -> connected = mysql_real_connect (connection, server, user, password, database, 0, 0, 0);
		if (! connected) mysql_close (connection);
	}
	~ conn (void) {
		if (connected) {
			mysql_close (connection);
		}
		if (atom != NULL) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
		}
	}
};

class mysql : public PrologNativeCode {
public:
	PrologRoot * root;
	bool fast;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * atom = parameters -> getLeft ();
		if (! atom -> isAtom () && ! atom -> isVar ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * server = parameters -> getLeft ();
		if (! server -> isText () && ! server -> isAtom ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * user = parameters -> getLeft ();
		if (! user -> isText () && ! user -> isAtom ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * password = parameters -> getLeft ();
		if (! password -> isText () && ! password -> isAtom ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * database = parameters -> getLeft ();
		if (! database -> isText () && ! database -> isAtom ()) return false;
		PrologAtom * conn_atom = NULL;
		if (atom -> isAtom ()) conn_atom = atom -> getAtom ();
		if (atom -> isVar ()) {conn_atom = new PrologAtom (); atom -> setAtom (conn_atom);}
		if (conn_atom == NULL) return false;
		conn * c = new conn (root, fast, conn_atom,
					server -> isText () ? server -> getText () : server -> getAtom () -> name (),
					user -> isText () ? user -> getText () : user -> getAtom () -> name (),
					password -> isText () ? password -> getText () : password -> getAtom () -> name (),
					database -> isText () ? database -> getText () : database -> getAtom () -> name ());
		if (! c -> connected) {delete c; return false;}
		if (conn_atom -> setMachine (c)) return true;
		delete c;
		return false;
	}
	mysql (PrologRoot * root, bool fast = false) {this -> root = root; this -> fast = fast;}
};

void MySQLServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {this -> root = root;}
PrologNativeCode * MySQLServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "mysql") == 0) return new mysql (root);
	if (strcmp (name, "mysql_fast") == 0) return new mysql (root, true);
	return NULL;
}

extern "C" {
PrologServiceClass * create_service_class (void) {return new MySQLServiceClass ();}
}

