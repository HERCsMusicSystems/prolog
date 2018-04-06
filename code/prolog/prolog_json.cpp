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
	void drop (FILE * fw, int tab, PrologElement * el) {
		if (el -> isAtom ()) {
			PrologAtom * atom = el -> getAtom ();
			if (atom == service -> true_atom) fprintf (fw, "true");
			else if (atom == service -> false_atom) fprintf (fw, "false");
			else if (atom == service -> null_atom) fprintf (fw, "null");
			else fprintf (fw, "\"%s\"", atom -> name ());
			return;
		}
		if (el -> isText ()) {
			char * text = el -> getText ();
			fprintf (fw, "\"");
			while (* text != '\0') {
				if (* text == '\"') fprintf (fw, "\\\"");
				else if (* text == '\\') fprintf (fw, "\\\\");
				else fprintf (fw, "%c", * text);
				text++;
			}
			fprintf (fw, "\"");
			return;
		}
		if (el -> isInteger ()) {fprintf (fw, "%i", el -> getInteger ()); return;}
		if (el -> isDouble ()) {fprintf (fw, "%.15g", el -> getDouble ()); return;}
		if (el -> isPair ()) {
			PrologElement * left = el -> getLeft ();
			PrologElement * right = el -> getRight ();
			if (! right -> isPair () && ! right -> isEarth ()) {drop (fw, tab, left); fprintf (fw, ": "); drop (fw, tab, right); return;}
			return;
		}
		if (el -> isFail ()) {fprintf (fw, "false"); return;}
		if (el -> isSlash ()) {fprintf (fw, "true"); return;}
		if (el -> isVar ()) {fprintf (fw, "null"); return ;}
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
		if (path != 0 && json != 0) {
			FILE * fw = fopen (path -> getText (), "wb");
			drop (fw, 0, json);
			fclose (fw);
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
}

PrologNativeCode * PrologJSONServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "json") == 0) return new json_native_class (this);
	return 0;
}

PrologJSONServiceClass :: PrologJSONServiceClass (void) {root = 0; true_atom = false_atom = null_atom = 0;}
