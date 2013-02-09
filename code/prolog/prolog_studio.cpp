///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2004 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "prolog_studio.h"
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

class pp : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		while (parameters -> isPair ()) {
			root -> getValue (parameters -> getLeft (), area, 0);
			root -> print (area);
			parameters = parameters -> getRight ();
		}
		return true;
	}
	pp (PrologRoot * root) {this -> root = root;}
};

class pt : public PrologNativeCode {
public :
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		while (parameters -> isPair ()) {
			root -> getTrueValue (parameters -> getLeft (), area, 0);
			root -> print (area);
			parameters = parameters -> getRight ();
		}
		return true;
	}
	pt (PrologRoot * root) {this -> root = root;}
};

class write : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (el -> isText ()) root -> print (el -> getText());
			if (el -> isInteger ()) root -> print_character (el -> getInteger ());
			while (el -> isPair ()) {
				root -> getValue (el -> getLeft (), area, 0);
				root -> print (area);
				el = el -> getRight ();
			}
			parameters = parameters -> getRight ();
		}
		return true;
	}
	write (PrologRoot * root) {this -> root = root;}
};

class nl : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {root -> print (root -> new_line_caption); return true;}
	nl (PrologRoot * root) {this -> root = root;}
};

void standard_in_reader :: message (char * text) {root -> print (text); root -> print (root -> new_line_caption);}
void standard_in_reader :: message_v (char * text, char * variable) {root -> message (text, variable);}
int standard_in_reader :: move_z (void) {return root -> get_character ();}
standard_in_reader :: standard_in_reader (PrologRoot * root) {setRoot (root);}

class read : public PrologNativeCode {
public:
	standard_in_reader * stdr;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (stdr -> root -> command) {
			if (stdr -> root -> command -> empty ()) {resolution -> callAgain (parameters); return true;}
			resolution -> callAgain ();
		}
		if (! parameters -> isPair ()) return false;
		AREA area;
		if (stdr -> getString (area, 0) < 0) {
			if (stdr -> root -> command) {
				stdr -> act_znak = 0;
				resolution -> callAgain (parameters);
				return true;
			}
			return false;
		}
		parameters -> getLeft () -> setText (area);
		return true;
	}
	read (standard_in_reader * stdr) {this -> stdr = stdr;}
};

class readln : public PrologNativeCode {
public:
	standard_in_reader * stdr;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (stdr -> root -> command) {
			if (stdr -> root -> command -> empty ()) {resolution -> callAgain (parameters); return true;}
			resolution -> callAgain ();
		}
		if (! parameters -> isPair ()) return false;
		AREA area;
		stdr -> readln (area, 0);
		parameters -> getLeft () -> setText (area);
		return true;
	}
	readln (standard_in_reader * stdr) {this -> stdr = stdr;}
};

class pr : public PrologNativeCode {
public:
	standard_in_reader * stdr;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (stdr -> root -> command) {
			if (stdr -> root -> command -> empty ()) {
				stdr -> act_znak = 0;
				resolution -> callAgain (parameters);
				return true;
			}
			if (stdr -> prefetch_whites ()) {
				stdr -> act_znak = 0;
				resolution -> callAgain (parameters);
				return true;
			}
			resolution -> callAgain ();
		}
		if (! parameters -> isPair ()) return false;
		if (stdr -> act_znak < 0) stdr -> act_znak = 0;
		PrologElement * dup = stdr -> readElement ();
		if (dup != NULL) {
			parameters -> setLeft (dup);
			return true;
		}
		return false;
	}
	pr (standard_in_reader * stdr) {this -> stdr = stdr;}
};

class list : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		int area_ind;
		if (parameters -> isEarth ()) {
			// directory list
			root -> list (area, 0);
			root -> print (area); root -> print (root -> new_line_caption);
			return true;
		}
		if (parameters -> isVar ()) {
			// directory drop
			PrologDirectory * dr = root -> root;
			while (dr != NULL) {
				parameters -> setPair (root -> text (dr -> directoryName), root -> earth ());
				parameters = parameters -> getRight ();
				dr = dr -> next;
			}
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * ret = parameters -> getRight ();
		parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {
			// directory drop list
			root -> list (area, 0);
			parameters -> setText (area);
			return true;
		}
		if (parameters -> isAtom ()) {
			PrologAtom * atom = parameters -> getAtom ();
			if (atom -> getMachine () != NULL) {
				area_ind = area_cat (area, 0, atom -> name ());
				area_ind = area_cat (area, area_ind, " ");
				area_ind = area_cat (area, area_ind, root -> if_atom_caption);
				area_ind = area_cat (area, area_ind, " ");
				area_ind = area_cat (area, area_ind, root -> machine_caption);
				if (ret -> isVar ()) {
					// machine clause drop
					ret -> setPair (root -> text (area), root -> earth ());
					return true;
				}
				// machine clause drop list / machine clause list
				area_ind = area_cat (area, area_ind, root -> new_line_caption);
				if (ret -> isPair ()) ret -> getLeft () -> setText (area); else root -> print (area);
				return true;
			}
			PrologElement * clausa = atom -> firstClause;
			PrologElement * dup = NULL;
			if (ret -> isVar ()) {
				// clause drop
				if (clausa == NULL) {ret -> setEarth (); return true;}
				while (clausa != NULL) {
					dup = clausa -> duplicate ();
					dup -> getLeft () -> getLeft () -> setAtom (atom);
					ret -> setPair (dup, root -> earth ());
					ret = ret -> getRight ();
					clausa = (PrologElement *) clausa -> getLeft () -> getLeft () -> getHead ();
				}
				return true;
			}
			area_ind = area_cat (area, 0, "");
			while (clausa != NULL) {
				dup = clausa -> duplicate ();
				dup -> getLeft () -> getLeft () -> setAtom (atom);
				area_ind = root -> getValue (dup, area, area_ind);
				delete dup;
				area_ind = area_cat (area, area_ind, root -> new_line_caption);
				clausa = (PrologElement *) clausa -> getLeft () -> getLeft () -> getHead ();
			}
			// clause drop list / clause list
			if (ret -> isPair ()) ret -> getLeft () -> setText (area); else root -> print (area);
			return true;
		}
		if (parameters -> isText ()) {
			if (ret -> isVar ()) {
				// atom drop
				PrologDirectory * dr = root -> searchDirectory (parameters -> getText ());
				if (dr == NULL) return false;
				PrologAtom * atom = dr -> firstAtom;
				ret -> setEarth ();
				while (atom != NULL) {
					ret -> setPair (root -> atom (atom), root -> earth ());
					ret = ret -> getRight ();
					atom = atom -> next;
				}
				return true;
			}
			area_ind = root -> list (parameters -> getText (), area, 0);
			area_ind = area_cat (area, area_ind, root -> new_line_caption);
			// atom drop list / atom list
			if (ret -> isPair ()) ret -> getLeft () -> setText (area); else root -> print (area);
		}
		return true;
	}
	list (PrologRoot * root) {this -> root = root;}
};

class set_uap32_captions : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_uap32_captions ();
		return true;
	}
	set_uap32_captions (PrologRoot * root) {this -> root = root;}
};

class set_standard_captions : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_standard_captions ();
		return true;
	}
	set_standard_captions (PrologRoot * root) {this -> root = root;}
};

class set_edinburg_captions : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_edinburg_captions ();
		return true;
	}
	set_edinburg_captions (PrologRoot * root) {this -> root = root;}
};

class set_marseille_captions : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_marseille_captions ();
		return true;
	}
	set_marseille_captions (PrologRoot * root) {this -> root = root;}
};

class set_functional_captions : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_functional_captions ();
		return true;
	}
	set_functional_captions (PrologRoot * root) {this -> root = root;}
};

class auto_atoms : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_auto_atoms ();
		return true;
	}
	auto_atoms (PrologRoot * root) {this -> root = root;}
};

class scripted_atoms : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> set_scripted_atoms ();
		return true;
	}
	scripted_atoms (PrologRoot * root) {this -> root = root;}
};

class CL : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (parameters -> getLeft () -> isAtom ()) {
			PrologAtom * atom = parameters -> getLeft () -> getAtom ();
			parameters = parameters -> getRight ();
			PrologElement * e_counter = parameters;
			if (parameters -> isPair ()) e_counter = parameters -> getLeft ();
			if (e_counter -> isVar () || ! parameters -> isPair ()) {
				PrologElement * clause = atom -> firstClause;
				int counter = 0;
				while (clause != NULL) {
					counter++;
					clause = (PrologElement *) clause -> getLeft () -> getLeft () -> getHead ();
				}
				e_counter -> setInteger (counter);
				return true;
			}
			if (! e_counter -> isInteger ()) return false;
			if (! parameters -> isPair ()) return false;
			PrologElement * e_clause = parameters;
			parameters = parameters -> getRight ();
			if (parameters -> isEarth ()) {
				PrologElement * clause = atom -> firstClause;
				int counter = 0;
				while (clause != NULL) {
					counter++;
					clause = (PrologElement *) clause -> getLeft () -> getLeft () -> getHead ();
				}
				return e_counter -> getInteger () == counter;
			}
			PrologElement * dup = atom -> firstClause;
			if (dup == NULL) return false;
			int ind = e_counter -> getInteger ();
			while (ind > 0) {
				dup = (PrologElement *) dup -> getLeft () -> getLeft () -> getHead ();
				if (dup == NULL) return false;
				ind--;
			}
			dup = dup -> duplicate ();
			dup -> getLeft () -> getLeft () -> setAtom (atom);
			if (parameters -> isVar ()) {e_clause -> setRight (dup); return true;}
			parameters -> setPair ();
			parameters -> setLeft (dup);
			return true;
		}
		if (! parameters -> getLeft () -> isInteger ()) return false;
		int ind = parameters -> getLeft () -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isAtom ()) return false;
		PrologAtom * atom = parameters -> getLeft () -> getAtom ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * dup = atom -> firstClause;
		if (dup == NULL) return false;
		while (ind > 0) {
			dup = (PrologElement *) dup -> getLeft () -> getLeft () -> getHead ();
			if (dup == NULL) return false;
			ind--;
		}
		dup = dup -> duplicate ();
		dup -> getLeft () -> getLeft () -> setAtom (atom);
		parameters -> setLeft (dup);
		return true;
	}
	CL (PrologRoot * root) {}
};

class delcl : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		int ind = parameters -> getLeft () -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isAtom ()) return false;
		PrologAtom * atom = parameters -> getLeft () -> getAtom ();
		if (atom -> Protected) return false;
		PrologElement * dup = atom -> firstClause;
		if (dup == NULL) return false;
		if (ind == 0) {
			atom -> firstClause = (PrologElement *) dup -> getLeft () -> getLeft () -> getHead ();
			delete dup;
			return true;
		}
		ind--;
		while (ind > 0) {
			dup = (PrologElement *) dup -> getLeft () -> getLeft () -> getHead ();
			if (dup == NULL) return false;
			ind--;
		}
		dup = dup -> getLeft () -> getLeft ();
		PrologElement * next = (PrologElement *) dup -> getHead ();
		if (next == NULL) return false;
		dup -> setHead ((PrologElement *) next -> getLeft () -> getLeft () -> getHead ());
		delete next;
		return true;
	}
	delcl (PrologRoot * root) {}
};

class overwrite : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		int ind = parameters -> getLeft () -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isPair ()) return false;
		if (! parameters -> getLeft () -> getLeft () -> isAtom ()) return false;
		PrologAtom * atom = parameters -> getLeft () -> getLeft () -> getAtom ();
		if (atom -> Protected) return false;
		PrologElement * dup = atom -> firstClause;
		if (dup == NULL) return false;
		while (ind > 0) {
			dup = (PrologElement *) dup -> getLeft () -> getLeft () -> getHead ();
			if (dup == NULL) return false;
			ind--;
		}
		dup -> getLeft () -> setRight (parameters -> getLeft () -> getRight () -> duplicate ());
		dup -> setRight (parameters -> getRight () -> duplicate ());
		return true;
	}
	overwrite (PrologRoot * root) {}
};

class addcl0 : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * dup = parameters -> getLeft () -> duplicate ();
		if (root -> attachClause (dup, 0) == 0) return true;
		delete dup;
		return false;
	}
	addcl0 (PrologRoot * root) {this -> root = root;}
};

class addcl : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * dup = parameters -> getLeft () -> duplicate ();
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {if (root -> attachClause (dup) == 0) return true;}
		if (parameters -> isPair ()) {
			parameters = parameters -> getLeft ();
			if (parameters -> isInteger ()) {
				if (root -> attachClause (dup, parameters -> getInteger ()) == 0) return true;
			}
		}
		delete dup;
		return false;
	}
	addcl (PrologRoot * root) {this -> root = root;}
};

class create_atom : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (parameters -> getLeft () -> isVar ()) {
			parameters -> getLeft () -> setAtom (new PrologAtom ());
			return true;
		}
		PrologElement * left = parameters -> getLeft ();
		if (! left -> isText ()) return false;
		parameters = parameters -> getRight ();
		PrologAtom * atom;
		if (parameters -> isPair ()) {
			atom = new PrologAtom (left -> getText ());
			if (atom == NULL) return false;
			parameters -> getLeft () -> setAtom (atom);
			return true;
		}
		root -> createAtom (left -> getText ());
		return true;
	}
	create_atom (PrologRoot * root) {this -> root = root;}
};

class search_atom : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isText ()) return false;
		if (! parameters -> getRight () -> isPair ()) return false;
		char * name = parameters -> getLeft () -> getText ();
		if (parameters -> getRight () -> getLeft () -> isText ()) {
			PrologDirectory * dir = root -> searchDirectory (name);
			if (dir == NULL) return false;
			parameters = parameters -> getRight ();
			name = parameters -> getLeft () -> getText ();
			if (! parameters -> getRight () -> isPair ()) return false;
			PrologAtom * atom = dir -> searchAtom (name);
			if (atom == NULL) return false;
			parameters -> getRight () -> getLeft () -> setAtom (atom);
			return true;
		}
		PrologElement * atom = root -> atom (name);
		if (atom == NULL) return false;
		parameters -> getRight () -> setLeft (atom);
		return true;
	}
	search_atom (PrologRoot * root) {this -> root = root;}
};

class preprocessor : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologAtom * preprocessor = root -> getPreprocessor ();
		if (! parameters -> isPair ()) {
			if (preprocessor != NULL) parameters -> setAtom (preprocessor);
			else parameters -> setEarth ();
			return true;
		}
		parameters = parameters -> getLeft ();
		if (parameters -> isAtom ()) {root -> setPreprocessor (parameters -> getAtom ()); return true;}
		if (parameters -> isEarth ()) {root -> setPreprocessor (NULL); return true;}
		if (parameters -> isVar ()) {
			if (preprocessor != NULL) parameters -> setAtom (preprocessor);
			else parameters -> setEarth ();
			return true;
		}
		return false;
	}
	preprocessor (PrologRoot * root) {this -> root = root;}
};

class prompt : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (root -> getCommander () == NULL) return false;
		if (! parameters -> isPair ()) {
			char * current_prompt = root -> getCommander () -> getPrompt ();
			if (current_prompt == NULL) return false;
			parameters -> setText (current_prompt);
			return true;
		}
		parameters = parameters -> getLeft ();
		if (parameters -> isText ()) {root -> getCommander () -> setPrompt (parameters -> getText ()); return true;}
		if (parameters -> isVar ()) {
			char * current_prompt = root -> getCommander () -> getPrompt ();
			if (current_prompt == NULL) return false;
			parameters -> setText (current_prompt);
			return true;
		}
		return false;
	}
	prompt (PrologRoot * root) {this -> root = root;}
};

class query_stack : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologQuery * query = resolution -> getQuery ();
		while (query != NULL) {
			drop_element (root, query -> query);
			query = query -> stack;
		}
		return true;
	}
	query_stack (PrologRoot * root) {this -> root = root;}
};

class object_counter_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			drop_object_counter ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		AREA area;
		drop_object_counters (area);
		parameters -> setText (area);
		return true;
	}
	object_counter_class (void) {}
};

class is_atom : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		return parameters -> getLeft () -> isAtom ();
	}
	is_atom (PrologRoot * root) {}
};

class is_integer : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		return parameters -> getLeft () -> isInteger ();
	}
	is_integer (PrologRoot * root) {}
};

class is_double : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		return parameters -> getLeft () -> isDouble ();
	}
	is_double (PrologRoot * root) {}
};

class is_number : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		return parameters -> isInteger () || parameters -> isDouble ();
	}
	is_number (PrologRoot * root) {}
};

