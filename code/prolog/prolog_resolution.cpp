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

#include "prolog.h"
#include <string.h>

PrologResolution :: PrologResolution (PrologRoot * root) TRACKING (7) {
	this -> root = root;
	q_root = NULL;
	external_query_pointer = NULL;
	root_actual = NULL;
	root_formal = NULL;
	var_root = NULL;
	var_counter = 0;
	next = this;
	this -> timeout = 0;
	this -> timeout_query = NULL;
}

PrologResolution :: ~ PrologResolution (void) {
	if (timeout_query) delete timeout_query;
}

PrologVariable * PrologResolution :: newVariable (void) {
	var_root = new PrologVariable (var_root);
	return var_root;
}

PrologVariable * PrologResolution :: newVariable (PrologElement * term, bool location) {
	var_root = new PrologVariable (term, location, var_root);
	return var_root;
}

PrologQuery * PrologResolution :: getQuery (void) {return q_root;}

void PrologResolution :: reset (void) {
	if (root_actual != NULL) delete root_actual;
	if (root_formal != NULL) delete root_formal;
	if (var_root != NULL) delete var_root;
	root_actual = NULL;
	root_formal = NULL;
	var_counter = 0;
	var_root = NULL;
}

bool PrologResolution :: match (PrologElement * actual, bool ac, PrologElement * formal, bool fc) {
	int typea, typef;
	do {
		typea = actual -> type;
		typef = formal -> type;
		PrologMatch * v1 = 0;
		PrologMatch * v2 = 0;
		PrologVariable * var = 0;
		if (typea == 2) {
			if (typef == 2) {
				if (ac) {if (root_actual != 0) v1 = root_actual -> getVar (actual -> integer);}
				else {if (root_formal != 0) v1 = root_formal -> getVar (actual -> integer);}
				if (fc) {if (root_actual != 0) v2 = root_actual -> getVar (formal -> integer);}
				else {if (root_formal != 0) v2 = root_formal -> getVar (formal -> integer);}
				if (v1 == 0) {
					if (v2 == 0) {
						var = newVariable (); //new PrologVariable ();
						if (fc) root_actual = new PrologMatch (formal -> integer, var, root_actual);
						else root_formal = new PrologMatch (formal -> integer, var, root_formal);
					} else var = v2 -> var;
					if (ac) root_actual = new PrologMatch (actual -> integer, var, root_actual);
					else root_formal = new PrologMatch (actual -> integer, var, root_formal);
					return true;
				}
				if (v2 == 0) {
					if (fc) root_actual = new PrologMatch (formal -> integer, v1 -> var, root_actual);
					else root_formal = new PrologMatch (formal -> integer, v1 -> var, root_formal);
					return true;
				}
				if (v1 -> var -> term == 0) {
					var = v1 -> var;
					root_actual -> connect_var (var, v2 -> var);
					root_formal -> connect_var (var, v2 -> var);
					return true;
				}
				if (v2 -> var -> term == 0) {
					var = v2 -> var;
					root_actual -> connect_var (var, v1 -> var);
					root_formal -> connect_var (var, v1 -> var);
					return true;
				}
				return match (v1 -> var -> term, v1 -> var -> location, v2 -> var -> term, v2 -> var -> location);
			} else {
				if (ac) {
					if (root_actual != 0) v1 = root_actual -> getVar (actual -> integer);
					if (v1 == 0) {
						root_actual = new PrologMatch (actual -> integer, formal, fc, root_actual, this);
						return true;
					}
				} else {
					if (root_formal != 0) v1 = root_formal -> getVar (actual -> integer);
					if (v1 == 0) {
						root_formal = new PrologMatch (actual -> integer, formal, fc, root_formal, this);
						return true;
					}
				}
				if (v1 -> var -> term == 0) {
					v1 -> var -> term = formal;
					v1 -> var -> location = fc;
					return true;
				}
				return match (v1 -> var -> term, v1 -> var -> location, formal, fc);
			}
		}

		if (typef == 2) {
			if (fc) {
				if (root_actual != 0) v1 = root_actual -> getVar (formal -> integer);
				if (v1 == 0) {
					root_actual = new PrologMatch (formal -> integer, actual, ac, root_actual, this);
					return true;
				}
			} else {
				if (root_formal != 0) v1 = root_formal -> getVar (formal -> integer);
				if (v1 == 0) {
					root_formal = new PrologMatch (formal -> integer, actual, ac, root_formal, this);
					return true;
				}
			}
			if (v1 -> var -> term == 0) {
				v1 -> var -> term = actual;
				v1 -> var -> location = ac;
				return true;
			}
			return match (actual, ac, v1 -> var -> term, v1 -> var -> location);
		}

		if (typea != typef) return false;

		switch (typea) {
		case 8: return actual -> integer == formal -> integer;
		case 9: return actual -> floating_point == formal -> floating_point;
		case 3: return actual -> atom == formal -> atom;
		case 0: return true;
		case 1:
			if (! match (actual -> left, ac, formal -> left, fc)) return false;
			actual = actual -> right; formal = formal -> right;
			break;
		case 4: case 5: return true;
		case 6:
			if (actual -> text == 0) return false;
			if (formal -> text == 0) return false;
			return strcmp (actual -> text, formal -> text) == 0;
		case 7: return actual -> head == formal -> head;
		default: return false;
		}
	} while (typea == 1);
	return false;
}

