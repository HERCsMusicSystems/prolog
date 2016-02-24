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

var_voc :: var_voc (char * text) TRACKING (8) {
	name = create_text (text);
	next = NULL;
}

var_voc :: ~ var_voc (void) {
	delete_text (name);
	if (next != NULL) delete next;
}

PrologReader :: PrologReader (void) TRACKING (9) {
	root = NULL;
	search_context = NULL;
	var_root = NULL;
	act_znak = 0;
	symbol [0] = '\0';
	symbol_control = 0;
	int_symbol = 0;
	double_symbol = 0.0;
}

PrologReader :: ~ PrologReader (void) {
	reset_var_number ();
	clear_context ();
}

void PrologReader :: clear_context (void) {
	PrologDirectory * next;
	PrologDirectory * sentinel = NULL;
	if (root != NULL) sentinel = root -> root;
	while (search_context != NULL && search_context != sentinel) {
		search_context -> firstAtom = NULL;
		search_context -> service_class = NULL;
		next = search_context -> next;
		search_context -> next = NULL;
		delete search_context;
		search_context = next;
	}
	search_context = NULL;
}

int PrologReader :: move_z (void) {return 0;}
void PrologReader :: message (char * text) {}
void PrologReader :: message_v (char * text, char * variable) {}
void PrologReader :: setRoot (PrologRoot * root) {this -> root = root;}

int PrologReader :: get_var_number (char * name) {
	if (var_root == NULL) {
		var_root = new var_voc (strlen (name) < 1 ? root -> new_line_caption : name);
		return 0;
	}
	int ind = 0;
	var_voc * ptr = var_root;
	while (ptr -> next != NULL) {
		if (strcmp (ptr -> name, name) == 0) return ind;
		ptr = ptr -> next;
		ind++;
	}
	if (strcmp (ptr -> name, name) == 0) return ind;
	ind++;
	ptr -> next = new var_voc (strlen (name) < 1 ? root -> new_line_caption : name);
	return ind;
}

void PrologReader :: reset_var_number (void) {
	if (var_root != NULL) delete var_root;
	var_root = NULL;
}

int PrologReader :: lastRead (void) {return act_znak;}

int PrologReader :: getString (char * area, int ind) {
	while (act_znak > 127 || act_znak <= 32) {
		if (act_znak < 0) return act_znak;
		act_znak = move_z ();
	}
	while (act_znak > 32) {
		if (ind < AREA_SIZE_1) area [ind++] = (char) act_znak;
		act_znak = move_z ();
	}
	area [ind] = '\0';
	return ind;
}

int PrologReader :: getString (char * area, int ind, char * char_set) {
	bool not_readed = true;
	if (strcmp (char_set, "line") == 0) {
		while (act_znak == 0 || act_znak == 13 || act_znak == 10) act_znak = move_z ();
		while (ind < AREA_SIZE_1 && act_znak >= 0 && act_znak != 13 && act_znak != 10) {
			area [ind++] = (char) act_znak;
			act_znak = move_z ();
			not_readed = false;
		}
		area [ind] = '\0';
		//while (act_znak == 0 || act_znak == 13 || act_znak == 10) act_znak = move_z ();
		if (act_znak < 0 && not_readed) return -1;
		return ind;
	}
	if (strcmp (char_set, "word") == 0) {
		while (act_znak >= 0 && act_znak <= 32) act_znak = move_z ();
		area [ind++] = (char) act_znak; act_znak = move_z ();
		while (ind < AREA_SIZE_1 && indexOf ("qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM-7894561230\"'`~.", (char) act_znak) >= 0) {
			area [ind++] = (char) act_znak;
			act_znak = move_z ();
			not_readed = false;
		}
		area [ind] = '\0';
		if (act_znak < 0 && not_readed) return -1;
		return ind;
	}
	while (act_znak >= 0 && indexOf (char_set, (char) act_znak) < 0) act_znak = move_z ();
	while (indexOf (char_set, (char) act_znak) >= 0) {
		if (ind < AREA_SIZE_1) area [ind++] = (char) act_znak;
		act_znak = move_z ();
		not_readed = false;
	}
	area [ind] = '\0';
	if (act_znak < 0 && not_readed) return -1;
	return ind;
}