class is_var : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		return parameters -> getLeft () -> isVar ();
	}
	is_var (PrologRoot * root) {}
};

class is_head : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		return parameters -> getLeft () -> isHead ();
	}
	is_head (PrologRoot * root) {}
};

class is_text : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		return parameters -> getLeft () -> isText ();
	}
	is_text (PrologRoot * root) {}
};

class has_machine : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (parameters -> isAtom ()) {
			return parameters -> getAtom () -> getMachine () != NULL;
		}
		if (parameters -> isText ()) {
			PrologDirectory * dir = root -> searchDirectory (parameters -> getText ());
			if (dir == NULL) return false;
			return dir -> service_class != NULL;
		}
		return false;
	}
	has_machine (PrologRoot * root) {this -> root = root;}
};

class text_list : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (parameters -> getLeft () -> isText ()) {
			char * text = parameters -> getLeft () -> getText ();
			parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			parameters = parameters -> getLeft ();
			while (* text != '\0') {
				parameters -> setPair (root -> integer (* text++), root -> earth ());
				parameters = parameters -> getRight ();
			}
			return true;
		}
		PrologElement * text_ptr = parameters -> getLeft ();
		AREA area;
		int area_ind = 0;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		while (parameters -> isPair ()) {
			if (! parameters -> getLeft () -> isInteger ()) return false;
			area_ind = area_cat (area, area_ind, (char) parameters -> getLeft () -> getInteger ());
			parameters = parameters -> getRight ();
		}
		text_ptr -> setText (area);
		return parameters -> isEarth ();
	}
	text_list (PrologRoot * root) {this -> root = root;}
};

class term_reader : public PrologReader {
public:
	char * text;
	virtual void message (char * text) {}
	virtual int move_z (void) {
		if (* text == '\0') return -1;
		return * text++;
	}
	void init (PrologRoot * root, char * text) {
		this -> text = text;
		setRoot (root);
	}
};

class text_term : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el;
		if (parameters -> getLeft () -> isText ()) {
			term_reader * tr = new term_reader ();
			tr -> init (root, parameters -> getLeft () -> getText ());
			parameters = parameters -> getRight ();
			parameters -> setEarth ();
			el = tr -> readElement ();
			while (el != NULL) {
				parameters -> setPair (el, root -> earth ());
				parameters = parameters -> getRight ();
				tr -> reset_var_number ();
				el = tr -> readElement ();
			}
			delete tr;
			return true;
		}
		el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		AREA area;
		int area_ind = area_cat (area, 0, "");
		while (parameters -> isPair ()) {
			if (area_ind > 0) area_ind = area_cat (area, area_ind, " ");
			area_ind = root -> getValue (parameters -> getLeft (), area, area_ind);
			parameters = parameters -> getRight ();
		}
		el -> setText (area);
		return true;
	}
	text_term (PrologRoot * root) {this -> root = root;}
};

class e32 : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		int ind;
		if (parameters -> getLeft () -> isInteger ()) {
			ind = parameters -> getLeft () -> getInteger ();
			parameters -> getRight () -> setPair (root -> integer (ind & 0xff), root -> pair (root -> integer ((ind >> 8) & 0xff), root -> pair (root -> integer ((ind >> 16) & 0xff), root -> pair (root -> integer ((ind >> 24) & 0xff), root -> earth ()))));
			return true;
		}
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () ->isInteger ()) return false;
		ind = parameters -> getLeft () -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		ind += parameters -> getLeft () -> getInteger () << 8;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		ind += parameters -> getLeft () -> getInteger () << 16;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		ind += parameters -> getLeft () -> getInteger () << 24;
		el -> setInteger (ind);
		return true;
	}
	e32 (PrologRoot * root) {this -> root = root;}
};

class sum : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * e3 = parameters -> isPair () ? parameters -> getLeft () : parameters;
		if (e1 -> isInteger ()) {
			if (e2 -> isInteger ()) {
				e3 -> setInteger (e1 -> getInteger () + e2 -> getInteger ());
				return true;
			}
			if (e2 -> isDouble ()) {
				e3 -> setDouble (e1 -> getInteger () + e2 -> getDouble ());
				return true;
			}
			if (e3 -> isInteger ()) {
				e2 -> setInteger (e3 -> getInteger () - e1 -> getInteger ());
				return true;
			}
			if (e3 -> isDouble ()) {
				e2 -> setDouble (e3 -> getDouble () - e1 -> getInteger ());
				return true;
			}
			return false;
		}
		if (e1 -> isDouble ()) {
			if (e2 -> isInteger ()) {
				e3 -> setDouble (e1 -> getDouble () + e2 -> getInteger ());
				return true;
			}
			if (e2 -> isDouble ()) {
				e3 -> setDouble (e1 -> getDouble () + e2 -> getDouble ());
				return true;
			}
			if (e3 -> isInteger ()) {
				e2 -> setDouble (e3 -> getInteger () - e1 -> getDouble ());
				return true;
			}
			if (e3 -> isDouble ()) {
				e2 -> setDouble (e3 -> getDouble () - e1 -> getDouble ());
				return true;
			}
			return false;
		}
		if (e2 -> isInteger ()) {
			if (e3 -> isInteger ()) {
				e1 -> setInteger (e3 -> getInteger () - e2 -> getInteger ());
				return true;
			}
			if (e3 -> isDouble ()) {
				e1 -> setDouble (e3 -> getDouble () - e2 -> getInteger ());
				return true;
			}
			return false;
		}
		if (e2 -> isDouble ()) {
			if (e3 -> isInteger ()) {
				e1 -> setDouble (e3 -> getInteger () - e2 -> getDouble ());
				return true;
			}
			if (e3 -> isDouble ()) {
				e1 -> setDouble (e3 -> getDouble () - e2 -> getDouble ());
				return true;
			}
		}
		return false;
	}
	sum (PrologRoot * root) {}
};

class times : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * e3 = parameters -> isPair () ? parameters -> getLeft () : parameters;
		int ind;
		double db;
		if (e1 -> isInteger ()) {
			if (e2 -> isInteger ()) {
				e3 -> setInteger (e1 -> getInteger () * e2 -> getInteger ());
				return true;
			}
			if (e2 -> isDouble ()) {
				e3 -> setDouble (e1 -> getInteger () * e2 -> getDouble ());
				return true;
			}
			ind = e1 -> getInteger ();
			if (ind == 0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e3 -> isInteger ()) {
				e2 -> setInteger (e3 -> getInteger () / ind);
				return true;
			}
			if (e3 -> isDouble ()) {
				e2 -> setDouble (e3 -> getDouble () / ind);
				return true;
			}
			return false;
		}
		if (e1 -> isDouble ()) {
			if (e2 -> isInteger ()) {
				e3 -> setDouble (e1 -> getDouble () * e2 -> getInteger ());
				return true;
			}
			if (e2 -> isDouble ()) {
				e3 -> setDouble (e1 -> getDouble () * e2 -> getDouble ());
				return true;
			}
			db = e1 -> getDouble ();
			if (db == 0.0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e3 -> isInteger ()) {
				e2 -> setDouble (e3 -> getInteger () / db);
				return true;
			}
			if (e3 -> isDouble ()) {
				e2 -> setDouble (e3 -> getDouble () / db);
				return true;
			}
			return false;
		}
		if (e2 -> isInteger ()) {
			ind = e2 -> getInteger ();
			if (ind == 0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e3 -> isInteger ()) {
				e1 -> setInteger (e3 -> getInteger () / ind);
				return true;
			}
			if (e3 -> isDouble ()) {
				e1 -> setDouble (e3 -> getDouble () / ind);
				return true;
			}
			return false;
		}
		if (e2 -> isDouble ()) {
			db = e2 -> getDouble ();
			if (db == 0.0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e3 -> isInteger ()) {
				e1 -> setDouble (e3 -> getInteger () / db);
				return true;
			}
			if (e3 -> isDouble ()) {
				e1 -> setDouble (e3 -> getDouble () / db);
				return true;
			}
		}
		return false;
	}
	times (PrologRoot * root) {this -> root = root;}
};

class add_strings : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * t1 = parameters -> getLeft ();
		if (! t1 -> isText ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * t2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		int length1 = (int) strlen (t1 -> getText ());
		int length2 = (int) strlen (t2 -> getText ());
		char * area = new char [length1 + length2 + 20];
		sprintf (area, "%s%s", t1 -> getText (), t2 -> getText ());
		parameters -> setText (area);
		delete area;
		return true;
	}
	add_strings (PrologRoot * root) {}
};

class add : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * result = parameters -> getLeft ();
		if (result -> isVar ()) parameters = parameters -> getRight ();
		int result_type = 0;
		int int_result = 0;
		double double_result = 0.0;
		AREA area;
		int area_ind = area_cat (area, 0, "");
		bool added = false;
		PrologElement * e1 = parameters -> getLeft ();
		while (parameters -> isPair ()) {
			if (e1 -> isInteger ()) {
				switch (result_type) {
				case 0: int_result += e1 -> getInteger (); break;
				case 1: double_result += (double) e1 -> getInteger (); break;
				case 2: area_ind = area_cat_number (area, area_ind, e1 -> getInteger ()); break;
				}
			}
			if (e1 -> isDouble ()) {
				switch (result_type) {
				case 0: double_result += e1 -> getDouble () + (double) int_result; result_type = 1; break;
				case 1: double_result += e1 -> getDouble (); break;
				case 2: area_ind = area_cat_number (area, area_ind, e1 -> getDouble ()); break;
				}
			}
			if (e1 -> isText () || e1 -> isAtom ()) {
				switch (result_type) {
				case 0:
					if (added) area_ind = area_cat_number (area, area_ind, int_result);
					area_ind = area_cat (area, area_ind, e1 -> isText () ? e1 -> getText () : e1 -> getAtom () -> name ());
					result_type = 2;
					break;
				case 1:
					area_ind = area_cat_number (area, area_ind, double_result);
					area_ind = area_cat (area, area_ind, e1 -> isText () ? e1 -> getText () : e1 -> getAtom () -> name ());
					result_type = 2;
					break;
				case 2: area_ind = area_cat (area, area_ind, e1 ->  isText () ? e1 -> getText () : e1 -> getAtom () -> name ()); break;
				}
			}
			if (e1 -> isVar ()) {
				switch (result_type) {
				case 0: e1 -> setInteger (int_result); break;
				case 1: e1 -> setDouble (double_result); break;
				case 2: e1 -> setText (area); break;
				}
				return true;
			}
			parameters = parameters -> getRight ();
			e1 = parameters -> getLeft ();
			added = true;
		}
		if (parameters -> isVar ()) result = parameters;
		switch (result_type) {
		case 0: result -> setInteger (int_result); break;
		case 1: result -> setDouble (double_result); break;
		case 2: result -> setText (area); break;
		}
		return true;
	}
	add (PrologRoot * root) {}
};

class division : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (e2 -> isInteger ()) {
			int ind = e2 -> getInteger ();
			if (ind == 0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e1 -> isInteger ()) {
				parameters -> setInteger (e1 -> getInteger () / ind);
				return true;
			}
			if (e1 -> isDouble ()) {
				parameters -> setDouble (e1 -> getDouble () / ind);
				return true;
			}
			return false;
		}
		if (e2 -> isDouble ()) {
			double db = e2 -> getDouble ();
			if (db == 0.0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e1 -> isInteger ()) {
				parameters -> setDouble (e1 -> getInteger () / db);
				return true;
			}
			if (e1 -> isDouble ()) {
				parameters -> setDouble (e1 -> getDouble () / db);
				return true;
			}
		}
		return false;
	}
	division (PrologRoot * root) {this -> root = root;}
};

class mod : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * e_div = NULL;
		if (parameters -> isPair ()) {
			PrologElement * e = parameters -> getRight ();
			if (e -> isPair ()) e_div = e -> getLeft ();
			parameters = parameters -> getLeft ();
		}
		if (e2 -> isInteger ()) {
			int ind = e2 -> getInteger ();
			if (ind == 0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e1 -> isInteger ()) {
				parameters -> setInteger (e1 -> getInteger () % ind);
				if (e_div != NULL) e_div -> setInteger (e1 -> getInteger () / ind);
				return true;
			}
			if (e1 -> isDouble ()) {
				parameters -> setDouble ((int) e1 -> getDouble () % ind);
				if (e_div != NULL) e_div -> setDouble ((int) e1 -> getDouble () / ind);
				return true;
			}
			return false;
		}
		if (e2 -> isDouble ()) {
			int db = (int) e2 -> getDouble ();
			if (db == 0) {root -> message ("by zero"); root -> message (root -> new_line_caption); return false;}
			if (e1 -> isInteger ()) {
				parameters -> setDouble (e1 -> getInteger () % db);
				if (e_div != NULL) e_div -> setDouble (e1 -> getInteger () / db);
				return true;
			}
			if (e1 -> isDouble ()) {
				parameters -> setDouble ((int) e1 -> getDouble () % db);
				if (e_div != NULL) e_div -> setDouble ((int) e1 -> getDouble () / db);
				return true;
			}
		}
		return false;
	}
	mod (PrologRoot * root) {this -> root = root;}
};

class DCMOD : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e_diatonic = parameters -> getLeft ();
		if (! e_diatonic -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_chromatic = parameters -> getLeft ();
		if (! e_chromatic -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_octave = parameters -> getLeft ();
		if (! e_octave -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_diatonic_shift = parameters -> getLeft ();
		if (! e_diatonic_shift -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_chromatic_shift = parameters -> getLeft ();
		if (! e_chromatic_shift -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		int diatonic = e_diatonic -> getInteger () + e_diatonic_shift -> getInteger ();

		int chromatic = e_chromatic -> getInteger () + e_chromatic_shift -> getInteger ();
		int octave = e_octave -> getInteger ();
		while (chromatic < 0) {chromatic += 12; diatonic += 7; octave--;}
		while (chromatic > 11) {chromatic -= 12; diatonic -= 7; octave++;}
		if (! parameters -> isPair ()) return false;
		parameters -> getLeft () -> setInteger (diatonic);
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters -> getLeft () -> setInteger (chromatic);
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters -> getLeft () -> setInteger (octave);
		return true;
	}
};

class sub : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (e1 -> isInteger ()) {
			if (e2 -> isInteger ()) {
				parameters -> setInteger (e1 -> getInteger () - e2 -> getInteger ());
				return true;
			}
			if (e2 -> isDouble ()) {
				parameters -> setDouble (e1 -> getInteger () - e2 -> getDouble ());
				return true;
			}
			return false;
		}
		if (e1 -> isDouble ()) {
			if (e2 -> isInteger ()) {
				parameters -> setDouble (e1 -> getDouble () - e2 -> getInteger ());
				return true;
			}
			if (e2 -> isDouble ()) {
				parameters -> setDouble (e1 -> getDouble () - e2 -> getDouble ());
				return true;
			}
		}
		return false;
	}
	sub (PrologRoot * root) {}
};

class mult : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * result = parameters -> getLeft ();
		if (result -> isVar ()) parameters = parameters -> getRight ();
		int result_type = 0;
		int int_result = 1;
		double double_result = 1.0;
		PrologElement * e1 = parameters -> getLeft ();
		while (parameters -> isPair ()) {
			if (e1 -> isInteger ()) {
				switch (result_type) {
				case 0: int_result *= e1 -> getInteger (); break;
				case 1: double_result *= (double) e1 -> getInteger (); break;
				}
			}
			if (e1 -> isDouble ()) {
				switch (result_type) {
				case 0: double_result *= e1 -> getDouble () * (double) int_result; result_type = 1; break;
				case 1: double_result *= e1 -> getDouble (); break;
				}
			}
			if (e1 -> isVar ()) {
				switch (result_type) {
				case 0: e1 -> setInteger (int_result); break;
				case 1: e1 -> setDouble (double_result); break;
				}
				return true;
			}
			parameters = parameters -> getRight ();
			e1 = parameters -> getLeft ();
		}
		if (parameters -> isVar ()) result = parameters;
		switch (result_type) {
		case 0: result -> setInteger (int_result); break;
		case 1: result -> setDouble (double_result); break;
		}
		return true;
	}
	mult (PrologRoot * root) {}
};

class logical : public PrologNativeCode {
public:
	virtual int operation (int a, int b) {return 0;}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		int a = parameters -> getLeft () -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (! parameters -> getLeft () -> isInteger ()) return false;
		int b = parameters -> getLeft () -> getInteger ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setInteger (operation (a, b));
		return true;
	}
};

