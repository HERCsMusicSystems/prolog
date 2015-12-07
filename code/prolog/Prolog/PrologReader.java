///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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

package Prolog;

import java . util . ArrayList;

public class PrologReader {
	public PrologRoot root;
	public PrologDirectory search_context;
	public ArrayList <String> var_root = null;
	public int act_znak = 0;
	public int symbol_control = 0;
	public String symbol = "";
	public int int_symbol;
	public double double_symbol;
	public void clear_context () {search_context = null;}
	public int move_z () {return root . get_character ();}
	public void setRoot (PrologRoot root) {this . root = root;}
	public int get_var_number (String name) {
		if (var_root == null) {var_root = new ArrayList <String> (); var_root . add (name . length () < 1 ? root . new_line_caption : name); return 0;}
		int ind = var_root . indexOf (name);
		if (ind >= 0) return ind;
		ind = var_root . size ();
		var_root . add (name . length () < 1 ? root . new_line_caption : name);
		return ind;
	}
	public void reset_var_number () {var_root = null;}
	public int lastRead () {return act_znak;}
	public String getString () {
		while (act_znak > 127 || act_znak <= 32) {if (act_znak < 0) return null; act_znak = move_z ();}
		String ret = "";
		while (act_znak > 32) {ret += (char) act_znak; act_znak = move_z ();}
		return ret;
	}
	public String getString (String char_set) {
		boolean not_readed = true;
		String ret = "";
		if (char_set . equals ("line")) {
			while (act_znak == 0 || act_znak == 13 || act_znak == 10) act_znak = move_z ();
			while (act_znak >= 0 && act_znak != 13 && act_znak != 10) {ret += (char) act_znak; act_znak = move_z (); not_readed = false;}
			if (act_znak < 0 && not_readed) return null;
			return ret;
		}
		if (char_set . equals ("word")) {
			while (act_znak >= 0 && act_znak <= 32) act_znak = move_z ();
			ret += (char) act_znak; act_znak = move_z ();
			while ("qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM-7894561230\"'`~." . indexOf ((char) act_znak) >= 0) {
				ret += (char) act_znak;
				act_znak = move_z ();
				not_readed = false;
			}
			if (act_znak < 0 && not_readed) return null;
			return ret;
		}
		while (act_znak >= 0 && char_set . indexOf ((char) act_znak) < 0) act_znak = move_z ();
		while (char_set . indexOf ((char) act_znak) >= 0) {ret += (char) act_znak; act_znak = move_z (); not_readed = false;}
		if (act_znak < 0 && not_readed) return null;
		return ret;
	}
	public String readln () {
		while (act_znak > 127 || act_znak <= 32) {if (act_znak < 0) return ""; act_znak = move_z ();}
		String ret = "";
		while (act_znak > 31) {ret += (char) act_znak; act_znak = move_z ();}
		return ret;
	}
	public boolean prefetch_whites () {
		while (act_znak < 33 || root . comment_caption . indexOf ((char) act_znak) >= 0) {
			if (root . comment_caption . indexOf ((char) act_znak) >= 0) {
				while (act_znak != 10 && act_znak != 13 && act_znak >= 0) act_znak = move_z ();
			}
			if (act_znak < 0) return true;
			act_znak = move_z ();
		}
		return false;
	}
	public void get_symbol () {
		if (act_znak < 0) act_znak = move_z ();
		while (act_znak < 33 || root . comment_caption . indexOf ((char) act_znak) >= 0) {
			if (root . comment_caption . indexOf ((char) act_znak) >= 0) {
				while (act_znak != 10 && act_znak != 13 && act_znak >= 0) act_znak = move_z ();
			}
			if (act_znak < 0) {symbol_control = 9; return;}
			act_znak = move_z ();
		}
		if (root . left_caption . indexOf ((char) act_znak) >= 0) {
			act_znak = move_z ();
			while (act_znak < 33 && act_znak >= 0) act_znak = move_z ();
			if (root . right_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 6; act_znak = move_z (); return;}
			symbol_control = 1;
			return;
		}
		if (root . secondary_left_caption . indexOf ((char) act_znak) >= 0) {
			act_znak = move_z ();
			while (act_znak < 33 && act_znak >= 0) act_znak = move_z ();
			if (root . secondary_right_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 56; act_znak = move_z (); return;}
			symbol_control = 51;
			return;
		}
		if (root . right_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 2; act_znak = move_z (); return;}
		if (root . secondary_right_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 52; act_znak = move_z (); return;}
		if (root . slash_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 5; act_znak = move_z (); return;}
		if (root . mid_caption . indexOf ((char) act_znak) >= 0) {
			symbol = "" + (char) act_znak;
			act_znak = move_z ();
			symbol += (char) act_znak;
			if (root . if_atom_caption . equals (symbol)) {act_znak = move_z (); symbol_control = 11;} else symbol_control = 3;
			return;
		}
		if (root . dot_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 21; symbol = "" + root . dot_caption; act_znak = move_z (); return;}
		if (root . at_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 22; act_znak = move_z (); return;}
		if (root . separator_caption . indexOf ((char) act_znak) >= 0) {symbol_control = 32; act_znak = move_z (); return;}
		// if number
		boolean negative = false;
		if (act_znak == '-') {negative = true; symbol = "-"; act_znak = move_z ();}
		if (act_znak >= '0' && act_znak <= '9') {
			int int_multiplier = 10;
			int int_shift = act_znak - '0';
			int_symbol = int_shift;
			double_symbol = (double) int_symbol;
			act_znak = move_z ();
			while ((int_shift = "0123456789abcdefABCDEFxX" . indexOf ((char) act_znak)) >= 0) {
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
				double fraction = 1.0 / (double) int_multiplier;
				int_symbol = 0;
				while ((int_shift = "0123456789abcdefABCDEFxX" . indexOf (act_znak)) >= 0) {
					if ((int_shift == 14 || int_shift == 20) && int_multiplier != 16) {
						boolean shift_negative = false;
						act_znak = move_z ();
						if (act_znak == '-') {shift_negative = true; act_znak = move_z ();}
						int position_shift = 0;
						while ((int_shift = "0123456789" . indexOf (act_znak)) >= 0) {position_shift *= 10; position_shift += int_shift; act_znak = move_z ();}
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
			if (root . atom_head_captions . indexOf ('-') >= 0) {
				symbol = "-";
				symbol_control = 11;
				while (root . atom_tail_captions . indexOf ((char) act_znak) >= 0) {
					if (root . if_atom_caption . equals (symbol)) return;
					if (root . and_atom_caption . equals (symbol)) return;
					if (root . operator_captions != null && root . operator_captions . indexOf (symbol) >= 0) return;
					symbol += ((char) act_znak);
					act_znak = move_z ();
				}
				return;
			}
			symbol_control = 0;
			root . message ("Lexical error (negative).");
			return;
		}
		// if directive
		if (root . directive_head_caption . indexOf ((char) act_znak) >= 0) {
			symbol = "";
			act_znak = move_z ();
			while (act_znak >= 'a' && act_znak <= 'z') {symbol += (char) act_znak; act_znak = move_z ();}
			symbol_control = 31;
			return;
		}
		// if text
		if (root . quotation_caption . indexOf ((char) act_znak) >= 0) {
			symbol = "";
			act_znak = move_z ();
			while (root . quotation_caption . indexOf ((char) act_znak) < 0) {
				if (act_znak < 1) {root . message ("Lexical error (unclosed text)."); symbol_control = 0; act_znak = move_z (); return;}
				if (root . escape_caption . indexOf ((char) act_znak) >= 0) {
					act_znak = move_z ();
					switch ((char) act_znak) {
					case 'n': symbol += root . new_line_caption; break;
					default: symbol += (char) act_znak; break;
					}
				} else symbol += (char) act_znak;
				act_znak = move_z ();
			}
			act_znak = move_z ();
			symbol_control = 8;
			return;
		}
		// if quoted atom
		if (root . atom_quotation_caption . indexOf ((char) act_znak) >= 0) {
			symbol = "";
			act_znak = move_z ();
			while (root . atom_quotation_caption . indexOf ((char) act_znak) < 0) {
				if (act_znak < 1) {root . message ("Lexical error (unclosed text)."); symbol_control = 0; act_znak = move_z (); return;}
				if (root . escape_caption . indexOf ((char) act_znak) >= 0) {
					act_znak = move_z ();
					switch ((char) act_znak) {
					case 'n': symbol += root . new_line_caption; break;
					default: symbol += (char) act_znak; break;
					}
				} else symbol += (char) act_znak;
				act_znak = move_z ();
			}
			act_znak = move_z ();
			symbol_control = 11;
			return;
		}
		// if variable
		if (root . var_head_captions . indexOf ((char) act_znak) >= 0) {
			boolean atom_possible = true;
			if (root . atom_head_captions . indexOf ((char) act_znak) < 0) atom_possible = false;
			symbol = "" + (char) act_znak;
			act_znak = move_z ();
			while (root . var_tail_captions . indexOf ((char) act_znak) >= 0) {
				symbol += (char) act_znak;
				if (root . atom_tail_captions . indexOf ((char) act_znak) < 0) atom_possible = false;
				act_znak = move_z ();
			}
			if (atom_possible) {
				symbol_control = 11;
				if (root . atom_tail_captions . indexOf ((char) act_znak) >= 0) {
					while (root . atom_tail_captions . indexOf ((char) act_znak) >= 0) {
						if (root . if_atom_caption . equals (symbol)) return;
						if (root . and_atom_caption . equals (symbol)) return;
						symbol += (char) act_znak;
						act_znak = move_z ();
					}
					if (root . fail_caption . equals (symbol)) symbol_control = 4;
					return;
				}
			}
			if (root . var_caption . equals (symbol)) symbol = "";
			symbol_control = 7;
			return;
		}
		// if identifier
		if (root . atom_head_captions . indexOf ((char) act_znak) >= 0) {
			symbol = "";
			symbol_control = 11;
			do {
				symbol += (char) act_znak;
				act_znak = move_z ();
				if (root . if_atom_caption . equals (symbol)) return;
				if (root . and_atom_caption . equals (symbol)) return;
				if (root . operator_captions != null && root . operator_captions . indexOf (symbol) >= 0) return;
			} while (root . atom_tail_captions . indexOf ((char) act_znak) >= 0);
			if (root . fail_caption . equals (symbol)) symbol_control = 4;
			return;
		}
		symbol_control = 0;
		symbol = "";
		act_znak = move_z ();
		root . message ("Lexical error (unknown character sequence).");
	}
	public PrologAtom searchAtom (String name) {
		if (search_context == null) return root . search (name);
		PrologDirectory dir = search_context;
		while (dir != null) {
			PrologAtom atom = dir . searchAtom (name);
			if (atom != null) return atom;
			dir = dir . next;
		}
		return null;
	}
	public PrologAtom searchAtomC (String name) {
		PrologAtom atom = searchAtom (name);
		if (atom != null) return atom;
		if (root . auto_atoms) {
			atom = root . createAtom (name);
			if (search_context != null) search_context . firstAtom = atom;
		}
		return atom;
	}
	public PrologElement atomC (String name) {
		PrologAtom atom = searchAtomC (name);
		if (atom == null) {root . message ("Atom not found: " + name); return null;}
		return new PrologElement (atom);
	}
	public PrologElement readElement () {
		PrologElement atom, left;
		get_symbol ();
		switch (symbol_control) {
		case 11: atom = atomC (symbol); if (atom == null) root . message ("Unknown atom: " + symbol); return atom;
		case 22:
			get_symbol ();
			if (symbol_control != 11) {root . message ("Syntax error (directory expected)."); return null;}
			String dir_name = "" + symbol;
			get_symbol ();
			if (symbol_control != 21) {root . message ("Syntax error (dot expected)."); return null;}
			get_symbol ();
			if (symbol_control != 11) {root . message ("Syntax error (atom after dot expected)."); return null;}
			atom = root . atom (dir_name, symbol);
			if (atom == null) root . message ("Unknown atom: " + symbol);
			return atom;
		case 10: return root . integer (int_symbol);
		case 56: case 6: return root . earth ();
		case 5: return root . slash ();
		case 4: return root . fail ();
		case 7: return root . var (get_var_number (symbol));
		case 8: return root . text (symbol);
		case 18: return root . Double (double_symbol);
		case 51: left = readElement (); if (left == null) return null; return readRightSide (left, true);
		case 1: left = readElement (); if (left == null) return null; return readRightSide (left, false);
		case 9: return null;
		case 21: atom = atomC (symbol); if (atom == null) root . message ("Dot atom not found."); return atom;
		default: break;
		}
		root . message ("Syntax error (unknown syntax).");
		return null;
	}
	public PrologElement readRightSide (PrologElement LEFT, boolean secondary) {
		PrologElement left, right;
		get_symbol ();
		if (root . separator_caption . length () > 0) {
			switch (symbol_control) {
			case 1:
				left = readElement (); if (left == null) return null;
				left = readRightSide (left, false); if (left == null) return null;
				LEFT = root . pair (LEFT, left);
				get_symbol ();
				break;
			case 6: LEFT = root . pair (LEFT, root . earth ()); get_symbol (); break;
			default: break;
			}
		}
		switch (symbol_control) {
		case 52:
			if (secondary) return root . pair (LEFT, root . earth ());
			root . message ("Syntax error (bracket mismatch).");
			return null;
		case 2:
			if (! secondary) return root . pair (LEFT, root . earth ());
			root . message ("Syntax error (bracket mismatch).");
			return null;
		case 3:
			right = readElement (); if (right == null) return null;
			if (right . isEarth ()) {root . message ("Syntax error (earth not allowed here)."); return null;}
			get_symbol ();
			if ((symbol_control != 2 && ! secondary) || (symbol_control != 52 && secondary)) {root . message ("Syntax error (closing bracket expected)."); return null;}
			return root . pair (LEFT, right);
		default: break;
		}
		if (root . separator_caption . length () > 0) {if (symbol_control == 23) get_symbol (); else {root . message ("Syntax error (separator expected)."); return null;}}
		switch (symbol_control) {
		case 11: left = atomC (symbol); if (left == null) root . message ("Unknown atom: " + symbol); break;
		case 22:
			get_symbol ();
			if (symbol_control != 11) {root . message ("Syntax error (directory expected)."); return null;}
			String dir_name = "" + symbol;
			get_symbol ();
			if (symbol_control != 21) {root . message ("Syntax error (atom after dot expected)."); return null;}
			get_symbol ();
			left = root . atom (dir_name, symbol);
			if (left == null) root . message ("Unknown atom: " + symbol);
			break;
		case 10: left = root . integer (int_symbol); break;
		case 56: case 6: left = root . earth (); break;
		case 5: left = root . slash (); break;
		case 4: left = root . fail (); break;
		case 7: left = root . var (get_var_number (symbol)); break;
		case 8: left = root . text (symbol); break;
		case 18: left = root . Double (double_symbol); break;
		case 51:
			left = readElement (); if (left == null) return null;
			right = readRightSide (left, true); if (right == null) return null;
			left = right;
			break;
		case 1:
			left = readElement (); if (left == null) return null;
			left = readRightSide (left, false); if (left == null) return null;
			break;
		case 21: left = atomC (symbol); if (left == null) root . message ("Dot atom does not exist."); break;
		default: return null;
		}
		if (left == null) return null;
		left = readRightSide (left, secondary);
		if (left == null) return null;
		return root . pair (LEFT, left);
	}
	public PrologElement readClause () {
		reset_var_number ();
		PrologElement clausa, left, right;
		boolean marseille_if = false;
		String dir_name;
		if (symbol_control == 1) {clausa = readElement (); if (clausa == null) return null; return readRightSide (clausa, false);}
		if (symbol_control == 11 && root . if_atom_caption . equals (symbol)) {marseille_if = true; get_symbol ();}
		switch (symbol_control) {
		case 22:
			get_symbol (); if (symbol_control != 11) {root . message ("Syntax error (directory expected)."); return null;}
			dir_name = "";
			get_symbol (); if (symbol_control != 21) {root . message ("Syntax error (atom after dot expected)."); return null;}
			get_symbol ();
			left = root . atom (dir_name, symbol); if (left == null) {root . message ("Unknown atom: " + symbol); return null;}
			break;
		case 11: left = atomC (symbol); if (left == null) return null; break;
		default: return null;
		}
		get_symbol ();
		switch (symbol_control) {
		case 6: right = root . earth (); get_symbol (); break;
		case 1:
			right = readElement (); if (right == null) return null;
			right = readRightSide (right, false); if (right == null) return null;
			get_symbol ();
			break;
		default: right = root . earth (); break;
		}
		clausa = root . pair (root . pair (left, right), root . earth ());
		if (symbol_control == 21) return clausa;
		if (symbol_control != 11 || (! root . if_atom_caption . equals (symbol) && ! marseille_if) || (marseille_if && ! root . and_atom_caption . equals (symbol))) return null;
		PrologElement el = clausa;
		while (symbol_control != 21) {
			left = readElement (); if (left == null) return null;
			get_symbol ();
			switch (symbol_control) {
			case 6: right = root . earth (); get_symbol (); el . setRight (root . pair (root . pair (left, right), root . earth ())); break;
			case 1:
				right = readElement (); if (right == null) return null;
				right = readRightSide (right, false); if (right == null) return null;
				get_symbol ();
				el . setRight (root . pair (root . pair (left, right), root . earth ()));
				break;
			default:
				if (left . isAtom ()) el . setRight (root . pair (root . pair (left, root . earth ()), root . earth ()));
				else el . setRight (root . pair (left, root . earth ()));
				break;
			}
			el = el . getRight ();
			if (symbol_control != 21 && symbol_control != 23 && (symbol_control != 11 || ! root . and_atom_caption . equals (symbol))) return null;
		}
		return clausa;
	}
}