int PrologReader :: readln (char * area, int ind) {
	while (act_znak > 127 || act_znak <= 32) {
		if (act_znak < 0) return act_znak;
		act_znak = move_z ();
	}
	while (act_znak > 31) {
		if (ind < AREA_SIZE_1) area [ind++] = (char) act_znak;
		act_znak = move_z ();
	}
	area [ind] = '\0';
	return ind;
}

bool PrologReader :: prefetch_whites (void) {
	while (act_znak < 33 || indexOf (root -> comment_caption, (char) act_znak) >= 0) {
		if (indexOf (root -> comment_caption, (char) act_znak) >= 0) {
			while (act_znak != 10 && act_znak != 13 && act_znak >= 0) act_znak = move_z ();
		}
		if (act_znak < 0) return true;
		act_znak = move_z ();
	}
	return false;
}

void PrologReader :: shebang (void) {
	act_znak = move_z ();
	if (act_znak != '#') return;
	while (act_znak != 10 && act_znak != 13 && act_znak >= 0) act_znak = move_z ();
}

void PrologReader :: get_symbol (void) {
	bool negative = false;
	bool atom_possible = true;
	double fraction;
	int ind;
	int int_multiplier;
	int int_shift;
	if (act_znak < 0) act_znak = move_z ();
	while (act_znak < 33 || indexOf (root -> comment_caption, (char) act_znak) >= 0) {
		if (indexOf (root -> comment_caption, (char) act_znak) >= 0) {
			while (act_znak != 10 && act_znak != 13 && act_znak >= 0) act_znak = move_z ();
		}
		if (act_znak < 0) {symbol_control = 9; return;}
		act_znak = move_z ();
	}
	if (indexOf (root -> left_caption, (char) act_znak) >= 0) {
		act_znak = move_z ();
		while (act_znak < 33 && act_znak >= 0) act_znak = move_z ();
		if (indexOf (root -> right_caption, (char) act_znak) >= 0) {
			symbol_control = 6;
			act_znak = move_z ();
			return;
		}
		symbol_control = 1;
		return;
	}
	if (indexOf (root -> secondary_left_caption, (char) act_znak) >= 0) {
		act_znak = move_z ();
		while (act_znak < 33 && act_znak >= 0) act_znak = move_z ();
		if (indexOf (root -> secondary_right_caption, (char) act_znak) >= 0) {
			symbol_control = 56;
			act_znak = move_z ();
			return;
		}
		symbol_control = 51;
		return;
	}
	if (indexOf (root -> right_caption, (char) act_znak) >= 0) {
		symbol_control = 2;
		act_znak = move_z ();
		return;
	}
	if (indexOf (root -> secondary_right_caption, (char) act_znak) >= 0) {
		symbol_control = 52;
		act_znak = move_z ();
		return;
	}
	if (indexOf (root -> slash_caption, (char) act_znak) >= 0) {
		symbol_control = 5;
		act_znak = move_z ();
		return;
	}
	if (indexOf (root -> mid_caption, (char) act_znak) >= 0) {
		symbol [0] = (char) act_znak;
		act_znak = move_z ();
		symbol [1] = (char) act_znak;
		symbol [2] = '\0';
		if (strcmp (root -> if_atom_caption, symbol) == 0) {
			act_znak = move_z ();
			symbol_control = 11;
		} else symbol_control = 3;
		return;
	}
	if (indexOf (root -> dot_caption, (char) act_znak) >= 0) {
		symbol_control = 21; strcpy (symbol, root -> dot_caption);
		act_znak = move_z ();
		return;
	}
	if (indexOf (root -> at_caption, (char) act_znak) >= 0) {
		symbol_control = 22;
		act_znak = move_z ();
		return;
	}
	if (indexOf (root -> separator_caption, (char) act_znak) >= 0) {
		symbol_control = 23;
		act_znak = move_z ();
		return;
	}
	// if liczba
	if (act_znak == '-') {
		negative = true;
		ind = area_cat (symbol, 0, '-');
		act_znak = move_z ();
	}
	if (act_znak >= '0' && act_znak <= '9') {
		int_multiplier = 10;
		int_shift = act_znak - '0';
		int_symbol = int_shift;
		double_symbol = (double) (act_znak - '0');
		act_znak = move_z ();
		while ((int_shift = indexOf ("0123456789abcdefABCDEFxX", act_znak)) >= 0) {
			if (int_shift >= 16) {int_shift -= 6; int_multiplier = 16;}
			if (int_shift >= 16) {int_shift = 0; int_multiplier = 16;}
			int_symbol *= int_multiplier;
			int_symbol += int_shift;
			double_symbol *= (double) int_multiplier;
			double_symbol += (double) int_shift;
			act_znak = move_z ();
		}
		if (act_znak == '.') {
			act_znak = move_z ();
			symbol_control = 18;
			fraction = 1.0 / (double) int_multiplier;
			int_symbol = 0;
			while ((int_shift = indexOf ("0123456789abcdefABCDEFxX", act_znak)) >= 0) {
				if ((int_shift == 14 || int_shift == 20) && int_multiplier != 16) {
					bool shift_negative = false;
					act_znak = move_z ();
					if (act_znak == '-') {shift_negative = true; act_znak = move_z ();}
					int position_shift = 0;
					while ((int_shift = indexOf ("0123456789", act_znak)) >= 0) {
						position_shift *= 10;
						position_shift += int_shift;
						act_znak = move_z ();
					}
					if (shift_negative) position_shift = - position_shift;
					while (position_shift > 0) {double_symbol *= 10.0; position_shift--;}
					while (position_shift < 0) {double_symbol /= 10.0; position_shift++;}
					break;
				}
				if (int_shift >= 16) {int_shift -= 6; int_multiplier = 16;}
				if (int_shift >= 16) {int_shift = 0; int_multiplier = 16;}
				double_symbol += (double) int_shift * fraction;
				fraction /= (double) int_multiplier;
				act_znak = move_z ();
			}
			if (negative) double_symbol = - double_symbol;
			return;
		}
		if (negative) int_symbol = - int_symbol;
		symbol_control = 10;
		return;
	}
	if (negative) {
		if (indexOf (root -> atom_head_captions, '-') >= 0) {
			ind = area_cat (symbol, 0, '-');
			symbol_control = 11;
			while (indexOf (root -> atom_tail_captions, (char) act_znak) >= 0) {
				if (strcmp (root -> if_atom_caption, symbol) == 0) return;
				if (strcmp (root -> and_atom_caption, symbol) == 0) return;
				char * opp = root -> operator_captions;
				while (* opp != '\0') {
					if (strcmp (opp, symbol) == 0) return;
					while (* opp != '\0') opp++; opp++;
				}
				ind = area_cat (symbol, ind, (char) act_znak);
				act_znak = move_z ();
			}
			return;
		}
		symbol_control = 0;
		message ("Lexical error (negative).");
		return;
	}
	// if dyrektywa
	// if (act_znak == '#') {
	if (indexOf (root -> directive_head_caption, (char) act_znak) >= 0) {
		//ind = area_cat (symbol, 0, '#');
		ind = area_cat (symbol, 0, "");
		act_znak = move_z ();
		while (act_znak >= 'a' && act_znak <= 'z') {
			ind = area_cat (symbol, ind, (char) act_znak);
			act_znak = move_z ();
		}
		symbol_control = 31;
		return;
	}
	// if text
	if (indexOf (root -> quotation_caption, (char) act_znak) >= 0) {
		symbol [0] = '\0';
		ind = 0;
		act_znak = move_z ();
		while (indexOf (root -> quotation_caption, (char) act_znak) < 0) {
			if (act_znak < 1) {
				message ("Lexical error (unclosed text).");
				symbol_control = 0;
				act_znak = move_z ();
				return;
			}
			if (indexOf (root -> escape_caption, (char) act_znak) >= 0) {
				act_znak = move_z ();
				switch ((char) act_znak) {
				case 'n': ind = area_cat (symbol, ind, root -> new_line_caption); break;
				default: ind = area_cat (symbol, ind, (char) act_znak); break;
				}
			} else ind = area_cat (symbol, ind, (char) act_znak);
			act_znak = move_z ();
		}
		act_znak = move_z ();
		symbol_control = 8;
		return;
	}
	// if quoted atom
	if (indexOf (root -> atom_quotation_caption, (char) act_znak) >= 0) {
		symbol [0] = '\0';
		ind = 0;
		act_znak = move_z ();
		while (indexOf (root -> atom_quotation_caption, (char) act_znak) < 0) {
			if (act_znak < 1) {
				message ("Lexical error (unclosed text).");
				symbol_control = 0;
				act_znak = move_z ();
				return;
			}
			if (indexOf (root -> escape_caption, (char) act_znak) >= 0) {
				act_znak = move_z ();
				switch ((char) act_znak) {
				case 'n': ind = area_cat (symbol, ind, root -> new_line_caption); break;
				default: ind = area_cat (symbol, ind, (char) act_znak); break;
				}
			} else ind = area_cat (symbol, ind, (char) act_znak);
			act_znak = move_z ();
		}
		act_znak = move_z ();
		symbol_control = 11;
		return;
	}
	// if zmienna
	if (indexOf (root -> var_head_captions, (char) act_znak) >= 0) {
		if (indexOf (root -> atom_head_captions, (char) act_znak) < 0) atom_possible = false;
		ind = area_cat (symbol, 0, (char) act_znak);
		act_znak = move_z ();
		while (indexOf (root -> var_tail_captions, (char) act_znak) >= 0) {
			ind = area_cat (symbol, ind, (char) act_znak);
			if (indexOf (root -> atom_tail_captions, (char) act_znak) < 0) atom_possible = false;
			act_znak = move_z ();
		}
		if (atom_possible) {
			symbol_control = 11;
			if (indexOf (root -> atom_tail_captions, (char) act_znak) >= 0) {
				while (indexOf (root -> atom_tail_captions, (char) act_znak) >= 0) {
					if (strcmp (root -> if_atom_caption, symbol) == 0) return;
					if (strcmp (root -> and_atom_caption, symbol) == 0) return;
					ind = area_cat (symbol, ind, (char) act_znak);
					act_znak = move_z ();
				}
				if (strcmp (root -> fail_caption, symbol) == 0) symbol_control = 4;
				return;
			}
		}
		if (strcmp (root -> var_caption, symbol) == 0) symbol [0] = '\0';
		symbol_control = 7;
		return;
	}
	// if identyfikator
	if (indexOf (root -> atom_head_captions, (char) act_znak) >= 0) {
//		symbol [0] = '\0';
		ind = 0;
		symbol_control = 11;
		do {
			ind = area_cat (symbol, ind, (char) act_znak);
			act_znak = move_z ();
			if (strcmp (root -> if_atom_caption, symbol) == 0) return;
			if (strcmp (root -> and_atom_caption, symbol) == 0) return;
			char * opp = root -> operator_captions;
			while (* opp != '\0') {
				if (strcmp (opp, symbol) == 0) return;
				while (* opp != '\0') opp++; opp++;
			}
		} while (indexOf (root -> atom_tail_captions, (char) act_znak) >= 0);
		if (strcmp (root -> fail_caption, symbol) == 0) symbol_control = 4;
		return;
	}

	symbol_control = 0;
	area_cat (symbol, 0, (char) act_znak);
	act_znak = move_z ();
	message ("Lexical error (unknown character sequence).");
}