class logical_and : public logical {
public:
	int operation (int a, int b) {return a & b;}
	logical_and (PrologRoot * root) {}
};

class logical_or : public logical {
public:
	int operation (int a, int b) {return a | b;}
	logical_or (PrologRoot * root) {}
};

class logical_xor : public logical {
public:
	int operation (int a, int b) {return a ^ b;}
	logical_xor (PrologRoot * root) {}
};
class abs_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		double a;
		int ind;
		if (el -> isDouble ()) {a = el -> getDouble (); parameters -> setDouble (a >= 0.0 ? a : - a); return true;}
		if (el -> isInteger ()) {ind = el -> getInteger (); parameters -> setInteger (ind >= 0 ? ind : - ind); return true;}
		return false;
	}
};
class cos_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (cos (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setDouble (cos ((double) el -> getInteger () * M_PI / 180.0)); return true;}
		if (el -> isVar ()) {
			double sub = 2.0;
			if (parameters -> isDouble ()) sub = parameters -> getDouble ();
			if (parameters -> isInteger ()) sub = (double) parameters -> getInteger ();
			if (sub > 1.0 || sub < -1.0) return false;
			el -> setDouble (acos (sub));
			return true;
		}
		return false;
	}
};
class degrad_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isInteger ()) {parameters -> setDouble ((double) el -> getInteger () * M_PI / 180.0); return true;}
		if (el -> isDouble ()) {parameters -> setDouble (el -> getDouble () * M_PI / 180.0); return true;}
		if (parameters -> isDouble ()) {el -> setDouble (parameters -> getDouble () * 180.0 / M_PI); return true;}
		return false;
	}
};
class e_operation : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setDouble (M_E);
		return true;
	}
};
class exp_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (exp (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setInteger ((int) (exp ((double) el -> getInteger ()))); return true;}
		return false;
	}
};
class log2_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		double x, n;
		bool integer = true;
		if (ex -> isDouble ()) {x = ex -> getDouble (); integer = false;}
		else {if (! ex -> isInteger ()) return false; x = (double) ex -> getInteger ();}
		if (x <= 0.0) return false;
		n = log (x) / log (2.0);
		if (integer) parameters -> setInteger ((int) n);
		else parameters -> setDouble (n);
		return true;
	}
};
class log10_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		double x, n;
		bool integer = true;
		if (ex -> isDouble ()) {x = ex -> getDouble (); integer = false;}
		else {if (! ex -> isInteger ()) return false; x = (double) ex -> getInteger ();}
		if (x <= 0.0) return false;
		n = log10 (x);
		if (integer) parameters -> setInteger ((int) n);
		else parameters -> setDouble (n);
		return true;
	}
};
class ln_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		double x, n;
		bool integer = true;
		if (ex -> isDouble ()) {x = ex -> getDouble (); integer = false;}
		else {if (! ex -> isInteger ()) return false; x = (double) ex -> getInteger ();}
		if (x <= 0.0) return false;
		n = log (x);
		if (integer) parameters -> setInteger ((int) n);
		else parameters -> setDouble (n);
		return true;
	}
};
class log_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * en = parameters -> isPair () ? parameters -> getLeft () : parameters;
		bool integer = true;
		double a, x, n;
		if (ea -> isDouble ()) {a = ea -> getDouble (); integer = false;}
		else {if (! ea -> isInteger ()) return false; a = (double) ea -> getInteger ();}
		if (ex -> isDouble ()) {x = ex -> getDouble (); integer = false;}
		else {if (! ex -> isInteger ()) return false; x = (double) ex -> getInteger ();}
		if (a <= 0.0) return false;
		if (a == 1.0) return false;
		if (x <= 0.0) return false;
		n = log (x) / log (a);
		if (integer) en -> setInteger ((int) n);
		else en -> setDouble (n);
		return true;
	}
};
class pi_operation : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setDouble (M_PI);
		return true;
	}
};
class pow_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * en = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * ex = parameters -> isPair () ? parameters -> getLeft () : parameters;
		double a, n, x;
		bool integer = true;
		if (ex -> isVar ()) {
			if (ea -> isDouble ()) {a = ea -> getDouble (); integer = false;}
			else {if (! ea -> isInteger ()) return false; a = (double) ea -> getInteger ();}
			if (en -> isDouble ()) {if (a < 0.0) return false; n = en -> getDouble (); integer = false;}
			else {if (! en -> isInteger ()) return false; n = (double) en -> getInteger ();}
			x = pow (a, n);
			if (integer) ex -> setInteger ((int) x);
			else ex -> setDouble (x);
			return true;
		}
		if (ea -> isVar ()) {
			if (ex -> isDouble ()) {a = ex -> getDouble (); integer = false;}
			else {if (! ex -> isInteger ()) return false; a = (double) ex -> getInteger ();}
			if (en -> isDouble ()) {n = en -> getDouble (); integer = false;}
			else {if (! en -> isInteger ()) return false; n = (double) en -> getInteger ();}
			if (n == 0.0) return false;
			n = 1.0 / n;
			if ((double) ((int) n) != n) {if (a < 0.0) return false;}
			x = pow (a, n);
			if (integer) ea -> setInteger ((int) x);
			else ea -> setDouble (x);
			return true;
		}
		if (ea -> isDouble ()) {a = ea -> getDouble (); integer = false;}
		else {if (! ea -> isInteger ()) return false; a = (double) ea -> getInteger ();}
		if (ex -> isDouble ()) {x = ex -> getDouble (); integer = false;}
		else {if (! ex -> isInteger ()) return false; x = (double) ex -> getInteger ();}
		if (a <= 0.0) return false;
		if (a == 1.0) return false;
		if (x <= 0.0) return false;
		n = log (x) / log (a);
		if (integer) en -> setInteger ((int) n);
		else en -> setDouble (n);
		return true;
	}
};
class sin_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (sin (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setDouble (sin ((double) el -> getInteger () * M_PI / 180.0)); return true;}
		if (el -> isVar ()) {
			double sub = 2.0;
			if (parameters -> isDouble ()) sub = parameters -> getDouble ();
			if (parameters -> isInteger ()) sub = (double) parameters -> getInteger ();
			if (sub > 1.0 || sub < -1.0) return false;
			el -> setDouble (asin (sub));
			return true;
		}
		return false;
	}
};
class tan_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (tan (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setDouble (tan ((double) el -> getInteger () * M_PI / 180.0)); return true;}
		if (parameters -> isDouble ()) {el -> setDouble (atan (parameters -> getDouble ())); return true;}
		if (parameters -> isInteger ()) {el -> setDouble (atan ((double) parameters -> getInteger ())); return true;}
		return false;
	}
};
class trunc_operation : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * left = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (left -> isDouble ()) {
			parameters -> setInteger ((int) left -> getDouble ());
			return true;
		}
		if (left -> isInteger ()) {
			parameters -> setDouble ((double) left -> getInteger ());
			return true;
		}
		if (left -> isVar ()) {
			if (parameters -> isInteger ()) {left -> setDouble ((double) parameters -> getInteger ()); return true;}
			if (parameters -> isDouble ()) {left -> setDouble ((double) ((int) parameters -> getDouble ())); return true;}
			return false;
		}
		return false;
	}
};

#ifdef LINUX_OPERATING_SYSTEM

#include <time.h>

class timestamp : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * stamp = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * year = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * month = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * day = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * year_day = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * week_day = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * hour = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * minute = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * second = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (year -> isVar () || year -> isEarth ()) {
			time_t time_stamp = 0;
			if (stamp -> isInteger ()) time_stamp = (time_t) stamp -> getInteger ();
			else {time_stamp = time (NULL); stamp -> setInteger ((int) time_stamp);}
			struct tm * ptm;
			ptm = localtime (& time_stamp);
			year -> setInteger (1900 + ptm -> tm_year);
			month -> setInteger (ptm -> tm_mon + 1);
			day -> setInteger (ptm -> tm_mday);
			year_day -> setInteger (ptm -> tm_yday);
			week_day -> setInteger (ptm -> tm_wday);
			hour -> setInteger (ptm -> tm_hour);
			minute -> setInteger (ptm -> tm_min);
			second -> setInteger (ptm -> tm_sec);
			return true;
		}
		time_t time_stamp = time (NULL);
		struct tm * tmp;
		tmp = localtime (& time_stamp);
		if (year -> isInteger ()) tmp -> tm_year = year -> getInteger () - 1900;
		if (month -> isInteger ()) tmp -> tm_mon = month -> getInteger () - 1;
		if (day -> isInteger ()) tmp -> tm_mday = day -> getInteger ();
		if (year_day -> isInteger ()) tmp -> tm_yday = year_day -> getInteger ();
		if (week_day -> isInteger ()) tmp -> tm_wday = week_day -> getInteger ();
		if (hour -> isInteger ()) tmp -> tm_hour = hour -> getInteger ();
		if (minute -> isInteger ()) tmp -> tm_min = minute -> getInteger ();
		if (second -> isInteger ()) tmp -> tm_sec = second -> getInteger ();
		time_stamp = mktime (tmp);
		if (! stamp -> isInteger ()) stamp -> setInteger ((int) time_stamp);
		if (! year -> isInteger ()) year -> setInteger (1900 + tmp -> tm_year);
		if (! month -> isInteger ()) month -> setInteger (tmp -> tm_mon + 1);
		if (! day -> isInteger ()) day -> setInteger (tmp -> tm_mday);
		if (! year_day -> isInteger ()) year_day -> setInteger (tmp -> tm_yday);
		if (! week_day -> isInteger ()) week_day -> setInteger (tmp -> tm_wday);
		if (! hour -> isInteger ()) hour -> setInteger (tmp -> tm_hour);
		if (! minute -> isInteger ()) minute -> setInteger (tmp -> tm_min);
		if (! second -> isInteger ()) second -> setInteger (tmp -> tm_sec);
		return true;
	}
};

#else

class timestamp : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		return true;
	}
};

#endif

class less : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		PrologElement * b;
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			b = parameters -> getLeft ();
			if (a -> isInteger ()) {
				if (b -> isInteger ()) {if (a -> getInteger () >= b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if ((double) a -> getInteger () >= b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isDouble ()) {
				if (b -> isInteger ()) {if (a -> getDouble () >= (double) b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if (a -> getDouble () >= b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isText ()) {
				if (! b -> isText ()) return false;
				if (strcmp (a -> getText (), b -> getText ()) >= 0) return false;
			}
			else return false;
			a = b;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	less (PrologRoot * root) {}
};

class less_eq : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		PrologElement * b;
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			b = parameters -> getLeft ();
			if (a -> isInteger ()) {
				if (b -> isInteger ()) {if (a -> getInteger () > b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if ((double) a -> getInteger () > b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isDouble ()) {
				if (b -> isInteger ()) {if (a -> getDouble () > (double) b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if (a -> getDouble () > b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isText ()) {
				if (! b -> isText ()) return false;
				if (strcmp (a -> getText (), b -> getText ()) > 0) return false;
			}
			else return false;
			a = b;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	less_eq (PrologRoot * root) {}
};

class greater : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		PrologElement * b;
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			b = parameters -> getLeft ();
			if (a -> isInteger ()) {
				if (b -> isInteger ()) {if (a -> getInteger () <= b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if ((double) a -> getInteger () <= b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isDouble ()) {
				if (b -> isInteger ()) {if (a -> getDouble () <= (double) b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if (a -> getDouble () <= b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isText ()) {
				if (! b -> isText ()) return false;
				if (strcmp (a -> getText (), b -> getText ()) <= 0) return false;
			}
			else return false;
			a = b;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	greater (PrologRoot * root) {}
};

class greater_eq : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		PrologElement * b;
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			b = parameters -> getLeft ();
			if (a -> isInteger ()) {
				if (b -> isInteger ()) {if (a -> getInteger () < b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if ((double) a -> getInteger () < b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isDouble ()) {
				if (b -> isInteger ()) {if (a -> getDouble () < (double) b -> getInteger ()) return false;}
				else {
					if (b -> isDouble ()) {if (a -> getDouble () < b -> getDouble ()) return false;}
					else return false;
				}
			}
			else if (a -> isText ()) {
				if (! b -> isText ()) return false;
				if (strcmp (a -> getText (), b -> getText ()) < 0) return false;
			}
			else return false;
			a = b;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	greater_eq (PrologRoot * root) {}
};

class file_write : public PrologNativeCode {
public:
	FILE * fw;
	PrologRoot * root;
	PrologAtom * atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		if (fw == NULL) return false;
		PrologElement * el;
		if (parameters -> isEarth ()) {
			fclose (fw);
			fw = NULL;
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (el -> isText ()) fprintf (fw, "%s", el -> getText ());
			if (el -> isInteger ()) fputc (el -> getInteger (), fw);
			while (el -> isPair ()) {
				root -> getValue (el -> getLeft (), area, 0);
				fprintf (fw, "%s", area);
				el = el -> getRight ();
			}
			parameters = parameters -> getRight ();
		}
		return true;
	}
	file_write (PrologAtom * atom, PrologRoot * root, char * file_name) {
		this -> atom = atom;
		this -> root = root;
		PROLOG_STRING command;
		sprintf (command, "%s%s", root -> root_directory, file_name);
		fw = fopen (command, "wb");
	}
	~ file_write (void) {if (fw != NULL) fclose (fw);}
};

class file_writer : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (ea -> isVar ()) {
			atom = new PrologAtom ();
			ea -> setAtom (atom);
		}
		else {
			if (! ea -> isAtom ()) return false;
			atom = ea -> getAtom ();
		}
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * et = parameters -> getLeft ();
		if (! et -> isText ()) return false;
		file_write * fw = new file_write (atom, root, et -> getText ());
		if (atom -> setMachine (fw)) return true;
		delete fw;
		return false;
	}
	file_writer (PrologRoot * root) {this -> root = root;}
};

class symbol_reader : public PrologReader {
public:
	FILE * fi;
	void message (char * text) {root -> print (text); root -> print (root -> new_line_caption);}
	void message_v (char * text, char * variable) {root -> message (text, variable);}
	int move_z (void) {return fgetc (fi);}
	void init (PrologRoot * root, FILE * fi) {
		this -> fi = fi;
		setRoot (root);
	}
};

class file_read : public PrologNativeCode {
public:
	FILE * fi;
	symbol_reader sr;
	PrologAtom * atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (fi == NULL) {
			if (! parameters -> isEarth ()) return false;
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		if (parameters -> isEarth ()) {
			fclose (fi);
			fi = NULL;
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		if (el -> isText ()) {
			parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			char * char_set = el -> getText ();
			AREA area;
			if (sr . getString (area, 0, char_set) < 0) return false;
			parameters -> getLeft () -> setText (area);
			return true;
		}
		el = sr . readElement ();
		if (el == NULL) return false;
		parameters -> setLeft (el);
		return true;
	}
	file_read (PrologAtom * atom, PrologRoot * root, char * file_name) {
		this -> atom = atom;
		if (
			(file_name [1] == ':' && file_name [2] == '\\')
			|| (file_name [0] == '~' && file_name [1] == '/')
			|| file_name [0] == '/'
			) fi = fopen (file_name, "rb");
		else {
			PROLOG_STRING command;
			sprintf (command, "%s%s", root -> root_directory, file_name);
			fi = fopen (command, "rb");
		}
		sr . init (root, fi);
	}
	~ file_read (void) {if (fi != NULL) fclose (fi);}
};

class file_reader : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (ea -> isVar ()) {
			atom = new PrologAtom ();
			ea -> setAtom (atom);
		} else {
			if (! ea -> isAtom ()) return false;
			atom = ea -> getAtom ();
		}
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * et = parameters -> getLeft ();
		if (! et -> isText ()) return false;
		file_read * fr = new file_read (atom, root, et -> getText ());
		if (fr -> fi == NULL) {delete fr; return false;}
		if (atom -> setMachine (fr)) return true;
		delete fr;
		return false;
	}
	file_reader (PrologRoot * root) {this -> root = root;}
};

class module_loader : public PrologNativeCode {
public:
	PrologRoot * root;
	bool echo;
	bool reload;
	bool code (PrologElement * parameters, PrologResolution * resulotion) {
		while (parameters -> isPair ()) {
			PrologElement * module_name = parameters -> getLeft ();
			if (! module_name -> isText ()) return false;
			PrologLoader loader (root);
			loader . echo = echo;
			loader . reload = reload;
			if (strstr (module_name -> getText (), ".prc") == NULL) {
				PROLOG_STRING command;
				prolog_string_copy (command, module_name -> getText ());
				prolog_string_cat (command, ".prc");
				if (! loader . load_without_main (command)) return false;
			} else {if (! loader . load_without_main (module_name -> getText ())) return false;}
			parameters = parameters -> getRight ();
		}
		return true;
	}
};

class import_loader : public module_loader {
public:
	import_loader (PrologRoot * root) {
		this -> root = root;
		this -> echo = false;
		this -> reload = false;
	}
};

class load_loader : public module_loader {
public:
	load_loader (PrologRoot * root) {
		this -> root = root;
		this -> echo = false;
		this -> reload = true;
	}
};

class consult_loader : public module_loader {
public:
	consult_loader (PrologRoot * root) {
		this -> root = root;
		this -> echo = true;
		this -> reload = true;
	}
};

class remove_module : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		return root -> drop (parameters -> getText ());
	}
	remove_module (PrologRoot * root) {this -> root = root;}
};

class create_module : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {root -> close (); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * em = parameters -> getLeft ();
		if (! em -> isText ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {root -> createDirectory (em -> getText ()); return true;}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		PrologServiceClass * service = root -> loadServiceClass (parameters -> getText ());
		if (service == NULL) return false;
		root -> createDirectory (em -> getText (), service);
		return true;
	}
	create_module (PrologRoot * root) {this -> root = root;}
};

class set_machine : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologNativeCode * native;
		bool ret;
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		if (! ea -> isAtom ()) return false;
		parameters = parameters -> getRight ();
//		if (parameters -> isEarth ()) {
//			PrologAtom * a = ea -> getAtom ();
//			a -> unProtect ();
//			native = a -> getMachine ();
//			ret = a -> setMachine (NULL);
//			a -> unProtect ();
//			if (ret) delete native;
//			return ret;
//		}
		if (! parameters -> isPair ()) return false;
		PrologElement * et = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologServiceClass * service;
		if (parameters -> isEarth ()) {
			service = root -> getServiceClass ();
			if (service == NULL) return false;
			native = service -> getNativeCode (et -> getText ());
			if (native == NULL) return false;
			ret = ea -> getAtom () -> setMachine (native);
			if (! ret) delete native;
			return ret;
		}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		service = root -> getServiceClass (et -> getText ());
		if (service == NULL) return false;
		native = service -> getNativeCode (parameters -> getText ());
		if (native == NULL ) return false;
		ret = ea -> getAtom () -> setMachine (native);
		if (! ret) delete native;
		return ret;
	}
	set_machine (PrologRoot * root) {this -> root = root;}
};

class add_search_directory : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		root -> addSearchDirectory (parameters -> getText ());
		return true;
	}
	add_search_directory (PrologRoot * root) {this -> root = root;}
};

class search_directories : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			printf ("SEARCH DIRECTORIES:\n");
			PrologString * search_directory = root -> search_directories;
			while (search_directory != NULL) {
				printf ("	[%s]\n", search_directory -> text);
				search_directory = search_directory -> next;
			}
			return true;
		}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isEarth ()) {root -> deleteSearchDirectories (); return true;}
		if (parameters -> isVar ()) {
			PrologString * search_directory = root -> search_directories;
			while (search_directory != NULL) {
				parameters -> setPair ();
				parameters -> getLeft () -> setText (search_directory -> text);
				parameters = parameters -> getRight ();
				search_directory = search_directory -> next;
			}
			return true;
		}
		if (parameters -> isPair ()) {
			PrologString * search_directory = NULL;
			root -> deleteSearchDirectories ();
			while (parameters -> isPair ()) {
				if (parameters -> getLeft () -> isText ()) {
					PrologString * dir = new PrologString (parameters -> getLeft () -> getText (), NULL);
					if (root -> search_directories == NULL) root -> search_directories = search_directory = dir;
					else {search_directory -> next = dir; search_directory = search_directory -> next;}
				}
				parameters = parameters -> getRight ();
			}
			return true;
		}
		return false;
	}
	search_directories (PrologRoot * root) {this -> root = root;}
};

class cd : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {return root -> change_directory ("..");}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setText (root -> getCWD ()); return true;}
		if (parameters -> isText ()) return root -> change_directory (parameters -> getText ());
		return false;
	}
	cd (PrologRoot * root) {this -> root = root;}
};

