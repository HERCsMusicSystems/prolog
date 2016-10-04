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

import java . io . FileOutputStream;

public class FileWriter extends PrologNativeCode {
	public FileOutputStream fw;
	public PrologAtom atom;
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (fw == null) return false;
		if (parameters . isEarth ()) {try {fw . close ();} catch (Exception ex) {} atom . setMachine (null); return true;}
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			try {
				if (el . isText ()) fw . write (el . getText () . getBytes ());
				if (el . isInteger ()) {
					int i = el . getInteger ();
					if (i >= 0) {
						if (i <= 0xff) fw . write (i);
						else if (i <= 0xffff) {fw . write (i >> 8); fw . write (i & 0xff);}
						else if (i <= 0xffffff) {fw . write (i >> 16); fw . write ((i >> 8) & 0xff); fw . write (i & 0xff);}
						else if (i <= 0xffffffff) {fw . write (i >> 24); fw . write ((i >> 16) & 0xff); fw . write ((i >> 8) & 0xff); fw . write (i & 0xff);}
					} else {
						i = -i;
						if (i <= 0x7f) fw . write (i);
						else if (i <= 0x7ff) {
							int msb = 0xc0 | (i >> 6);
							int lsb = 0x80 | (i & 0x3f);
							fw . write (msb); fw . write (lsb);
						} else if (i <= 0xffff) {
							int hsb = 0xe0 | (i >> 12);
							int msb = 0x80 | ((i >> 6) & 0x3f);
							int lsb = 0x80 | (i & 0x3f);
							fw . write (hsb); fw . write (msb); fw . write (lsb);
						} else if (i <= 0x1fffff) {
							int xlsb = 0xf0 | (i >> 18);
							int hsb = 0x80 | ((i >> 12) & 0x3f);
							int msb = 0x80 | ((i >> 6) & 0x3f);
							int lsb = 0x80 | (i & 0x3f);
							fw . write (xlsb); fw . write (hsb); fw . write (msb); fw . write (lsb);
						} else if (i <= 0x3ffffff) {
							int xmsb = 0xf8 | (i >> 24);
							int xlsb = 0x80 | ((i >> 18) & 0x3f);
							int hsb = 0x80 | ((i >> 12) & 0x3f);
							int msb = 0x80 | ((i >> 6) & 0x3f);
							int lsb = 0x80 | (i & 0x3f);
							fw . write (xmsb); fw . write (xlsb); fw . write (hsb); fw . write (msb); fw . write (lsb);
						} else if (i <= 0x7fffffff) {
							int xhsb = 0xfc | (i >> 30);
							int xmsb = 0x80 | ((i >> 24) & 0x3f);
							int xlsb = 0x80 | ((i >> 18) & 0x3f);
							int hsb = 0x80 | ((i >> 12) & 0x3f);
							int msb = 0x80 | ((i >> 6) & 0x3f);
							int lsb = 0x80 | (i & 0x3f);
							fw . write (xhsb); fw . write (xmsb); fw . write (xlsb); fw . write (hsb); fw . write (msb); fw . write (lsb);
						}
					}
				}
				while (el . isPair ()) {fw . write (root . getValue (el . getLeft ()) . getBytes ()); el = el . getRight ();}
			} catch (Exception ex) {}
			parameters = parameters . getRight ();
		}
		return true;
	}
	public FileWriter (PrologAtom atom, PrologRoot root, String name) {
		this . atom = atom;
		this . root = root;
		try {this . fw = new FileOutputStream (name);} catch (Exception ex) {this . fw = null;}
	}
	public FileWriter (PrologAtom atom, PrologRoot root, FileOutputStream fw) {
		this . atom = atom;
		this . root = root;
		this . fw = fw;
	}
}
