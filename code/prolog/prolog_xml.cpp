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

class xml_term_reader : public PrologReader {
public:
	char * text;
	virtual void message (char * text) {}
	virtual int move_z (void) {
		if (text == 0) return -1;
		if (* text == '\0') return -1;
		return * text++;
	}
	void init (char * text) {this -> text = text; act_znak = '\0';}
};

class xml_native_class : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * drop_node (int level, tinyxml2 :: XMLElement * node, xml_term_reader * reader, PrologAtom * atom, PrologAtom * node_atom) {
		AREA area;
		int ap = area_cat (area, 0, "<"); ap = area_cat (area, ap, (char *) node -> Name ()); ap = area_cat (area, ap, ">");
		PrologAtom * machine_atom = new PrologAtom (area);
		PrologElement * clause = root -> pair (root -> pair (root -> atom (atom),
			root -> pair (root -> atom (node_atom), root -> pair (root -> atom (machine_atom), root -> earth ()
			))), root -> earth ());
		root -> attachClause (clause);
		level += 2;
		const tinyxml2 :: XMLAttribute * attr = node -> FirstAttribute ();
		while (attr != 0) {
			PrologAtom * attribute_atom = reader -> searchAtomC ((char *) attr -> Name ());
			if (attribute_atom == 0) return 0;
			reader -> init ((char *) attr -> Value ());
			PrologElement * el = reader -> readElement ();
			if (el == 0) {
				el = root -> text ((char *) attr -> Value ());
			}
			if (el -> isAtom () && strcmp (el -> getAtom () -> name (), (char *) attr -> Value ()) != 0) {
				delete el;
				el = root -> text ((char *) attr -> Value ());
			}
			clause = root -> pair (root -> pair (root -> atom (machine_atom),
				root -> pair (root -> atom (attribute_atom), root -> pair (el, root -> earth ()
				))), root -> earth ());
			root -> attachClause (clause);
			attr = attr -> Next ();
		}
		char * text = (char *) node -> GetText ();
		clause = root -> pair (root -> pair (root -> atom (machine_atom), root -> pair (
			root -> text (text != 0 ? text : (char *) ""),
			root -> earth ())), root -> earth ());
		root -> attachClause (clause);
		return machine_atom;
	}
	PrologAtom * drop_nodes (int level, tinyxml2 :: XMLElement * node, xml_term_reader * reader, PrologAtom * atom) {
		PrologAtom * node_atom = 0;
		while (node != 0) {
			node_atom = reader -> searchAtomC ((char *) node -> Value ());
			if (node_atom == 0) return 0;
			PrologAtom * machine_atom = drop_node (level, node, reader, atom, node_atom);
			tinyxml2 :: XMLElement * nel = node -> FirstChildElement ();
			drop_nodes (level + 2, nel, reader, machine_atom);
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
		if (atom == 0 || path == 0) return false;
		tinyxml2 :: XMLDocument doc;
		if (tinyxml2 :: XML_NO_ERROR != doc . LoadFile ((const char *) path -> getText ())) return false;
		tinyxml2 :: XMLElement * node = doc . FirstChildElement ();
		if (node == 0) return false;
		if (atom -> isVar ()) atom -> setAtom (path != 0 ? new PrologAtom ((char *) node -> Name ()) : new PrologAtom ());
		xml_term_reader reader;
		reader . setRoot (root);
		PrologAtom * node_atom = drop_nodes (0, node, & reader, atom -> getAtom ());
		if (node_atom == 0) return false;
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