class root_directory : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {printf ("ROOT DIRECTORY = [%s]\n", root -> root_directory); return true;}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setText (root -> root_directory); return true;}
		if (parameters -> isText ()) {prolog_string_copy (root -> root_directory, parameters -> getText ()); return true;}
		return false;
	}
	root_directory (PrologRoot * root) {this -> root = root;}
};

class crd : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			int ind = (int) strlen (root -> root_directory) - 2;
			if (ind < 0) ind = 0;
			while (ind > 0) {
				if (root -> root_directory [ind] == '/') {
					ind++;
					root -> root_directory [ind] = '\0';
					return true;
				}
				ind--;
			}
			root -> root_directory [ind] = '\0';
			return true;
		}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setText (root -> root_directory); return true;}
		if (! parameters -> isText ()) return false;
		prolog_string_cat (root -> root_directory, parameters -> getText ());
		prolog_string_cat (root -> root_directory, "/");
		return true;
	}
	crd (PrologRoot * root) {this -> root = root;}
};

class DIR : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA location;
		int ind = area_cat (location, 0, root -> root_directory);
		if (! parameters -> isPair ()) return false;
		PrologElement * left = parameters -> getLeft ();
		if (! left -> isText ()) return false;
		area_cat (location, ind, left -> getText ());
		PrologElement * el = root -> dir (location);
		if (el == NULL) return false;
		parameters -> setRight (el);
		return true;
	}
	DIR (PrologRoot * root) {this -> root = root;}
};

class ARGS : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (root -> args == NULL) {parameters -> setEarth (); return true;}
		PrologString * sp = root -> args;
		while (sp != NULL) {parameters -> setPair (); parameters -> getLeft () -> setText (sp -> text); parameters = parameters -> getRight (); sp = sp -> next;}
		return true;
	}
	ARGS (PrologRoot * root) {this -> root = root;}
};

class edit : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		AREA location;
		int ind = area_cat (location, 0, root -> root_directory);
		area_cat (location, ind, parameters -> getText ());
		return root -> edit (location);
	}
	edit (PrologRoot * root) {this -> root = root;}
};

class execute : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		return root -> execute (parameters -> getText ());
	}
	execute (PrologRoot * root) {this -> root = root;}
};

class make_directory : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		AREA location;
		int ind = area_cat (location, 0, root -> root_directory);
		area_cat (location, ind, parameters -> getText ());
		return root -> make_directory (location);
	}
	make_directory (PrologRoot * root) {this -> root = root;}
};

class erase_file : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		AREA location;
		int ind = area_cat (location, 0, root -> root_directory);
		area_cat (location, ind, parameters -> getText ());
		return root -> erase_file (location);
	}
	erase_file (PrologRoot * root) {this -> root = root;}
};

class erase_directory : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		AREA location;
		int ind = area_cat (location, 0, root -> root_directory);
		area_cat (location, ind, parameters -> getText ());
		return root -> erase_directory (location);
	}
	erase_directory (PrologRoot * root) {this -> root = root;}
};

class move_file : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * from = parameters -> getLeft ();
		if (! from -> isText ()) return false;
		parameters = parameters -> getRight ();

		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		int ind;
		AREA from_area;
		AREA to_area;
		ind = area_cat (from_area, 0, root -> root_directory);
		area_cat (from_area, ind, from -> getText ());
		ind = area_cat (to_area, 0, root -> root_directory);
		area_cat (to_area, ind, parameters -> getText ());
		return root -> move_file (from_area, to_area);
	}
	move_file (PrologRoot * root) {this -> root = root;}
};

class copy_file : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * from = parameters -> getLeft ();
		if (! from -> isText ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		int ind;
		AREA from_area;
		AREA to_area;
		ind = area_cat (from_area, 0, root -> root_directory);
		area_cat (from_area, ind, from -> getText ());
		ind = area_cat (to_area, 0, root -> root_directory);
		area_cat (to_area, ind, parameters -> getText ());
		return root -> copy_file (from_area, to_area);
	}
	copy_file (PrologRoot * root) {this -> root = root;}
};

//////////
// META //
//////////

class constant : public PrologNativeCode {
private:
	PrologElement * container;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		parameters -> duplicate (container);
		return true;
	}
	constant (PrologElement * term) {container = term -> duplicate ();}
	~ constant (void) {delete container;}
};

class CONSTANT : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (e -> isVar ()) {
			atom = new PrologAtom ();
			e -> setAtom (atom);
		} else {
			if (! e -> isAtom ()) return false;
			atom = e -> getAtom ();
		}
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		constant * c = new constant (parameters -> getLeft ());
		if (atom -> setMachine (c)) return true;
		delete c;
		return false;
	}
};

class variable : public PrologNativeCode {
private:
	PrologAtom * atom;
	PrologElement * container;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) {
			delete container;
			container = parameters -> getLeft () -> duplicate ();
			return true;
		}
		if (parameters -> isVar ()) {
			parameters -> duplicate (container);
			return true;
		}
		if (parameters -> isEarth ()) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		return false;
	}
	variable (PrologAtom * atom) {
		this -> atom = atom;
		container = new PrologElement ();
	}
	variable (PrologAtom * atom, PrologElement * element) {
		this -> atom = atom;
		container = element -> duplicate ();
	}
	~ variable (void) {delete container;}
};

class VARIABLE : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * initial_value = parameters -> getRight ();
		parameters = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (parameters -> isVar ()) {
			atom = new PrologAtom ();
			parameters -> setAtom (atom);
		} else {
			if (! parameters -> isAtom ()) return false;
			atom = parameters -> getAtom ();
		}
		variable * v;
		if (initial_value -> isPair ()) v = new variable (atom, initial_value -> getLeft ());
		else v = new variable (atom);
		if (atom -> setMachine (v)) return true;
		delete v;
		return false;
	}
};

class accumulator : public PrologNativeCode {
private:
	PrologAtom * atom;
	PrologElement * container;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) {
			resolution -> reset ();
			PrologElement * e = new PrologElement ();
			e -> setPair (resolution -> match_product (parameters -> getLeft (), true), resolution -> match_product (container, false));
			delete container;
			container = e;
			return true;
		}
		if (parameters -> isVar ()) {
			parameters -> duplicate (container);
			return true;
		}
		if (parameters -> isEarth ()) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		return false;
	}
	accumulator (PrologAtom * atom) {
		this -> atom = atom;
		container = new PrologElement ();
	}
	~ accumulator (void) {delete container;}
};

class ACCUMULATOR : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (parameters -> isVar ()) {
			atom = new PrologAtom ();
			parameters -> setAtom (atom);
		} else {
			if (! parameters -> isAtom ()) return false;
			atom = parameters -> getAtom ();
		}
		accumulator * accu = new accumulator (atom);
		if (atom -> setMachine (accu)) return true;
		delete accu;
		return false;
	}
};

class closure : public PrologNativeCode {
public:
	PrologDirectory * directory;
	PrologElement * container;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (container) {parameters -> duplicate (container); return true;}
		else {
			if (parameters -> isPair ()) {
				container = parameters -> getLeft () -> duplicate ();
				return true;
			}
		}
		PrologAtom * atom = directory -> firstAtom;
		parameters -> setEarth ();
		while (atom != NULL) {
			parameters -> setPair ();
			parameters -> getLeft () -> setAtom (atom);
			parameters = parameters -> getRight ();
			parameters -> setEarth ();
			atom = atom -> next;
		}
		return true;
	}
	void add_atom (PrologElement * parameters) {
		if (! parameters -> isPair ()) return;
		add_atom (parameters -> getRight ());
		PrologAtom * atom = directory -> createAtom ("#<closed>");
		atom -> setMachine (new variable (atom, parameters -> getLeft ()));
	}
	closure (PrologElement * parameters) {
		directory = new PrologDirectory ("#<closure>", NULL);
		add_atom (parameters);
		container = NULL;
	}
	~ closure (void) {
		if (container) delete container;
		delete directory;
	}
};

class CLOSURE : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (ea -> isVar ()) {
			atom = new PrologAtom ();
			ea -> setAtom (atom);
		} else {
			if (! ea -> isAtom ()) return false;
			atom = ea -> getAtom ();
		}
		closure * cl = new closure (parameters -> getRight ());
		if (atom -> setMachine (cl)) return true;
		delete cl;
		return false;
	}
};

typedef void * void_pointer;
class array_dimension {
public:
	void_pointer * content;
	int size;
	bool locked;
	bool elements;
	void insert_dimension (int size) {
		int ind;
		if (locked)
			for (ind = 0; ind < this -> size; ind++)
				((array_dimension *) (content [ind])) -> insert_dimension (size);
		else
			for (ind = 0; ind < this -> size; ind++)
				content [ind] = (void *) new array_dimension (size);
		locked = true;
	}
	void insert_elements (void) {
		int ind;
		if (elements) return;
		if (locked)
			for (ind = 0; ind < size; ind++)
				((array_dimension *) (content [ind])) -> insert_elements ();
		else {
			for (ind = 0; ind < size; ind++)
				content [ind] = (void *) new PrologElement ();
			elements = true;
		}
		locked = true;
	}
	array_dimension * get_dimension (int location) {
		if (location < 0 || location >= size || elements) return NULL;
		return (array_dimension *) (content [location]);
	}
	PrologElement * * get_element (int location) {
		if (location < 0 || location >= size || ! elements) return NULL;
		return (PrologElement * *) (& content [location]);
	}
	array_dimension (int size) {
		content = new void_pointer [size];
		this -> size = size;
		locked = false;
		elements = false;
	}
	~ array_dimension (void) {
		int ind;
		if (elements)
			for (ind = 0; ind < size; ind++)
				delete (PrologElement *) (content [ind]);
		else
			for (ind = 0; ind < size; ind++)
				delete (array_dimension *) (content [ind]);
		delete [] content;
	}
};

class array : public PrologNativeCode {
private:
	PrologAtom * atom;
	array_dimension * dimension;
	int dimensions;
public:
	void insert_dimension (int size) {
		if (dimension == NULL) dimension = new array_dimension (size);
		else dimension -> insert_dimension (size);
		dimensions++;
	}
	void insert_elements (void) {
		if (dimension == NULL) return;
		dimension -> insert_elements ();
	}
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		int ind = dimensions;
		array_dimension * current_dimension = dimension;
		PrologElement * left;
		while (ind > 1) {
			if (! parameters -> isPair ()) return false;
			left = parameters -> getLeft ();
			if (! left -> isInteger ()) return false;
			parameters = parameters -> getRight ();
			current_dimension = current_dimension -> get_dimension (left -> getInteger ());
			if (current_dimension == NULL) return false;
			ind--;
		}
		if (! parameters -> isPair ()) return false;
		left = parameters -> getLeft ();
		if (! left -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		PrologElement * * container = current_dimension -> get_element (left -> getInteger ());
		if (container == NULL) return false;
		if (parameters -> isPair ()) {
			delete * container;
			* container = parameters -> getLeft () -> duplicate ();
			return true;
		}
		if (parameters -> isVar ()) {
			parameters -> duplicate (* container);
			return true;
		}
		return false;
	}
	array (PrologAtom * atom) {
		this -> atom = atom;
		dimension = NULL;
		dimensions = 0;
	}
	~ array (void) {if (dimension) delete dimension;}
};

class ARRAY : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * left = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (left -> isVar ()) {
			atom = new PrologAtom ();
			left -> setAtom (atom);
		} else {
			if (! left -> isAtom ()) return false;
			atom = left -> getAtom ();
		}
		array * a = new array (atom);
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			left = parameters -> getLeft ();
			if (! left -> isInteger ()) {
				delete a;
				return false;
			}
			a -> insert_dimension (left -> getInteger ());
			parameters = parameters -> getRight ();
		}
		a -> insert_elements ();
		if (atom -> setMachine (a)) return true;
		delete a;
		return false;
	}
};

/////////////////////
// NOISE GENERATOR //
/////////////////////

int PrologNoise :: get (void) {v = (v * a + c) & m; return v;}
int PrologNoise :: get (int min, int max) {
	float out = (float) get () * (float) (max - min);
	out /= (float) range;
	return min + (int) out;
}
void PrologNoise :: resolution (int bits) {range = 1 << bits; m = range - 1;}
void PrologNoise :: control (int v) {this -> v = v;}
void PrologNoise :: control (int a, int c) {this -> a = a; this -> c = c;}
PrologNoise :: PrologNoise (void) TRACKING (4) {resolution (24); control (0); control (0x5599d1, 1);}