PrologElement * PrologResolution :: match_product (PrologElement * actual, bool ac) {
	PrologMatch * v1 = 0;
	PrologVariable * var = 0;
	int ind;
	PrologElement * ret;
	PrologElement * main_return = 0;
	PrologElement * target = 0;
	int type;
	do {
		if (actual == 0) {if (target) target -> right = 0; return main_return;}
		type = actual -> type;
		switch (type) {
		case 2:
			if (ac) {
				if (root_actual != 0) v1 = root_actual -> getVar (actual -> integer);
				if (v1 == 0) {
					ret = new PrologElement ();
					ret -> type = 2;
					root_actual = new PrologMatch (actual -> integer, root_actual, this);
					root_actual -> var -> variable_id = var_counter;
					ret -> integer = var_counter;
					var_counter++;
					if (target) target -> right = ret;
					return main_return != 0 ? main_return : ret;
				}
			} else {
				if (root_formal != 0) v1 = root_formal -> getVar (actual -> integer);
				if (v1 == 0) {
					ret = new PrologElement ();
					ret -> type = 2;
					root_formal = new PrologMatch (actual -> integer, root_formal, this);
					root_formal -> var -> variable_id = var_counter;
					ret -> integer = var_counter;
					var_counter++;
					if (target) target -> right = ret;
					return main_return != 0 ? main_return : ret;
				}
			}
			var = v1 -> var;
			if (var -> term == 0) {
				ret = new PrologElement ();
				ret -> type = 2;
				ind = var -> variable_id;
				if (ind < 0) {
					var -> variable_id = var_counter;
					ind = var_counter;
					var_counter++;
				}
				ret -> integer = ind;
				if (target) target -> right = ret;
				return main_return != 0 ? main_return : ret;
			}
			ret = match_product (var -> term, var -> location);
			if (target) target -> right = ret;
			return main_return != 0 ? main_return : ret;
			break;
		case 1:
			ret = new PrologElement ();
			if (target) target -> right = ret;
			ret -> type = 1;
			ret -> left = match_product (actual -> left, ac);
			if (main_return == 0) main_return = ret;
			target = ret;
			actual = actual -> right;
			break;
		case 8: ret = new PrologElement (actual -> integer); if (target) target -> right = ret; return main_return != 0 ? main_return : ret; break;
		case 9: ret = new PrologElement (actual -> floating_point); if (target) target -> right = ret; return main_return != 0 ? main_return : ret; break;
		case 6: ret = new PrologElement (actual -> text); if (target) target -> right = ret; return main_return != 0 ? main_return : ret; break;
		case 7: ret = new PrologElement (actual -> head); if (target) target -> right = ret; return main_return != 0 ? main_return : ret; break;
		case 3: ret = new PrologElement (actual -> atom); if (target) target -> right = ret; return main_return != 0 ? main_return : ret; break;
		default: ret = new PrologElement (); ret -> type = type; if (target) target -> right = ret; return main_return != 0 ? main_return : ret; break;
		}
	} while (type == 1);
	return main_return;
}

