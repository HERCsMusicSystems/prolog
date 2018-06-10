///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2018 Robert P. Wolf                       //
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

#include "prolog_json.h"
#include "prolog_studio.h"

#include <string.h>
#include <string>

class json_native_class : public PrologNativeCode {
public:
	PrologJSONServiceClass * service;
	PrologRoot root;
	PrologAtom * atomC (char * name) {
		PrologAtom * atom = service -> root -> search (name);
		if (atom == 0) atom = service -> root -> createAtom (name);
		return atom;
	}
	void read_json_pair (PrologElement * el, PrologReader * reader) {
		el -> setPair (); el -> getLeft () -> setAtom (service -> equal_atom);
		if (reader -> symbol_control != 8) {reader -> get_symbol (); return;}
		el = el -> getRight (); el -> setPair (); el -> getLeft () -> setAtom (atomC (reader -> symbol));
		el = el -> getRight (); el -> setPair ();
		reader -> get_symbol (); if (reader -> symbol_control != 3) return;
		reader -> get_symbol ();
		read_json (el -> getLeft (), reader);
	}
	void read_json (PrologElement * el, PrologReader * reader) {
		switch (reader -> symbol_control) {
		case 10: el -> setInteger (reader -> int_symbol); reader -> get_symbol (); break;
		case 18: el -> setDouble (reader -> double_symbol); reader -> get_symbol (); break;
		case 11:
			if (strcmp (reader -> symbol, "true")) el -> setAtom (service -> true_atom);
			else if (strcmp (reader -> symbol, "false")) el -> setAtom (service -> false_atom);
			else if (strcmp (reader -> symbol, "null")) el -> setAtom (service -> null_atom);
			reader -> get_symbol ();
			break;
		case 8: el -> setAtom (atomC (reader -> symbol)); reader -> get_symbol (); break;
		case 1:
			el -> setPair ();
			reader -> get_symbol ();
			read_json (el -> getLeft (), reader);
			while (reader -> symbol_control == 23) {el = el -> getRight (); el -> setPair (); reader -> get_symbol (); read_json (el -> getLeft (), reader);}
			if (reader -> symbol_control == 2) reader -> get_symbol ();
			break;
		case 51:
			el -> setPair ();
			reader -> get_symbol ();
			read_json_pair (el -> getLeft (), reader);
			while (reader -> symbol_control == 23) {el = el -> getRight (); el -> setPair (); reader -> get_symbol (); read_json_pair (el -> getLeft (), reader);}
			if (reader -> symbol_control == 52) reader -> get_symbol ();
			break;
		case 6: case 56: el -> setEarth (); reader -> get_symbol (); break;
		default: break;
		}
	}
	void tabs (std :: string * area, int tab) {while (tab-- > 0) * area += ' ';}
	void drop_text (std :: string * area, char * text) {
		area -> append ("\"");
		while (* text != '\0') {
			if (* text == '\"') area -> append ("\\\"");
			else if (* text == '\\') area -> append ("\\\\");
			else * area += * text;
			text++;
		}
		area -> append ("\"");
	}
	void multiline (std :: string * area, int tab, PrologElement * el, char bracket) {
		int tabss = tab + 2;
		bool comma = false;
		while (el -> isPair ()) {
			if (comma) {area -> append (","); if (tab < 0) area -> append (" ");} comma = true;
			if (tab >= 0) {area -> append ("\n"); tabs (area, tabss);}
			drop (area, tabss, el -> getLeft ());
			el = el -> getRight ();
		}
		if (tab >= 0) {area -> append ("\n"); tabs (area, tab);}
		* area += bracket;
	}
	void drop (std :: string * area, int tab, PrologElement * el) {
		if (el -> isAtom ()) {
			PrologAtom * atom = el -> getAtom ();
			if (atom == service -> true_atom) area -> append ("true");
			else if (atom == service -> false_atom) area -> append ("false");
			else if (atom == service -> null_atom) area -> append ("null");
			else drop_text (area, atom -> name ());
			return;
		} else if (el -> isText ()) {drop_text (area, el -> getText ()); return;}
		else if (el -> isInteger ()) {PROLOG_STRING command; sprintf (command, "%i", el -> getInteger ()); area -> append (command); return;}
		else if (el -> isDouble ()) {PROLOG_STRING command; sprintf (command, "%.15g", el -> getDouble ()); area -> append (command); return;}
		if (el -> isPair ()) {
			PrologElement * left = el -> getLeft ();
			PrologElement * right = el -> getRight ();
			if (left -> isAtom () && left -> getAtom () == service -> equal_atom) {
				if (! right -> isPair ()) return; left = right -> getLeft (); right = right -> getRight ();
				if (! right -> isPair ()) return; right = right -> getLeft ();
				drop (area, tab + 2, left); area -> append (": "); drop (area, tab, right);
				return;
			}
			char left_bracket = '[', right_bracket = ']';
			if (left -> isPair ()) left = left -> getLeft ();
			if (left -> isAtom () && left -> getAtom () == service -> equal_atom) {left_bracket = '{'; right_bracket = '}';}
			* area += left_bracket;
			multiline (area, tab, el, right_bracket);
			return;
		}
		if (el -> isFail ()) area -> append ("false");
		else if (el -> isSlash ()) area -> append ("true");
		else if (el -> isVar ()) area -> append ("null");
		else if (el -> isEarth ()) area -> append ("[]");
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * path = 0;
		PrologElement * variable = 0;
		PrologElement * json = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) {if (path == 0) path = el; else json = el;}
			else if (el -> isVar ()) variable = el;
			else json = el;
			parameters = parameters -> getRight ();
		}
		if (json != 0) {
			std :: string area = "";
			if (path == 0) {
				if (variable == 0) {drop (& area, 0, json); printf ("%s", area . c_str ()); printf ("\n");}
				else {drop (& area, -65535, json); variable -> setText ((char *) area . c_str ());}
			} else {FILE * fw = fopen (path -> getText (), "wb"); drop (& area, 0, json); fprintf (fw, "%s", area . c_str ()); fprintf (fw, "\n"); fclose (fw);}
			return true;
		} else {
			if (variable != 0 && path != 0) {
				char * command = path -> getText ();
				if (command [0] == '[' || command [0] == '{') {
					term_reader reader;
					reader . init (& root, path -> getText ());
					reader . get_symbol ();
					read_json (variable, & reader);
				} else {
					symbol_reader reader;
					FILE * fr = fopen (command, "rb");
					if (fr == 0) {
						PrologString * dir = service -> root -> search_directories;
						while (dir != 0 && fr == 0) {
							AREA area; sprintf (area, "%s%s", dir -> text, command);
							fr = fopen (area, "rb");
							dir = dir -> next;
						}
					}
					if (fr == 0) return false;
					reader . init (& root, fr);
					reader . get_symbol ();
					read_json (variable, & reader);
					fclose (fr);
				}
				return true;
			}
		}
		return false;
	}
	json_native_class (PrologJSONServiceClass * service) {
		this -> service = service;
		strcpy (root . separator_caption, ",");
		strcpy (root . left_caption, "[");
		strcpy (root . right_caption, "]");
		strcpy (root . secondary_left_caption, "{");
		strcpy (root . secondary_right_caption, "}");
		strcpy (root . mid_caption, ":");
	}
};

void PrologJSONServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	if (root == 0) return;
	this -> root = root;
	PrologDirectory * studio = root -> searchDirectory ("studio");
	if (studio != 0) equal_atom =studio -> searchAtom ("=");
	if (directory == 0) return;
	true_atom = directory -> searchAtom ("true");
	false_atom = directory -> searchAtom ("false");
	null_atom = directory -> searchAtom ("null");
}

PrologNativeCode * PrologJSONServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "json") == 0) return new json_native_class (this);
	return 0;
}

PrologJSONServiceClass :: PrologJSONServiceClass (void) {root = 0; true_atom = false_atom = null_atom = equal_atom = 0;}