PrologAtom * PrologReader :: searchAtom (char * name) {
	if (search_context == NULL) {return root -> search (name);}
	PrologDirectory * dir = search_context;
	PrologAtom * atom = NULL;
	while (dir != NULL) {
		atom = dir -> searchAtom (name);
		if (atom != NULL) return atom;
		dir = dir -> next;
	}
	return NULL;
}

PrologAtom * PrologReader :: searchAtomC (char * name) {
	PrologAtom * atom = searchAtom (name);
	if (atom != NULL) return atom;
	if (root -> auto_atoms) {
		atom = root -> createAtom (name);
		if (search_context != NULL) search_context -> firstAtom = atom;
	}
	return atom;
}

PrologElement * PrologReader :: atomC (char * name) {
	PrologAtom * atom = searchAtomC (name);
	if (atom == NULL) {
		message_v ("Atom not found:", name);
		return NULL;
	}
	PrologElement * el = new PrologElement ();
	el -> setAtom (atom);
	return el;
}

PrologElement * PrologReader :: readElement (void) {
	PROLOG_STRING dir_name;
	PrologElement * atom = NULL;
	PrologElement * left = NULL;
	get_symbol ();
	switch (symbol_control) {
	case 11:
		atom = atomC (symbol);
		if (atom == NULL) message_v ("Unknown atom:", symbol);
		return atom;
	case 22:
		get_symbol ();
		if (symbol_control != 11) {
			message ("Syntax error (directory expected).");
			return NULL;
		}
		prolog_string_copy (dir_name, symbol);
		get_symbol ();
		if (symbol_control != 21) {
			message ("Syntax error (dot expected).");
			return NULL;
		}
		get_symbol ();
		if (symbol_control != 11) {
			message ("Syntax error (atom after dot expected).");
			return NULL;
		}
		atom = root -> atom (dir_name, symbol);
		if (atom == NULL) message_v ("Unknown atom:", symbol);
		return atom;
	case 10: return root -> integer (int_symbol);
	case 56: case 6: return root -> earth ();
	case 5: return root -> slash ();
	case 4: return root -> fail ();
	case 7: return root -> var (get_var_number (symbol));
	case 8: return root -> text (symbol);
	case 18: return root -> Double (double_symbol);
	case 51:
		left = readElement ();
		if (left == NULL) return NULL;
		return readRightSide (left, true);
	case 1:
		left = readElement ();
		if (left == NULL) return NULL;
		return readRightSide (left, false);
	case 9: return NULL;
	case 21:
		atom = atomC (symbol);
		if (atom == NULL) message ("Dot atom not found.");
		return atom;
	default: break;
	}
	message ("Syntax error (unknown syntax).");
	return NULL;
}

