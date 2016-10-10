///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2016 Robert P. Wolf                       //
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

package prolog;

import prolog . studio . TermReader;
import javax . xml . parsers . *;
import org . w3c . dom . *;

class xml_native_class extends PrologNativeCode {
	public PrologRoot root;
	public PrologAtom atomC (String name) {
		PrologAtom ret = root . search (name);
		if (ret == null) ret = root . createAtom (name);
		return ret;
	}
	public void drop_node (PrologAtom atom, Node node) {
		if (node . getNodeType () == Node . TEXT_NODE) {
			String text = node . getNodeValue ();
			if (text . trim () . length () < 1) return;
			root . attachClause (root . pair (root . pair (root . atom (atom), root . pair (root . text (text), root . earth ())), root . earth ())); return;
		}
		if (node . getNodeType () != Node . ELEMENT_NODE) return;
		String node_name = node . getNodeName ();
		PrologAtom node_atom = new PrologAtom ("<" + node_name + ">");
		root . attachClause (root . pair (  root . pair (root . atom (atom),
											root . pair (root . atom (atomC (node_name)),
											root . pair (root . atom (node_atom),
											root . earth ()))), root . earth ()));
		NamedNodeMap attributes = node . getAttributes ();
		int attribute_count = attributes . getLength ();
		for (int ind = 0; ind < attribute_count; ind++) {
			Attr attribute = (Attr) attributes . item (ind);
			String vv = attribute . getNodeValue ();
			PrologElement v = null;
			try {
				v = root . integer (Integer . parseInt (vv));
			} catch (Exception ex) {
				try {
					v = root . Double (Double . parseDouble (vv));
				} catch (Exception exx) {
					v = root . atomC (vv);
				}
			}
			root . attachClause (root . pair (  root . pair (root . atom (node_atom),
												root . pair (root . atom (atomC (attribute . getNodeName ())),
												root . pair (v,
												root . earth ()))), root . earth ()));
		}
		Node child = node . getFirstChild ();
		while (child != null) {drop_node (node_atom, child); child = child . getNextSibling ();}
	}
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologElement path = null;
		PrologElement atom = null;
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isText ()) path = el;
			if (el . isAtom ()) atom = el;
			if (el . isVar ()) atom = el;
			parameters = parameters . getRight ();
		}
		if (atom == null || path == null) return false;
		Document doc = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory . newInstance ();
			factory . setIgnoringComments (true);
			doc = factory . newDocumentBuilder () . parse (path . getText ());
		} catch (Exception ex) {return false;}
		if (doc == null) return false;
		Node node = doc . getFirstChild ();
		if (node == null) return false;
		if (atom . isVar ()) atom . setAtom (atomC (node . getNodeName ()));
		drop_node (atom . getAtom (), node);
		return true;
	}
	public xml_native_class (PrologRoot root) {this . root = root;}
}

public class xml extends PrologServiceClass {
	public PrologRoot root;
	public void init (PrologRoot root, PrologDirectory directory) {this . root = root;}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("xml")) return new xml_native_class (root);
		return null;
	}
}

