///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2007 Robert P. Wolf                       //
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

#include "prolog_xml.h"
#include "prolog_studio.h"
#include "../tinyxml2/tinyxml2.h"

class xml_native_class : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atomC (char * name) {
		PrologAtom * atom = root -> search (name);
		if (atom == 0) atom = root -> createAtom (name);
		return atom;
	}
	void drop_node (PrologAtom * atom, tinyxml2 :: XMLElement * node) {
		char * node_name = (char *) node -> Name ();
		AREA area;
		int ap = area_cat (area, 0, "<"); ap = area_cat (area, ap, node_name); ap = area_cat (area, ap, ">");
		PrologAtom * node_atom = new PrologAtom (area);
		root -> attachClause (root -> pair (root -> pair (root -> atom (atom),
											root -> pair (root -> atom (atomC (node_name)),
											root -> pair (root -> atom (node_atom),
											root -> earth ()))), root -> earth ()));
		const tinyxml2 :: XMLAttribute * attr = node -> FirstAttribute ();
		term_reader reader;
		while (attr != 0) {
			PrologAtom * attribute_atom = atomC ((char *) attr -> Name ());
			if (attribute_atom == 0) return;
			reader . init (root, (char *) attr -> Value ());
			PrologElement * el = reader . readElement ();
			if (el == 0) el = root -> text ((char *) attr -> Value ());
			else if (el -> isAtom () && strcmp (el -> getAtom () -> name (), (char *) attr -> Value ()) != 0) {
				delete el;
				el = root -> atomC ((char *) attr -> Value ());
			}
			root -> attachClause (root -> pair (root -> pair (root -> atom (node_atom),
				root -> pair (root -> atom (attribute_atom), root -> pair (el, root -> earth ()
				))), root -> earth ()));
			attr = attr -> Next ();
		}
		char * text = (char *) node -> GetText ();
		if (text != 0) {root -> attachClause (root -> pair (root -> pair (root -> atom (node_atom), root -> pair (root -> text (text), root -> earth ())), root -> earth ()));}
		tinyxml2 :: XMLElement * child = node -> FirstChildElement ();
		while (child != 0) {drop_node (node_atom, child); child = child -> NextSiblingElement ();}
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * path = 0;
		PrologElement * atom = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) path = el;
			if (el -> isAtom ()) atom = el;
			if (el -> isVar ()) atom = el;
			parameters = parameters -> getRight ();
		}
		if (atom == 0 || path == 0) return false;
		tinyxml2 :: XMLDocument doc;
		if (tinyxml2 :: XML_NO_ERROR != doc . LoadFile ((const char *) path -> getText ())) return false;
		tinyxml2 :: XMLElement * node = doc . FirstChildElement ();
		if (node == 0) return false;
		if (atom -> isVar ()) atom -> setAtom (atomC ((char *) node -> Name ()));
		drop_node (atom -> getAtom (), node);
		return true;
	}
	xml_native_class (PrologRoot * root) {this -> root = root;}
};

void PrologXMLServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
}

PrologNativeCode * PrologXMLServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "xml") == 0) return new xml_native_class (root);
	return 0;
}
