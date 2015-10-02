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

import java . io . InputStream;

public class PrologLoader extends PrologReader {
	public boolean drop_main = false;
	public boolean echo = false;
	public boolean reload = false;
	public int old_caption_id = 0;
	public boolean old_auto_atoms = false;
	public InputStream fi = null;
	public PrologLoader (PrologRoot root) {setRoot (root);}
	public void close () {if (fi != null) try {fi . close ();} catch (Exception ex) {} fi = null;}
	public int move_z () {
		if (fi == null) return -1;
		int ch;
		try {ch = fi . read ();} catch (Exception ex) {return -1;}
		if (echo && ch > 0) root . print_character (ch);
		return ch;
	}
	public boolean load (String file_name) {
		drop_main = false;
		old_caption_id = root . captionId ();
		old_auto_atoms = root . autoAtoms ();
		root . setCaptions (0, false);
		boolean ret = LOAD (file_name);
		clear_context ();
		root . setCaptions (old_caption_id, old_auto_atoms);
		return ret;
	}
	public boolean load_without_main (String file_name) {
		drop_main = true;
		old_caption_id = root . captionId ();
		old_auto_atoms = root . autoAtoms ();
		root . setCaptions (0, false);
		boolean ret = LOAD (file_name);
		clear_context ();
		root . setCaptions (old_caption_id, old_auto_atoms);
		return ret;
	}
	public boolean LOAD (String file_name) {
		System . out . println ("LOADING: " + file_name);
		return false;
	}
}