int PrologResolution :: res_forward (void) {
	PrologElement * term = q_root -> query;
	PrologElement * e;
	PrologQuery * q;
	if (term -> type != 1) {root -> print ("Wrong query (querries expected)."); root -> print (root -> new_line_caption); return 0;}
	term = term -> right;
	if (term -> type != 1) {root -> print ("Wrong query (query expected)."); root -> print (root -> new_line_caption); return 0;}
	term = term -> left;
	if (term -> type == 5) return 0; // genuine fail
	if (term -> type == 4) {
		// slash maker
		q_root -> query -> drop_one_right ();
		q_root -> drop_stack_to_fail_target ();
		if (q_root -> original && q_root -> stack == q_root -> context && q_root -> stack != NULL) {
			if (q_root -> stack -> query -> right -> right -> type == 0) {
				reset ();
				match (q_root -> stack -> query -> right -> left -> right, true, q_root -> query -> left -> right, false);
				e = q_root -> stack -> query;
				q_root -> stack -> query = root -> pair (match_product (q_root -> stack -> query -> left, true), match_product (q_root -> query -> right, false));
				delete e;
				q = q_root -> stack;
				drop_stack (q_root, q);
				q_root = q;
				q_root -> fail_target = q_root -> stack;
				q_root -> original = false;
			} else q_root -> stack -> query -> right -> left -> left -> head = NULL;
		}
		if (q_root -> query -> right -> type == 0) return 2;
		return 1;
	}
	if (term -> type != 1) {root -> print ("Wrong query (query is not a pair)."); root -> print (root -> new_line_caption); return 0;}
	if (term -> left == NULL) {root -> print ("Wrong query (query head does not exist)."); root -> print (root -> new_line_caption); return 0;}
	if (term -> left -> type != 3) {root -> print ("Wrong query (query head is not an atom)."); root -> print (root -> new_line_caption); return 0;}
	PrologNativeCode * m = term -> left -> atom -> machine;
	if (m != NULL) {
		//	Machine goes here
		reset ();
		PrologElement * parameters = match_product (term -> right, true);
		if (! m -> code (parameters, this)) {delete parameters; return 0;}
		reset ();
		if (match (term -> right, true, parameters, false)) {
			PrologElement * new_head = match_product (q_root -> query -> left, true);
			PrologElement * new_tail = match_product (q_root -> query -> right -> right, true);
			delete q_root -> query;
			q_root -> query = root -> pair (new_head, new_tail);
			delete parameters;
			if (new_tail -> type == 0) return 2;
			return 1;
		}
		delete parameters;
		return 0;
	}
	PrologElement * clausa = term -> left -> atom -> firstClause;
	if (clausa == NULL) {root -> print ("Free atom ["); root -> print (term -> left -> atom -> atomName); root -> print ("]."); root -> print (root -> new_line_caption); return 0;}
	PrologElement * relation_atom = new PrologElement ();
	relation_atom -> type = 3;
	relation_atom -> atom = term -> left -> atom;
	COLLECTOR_REFERENCE_INC (relation_atom -> atom)
//	relation_atom -> atom -> reference_counter++;
	return sub_res_forward (relation_atom, term, clausa);
}

int PrologResolution :: sub_res_forward (PrologElement * relation_atom, PrologElement * term, PrologElement * clausa) {
	void * next_clausa;
	PrologElement * new_tail;
	PrologElement * after_unar;
	PrologElement * e;
	while (clausa != NULL) {
		reset ();
		next_clausa = clausa -> left -> left -> head;
		if (match (term -> right, true, clausa -> left -> right, false)) {
			new_tail = match_product (clausa -> right, false);
			if (new_tail -> type == 0) {
				delete relation_atom;
				delete new_tail;
				new_tail = match_product (q_root -> query -> right -> right, true);
				after_unar = root -> pair (match_product (q_root -> query -> left, true), new_tail);
				if (next_clausa == NULL) {
					e = q_root -> query;
					q_root -> query = after_unar;
					delete e;
				} else {
					term -> left -> head = next_clausa;
					q_root = new PrologQuery (q_root, q_root -> context, q_root -> fail_target, q_root -> original, after_unar);
				}
				if (new_tail -> type == 0) return 2;
				return 1;
			} else
				if (next_clausa == NULL && q_root -> query -> right -> right -> type == 0) {
					e = q_root -> query;
					q_root -> query = root -> pair (match_product (q_root -> query -> left, true), new_tail);
					q_root -> fail_target = q_root -> stack;
					q_root -> original = false;
					delete e;
					delete relation_atom;
					return 1;
				}
			term -> left -> head = next_clausa;
			q_root = new PrologQuery (q_root, q_root, q_root, true, root -> pair (root -> pair (relation_atom, match_product (clausa -> left -> right, false)), new_tail));
			return 1;
		}
		clausa = (PrologElement *) next_clausa;
	}
	delete relation_atom;
	return 0;
}

