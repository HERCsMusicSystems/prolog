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

class json_native_class : public PrologNativeCode {
public:
	PrologJSONServiceClass * service;
	void tabs (FILE * fw, int tabs) {while (tabs-- > 0) fprintf (fw, " ");}
	void multiline (FILE * fw, int tab, PrologElement * el, char bracket) {
		int tabss = tab + 2;
		bool comma = false;
		while (el -> isPair ()) {
			if (comma) fprintf (fw, ","); comma = true;
			fprintf (fw, "\n"); tabs (fw, tabss);
			drop (fw, tabss, el -> getLeft ());
			el = el -> getRight ();
		}
		fprintf (fw, "\n"); tabs (fw, tab); fprintf (fw, "%c", bracket);
	}
	int multiline (char * area, int ind, int tab, PrologElement * el, char bracket) {
		int tabss = tab + 2;
		bool comma = false;
		while (el -> isPair ()) {
			if (comma) ind = area_cat (area, ind, ", "); comma = true;
			ind = drop (area, ind, tabss, el -> getLeft ());
			el = el -> getRight ();
		}
		return area_cat (area, ind, bracket);
	}
	void drop_text (FILE * fw, char * text) {
		fprintf (fw, "\"");
		while (* text != '\0') {
			if (* text == '\"') fprintf (fw, "\\\"");
			else if (* text == '\\') fprintf (fw, "\\\\");
			else fprintf (fw, "%c", * text);
			text++;
		}
		fprintf (fw, "\"");
	}
	int drop_text (char * area, int ind, char * text) {
		ind = area_cat (area, ind, '\"');
		while (* text != '\0') {
			if (* text == '\"') ind = area_cat (area, ind, "\\\"");
			else if (* text == '\\') ind = area_cat (area, ind, "\\\\");
			else ind = area_cat (area, ind, * text);
			text++;
		}
		return area_cat (area, ind, '\"');
	}
	void drop (FILE * fw, int tab, PrologElement * el) {
		if (el -> isAtom ()) {
			PrologAtom * atom = el -> getAtom ();
			if (atom == service -> true_atom) fprintf (fw, "true");
			else if (atom == service -> false_atom) fprintf (fw, "false");
			else if (atom == service -> null_atom) fprintf (fw, "null");
			else drop_text (fw, atom -> name ());
			return;
		}
		if (el -> isText ()) {drop_text (fw, el -> getText ()); return;}
		if (el -> isInteger ()) {fprintf (fw, "%i", el -> getInteger ()); return;}
		if (el -> isDouble ()) {fprintf (fw, "%.15g", el -> getDouble ()); return;}
		if (el -> isPair ()) {
			PrologElement * left = el -> getLeft ();
			PrologElement * right = el -> getRight ();
			if (left -> isAtom () && left -> getAtom () == service -> equal_atom) {
				if (! right -> isPair ()) return; left = right -> getLeft (); right = right -> getRight ();
				if (! right -> isPair ()) return; right = right -> getLeft ();
				drop (fw, tab + 2, left); fprintf (fw, ": "); drop (fw, tab, right);
				return;
			}
			char left_bracket = '[', right_bracket = ']';
			if (left -> isPair ()) left = left -> getLeft ();
			if (left -> isAtom () && left -> getAtom () == service -> equal_atom) {left_bracket = '{'; right_bracket = '}';}
			fprintf (fw, "%c", left_bracket);
			multiline (fw, tab, el, right_bracket);
			return;
		}
		if (el -> isFail ()) {fprintf (fw, "false"); return;}
		if (el -> isSlash ()) {fprintf (fw, "true"); return;}
		if (el -> isVar ()) {fprintf (fw, "null"); return ;}
	}
	int drop (char * area, int ind, int tab, PrologElement * el) {
		if (el -> isAtom ()) {
			PrologAtom * atom = el -> getAtom ();
			if (atom == service -> true_atom) ind = area_cat (area, ind, "true");
			else if (atom == service -> false_atom) ind = area_cat (area, ind, "false");
			else if (atom == service -> null_atom) ind = area_cat (area, ind, "null");
			else ind = drop_text (area, ind, atom -> name ());
			return ind;
		}
		if (el -> isText ()) return drop_text (area, ind, el -> getText ());
		if (el -> isInteger ()) {PROLOG_STRING command; sprintf (command, "%i", el -> getInteger ()); return area_cat (area, ind, command);}
		if (el -> isDouble ()) {PROLOG_STRING command; sprintf (command, "%.15g", el -> getDouble ()); return area_cat (area, ind, command);}
		if (el -> isPair ()) {
			PrologElement * left = el -> getLeft ();
			PrologElement * right = el -> getRight ();
			if (left -> isAtom () && left -> getAtom () == service -> equal_atom) {
				if (! right -> isPair ()) return ind; left = right -> getLeft (); right = right -> getRight ();
				if (! right -> isPair ()) return ind; right = right -> getLeft ();
				ind = drop (area, ind, tab + 2, left); ind = area_cat (area, ind, ": "); return drop (area, ind, tab, right);
			}
			char left_bracket = '[', right_bracket = ']';
			if (left -> isPair ()) left = left -> getLeft ();
			if (left -> isAtom () && left -> getAtom () == service -> equal_atom) {left_bracket = '{'; right_bracket = '}';}
			ind = area_cat (area, ind, left_bracket);
			return multiline (area, ind, tab, el, right_bracket);
		}
		if (el -> isFail ()) return area_cat (area, ind, "false");
		if (el -> isSlash ()) return area_cat (area, ind, "true");
		if (el -> isVar ()) return area_cat (area, ind, "null");
		return ind;
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
			if (path == 0) {
				if (variable == 0) {drop (stdout, 0, json); printf ("\n");}
				else {AREA area; area [0] = '\0'; int ind = drop (area, 0, 0, json); area_cat (area, ind, '\0'); variable -> setText (area);}
			} if (path != 0) {FILE * fw = fopen (path -> getText (), "wb"); drop (fw, 0, json); fprintf (fw, "\n"); fclose (fw);}
			return true;
		}
		return false;
	}
	json_native_class (PrologJSONServiceClass * service) {this -> service = service;}
};

void PrologJSONServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
	if (directory == 0) return;
	true_atom = directory -> searchAtom ("true");
	false_atom = directory -> searchAtom ("false");
	null_atom = directory -> searchAtom ("null");
	PrologDirectory * studio = root -> searchDirectory ("studio");
	if (studio != 0) equal_atom =studio -> searchAtom ("=");
}

PrologNativeCode * PrologJSONServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "json") == 0) return new json_native_class (this);
	return 0;
}

PrologJSONServiceClass :: PrologJSONServiceClass (void) {root = 0; true_atom = false_atom = null_atom = equal_atom = 0;}
