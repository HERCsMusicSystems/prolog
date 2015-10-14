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

class studio_code extends PrologNativeCode {
	public String name;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		System . out . println ("CALLED [" + name + "]");
		return true;
	}
	public studio_code (String name) {this . name = name;}
}

class write extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isText ()) root . print (el . getText ());
			if (el . isInteger ()) root . print_character (el . getInteger ());
			while (el . isPair ()) {
				root . print (root . getValue (el . getLeft ()));
				el = el . getRight ();
			}
			parameters = parameters . getRight ();
		}
		return true;
	}
	public write (PrologRoot root) {this . root = root;}
};

class nl extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . print (root . new_line_caption); return true;}
	public nl (PrologRoot root) {this . root = root;}
}

class pr extends PrologNativeCode {
	public PrologReader stdr;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement dup = stdr . readElement ();
		if (dup != null) {parameters . setLeft (dup); return true;}
		return false;
	}
	public pr (PrologReader stdr) {this . stdr = stdr;}
}

class preprocessor extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologAtom preprocessor = root . getPreprocessor ();
		if (parameters . isEarth ()) {root . setPreprocessor (null); return true;}
		if (! parameters . isPair ()) {if (preprocessor != null) parameters . setAtom (preprocessor); else parameters . setEarth (); return true;}
		parameters = parameters . getLeft ();
		if (parameters . isAtom ()) {root . setPreprocessor (parameters . getAtom ()); return true;}
		if (parameters . isEarth ()) {root . setPreprocessor (null); return true;}
		if (parameters . isVar ()) {if (preprocessor != null) parameters . setAtom (preprocessor); else parameters . setEarth (); return true;}
		return false;
	}
	public preprocessor (PrologRoot root) {this . root = root;}
}

class is_atom extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isAtom ();}
}

class PrologStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public PrologReader stdr = new PrologReader ();
	public PrologStudio () {
		System . out . println ("Studio created.");
	}
	public void init (PrologRoot root, PrologDirectory directory) {this . root = root; this . directory = directory; stdr . setRoot (root);}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("write")) return new write (root);
		if (name . equals ("nl")) return new nl (root);
		if (name . equals ("pr")) return new pr (stdr);
		if (name . equals ("preprocessor")) return new preprocessor (root);
		if (name . equals ("is_atom")) return new is_atom ();
		return new studio_code (name);
	}
}