class rnd : public PrologNativeCode {
public:
	PrologNoise * n;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) {parameters -> setInteger (n -> get ()); return true;}
		PrologElement * result = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {result -> setInteger (n -> get ()); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * min = parameters -> getLeft ();
		if (! min -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		PrologElement * max = parameters -> getLeft ();
		if (! max -> isInteger ()) return false;
		result -> setInteger (n -> get (min -> getInteger (), max -> getInteger ()));
		return true;
	}
	rnd (PrologNoise * n) {this -> n = n;}
};

class rnd_control : public PrologNativeCode {
public:
	PrologNoise * n;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		// rnd_control [out]
		// rnd_control [a c]
		// rnd_control [out a c]
		// rnd_control [out bits a c]
		if (! parameters -> isPair ()) return false;
		PrologElement * first_parameter = parameters -> getLeft ();
		if (! first_parameter -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {n -> control (first_parameter -> getInteger ()); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * second_parameter = parameters -> getLeft ();
		if (! second_parameter -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {n -> control (first_parameter -> getInteger (), second_parameter -> getInteger ()); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * third_parameter = parameters -> getLeft ();
		if (! third_parameter -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			n -> control (first_parameter -> getInteger ());
			n -> control (second_parameter -> getInteger (), third_parameter -> getInteger ());
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * fourth_parameter = parameters -> getLeft ();
		if (! fourth_parameter -> isInteger ()) return false;
		n -> resolution (second_parameter -> getInteger ());
		n -> control (first_parameter -> getInteger ());
		n -> control (third_parameter -> getInteger (), fourth_parameter -> getInteger ());
		return true;
	}
	rnd_control (PrologNoise * n) {this -> n = n;}
};

typedef PrologElement * PrologElementPointer;
class series : public PrologNativeCode {
public:
	PrologNoise * n;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e_root = parameters -> getLeft ();
		if (e_root -> isPair () || e_root -> isEarth ()) {
			PrologElement * ret = parameters -> getRight ();
			if (ret -> isPair ()) ret = ret -> getLeft ();
			int length = 0;
			while (e_root -> isPair ()) {
				e_root = e_root -> getRight ();
				length++;
			}
			if (length < 1) {ret -> setEarth (); return true;}
			PrologElementPointer * table = new PrologElementPointer [length + 32];
			length = 0;
			parameters = parameters -> getLeft ();
			while (parameters -> isPair ()) {
				table [length++] = parameters -> getLeft ();
				parameters = parameters -> getRight ();
			}
			while (length > 0) {
				int select = n -> get (0, length--);
				ret -> setPair ();
				ret -> setLeft (table [select] -> duplicate ());
				ret = ret -> getRight ();
				table [select] = table [length];
			}
			delete [] table;
			return true;
		}
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_length = parameters -> getLeft ();
		if (! e_length -> isInteger ()) return false;
		int length = e_length -> getInteger ();
		if (length < 1) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_step = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		double root;
		if (e_root -> isInteger ()) root = (double) e_root -> getInteger ();
		else {if (! e_root -> isDouble ()) return false; root = e_root -> getDouble ();}
		double step;
		if (e_step -> isInteger ()) step = (double) e_step -> getInteger ();
		else {if (! e_step -> isDouble ()) return false; step = e_step -> getDouble ();}
		double * table = new double [length + 32];
		for (int ind = 0; ind < length; ind++) {table [ind] = root; root += step;}
		int select;
		bool double_type = e_root -> isDouble () || e_step -> isDouble ();
		while (length > 0) {
			select = n -> get (0, length--);
			parameters -> setPair ();
			if (double_type) parameters -> getLeft () -> setDouble (table [select]);
			else parameters -> getLeft () -> setInteger ((int) table [select]);
			parameters = parameters -> getRight ();
			table [select] = table [length];
		}
		delete [] table;
		return true;
	}
	series (PrologNoise * n) {this -> n = n;}
};

/////////////
// CRACKER //
/////////////

class wait : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isVar ()) {parameters -> setInteger (root -> get_system_time ()); return true;}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setInteger (root -> get_system_time ()); return true;}
		if (! parameters -> isInteger ()) return false;
		root -> wait (parameters -> getInteger ());
		return true;
	}
	wait (PrologRoot * root) {this -> root = root;}
};

class opaque_wait : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologTransportPool * transport;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isVar ()) {parameters -> setInteger (transport -> getMiliseconds ()); return true;}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setInteger (transport -> getMiliseconds ()); return true;}
		if (! parameters -> isInteger ()) return false;
		int sentinel = transport -> getMiliseconds () + parameters -> getInteger ();
		while (sentinel > transport -> getMiliseconds ()) root -> wait (0);
		return true;
	}
	opaque_wait (PrologRoot * root, PrologTransportPool * transport) {
		this -> root = root;
		this -> transport = transport;
	}
};

class green_wait : public PrologNativeCode {
public:
	PrologTransportPool * transport;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (resolution -> callAgain ()) {
			if (parameters -> getInteger () > transport -> getMiliseconds ()) {resolution -> callAgain (parameters); return true;}
			parameters -> setVar (0);
			return true;
		}
		if (parameters -> isVar ()) {parameters -> setInteger (transport -> getMiliseconds ()); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * query = parameters;
		parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setInteger (transport -> getMiliseconds ()); return true;}
		if (! parameters -> isInteger ()) return false;
		query -> setInteger (parameters -> getInteger () + transport -> getMiliseconds ());
		resolution -> callAgain (query);
		return true;
	}
	green_wait (PrologTransportPool * transport) {this -> transport = transport;}
};

class timeout_class : public PrologNativeCode {
public:
	PrologRoot * root;
	// [timeout]               immediate (clears previous commands)
	// [timeout []]            clears everything
	// [timeout 127]           obvious (clears previous commands)
	// [timeout [sonda]]       immediate (clears previous commands)
	// [timeout 127 [sonda]]   obvious (clears previous commands)
	void change_query (PrologResolution * resolution, PrologElement * query) {
		PrologElement * original_query = resolution -> timeout_query;
		resolution -> timeout_query = query;
		if (original_query != NULL) delete original_query;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			// [timeout]               immediate (clears previous commands)
			change_query (resolution, NULL);
			resolution -> timeout = root -> get_system_time ();
			return true;
		}
		PrologElement * timeout = parameters -> getLeft ();
		if (timeout -> isEarth ()) {
			// [timeout []]            clears everything
			change_query (resolution, NULL);
			resolution -> timeout = 0;
			return true;
		}
		if (! timeout -> isInteger ()) {
			// [timeout [sonda]]       immediate (clears previous commands)
			change_query (resolution, parameters -> duplicate ());
			resolution -> timeout = root -> get_system_time ();
			return true;
		}
		parameters = parameters -> getRight ();
		// [timeout 127 [sonda]]   obvious (clears previous commands)
		change_query (resolution, parameters -> isEarth () ? NULL : parameters -> duplicate ());
		// [timeout 127]           obvious (clears previous commands)
		resolution -> timeout = root -> get_system_time () + timeout -> getInteger ();
		return true;
	}
	timeout_class (PrologRoot * root) {this -> root = root;}
};

class crack : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return true;
		root -> start (parameters);
		return true;
	}
	crack (PrologRoot * root) {this -> root = root;}
};

class green_crack : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologResolutionPool * pool;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return true;
		if (resolution -> callAgain ()) return true;
		pool -> insert (root -> pair (root -> head (NULL), parameters -> duplicate ()));
		resolution -> callAgain (parameters);
		return true;
	}
	green_crack (PrologRoot * root, PrologResolutionPool * pool) {
		this -> root = root;
		this -> pool = pool;
	}
};

class semaphore : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * wait_atom;
	PrologAtom * enter_atom;
	PrologAtom * signal_atom;
	void * system_semaphore;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isAtom ()) return false;
		PrologAtom * atom = parameters -> getAtom ();
		if (atom == signal_atom) {root -> signal_system_semaphore (system_semaphore); return true;}
		if (atom == wait_atom) {root -> wait_system_semaphore (system_semaphore); return true;}
		if (atom == enter_atom) {return root -> enter_system_semaphore (system_semaphore);}
		return false;
	}
	semaphore (PrologRoot * root, PrologAtom * atom, PrologAtom * wait_atom, PrologAtom * enter_atom, PrologAtom * signal_atom, int ind) {
		this -> root = root;
		this -> atom = atom;
		this -> wait_atom = wait_atom;
		this -> enter_atom = enter_atom;
		this -> signal_atom = signal_atom;
		system_semaphore = root -> create_system_semaphore (ind);
	}
	~ semaphore (void) {root -> destroy_system_semaphore (system_semaphore);}
};

class green_semaphore : public PrologNativeCode {
public:
	PrologAtom * atom;
	PrologAtom * wait_atom;
	PrologAtom * enter_atom;
	PrologAtom * signal_atom;
	int ind;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (resolution -> callAgain ()) {
			if (ind < 1) {resolution -> callAgain (parameters); return true;}
			ind--;
			return true;
		}
		PrologElement * query = parameters;
		if (parameters -> isEarth ()) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isAtom ()) return false;
		PrologAtom * atom = parameters -> getAtom ();
		if (atom == signal_atom) {ind++; return true;}
		if (atom == wait_atom) {
			if (ind > 0) {ind--; return true;}
			resolution -> callAgain (query);
			return true;
		}
		if (atom == enter_atom) {
			if (ind < 1) return false;
			ind--;
			return true;
		}
		return false;
	}
	green_semaphore (PrologAtom * atom, PrologAtom * wait_atom, PrologAtom * enter_atom, PrologAtom * signal_atom, int ind) {
		this -> atom = atom;
		this -> wait_atom = wait_atom;
		this -> enter_atom = enter_atom;
		this -> signal_atom = signal_atom;
		this -> ind = ind;
	}
};

class semaphore_maker : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologResolutionPool * pool;
	PrologAtom * wait_atom;
	PrologAtom * enter_atom;
	PrologAtom * signal_atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (wait_atom == NULL || enter_atom == NULL || signal_atom == NULL) return false;
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (ea -> isVar ()) {
			atom = new PrologAtom ();
			ea -> setAtom (atom);
		} else {
			if (! ea -> isAtom ()) return false;
			atom = ea -> getAtom ();
		}
		int ind = 1;
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) {
			parameters = parameters -> getLeft ();
			if (! parameters -> isInteger ()) return false;
			ind = parameters -> getInteger ();
			if (ind < 0) return false;
		}
		if (pool != NULL) {
			green_semaphore * s = new green_semaphore (atom, wait_atom, enter_atom, signal_atom, ind);
			if (atom -> setMachine (s)) return true;
			delete s;
			return false;
		}
		semaphore * s = new semaphore (root, atom, wait_atom, enter_atom, signal_atom, ind);
		if (atom -> setMachine (s)) return true;
		delete s;
		return false;
	}
	semaphore_maker (PrologRoot * root, PrologResolutionPool * pool) {
		this -> root = root;
		this -> pool = pool;
		PrologDirectory * dir = root -> searchDirectory ("studio");
		wait_atom = dir -> searchAtom ("wait");
		enter_atom = dir -> searchAtom ("enter");
		signal_atom = dir -> searchAtom ("signal");
	}
};

///////////////
// TRANSPORT //
///////////////

static bool green_wt_function (PrologElement * original, PrologElement * parameters, PrologResolution * resolution, PrologTransport * transport) {
	if (resolution -> callAgain ()) {
		if (! transport -> isActive ()) return false;
		if (parameters -> getInteger () > transport -> getTick ()) {resolution -> callAgain (original); return true;}
		original -> setVar (0);
		return true;
	}
	if (parameters -> isEarth ()) {parameters -> setInteger (transport -> getTick () + 1); resolution -> callAgain (original); return true;}
	if (parameters -> isVar ()) {parameters -> setInteger (transport -> getTick ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * el = parameters -> getLeft ();
	if (el -> isVar ()) {el -> setInteger (transport -> getTick ()); return true;}
	if (! el -> isInteger ()) return false;
	parameters -> setInteger (el -> getInteger () + transport -> getTick ());
	resolution -> callAgain (original);
	return true;
}

static bool wt_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) return transport -> wt (1);
	if (parameters -> isVar ()) {parameters -> setInteger (transport -> getTick ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (parameters -> isVar ()) {parameters -> setInteger (transport -> getTick ()); return true;}
	if (! parameters -> isInteger ()) return false;
	return transport -> wt (parameters -> getInteger ());
}

static bool green_beat_function (PrologElement * original, PrologElement * parameters, PrologResolution * resolution, PrologTransport * t) {
	if (resolution -> callAgain ()) {
		if (! t -> isActive ()) return false;
		if (parameters -> isDouble ()) {
			if ((int) parameters -> getDouble () > t -> getTick ()) {resolution -> callAgain (original); return true;}
			parameters -> setVar (0);
			return true;
		}
		if (parameters -> getInteger () > t -> getBeat ()) {resolution -> callAgain (original); return true;}
		parameters -> setVar (0);
		return true;
	}
	if (parameters -> isEarth ()) {parameters -> setInteger (t -> getBeat () + 1); resolution -> callAgain (original); return true;}
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBeat ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * el = parameters -> getLeft ();
	if (el -> isVar ()) {el -> setInteger (t -> getBeat ()); return true;}
	if (! el -> isInteger ()) {
		if (! el -> isDouble ()) return false;
		parameters -> setDouble (el -> getDouble () * (double) t -> get_ticks_per_beat () + (double) t -> getTick ());
		resolution -> callAgain (original);
		return true;
	}
	parameters -> setInteger (el -> getInteger () + t -> getBeat ());
	resolution -> callAgain (original);
	return true;
}

static bool beat_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) return t -> Beat (1);
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBeat ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBeat ()); return true;}
	if (! parameters -> isInteger ()) {
		if (! parameters -> isDouble ()) return false;
		return t -> wt ((int) ((double) t -> get_ticks_per_beat () * parameters -> getDouble ()));
	}
	return t -> Beat (parameters -> getInteger ());
}

static bool green_bar_function (PrologElement * original, PrologElement * parameters, PrologResolution * resolution, PrologTransport * t) {
	if (resolution -> callAgain ()) {
		if (! t -> isActive ()) return false;
		if (parameters -> getInteger () > t -> getBar ()) {resolution -> callAgain (original); return true;}
		parameters -> setVar (0);
		return true;
	}
	if (parameters -> isEarth ()) {parameters -> setInteger (t -> getBar () + 1); resolution -> callAgain (original); return true;}
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBar ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * el = parameters -> getLeft ();
	if (el -> isVar ()) {el -> setInteger (t -> getBar ()); return true;}
	if (! el -> isInteger ()) return false;
	parameters -> setInteger (el -> getInteger () + t -> getBar ());
	resolution -> callAgain (original);
	return true;
}

static bool bar_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) return t -> Bar (1);
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBar ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBar ()); return true;}
	if (! parameters -> isInteger ()) return false;
	return t -> Bar (parameters -> getInteger ());
}

static bool signal_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) {transport -> signal (); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	int sentinel = parameters -> getInteger ();
	for (int ind = 0; ind < sentinel; ind++) transport -> signal ();
	return true;
}

static bool signal_beat_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) {transport -> signal_beat (); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	int sentinel = parameters -> getInteger ();
	for (int ind = 0; ind < sentinel; ind++) transport -> signal_beat ();
	return true;
}

static bool signal_bar_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) {transport -> signal_bar (); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	int sentinel = parameters -> getInteger ();
	for (int ind = 0; ind < sentinel; ind++) transport -> signal_bar ();
	return true;
}