int PrologResolution :: res_back_back (void) {
	PrologQuery * context = q_root -> context;
	if (context == NULL) return 2;
	reset ();
	match (context -> query -> right -> left -> right, true, q_root -> query -> left -> right, false);
	PrologElement * new_head = match_product (context -> query -> left, true);
	PrologElement * new_tail = match_product (context -> query -> right -> right, true);
	q_root -> fail_target = context -> fail_target;
	q_root -> original = context -> original;
	q_root -> context = context -> context;
	delete q_root -> query;
	q_root -> query = root -> pair (new_head, new_tail);
	if (new_tail -> type == 0) return 1;
	return 0;
}

int PrologResolution :: res_fail_back (void) {
	PrologQuery * q = q_root;
	q_root = q_root -> stack;
	delete q;
	if (q_root == NULL) return 5;
	PrologElement * term = q_root -> query -> right -> left;
	PrologElement * relation_atom = new PrologElement ();
	relation_atom -> type = 3;
	relation_atom -> atom = term -> left -> atom;
	COLLECTOR_REFERENCE_INC (relation_atom -> atom)
//	relation_atom -> atom -> reference_counter++;
	PrologElement * clausa = (PrologElement *) term -> left -> head;
	return sub_res_forward (relation_atom, term, clausa);
}

int PrologResolution :: resolution (PrologElement * query) {
	// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query
	// query is a clause, which is a list anyway
	// the head of the query is a return interface and can be any term
	// the rest of the query are calls
	// after successfull processing only the interface
	// is left from the original query matched with the result
	// after failure the query remains unchanged
	// query egzample: [[*x *y *z] [c1 *x] [c2 *y] [c3 *z]]
	if (query == NULL) return 3;
	if (! query -> isPair ()) return 3;
	reset ();
	q_root = new PrologQuery (match_product (query, true));
	int ctrl;
	do {
		if (timeout > 0) {
			if (root -> get_system_time () >= timeout) {
				drop_stack (q_root, NULL); q_root = NULL;
				reset ();
				if (timeout_query != NULL) {
					PrologElement * clausa = root -> pair (root -> head (NULL), timeout_query -> duplicate ());
					root -> resolution (clausa);
					delete clausa;
				}
				return 2;
			}
		}
		ctrl = res_forward ();
		while (ctrl == 0) ctrl = res_fail_back ();
		if (ctrl == 2)
			while (ctrl != 0) {
				ctrl = res_back_back ();
				if (ctrl == 2) {
					reset ();
					delete query -> left;
					delete query -> right;
					query -> left = match_product (q_root -> query -> left, true);
					query -> right = match_product (q_root -> query -> right, true);
					drop_stack (q_root, NULL);
//					delete q_root;
					q_root = NULL;
					reset ();
					return 1;
				}
			}
	} while (ctrl != 5);
	delete q_root;
	reset ();
	return 0;
}

bool PrologResolution :: query (PrologElement * query) {
	if (query == NULL) return false;
	if (! query -> isPair ()) return false;
	external_query_pointer = query;
	reset ();
	q_root = new PrologQuery (match_product (query, true));
	return true;
}

PrologElement * PrologResolution :: query (void) {return external_query_pointer;}
void PrologResolution :: removeQuery (void) {
	if (external_query_pointer) delete external_query_pointer;
	external_query_pointer = NULL;
}

