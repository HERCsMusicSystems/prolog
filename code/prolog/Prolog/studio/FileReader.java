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

package Prolog . studio;

import Prolog . *;

import java . io . FileInputStream;

public class FileReader extends PrologNativeCode {
	public FileInputStream fi;
	public SymbolReader sr;
	public PrologAtom atom;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (fi == null) {if (! parameters . isEarth ()) return false; atom . setMachine (null); return true;}
		if (parameters . isEarth ()) {try {fi . close ();} catch (Exception ex) {} fi = null; atom . setMachine (null); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement el = parameters . getLeft ();
		if (el . isText ()) {
			parameters = parameters . getRight ();
			if (! parameters . isPair ()) return false;
			String area = sr . getString (el . getText ());
			if (area == null) return false;
			parameters . getLeft () . setText (area);
			return true;
		}
		el = sr . readElement ();
		if (el == null) return false;
		parameters . setLeft (el);
		return true;
	}
	public FileReader (PrologAtom atom, PrologRoot root, String file_name) {
		this . atom = atom;
		try {fi = new FileInputStream (file_name);} catch (Exception ex) {fi = null;}
		sr = new SymbolReader (root, fi);
	}
	public FileReader (PrologAtom atom, PrologRoot root, FileInputStream fi) {
		this . atom = atom;
		this . fi = fi;
		sr = new SymbolReader (root, fi);
	}
}