static bool tempo_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isVar ()) {
		int seconds = transport -> get_seconds ();
		if (seconds < 1) seconds = 1;
		parameters -> setInteger (transport -> get_beats_per_seconds () * 60 / seconds);
		return true;
	}
	if (! parameters -> isPair ()) return false;
	PrologElement * beats = parameters -> getLeft ();
	if (beats -> isVar ()) {
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			int seconds = transport -> get_seconds ();
			if (seconds < 1) seconds = 1;
			beats -> setInteger (transport -> get_beats_per_seconds () * 60 / seconds);
			return true;
		}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		beats -> setInteger (transport -> get_beats_per_seconds ());
		parameters -> setInteger (transport -> get_seconds ());
		return true;
	}
	if (! beats -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {transport -> tempo (beats -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	transport -> tempo (beats -> getInteger (), parameters -> getInteger ());
	return true;
}

static bool tempo_division_function (PrologElement * parameters, PrologTransport * t) {
	if (! parameters -> isPair ()) return false;
	PrologElement * beats = parameters -> getLeft ();
	if (beats -> isVar ()) {
		beats -> setInteger (t -> get_beats_per_bar ());
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) return true;
		parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		parameters -> setInteger (t -> get_ticks_per_beat ());
		return true;
	}
	if (! beats -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> division (beats -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	t -> division (beats -> getInteger (), parameters -> getInteger ());
	return true;
}

static bool atempo_function (PrologTransport * t) {t -> atempo (); return true;}

static bool accel_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) {t -> accelerando (); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * steps = parameters -> getLeft ();
	if (! steps -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> accelerando (steps -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * ticks = parameters -> getLeft ();
	if (! ticks -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> accelerando (steps -> getInteger (), ticks -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * sentinel = parameters -> getLeft ();
	if (! sentinel -> isInteger ()) return false;
	t -> accelerando (steps -> getInteger (), ticks -> getInteger (), sentinel -> getInteger ());
	return true;
}

static bool rit_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) {t -> ritardando (); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * steps = parameters -> getLeft ();
	if (! steps -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> ritardando (steps -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * ticks = parameters -> getLeft ();
	if (! ticks -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> ritardando (steps -> getInteger (), ticks -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * sentinel = parameters -> getLeft ();
	if (! sentinel -> isInteger ()) return false;
	t -> ritardando (steps -> getInteger (), ticks -> getInteger (), sentinel -> getInteger ());
	return true;
}

static bool metrum_function (PrologElement * parameters, PrologTransport * t) {
	if (! parameters -> isPair ()) return false;
	PrologElement * top = parameters -> getLeft ();
	if (top -> isVar ()) {
		top -> setInteger (t -> get_beats_per_bar ());
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		parameters -> setInteger (96 / t -> get_ticks_per_beat ());
		return true;
	}
	if (! top -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	t -> metrum (top -> getInteger (), parameters -> getInteger ());
	return true;
}

class green_start : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return t -> green_start ();}
	green_start (PrologTransport * t) {this -> t = t;}
};

class start : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {t -> delay = root -> native_threads_delay; return t -> start ();}
	start (PrologRoot * root, PrologTransport * t) {this -> root = root; this -> t = t;}
};

class pause : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return t -> pause ();}
	pause (PrologTransport * t) {this -> t = t;}
};

class stop : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return t -> stop ();}
	stop (PrologTransport * t) {this -> t = t;}
};

class green_wt : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return green_wt_function (parameters, parameters, resolution, t);}
	green_wt (PrologTransport * t) {this -> t = t;}
};

class wt : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return wt_function (parameters, t);}
	wt (PrologTransport * t) {this -> t = t;}
};

class green_beat : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return green_beat_function (parameters, parameters, resolution, t);}
	green_beat (PrologTransport * t) {this -> t = t;}
};

class beat : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return beat_function (parameters, t);}
	beat (PrologTransport * t) {this -> t = t;}
};

class green_bar : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return green_bar_function (parameters, parameters, resolution, t);}
	green_bar (PrologTransport * t) {this -> t = t;}
};

class bar : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return bar_function (parameters, t);}
	bar (PrologTransport * t) {this -> t = t;}
};

class signal : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return signal_function (parameters, t);}
	signal (PrologTransport * t) {this -> t = t;}
};

class signal_beat : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return signal_beat_function (parameters, t);}
	signal_beat (PrologTransport * t) {this -> t = t;}
};

class signal_bar : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return signal_bar_function (parameters, t);}
	signal_bar (PrologTransport * t) {this -> t = t;}
};

class reset : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {t -> reset (); return true;}
	reset (PrologTransport * t) {this -> t = t;}
};

class tempo : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return tempo_function (parameters, t);}
	tempo (PrologTransport * t) {this -> t = t;}
};

class tempo_division : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return tempo_division_function (parameters, t);}
	tempo_division (PrologTransport * t) {this -> t = t;}
};

class atempo : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return atempo_function (t);}
	atempo (PrologTransport * t) {this -> t = t;}
};

class accel : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return accel_function (parameters, t);}
	accel (PrologTransport * t) {this -> t = t;}
};

class rit : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return rit_function (parameters, t);}
	rit (PrologTransport * t) {this -> t = t;}
};

class metrum : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return metrum_function (parameters, t);}
	metrum (PrologTransport * t) {this -> t = t;}
};

///////////////
// CONDUCTOR //
///////////////

class conductor : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	bool green;
	PrologTransport * transport;
	PrologAtom * wt_atom;
	PrologAtom * beat_atom;
	PrologAtom * bar_atom;
	PrologAtom * signal_atom;
	PrologAtom * signal_beat_atom;
	PrologAtom * signal_bar_atom;
	PrologAtom * reset_atom;
	PrologAtom * tempo_atom;
	PrologAtom * atempo_atom;
	PrologAtom * accel_atom;
	PrologAtom * rit_atom;
	PrologAtom * metrum_atom;
	PrologAtom * division_atom;
	PrologAtom * start_atom;
	PrologAtom * pause_atom;
	PrologAtom * stop_atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> unProtect ();
			atom -> setMachine (NULL);
			atom -> unProtect ();
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * atom = parameters -> getLeft ();
		if (! atom -> isAtom ()) return false;
		PrologAtom * a = atom -> getAtom ();
		PrologElement * original = parameters;
		parameters = parameters -> getRight ();
		if (a == start_atom) {if (green) return transport -> green_start (); else return transport -> start ();}
		if (a == pause_atom) return transport -> pause ();
		if (a == stop_atom) return transport -> stop ();
		if (a == reset_atom) {transport -> reset (); return true;}
		if (a == signal_atom) return signal_function (parameters, transport);
		if (a == signal_beat_atom) return signal_beat_function (parameters, transport);
		if (a == signal_bar_atom) return signal_bar_function (parameters, transport);
		if (a == wt_atom) {if (green) return green_wt_function (original, parameters, resolution, transport); else return wt_function (parameters, transport);}
		if (a == beat_atom) {if (green) return green_beat_function (original, parameters, resolution, transport); else return beat_function (parameters, transport);}
		if (a == bar_atom) {if (green) return green_bar_function (original, parameters, resolution, transport); else return bar_function (parameters, transport);}
		if (a == tempo_atom) return tempo_function (parameters, transport);
		if (a == atempo_atom) return atempo_function (transport);
		if (a == accel_atom) return accel_function (parameters, transport);
		if (a == rit_atom) return rit_function (parameters, transport);
		if (a == division_atom) return tempo_division_function (parameters, transport);
		if (a == metrum_atom) return metrum_function (parameters, transport);
		return false;
	}
	conductor (PrologRoot * root, PrologAtom * atom, PrologTransport * transport, PrologAtom * wt_atom, PrologAtom * beat_atom, PrologAtom * bar_atom, PrologAtom * signal_atom, PrologAtom * signal_beat_atom, PrologAtom * signal_bar_atom, PrologAtom * reset_atom, PrologAtom * tempo_atom, PrologAtom * atempo_atom, PrologAtom * accel_atom, PrologAtom * rit_atom, PrologAtom * metrum_atom, PrologAtom * division_atom, PrologAtom * start_atom, PrologAtom * pause_atom, PrologAtom * stop_atom) {
		this -> root = root;
		this -> atom = atom;
		green = root -> getResolutionPool () != NULL;
		this -> transport = transport;
		this -> wt_atom = wt_atom;
		this -> beat_atom = beat_atom;
		this -> bar_atom = bar_atom;
		this -> signal_atom = signal_atom;
		this -> signal_beat_atom = signal_beat_atom;
		this -> signal_bar_atom = signal_bar_atom;
		this -> reset_atom = reset_atom;
		this -> tempo_atom = tempo_atom;
		this -> atempo_atom = atempo_atom;
		this -> accel_atom = accel_atom;
		this -> rit_atom = rit_atom;
		this -> metrum_atom = metrum_atom;
		this -> division_atom = division_atom;
		this -> start_atom = start_atom;
		this -> pause_atom = pause_atom;
		this -> stop_atom = stop_atom;
	}
	~ conductor (void) {root -> dropTransport (transport);}
};

class conductor_maker : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * wt_atom;
	PrologAtom * beat_atom;
	PrologAtom * bar_atom;
	PrologAtom * signal_atom;
	PrologAtom * signal_beat_atom;
	PrologAtom * signal_bar_atom;
	PrologAtom * reset_atom;
	PrologAtom * tempo_atom;
	PrologAtom * atempo_atom;
	PrologAtom * accel_atom;
	PrologAtom * rit_atom;
	PrologAtom * metrum_atom;
	PrologAtom * division_atom;
	PrologAtom * start_atom;
	PrologAtom * pause_atom;
	PrologAtom * stop_atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (wt_atom == NULL || beat_atom == NULL || bar_atom == NULL
			|| signal_atom == NULL || signal_beat_atom == NULL || signal_bar_atom == NULL
			|| reset_atom == NULL || tempo_atom == NULL || atempo_atom == NULL
			|| accel_atom == NULL || rit_atom == NULL
			|| metrum_atom == NULL || division_atom == NULL
			|| start_atom == NULL || pause_atom == NULL || stop_atom == NULL) return false;
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (ea -> isVar ()) {
			atom = new PrologAtom ();
			ea -> setAtom (atom);
		} else {
			if (! ea -> isAtom ()) return false;
			atom = ea -> getAtom ();
		}
		conductor * c = new conductor (root, atom, root -> insertTransport (), wt_atom, beat_atom, bar_atom, signal_atom, signal_beat_atom, signal_bar_atom, reset_atom, tempo_atom, atempo_atom, accel_atom, rit_atom, metrum_atom, division_atom, start_atom, pause_atom, stop_atom);
		if (atom -> setMachine (c)) return true;
		delete c;
		return false;
	}
	conductor_maker (PrologRoot * root) {
		this -> root = root;
		PrologDirectory * dir = root -> searchDirectory ("studio");
		wt_atom = dir -> searchAtom ("wt");
		beat_atom = dir -> searchAtom ("beat");
		bar_atom = dir -> searchAtom ("bar");
		signal_atom = dir -> searchAtom ("signal");
		signal_beat_atom = dir -> searchAtom ("signal_beat");
		signal_bar_atom = dir -> searchAtom ("signal_bar");
		reset_atom = dir -> searchAtom ("reset");
		tempo_atom = dir -> searchAtom ("tempo");
		atempo_atom = dir -> searchAtom ("atempo");
		accel_atom = dir -> searchAtom ("accel");
		rit_atom = dir -> searchAtom ("rit");
		metrum_atom = dir -> searchAtom ("metrum");
		division_atom = dir -> searchAtom ("division");
		start_atom = dir -> searchAtom ("start");
		pause_atom = dir -> searchAtom ("pause");
		stop_atom = dir -> searchAtom ("stop");
	}
};

////////////////
// MIDI Ports //
////////////////

class midi_in_info : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		int inputs = service -> getNumberOfInputs ();
		int active = service -> getInputPort ();
		PROLOG_STRING command;
		bool exporting;
		if (parameters -> isEarth ()) {
			sprintf (command, "MIDI Inputs [%i]\n", inputs);
			root -> print (command);
			exporting = false;
		} else exporting = true;
		parameters -> setEarth ();
		for (int ind = 0; ind < inputs; ind++) {
			if (exporting) {
				sprintf (command, "%s%s", service -> getInputInfo (ind), active == ind ? " [ACTIVE]" : "");
				parameters -> setPair ();
				parameters -> getLeft () -> setText (command);
				parameters = parameters -> getRight ();
			} else {
				sprintf (command, "	%i %s=> %s\n", ind, active == ind ? "[ACTIVE] " : "", service -> getInputInfo (ind));
				root -> print (command);
			}
		}
		return true;
	}
	midi_in_info (PrologRoot * root) {this -> root = root;}
};

class midi_out_info : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		int outputs = service -> getNumberOfOutputs ();
		int active = service -> getOutputPort ();
		PROLOG_STRING command;
		bool exporting;
		if (parameters -> isEarth ()) {
			sprintf (command, "MIDI Outputs [%i]\n", outputs);
			root -> print (command);
			exporting = false;
		} else exporting = true;
		parameters -> setEarth ();
		for (int ind = 0; ind < outputs; ind++) {
			if (exporting) {
				sprintf (command, "%s%s", service -> getOutputInfo (ind), active == ind ? " [ACTIVE]" : "");
				parameters -> setPair ();
				parameters -> getLeft () -> setText (command);
				parameters = parameters -> getRight ();
			} else {
				sprintf (command, "	%i %s=> %s\n", ind, active == ind ? "[ACTIVE] " : "", service -> getOutputInfo (ind));
				root -> print (command);
			}
		}
		return true;
	}
	midi_out_info (PrologRoot * root) {this -> root = root;}
};

class midi_in_port : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isInteger ()) return false;
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		return service -> setInputPort (parameters -> getInteger ());
	}
	midi_in_port (PrologRoot * root) {this -> root = root;}
};

class midi_out_port : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isInteger ()) return false;
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		return service -> setOutputPort (parameters -> getInteger ());
	}
	midi_out_port (PrologRoot * root) {this -> root = root;}
};

//////////
// MIDI //
//////////

class void_midi_message : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> print ("MIDI interfaces not connected.");
		root -> print (root -> new_line_caption);
		return true;
	}
	void_midi_message (PrologRoot * root) {this -> root = root;}
};

static void * midi_semaphore = NULL;
static PrologRoot * midi_root = NULL;
void midi_wait (void) {if (midi_root == NULL) return; midi_root -> wait_system_semaphore (midi_semaphore);}
void midi_signal (void) {if (midi_root == NULL) return; midi_root -> signal_system_semaphore (midi_semaphore);}

class midi_short_msg : public PrologNativeCode {
public:
	midi_stream * line;
	PrologStudio * studio;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		int msg, channel, msb, lsb;
		PrologElement * el;
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		msg = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			midi_wait ();
			line -> insert (msg);
			midi_signal ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		channel = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (el -> isInteger ()) msb = el -> getInteger ();
		else if (el -> isPair ()) {
			PrologElement * note = el -> getLeft ();
			el = el -> getRight ();
			if (! el -> isPair ()) return false;
			PrologElement * octave = el -> getLeft ();
			if (! note -> isAtom ()) return false;
			if (! octave -> isInteger ()) return false;
			msb = 48 + octave -> getInteger () * 12 + studio -> chromatic (note -> getAtom ());
			if (msb < 0 || msb > 127) return false;
		} else return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			midi_wait ();
			line -> insert (msg + line -> chex (channel), msb);
			midi_signal ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		lsb = el -> getInteger ();
		midi_wait ();
		line -> insert (msg + line -> chex (channel), msb, lsb);
		midi_signal ();
		return true;
	}
	midi_short_msg (midi_stream * line, PrologStudio * studio) {this -> line = line; this -> studio = studio;}
};

class MIDI_SHORT_MSG : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		int msg, channel, ctrl, msb, lsb;
		PrologElement * el;
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		msg = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		channel = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		ctrl = el -> getInteger ();
		if (ctrl >= 96) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		msb = el -> getInteger ();
		msb &= 0x3fff;
		lsb = msb & 0x7f;
		msb >>= 7;
		midi_wait ();
		msg += line -> chex (channel);
		line -> insert (msg, ctrl + 32, lsb);
		line -> insert (msg, ctrl, msb);
		midi_signal ();
		return true;
	}
	MIDI_SHORT_MSG (midi_stream * line) {this -> line = line;}
};

class midi_long_msg : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * el;
		midi_wait ();
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger ()) return false;
			line -> insert (el -> getInteger ());
			parameters = parameters -> getRight ();
		}
		line -> close_message ();
		midi_signal ();
		return true;
	}
	midi_long_msg (midi_stream * line) {this -> line = line;}
};

class sysex : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {line -> close_system_exclusive (); midi_signal (); return false;}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	sysex (midi_stream * line) {this -> line = line;}
};

class SYSEX : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_generic_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {line -> close_system_exclusive (); midi_signal (); return false;}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	SYSEX (midi_stream * line) {this -> line = line;}
};

class sysexch : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {
				line -> insert_checksum ();
				line -> close_system_exclusive ();
				midi_signal ();
				return false;
			}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> insert_checksum ();
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	sysexch (midi_stream * line) {this -> line = line;}
};

