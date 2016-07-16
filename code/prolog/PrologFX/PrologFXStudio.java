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

package PrologFX;

import Prolog . *;

class fx_start extends PrologNativeCode {
	public PrologRoot root = null;
	public java . io . PrintStream oout = null;
	public boolean code (PrologElement parameters, PrologResolution resolution) {PrologMainFX . main (new String [0], root, oout); return true;}
	fx_start (PrologRoot root, java . io . PrintStream oout) {this . root = root; this . oout = oout;}
}

class fx_stop extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {PrologMainFX . stop_fx (); return true;}
}

public class PrologFXStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public java . io . PrintStream oout;
	public void init (PrologRoot root, PrologDirectory directory) {
		this . root = root; this . directory = directory;
		java . io . PipedInputStream pipedInput = new java . io . PipedInputStream ();
		java . io . PipedOutputStream pipedOutput = new java . io . PipedOutputStream ();
		oout = new java . io . PrintStream (pipedOutput);
		try {pipedInput . connect (pipedOutput);} catch (Exception ex) {ex . printStackTrace ();}
		root . insertReader (pipedInput);
	}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("fx_start")) return new fx_start (root, oout);
		if (name . equals ("fx_stop")) return new fx_stop ();
		return null;
	}
}