PrologElement * PrologReader :: readRightSide (PrologElement * LEFT, bool secondary) {
	PROLOG_STRING dir_name;
	PrologElement * left = NULL;
	PrologElement * right = NULL;
	get_symbol ();
	if (strlen (root -> separator_caption) > 0) {
		switch (symbol_control) {
		case 1:
			left = readElement ();
			if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
			left = readRightSide (left, false);
			if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
			LEFT = root -> pair (LEFT, left);
			get_symbol ();
			break;
		case 6:
			LEFT = root -> pair (LEFT, root -> earth ());
			get_symbol ();
			break;
		default: break;
		}
	}
	switch (symbol_control) {
	case 52:
		if (secondary) return root -> pair (LEFT, root -> earth ());
		message ("Syntax error (bracket mismatch).");
		if (LEFT) delete LEFT;
		return NULL;
	case 2:
		if (! secondary) return root -> pair (LEFT, root -> earth ());
		message ("Syntax error (bracket mismatch).");
		if (LEFT) delete LEFT;
		return NULL;
	case 3:
		right = readElement ();
		if (right == NULL) {if (LEFT) delete LEFT; return NULL;}
		if (right -> isEarth ()) {
			message ("Syntax error (earth not allowed here).");
			if (LEFT) delete LEFT;
			return NULL;
		}
		get_symbol ();
		if ((symbol_control != 2 && ! secondary) || (symbol_control != 52 && secondary)) {
			message ("Syntax error (closing bracket expected).");
			if (LEFT) delete LEFT;
			return NULL;
		}
		return root -> pair (LEFT, right);
	}
	if (strlen (root -> separator_caption) > 0) {
		if (symbol_control == 23) get_symbol ();
		else {message ("Syntax error (separator expected)."); if (LEFT) delete LEFT; return NULL;}
	}
	switch (symbol_control) {
	case 11:
		left = atomC (symbol);
		if (left == NULL) message_v ("Unknown atom:", symbol);
		break;
	case 22:
		get_symbol ();
		if (symbol_control != 11) {message ("Syntax error (directory expected)."); if (LEFT) delete LEFT; return NULL;}
		prolog_string_copy (dir_name, symbol);
		get_symbol ();
		if (symbol_control != 21) {message ("Syntax error (atom after dot expected)."); if (LEFT) delete LEFT; return NULL;}
		get_symbol ();
		left = root -> atom (dir_name, symbol);
		if (left == NULL) message_v ("Unknown atom:", symbol);
		break;
	case 10: left = root -> integer (int_symbol); break;
	case 56: case 6: left = root -> earth (); break;
	case 5: left = root -> slash (); break;
	case 4: left = root -> fail (); break;
	case 7: left = root -> var (get_var_number (symbol)); break;
	case 8: left = root -> text (symbol); break;
	case 18: left = root -> Double (double_symbol); break;
	case 51:
		left = readElement ();
		if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
		right = readRightSide (left, true);
		if (right == NULL) {if (LEFT) delete LEFT; return NULL;}
		left = right;
		break;
	case 1:
		left = readElement ();
		if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
		left = readRightSide (left, false);
		if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
		break;
	case 21:
		left = atomC (symbol);
		if (left == NULL) message ("Dot atom does not exist.");
		break;
	default: if (LEFT) delete LEFT; return NULL;
	}
	if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
	left = readRightSide (left, secondary);
	if (left == NULL) {if (LEFT) delete LEFT; return NULL;}
	return root -> pair (LEFT, left);
}