class SYSEXCH : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_generic_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {
				line -> insert_checksum ();
				line -> close_system_exclusive ();
				midi_signal ();
				return false;
			}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> insert_checksum ();
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	SYSEXCH (midi_stream * line) {this -> line = line;}
};

class midi_manufacturers_id : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_stream * line_in = root -> getMidiInput ();
		midi_stream * line_out = root -> getMidiOutput ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (parameters -> isEarth ()) {
			if (line_in != NULL) line_in -> set_manufacturers_id ();
			if (line_out != NULL) line_out -> set_manufacturers_id ();
			if (service != NULL) service -> changeManufacturersId ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * m1 = parameters -> getLeft ();
		if (! m1 -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			if (line_in != NULL) line_in -> set_manufacturers_id (m1 -> getInteger ());
			if (line_out != NULL) line_out -> set_manufacturers_id (m1 -> getInteger ());
			if (service != NULL) service -> changeManufacturersId (m1 -> getInteger ());
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * m2 = parameters -> getLeft ();
		if (! m2 -> isInteger ()) return false;
		if (line_in != NULL) line_in -> set_manufacturers_id (m1 -> getInteger (), m2 -> getInteger ());
		if (line_out != NULL) line_out -> set_manufacturers_id (m1 -> getInteger (), m2 -> getInteger ());
		if (service != NULL) service -> changeManufacturersId (m1 -> getInteger (), m2 -> getInteger ());
		return true;
	}
	midi_manufacturers_id (PrologRoot * root) {this -> root = root;}
};

class midi_product_id : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_stream * line_in = root -> getMidiInput ();
		midi_stream * line_out = root -> getMidiOutput ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		char id1 = -1, id2 = -1, id3 = -1, id4 = -1;
		if (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (! e -> isInteger ()) return false;
			id1 = (char) e -> getInteger ();
			parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				e = parameters -> getLeft ();
				if (! e -> isInteger ()) return false;
				id2 = (char) e -> getInteger ();
				parameters = parameters -> getRight ();
				if (parameters -> isPair ()) {
					e = parameters -> getLeft ();
					if (! e -> isInteger ()) return false;
					id3 = (char) e -> getInteger ();
					parameters = parameters -> getRight ();
					if (parameters -> isPair ()) {
						e = parameters -> getLeft ();
						if (! e -> isInteger ()) return false;
						id4 = (char) e -> getInteger ();
					}
				}
			}
		}
		if (line_in != NULL) line_in -> set_product_id (id1, id2, id3, id4);
		if (line_out != NULL) line_out -> set_product_id (id1, id2, id3, id4);
		if (service != NULL) service -> changeProductId (id1, id2, id3, id4);
		return true;
	}
	midi_product_id (PrologRoot * root) {this -> root = root;}
};

class midi_product_version : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_stream * line_in = root -> getMidiInput ();
		midi_stream * line_out = root -> getMidiOutput ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		char id1 = -1, id2 = -1, id3 = -1, id4 = -1;
		if (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (! e -> isInteger ()) return false;
			id1 = (char) e -> getInteger ();
			parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				e = parameters -> getLeft ();
				if (! e -> isInteger ()) return false;
				id2 = (char) e -> getInteger ();
				parameters = parameters -> getRight ();
				if (parameters -> isPair ()) {
					e = parameters -> getLeft ();
					if (! e -> isInteger ()) return false;
					id3 = (char) e -> getInteger ();
					parameters = parameters -> getRight ();
					if (parameters -> isPair ()) {
						e = parameters -> getLeft ();
						if (! e -> isInteger ()) return false;
						id4 = (char) e -> getInteger ();
					}
				}
			}
		}
		if (line_in != NULL) line_in -> set_product_version (id1, id2, id3, id4);
		if (line_out != NULL) line_out -> set_product_version (id1, id2, id3, id4);
		if (service != NULL) service -> changeProductVersion (id1, id2, id3, id4);
		return true;
		return true;
	}
	midi_product_version (PrologRoot * root) {this -> root = root;}
};

void prolog_midi_reader :: call (PrologElement * query) {
	query = root -> pair (root -> atom (income_midi_atom), query);
	query = root -> pair (query, root -> earth ());
	query = root -> pair (root -> head (NULL), query);
	PrologResolutionPool * pool = root -> getResolutionPool ();
	if (pool) pool -> insert (query);
	else {root -> resolution (query); delete query;}
}
PrologElement * prolog_midi_reader :: build_call (PrologAtom * atom, int channel, int ind) {
	return root -> pair (root -> atom (atom), root -> pair (root -> integer (channel + (midi_channel_extension != 0x7f ? midi_channel_extension << 4 : 0)), root -> pair (root -> integer (ind), root -> earth ())));
}
PrologElement * prolog_midi_reader :: build_call (PrologAtom * atom, int channel, int ind, int sub) {
	return root -> pair (root -> atom (atom), root -> pair (root -> integer (channel + (midi_channel_extension != 0x7f ? midi_channel_extension << 4 : 0)), root -> pair (root -> integer (ind), root -> pair (root -> integer (sub), root -> earth ()))));
}
void prolog_midi_reader :: midi_keyoff (int channel, int key) {call (build_call (keyoff_atom, channel, key));}
void prolog_midi_reader :: midi_keyon (int channel, int key, int velocity) {
	call (build_call (keyon_atom, channel, key, velocity));
}
void prolog_midi_reader :: midi_pat (int channel, int key, int value) {call (build_call (polyaftertouch_atom, channel, key, value));}
void prolog_midi_reader :: midi_control (int channel, int controller, int value) {call (build_call (control_atom, channel, controller, value));}
void prolog_midi_reader :: midi_programchange (int channel, int program) {call (build_call (programchange_atom, channel, program));}
void prolog_midi_reader :: midi_cat (int channel, int value) {call (build_call (aftertouch_atom, channel, value));}
void prolog_midi_reader :: midi_pitchbend (int channel, int v1, int v2) {call (build_call (pitch_atom, channel, v1, v2));}
void prolog_midi_reader :: midi_system_exclusive (midi_stream * line) {
	// prolog command sysex filter
	line -> mark ();
	PrologAtom * atom = SYSEX_atom;
	if (line -> check_system_exclusive ()) atom = sysex_atom;
	else line -> restore ();
	PrologElement * query = root -> pair (root -> atom (atom), root -> earth ());
	PrologElement * el = query -> getRight ();
	int ind = line -> get ();
	while (ind < 247) {
		el -> setPair (root -> integer (ind), root -> earth ());
		el = el -> getRight ();
		ind = line -> get ();
	}
	call (query);
}
void prolog_midi_reader :: midi_timing_clock (void) {call (root -> pair (root -> atom (timingclock_atom), root -> earth ()));}
void prolog_midi_reader :: midi_start (void) {call (root -> pair (root -> atom (start_atom), root -> earth ()));}
void prolog_midi_reader :: midi_continue (void) {call (root -> pair (root -> atom (continue_atom), root -> earth ()));}
void prolog_midi_reader :: midi_stop (void) {call (root -> pair (root -> atom (stop_atom), root -> earth ()));}
void prolog_midi_reader :: midi_active_sensing (void) {call (root -> pair (root -> atom (activesensing_atom), root -> earth ()));}
bool prolog_midi_reader :: is_ready (void) {
	if (studio_dir != NULL) return true;
	studio_dir = root -> searchDirectory ("studio");
	if (studio_dir == NULL) return false;
	income_midi_atom = studio_dir -> searchAtom ("income_midi");
	keyoff_atom = studio_dir -> searchAtom ("keyoff");
	keyon_atom = studio_dir -> searchAtom ("keyon");
	polyaftertouch_atom = studio_dir -> searchAtom ("polyaftertouch");
	control_atom = studio_dir -> searchAtom ("control");
	programchange_atom = studio_dir -> searchAtom ("programchange");
	aftertouch_atom = studio_dir -> searchAtom ("aftertouch");
	pitch_atom = studio_dir -> searchAtom ("pitch");
	sysex_atom = studio_dir -> searchAtom ("sysex");
	SYSEX_atom = studio_dir -> searchAtom ("SYSEX");
	timingclock_atom = studio_dir -> searchAtom ("timingclock");
	start_atom = studio_dir -> searchAtom ("START");
	continue_atom = studio_dir -> searchAtom ("CONTINUE");
	stop_atom = studio_dir -> searchAtom ("STOP");
	activesensing_atom = studio_dir -> searchAtom ("activesensing");
	return true;
}
prolog_midi_reader :: prolog_midi_reader (PrologRoot * root) {
	this -> root = root;
	studio_dir = NULL;
	income_midi_atom = NULL;
	keyoff_atom = NULL;
	keyon_atom = NULL;
	polyaftertouch_atom = NULL;
	control_atom = NULL;
	programchange_atom = NULL;
	aftertouch_atom = NULL;
	pitch_atom = NULL;
	sysex_atom = NULL;
	SYSEX_atom = NULL;
	timingclock_atom = NULL;
	start_atom = NULL;
	continue_atom = NULL;
	stop_atom = NULL;
	activesensing_atom = NULL;
	is_ready ();
}

class interval_processor : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologStudio * studio;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * left_note = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * right_note = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * interval = parameters -> getLeft ();
		if (left_note -> isPair ()) {
			PrologElement * note_left = left_note -> getLeft ();
			if (! note_left -> isAtom ()) return false;
			int diatonic_left = studio -> diatonic (note_left -> getAtom ());
			if (diatonic_left < 0) return false;
			int chromatic_left = studio -> chromatic (note_left -> getAtom ());
			left_note = left_note -> getRight ();
			if (! left_note -> isPair ()) return false;
			PrologElement * octave_left = left_note -> getLeft ();
			if (! octave_left -> isInteger ()) return false;
			if (right_note -> isPair ()) {
				diatonic_left += 28 + octave_left -> getInteger () * 7;
				chromatic_left += 48 + octave_left -> getInteger () * 12;
				PrologElement * note_right = right_note -> getLeft ();
				if (! note_right -> isAtom ()) return false;
				int diatonic_right = studio -> diatonic (note_right -> getAtom ());
				if (diatonic_right < 0) return false;
				int chromatic_right = studio -> chromatic (note_right -> getAtom ());
				right_note = right_note -> getRight ();
				if (! right_note -> isPair ()) return false;
				PrologElement * octave_right = right_note -> getLeft ();
				if (! octave_right -> isInteger ()) return false;
				diatonic_right += 28 + octave_right -> getInteger () * 7;
				chromatic_right += 48 + octave_right -> getInteger () * 12;
				interval -> setPair (root -> integer (diatonic_right - diatonic_left), root -> pair (root -> integer (chromatic_right - chromatic_left), root -> earth ()));
				return true;
			}
			if (interval -> isPair ()) {
				PrologElement * diatonic_interval = interval -> getLeft ();
				if (! diatonic_interval -> isInteger ()) return false;
				interval = interval -> getRight ();
				if (! interval -> isPair ()) return false;
				PrologElement * chromatic_interval = interval -> getLeft ();
				if (! chromatic_interval -> isInteger ()) return false;
				int diatonic_right = diatonic_left + diatonic_interval -> getInteger ();
				int chromatic_right = chromatic_left + chromatic_interval -> getInteger ();
				int octave_right = octave_left -> getInteger ();
				while (diatonic_right < 0) {diatonic_right += 7; chromatic_right += 12; octave_right--;}
				while (diatonic_right > 6) {diatonic_right -= 7; chromatic_right -= 12; octave_right++;}
				PrologAtom * ret = studio -> note (diatonic_right, chromatic_right);
				if (ret == NULL) return false;
				right_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_right), root -> earth ()));
				return true;
			}
			return false;
		}
		if (! right_note -> isPair ()) return false;
		PrologElement * note_right = right_note -> getLeft ();
		if (! note_right -> isAtom ()) return false;
		int diatonic_right = studio -> diatonic (note_right -> getAtom ());
		if (diatonic_right < 0) return false;
		int chromatic_right = studio -> chromatic (note_right -> getAtom ());
		right_note = right_note -> getRight ();
		if (! right_note -> isPair ()) return false;
		PrologElement * octave_right = right_note -> getLeft ();
		if (! octave_right -> isInteger ()) return false;
		if (! interval -> isPair ()) return false;
		PrologElement * diatonic_interval = interval -> getLeft ();
		if (! diatonic_interval -> isInteger ()) return false;
		interval = interval -> getRight ();
		if (! interval -> isPair ()) return false;
		PrologElement * chromatic_interval = interval -> getLeft ();
		if (! chromatic_interval -> isInteger ()) return false;
		int diatonic_left = diatonic_right - diatonic_interval -> getInteger ();
		int chromatic_left = chromatic_right - chromatic_interval -> getInteger ();
		int octave_left = octave_right -> getInteger ();
		while (diatonic_left < 0) {diatonic_left += 7; chromatic_left += 12; octave_left--;}
		while (diatonic_left > 6) {diatonic_left -= 7; chromatic_left -= 12; octave_left++;}
		PrologAtom * ret = studio -> note (diatonic_left, chromatic_left);
		if (ret == NULL) return false;
		left_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_left), root -> earth ()));
		return true;
	}
	interval_processor (PrologRoot * root, PrologStudio * studio) {this -> root = root; this -> studio = studio;}
};

////////////////////////
// console procedures //
////////////////////////

class set_colors : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * foreground = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (foreground -> isInteger () && parameters -> isInteger ()) {
			root -> setColors (foreground -> getInteger (), parameters -> getInteger ());
			return true;
		}
		foreground -> setInteger (root -> current_foreground);
		parameters -> setInteger (root -> current_background);
		return true;
	}
	set_colors (PrologRoot * root) {this -> root = root;}
};

class open_editor : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			root -> openEditor ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * selector = parameters -> getLeft ();
		if (! selector -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			root -> openEditor (selector -> getInteger ());
			return true;
		}
		PrologElement * sub_selector = parameters -> getLeft ();
		if (! sub_selector -> isInteger ()) return false;
		root -> openEditor (selector -> getInteger (), sub_selector -> getInteger ());
		return true;
	}
	open_editor (PrologRoot * root) {this -> root = root;}
};

class close_editor : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			root -> closeEditor ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * selector = parameters -> getLeft ();
		if (! selector -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			root -> closeEditor (selector -> getInteger ());
			return true;
		}
		PrologElement * sub_selector = parameters -> getLeft ();
		if (! sub_selector -> isInteger ()) return false;
		root -> closeEditor (selector -> getInteger (), sub_selector -> getInteger ());
		return true;
	}
	close_editor (PrologRoot * root) {this -> root = root;}
};

class screen_coordinates : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * x = parameters -> getLeft ();
		if (! x -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isInteger ()) return false;
		root -> setScreenCoordinates (x -> getInteger (), parameters -> getInteger ());
		return true;
	}
	screen_coordinates (PrologRoot * root) {this -> root = root;}
};

///////////////////////////
// SERIAL NUMBER SERVICE //
///////////////////////////

#include "encoder.h"

class get_volume_serial_number : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		char serial_number [64];
		encoder e;
		e . volumize_serial (serial_number, root -> serial_number, root -> volume_id);
		parameters -> setPair (root -> text (serial_number), root -> earth ());
		return true;
	}
	get_volume_serial_number (PrologRoot * root) {this -> root = root;}
};

class security_check : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		encoder e;
		return e . check_serial (root -> serial_number, root -> volume_id, root -> key, root -> serial_shift);
	}
	security_check (PrologRoot * root) {this -> root = root;}
};

///////////////////
// SERVICE CLASS //
///////////////////

int PrologStudio :: diatonic (PrologAtom * atom) {
	if (atom == c || atom == cb || atom == cbb || atom == cx || atom == cxx) return 0;
	if (atom == d || atom == db || atom == dbb || atom == dx || atom == dxx) return 1;
	if (atom == e || atom == eb || atom == ebb || atom == ex || atom == exx) return 2;
	if (atom == f || atom == fb || atom == fbb || atom == fx || atom == fxx) return 3;
	if (atom == g || atom == gb || atom == gbb || atom == gx || atom == gxx) return 4;
	if (atom == a || atom == ab || atom == abb || atom == ax || atom == axx) return 5;
	if (atom == b || atom == bb || atom == bbb || atom == bx || atom == bxx) return 6;
	return -127;
}

