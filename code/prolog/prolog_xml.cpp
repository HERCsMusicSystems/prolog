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
#include "../tinyxml2/tinyxml2.h"

void drop_level (int level) {while (level-- > 0) printf (" ");}

PrologAtom * drop_node (int level, tinyxml2 :: XMLElement * node, PrologRoot * root, PrologAtom * atom, PrologAtom * node_atom) {
	PrologAtom * machine_atom = new PrologAtom ();
	PrologElement * clause = root -> pair (root -> pair (root -> atom (atom),
		root -> pair (root -> atom (node_atom), root -> pair (root -> atom (machine_atom), root -> earth ()
		))), root -> earth ());
	root -> attachClause (clause);
	drop_level (level); printf ("node [%s] = \"%s\"\n", node -> Value (), node -> GetText ());
	level += 2;
	const tinyxml2 :: XMLAttribute * attr = node -> FirstAttribute ();
	while (attr != 0) {
		PrologAtom * attribute_atom = root -> search ((char *) attr -> Name ());
		if (attribute_atom == 0) attribute_atom = root -> createAtom ((char *) attr -> Name ());
		clause = root -> pair (root -> pair (root -> atom (machine_atom),
			root -> pair (root -> atom (attribute_atom), root -> pair (root -> text ((char *) attr -> Value ()), root -> earth ()
			))), root -> earth ());
		root -> attachClause (clause);
		drop_level (level); printf ("attr [%s] = [%s]\n", attr -> Name (), attr -> Value ());
		attr = attr -> Next ();
	}
	//clause = root -> pair (root -> pair (root -> atom (machine_atom), root -> pair (root -> text ((char *) node -> GetText ()), root -> earth ())), root -> earth ());
	char * text = (char *) node -> GetText ();
	clause = root -> pair (root -> pair (root -> atom (machine_atom), root -> pair (root -> text (text != 0 ? text : ""), root -> earth ())), root -> earth ());
	root -> attachClause (clause);
	return machine_atom;
}

class xml_native_class : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * drop_nodes (int level, tinyxml2 :: XMLElement * node, PrologRoot * root, PrologAtom * atom) {
		PrologAtom * node_atom = 0;
		while (node != 0) {
			node_atom = root -> search ((char *) node -> Value ());
			if (node_atom == 0) node_atom = root -> createAtom ((char *) node -> Value ());
			PrologAtom * machine_atom = drop_node (level, node, root, atom, node_atom);
			tinyxml2 :: XMLElement * el = node -> FirstChildElement ();
			drop_nodes (level + 2, el, root, machine_atom);
			node = node -> NextSiblingElement ();
		}
		return node_atom;
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
		if (atom -> isVar ()) atom -> setAtom (path != 0 ? new PrologAtom (path -> getText ()) : new PrologAtom ());
		if (path != 0) {
			tinyxml2 :: XMLDocument doc;
			if (tinyxml2 :: XML_NO_ERROR != doc . LoadFile ((const char *) path -> getText ())) return false;
			tinyxml2 :: XMLElement * node = doc . FirstChildElement ();
			if (node == 0) return false;
			PrologAtom * node_atom = drop_nodes (0, node, root, atom -> getAtom ());
			if (node_atom == 0) return false;
			parameters -> setAtom (node_atom);
			return true;
		}
		return true;
	}
	xml_native_class (PrologRoot * root) {this -> root = root;}
};

void PrologXMLServiceClass :: init (PrologRoot * root) {
	this -> root = root;
}

PrologNativeCode * PrologXMLServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "xml") == 0) return new xml_native_class (root);
	return 0;
}