PrologElement * PrologReader :: readClause (void) {
	bool marseille_if = false;
	reset_var_number ();
	PrologElement * clausa = NULL;
	PROLOG_STRING dir_name;
	if (symbol_control == 1) {
		clausa = readElement ();
		if (clausa == NULL) return NULL;
		return readRightSide (clausa, false);
	}

	PrologElement * right = NULL;
	PrologElement * left = NULL;
	if (symbol_control == 11 && strcmp (root -> if_atom_caption, symbol) == 0) {
		marseille_if = true;
		get_symbol ();
	}
	switch (symbol_control) {
	case 22:
		get_symbol ();
		if (symbol_control != 11) {message ("Syntax error (directory expected)."); return NULL;}
		dir_name [0] = '\0';
		get_symbol ();
		if (symbol_control != 21) {message ("Syntax error (atom after dot expected)."); return NULL;}
		get_symbol ();
		left = root -> atom (dir_name, symbol);
		if (left == NULL) {message_v ("Unknown atom:", symbol); return NULL;}
		break;
	case 11:
		left = atomC (symbol);
		if (left == NULL) return NULL;
		break;
	default: return NULL;
	}
	get_symbol ();
	switch (symbol_control) {
	case 6:
		right = root -> earth ();
		get_symbol ();
		break;
	case 1:
		right = readElement ();
		if (right == NULL) return NULL;
		right = readRightSide (right, false);
		if (right == NULL) return NULL;
		get_symbol ();
		break;
	default: right = root -> earth (); break;
	}
	clausa = root -> pair (root -> pair (left, right), root -> earth ());
	if (symbol_control == 21) return clausa;
	if (symbol_control != 11 || (strcmp (root -> if_atom_caption, symbol) != 0 && ! marseille_if) || (marseille_if && strcmp (root -> and_atom_caption, symbol) != 0)) {
		if (clausa) delete clausa;
		return NULL;
	}
	PrologElement * el = clausa;
	while (symbol_control != 21) {
		left = readElement ();
		if (left == NULL) {if (clausa) delete clausa; return NULL;}
		get_symbol ();
		switch (symbol_control) {
		case 6:
			right = root -> earth ();
			get_symbol ();
			el -> setRight (root -> pair (root -> pair (left, right), root -> earth ()));
			break;
		case 1:
			right = readElement ();
			if (right == NULL) {if (clausa) delete clausa; return NULL;}
			right = readRightSide (right, false);
			if (right == NULL) {if (clausa) delete clausa; return NULL;}
			get_symbol ();
			el -> setRight (root -> pair (root -> pair (left, right), root -> earth ()));
			break;
		default:
			if (left -> isAtom ()) el -> setRight (root -> pair (root -> pair (left, root -> earth ()), root -> earth ()));
			else el -> setRight (root -> pair (left, root -> earth ()));
			break;
		}
		el = el -> getRight ();
		if (symbol_control != 21 && symbol_control != 23 && (symbol_control != 11 || strcmp (root -> and_atom_caption, symbol) != 0)) {
			if (clausa) delete clausa;
			return NULL;
		}
	}
	return clausa;
}