int PrologStudio :: chromatic (PrologAtom * atom) {
	if (atom == cbb) return -2;
	if (atom == cb) return -1;
	if (atom == c || atom == dbb) return 0;
	if (atom == cx || atom == db) return 1;
	if (atom == cxx || atom == d || atom == ebb) return 2;
	if (atom == dx || atom == eb || atom == fbb) return 3;
	if (atom == dxx || atom == e || atom == fb) return 4;
	if (atom == ex || atom == f || atom == gbb) return 5;
	if (atom == exx || atom == fx || atom == gb) return 6;
	if (atom == fxx || atom == g || atom == abb) return 7;
	if (atom == gx || atom == ab) return 8;
	if (atom == gxx || atom == a || atom == bbb) return 9;
	if (atom == ax || atom == bb || atom == cbb) return 10;
	if (atom == axx || atom == b || atom == cb) return 11;
	if (atom == bx) return 12;
	if (atom == bxx) return 13;
	return 0;
}

PrologAtom * PrologStudio :: note (int diatonic, int chromatic) {
	switch (diatonic) {
	case 0:
		switch (chromatic) {
		case -2: return cbb;
		case -1: return cb;
		case 0: return c;
		case 1: return cx;
		case 2: return cxx;
		default: return NULL;
		} return NULL;
	case 1:
		switch (chromatic) {
		case 0: return dbb;
		case 1: return db;
		case 2: return d;
		case 3: return dx;
		case 4: return dxx;
		default: return NULL;
		} return NULL;
	case 2:
		switch (chromatic) {
		case 2: return ebb;
		case 3: return eb;
		case 4: return e;
		case 5: return ex;
		case 6: return exx;
		default: return NULL;
		} return NULL;
	case 3:
		switch (chromatic) {
		case 3: return fbb;
		case 4: return fb;
		case 5: return f;
		case 6: return fx;
		case 7: return fxx;
		default: return NULL;
		} return NULL;
	case 4:
		switch (chromatic) {
		case 5: return gbb;
		case 6: return gb;
		case 7: return g;
		case 8: return gx;
		case 9: return gxx;
		default: return NULL;
		} return NULL;
	case 5:
		switch (chromatic) {
		case 7: return abb;
		case 8: return ab;
		case 9: return a;
		case 10: return ax;
		case 11: return axx;
		default: return NULL;
		} return NULL;
	case 6:
		switch (chromatic) {
		case 9: return bbb;
		case 10: return bb;
		case 11: return b;
		case 12: return bx;
		case 13: return bxx;
		default: return NULL;
		} return NULL;
	default: return NULL;
	}
	return NULL;
}

void PrologStudio :: init (PrologRoot * root) {
	this -> root = root;
	n = new PrologNoise ();
	stdr = new standard_in_reader (root);
	transport = root -> getTransportPool ();
	t = root -> getRootTransport ();
	if (t == NULL) t = root -> insertTransport ();
	pool = root -> getResolutionPool ();
	c = cb = cbb = cx = cxx = NULL;
	d = db = dbb = dx = dxx = NULL;
	e = eb = ebb = ex = exx = NULL;
	f = fb = fbb = fx = fxx = NULL;
	g = gb = gbb = gx = gxx = NULL;
	a = ab = abb = ax = axx = NULL;
	b = bb = bbb = bx = bxx = NULL;
}

void PrologStudio :: set_atoms (void) {
	if (dir != NULL) return;
	dir = root -> searchDirectory ("studio");
	if (dir == NULL) return;
	c = dir -> searchAtom ("C"); cb = dir -> searchAtom ("Cb"); cbb = dir -> searchAtom ("Cbb"); cx = dir -> searchAtom ("C#"); cxx = dir -> searchAtom ("Cx");
	d = dir -> searchAtom ("D"); db = dir -> searchAtom ("Db"); dbb = dir -> searchAtom ("Dbb"); dx = dir -> searchAtom ("D#"); dxx = dir -> searchAtom ("Dx");
	e = dir -> searchAtom ("E"); eb = dir -> searchAtom ("Eb"); ebb = dir -> searchAtom ("Ebb"); ex = dir -> searchAtom ("E#"); exx = dir -> searchAtom ("Ex");
	f = dir -> searchAtom ("F"); fb = dir -> searchAtom ("Fb"); fbb = dir -> searchAtom ("Fbb"); fx = dir -> searchAtom ("F#"); fxx = dir -> searchAtom ("Fx");
	g = dir -> searchAtom ("G"); gb = dir -> searchAtom ("Gb"); gbb = dir -> searchAtom ("Gbb"); gx = dir -> searchAtom ("G#"); gxx = dir -> searchAtom ("Gx");
	a = dir -> searchAtom ("A"); ab = dir -> searchAtom ("Ab"); abb = dir -> searchAtom ("Abb"); ax = dir -> searchAtom ("A#"); axx = dir -> searchAtom ("Ax");
	b = dir -> searchAtom ("B"); bb = dir -> searchAtom ("Bb"); bbb = dir -> searchAtom ("Bbb"); bx = dir -> searchAtom ("B#"); bxx = dir -> searchAtom ("Bx");
}

PrologStudio :: ~ PrologStudio (void) {
	if (transport == NULL) delete t;
	delete n;
	delete stdr;
	if (midi_semaphore != NULL) midi_root -> destroy_system_semaphore (midi_semaphore);
	midi_semaphore = NULL;
}

PrologNativeCode * PrologStudio :: getNativeCode (char * name) {
	set_atoms ();
	if (strcmp (name, "sum") == 0) return new sum (root);
	if (strcmp (name, "add_strings") == 0) return new add_strings (root);
	if (strcmp (name, "add") == 0) return new add (root);
	if (strcmp (name, "sub") == 0) return new sub (root);
	if (strcmp (name, "times") == 0) return new times (root);
	if (strcmp (name, "mult") == 0) return new mult (root);
	if (strcmp (name, "div") == 0) return new division (root);
	if (strcmp (name, "mod") == 0) return new mod (root);
	if (strcmp (name, "and") == 0) return new logical_and (root);
	if (strcmp (name, "or") == 0) return new logical_or (root);
	if (strcmp (name, "xor") == 0) return new logical_xor (root);

	if (strcmp (name, "abs") == 0) return new abs_operation ();
	if (strcmp (name, "cos") == 0) return new cos_operation ();
	if (strcmp (name, "degrad") == 0) return new degrad_operation ();
	if (strcmp (name, "e") == 0) return new e_operation ();
	if (strcmp (name, "exp") == 0) return new exp_operation ();
	if (strcmp (name, "log2") == 0) return new log2_operation ();
	if (strcmp (name, "log10") == 0) return new log10_operation ();
	if (strcmp (name, "ln") == 0) return new ln_operation ();
	if (strcmp (name, "log") == 0) return new log_operation ();
	if (strcmp (name, "pi") == 0) return new pi_operation ();
	if (strcmp (name, "pow") == 0) return new pow_operation ();
	if (strcmp (name, "sin") == 0) return new sin_operation ();
	if (strcmp (name, "tan") == 0) return new tan_operation ();
	if (strcmp (name, "trunc") == 0) return new trunc_operation ();

	if (strcmp (name, "timestamp") == 0) return new timestamp ();

	if (strcmp (name, "is_atom") == 0) return new is_atom (root);
	if (strcmp (name, "is_integer") == 0) return new is_integer (root);
	if (strcmp (name, "is_double") == 0) return new is_double (root);
	if (strcmp (name, "is_number") == 0) return new is_number (root);
	if (strcmp (name, "is_var") == 0) return new is_var (root);
	if (strcmp (name, "is_head") == 0) return new is_head (root);
	if (strcmp (name, "is_text") == 0) return new is_text (root);
	if (strcmp (name, "has_machine") == 0) return new has_machine (root);
	if (strcmp (name, "text_list") == 0) return new text_list (root);
	if (strcmp (name, "text_term") == 0) return new text_term (root);
	if (strcmp (name, "e32") == 0) return new e32 (root);
	if (strcmp (name, "less") == 0) return new less (root);
	if (strcmp (name, "less_eq") == 0) return new less_eq (root);
	if (strcmp (name, "greater") == 0) return new greater (root);
	if (strcmp (name, "greater_eq") == 0) return new greater_eq (root);
	if (strcmp (name, "set_uap32_captions") == 0) return new set_uap32_captions (root);
	if (strcmp (name, "set_standard_captions") == 0) return new set_standard_captions (root);
	if (strcmp (name, "set_edinburg_captions") == 0) return new set_edinburg_captions (root);
	if (strcmp (name, "set_marseille_captions") == 0) return new set_marseille_captions (root);
	if (strcmp (name, "set_functional_captions") == 0) return new set_functional_captions (root);
	if (strcmp (name, "auto_atoms") == 0) return new auto_atoms (root);
	if (strcmp (name, "scripted_atoms") == 0) return new scripted_atoms (root);
	if (strcmp (name, "pr") == 0) return new pr (stdr);
	if (strcmp (name, "read") == 0) return new read (stdr);
	if (strcmp (name, "readln") == 0) return new readln (stdr);
	if (strcmp (name, "pp") == 0) return new pp (root);
	if (strcmp (name, "pt") == 0) return new pt (root);
	if (strcmp (name, "write") == 0) return new write (root);
	if (strcmp (name, "nl") == 0) return new nl (root);
	if (strcmp (name, "list") == 0) return new list (root);
	if (strcmp (name, "CL") == 0) return new CL (root);
	if (strcmp (name, "addcl") == 0) return new addcl (root);
	if (strcmp (name, "addcl0") == 0) return new addcl0 (root);
	if (strcmp (name, "delcl") == 0) return new delcl (root);
	if (strcmp (name, "overwrite") == 0) return new overwrite (root);
	if (strcmp (name, "create_atom") == 0) return new create_atom (root);
	if (strcmp (name, "search_atom") == 0) return new search_atom (root);
	if (strcmp (name, "preprocessor") == 0) return new preprocessor (root);
	if (strcmp (name, "prompt") == 0) return new prompt (root);
	if (strcmp (name, "query_stack") == 0) return new query_stack (root);
	if (strcmp (name, "object_counter") == 0) return new object_counter_class ();
	if (strcmp (name, "crack") == 0) {
		if (pool == NULL) return new crack (root);
		return new green_crack (root, pool);
	}
	if (strcmp (name, "wait") == 0) {
		if (transport == NULL) return new wait (root);
		if (pool != NULL) return new green_wait (transport);
		return new opaque_wait (root, transport);
	}
	if (strcmp (name, "timeout") == 0) return new timeout_class (root);
	if (strcmp (name, "semaphore") == 0) return new semaphore_maker (root, pool);
	if (strcmp (name, "conductor") == 0) return new conductor_maker (root);
	if (strcmp (name, "file_writer") == 0) return new file_writer (root);
	if (strcmp (name, "file_reader") == 0) return new file_reader (root);
	if (strcmp (name, "import_loader") == 0) return new import_loader (root);
	if (strcmp (name, "load_loader") == 0) return new load_loader (root);
	if (strcmp (name, "consult_loader") == 0) return new consult_loader (root);
	if (strcmp (name, "remove_module") == 0) return new remove_module (root);
	if (strcmp (name, "set_machine") == 0) return new set_machine (root);
	if (strcmp (name, "create_module") == 0) return new create_module (root);
	if (strcmp (name, "crd") == 0) return new crd (root);
	if (strcmp (name, "root_directory") == 0) return new root_directory (root);
	if (strcmp (name, "add_search_directory") == 0) return new add_search_directory (root);
	if (strcmp (name, "search_directories") == 0) return new search_directories (root);
	if (strcmp (name, "cd") == 0) return new cd (root);
	if (strcmp (name, "DIR") == 0) return new DIR (root);
	if (strcmp (name, "ARGS") == 0) return new ARGS (root);
	if (strcmp (name, "edit") == 0) return new edit (root);
	if (strcmp (name, "execute") == 0) return new execute (root);
	if (strcmp (name, "make_directory") == 0) return new make_directory (root);
	if (strcmp (name, "erase") == 0) return new erase_file (root);
	if (strcmp (name, "erase_directory") == 0) return new erase_directory (root);
	if (strcmp (name, "move") == 0) return new move_file (root);
	if (strcmp (name, "copy") == 0) return new copy_file (root);

	if (strcmp (name, "rnd") == 0) return new rnd (n);
	if (strcmp (name, "rnd_control") == 0) return new rnd_control (n);
	if (strcmp (name, "series") == 0) return new series (n);

	if (strcmp (name, "CONSTANT") == 0) return new CONSTANT ();
	if (strcmp (name, "VARIABLE") == 0) return new VARIABLE ();
	if (strcmp (name, "ACCUMULATOR") == 0) return new ACCUMULATOR ();
	if (strcmp (name, "CLOSURE") == 0) return new CLOSURE ();
	if (strcmp (name, "ARRAY") == 0) return new ARRAY ();

	if (strcmp (name, "start") == 0) {
		if (transport == NULL) return new start (root, t);
		return new green_start (t);

	}
	if (strcmp (name, "pause") == 0) return new pause (t);
	if (strcmp (name, "stop") == 0) return new stop (t);
	if (strcmp (name, "wt") == 0) {
		if (pool == NULL) return new wt (t);
		return new green_wt (t);
	}
	if (strcmp (name, "beat") == 0) {
		if (pool == NULL) return new beat (t);
		return new green_beat (t);
	}
	if (strcmp (name, "bar") == 0) {
		if (pool == NULL) return new bar (t);
		return new green_bar (t);
	}
	if (strcmp (name, "signal") == 0) return new signal (t);
	if (strcmp (name, "signal_beat") == 0) return new signal_beat (t);
	if (strcmp (name, "signal_bar") == 0) return new signal_bar (t);
	if (strcmp (name, "reset") == 0) return new reset (t);
	if (strcmp (name, "tempo") == 0) return new tempo (t);
	if (strcmp (name, "atempo") == 0) return new atempo (t);
	if (strcmp (name, "accel") == 0) return new accel (t);
	if (strcmp (name, "rit") == 0) return new rit (t);
	if (strcmp (name, "tempo_division") == 0) return new tempo_division (t);
	if (strcmp (name, "metrum") == 0) return new metrum (t);

	if (strcmp (name, "midi_in_info") == 0) return new midi_in_info (root);
	if (strcmp (name, "midi_out_info") == 0) return new midi_out_info (root);
	if (strcmp (name, "midi_in_port") == 0) return new midi_in_port (root);
	if (strcmp (name, "midi_out_port") == 0) return new midi_out_port (root);
	if (strcmp (name, "DCMOD") == 0) return new DCMOD ();
	if (strcmp (name, "INTERVAL") == 0) return new interval_processor (root, this);
	if (strcmp (name, "midi_manufacturers_id") == 0) return new midi_manufacturers_id (root);
	if (strcmp (name, "midi_product_id") == 0) return new midi_product_id (root);
	if (strcmp (name, "midi_product_version") == 0) return new midi_product_version (root);
	midi_stream * line = root -> getMidiOutput ();
	if (pool == NULL) {midi_semaphore = root -> create_system_semaphore (1); midi_root = root;}
	if (line) {
		if (strcmp (name, "midi_short_msg") == 0) return new midi_short_msg (line, this);
		if (strcmp (name, "MIDI_SHORT_MSG") == 0) return new MIDI_SHORT_MSG (line);
		if (strcmp (name, "midi_long_msg") == 0) return new midi_long_msg (line);
		if (strcmp (name, "sysex") == 0) return new sysex (line);
		if (strcmp (name, "sysexch") == 0) return new sysexch (line);
		if (strcmp (name, "SYSEX") == 0) return new SYSEX (line);
		if (strcmp (name, "SYSEXCH") == 0) return new SYSEXCH (line);
	} else {
		if (strcmp (name, "midi_short_msg") == 0) return new void_midi_message (root);
		if (strcmp (name, "MIDI_SHORT_MSG") == 0) return new void_midi_message (root);
		if (strcmp (name, "midi_long_msg") == 0) return new void_midi_message (root);
		if (strcmp (name, "sysex") == 0) return new void_midi_message (root);
		if (strcmp (name, "sysexch") == 0) return new void_midi_message (root);
		if (strcmp (name, "SYSEX") == 0) return new void_midi_message (root);
		if (strcmp (name, "SYSEXCH") == 0) return new void_midi_message (root);
	}

	if (strcmp (name, "set_colors") == 0) return new set_colors (root);
	if (strcmp (name, "open_editor") == 0) return new open_editor (root);
	if (strcmp (name, "close_editor") == 0) return new close_editor (root);
	if (strcmp (name, "screen_coordinates") == 0) return new screen_coordinates (root);

	if (strcmp (name, "get_volume_serial_number") == 0) return new get_volume_serial_number (root);
	if (strcmp (name, "security_check") == 0) return new security_check (root);

	return NULL;
}

