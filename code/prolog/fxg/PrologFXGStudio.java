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

package fxg;

import Prolog . *;
import Prolog . geometry . *;

import java . io . FileWriter;

class viewport_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologElement name = null;
		PrologElement atom = null;
		double locations [] = {100.0, 100.0, 160.0, 90.0};
		int location_index = 0;
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isAtom ()) atom = el;
			if (el . isVar ()) atom = el;
			if (el . isText ()) name = el;
			if (el . isNumber () && location_index < 4) locations [location_index++] = el . getNumber ();
			parameters = parameters . getRight ();
		}
		if (atom == null) return false;
		if (atom . isVar ()) atom . setAtom (new PrologAtom ());
		if (! atom . isAtom ()) return false;
		if (atom . getAtom () . getMachine () != null) return false;
		Rect location = new Rect (locations [0], locations [1], locations [2], locations [3]);
		fxg . viewports = new Viewport (fxg, atom . getAtom (), name != null ? name . getText () : atom . getAtom () . name (), location, fxg . viewports);
		if (! atom . getAtom () . setMachine (fxg . viewports)) return false;
		return true;
	}
	public viewport_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class erase_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {fxg . erase (); return true;}
	public erase_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class clean_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {fxg . clean = true; return true;}
	public clean_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class is_clean_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {return fxg . clean;}
	public is_clean_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class save_board_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false; parameters = parameters . getLeft (); if (! parameters . isText ()) return false;
		fxg . save (parameters . getText ());
		return true;
	}
	public save_board_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

public class PrologFXGStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public PrologAtom location_atom, size_atom, position_atom, scaling_atom, repaint_atom, mode_atom;
	public boolean clean = true;
	public Viewport viewports;
	public void remove_viewport (Viewport viewport) {
		if (viewports == null) return;
		if (viewports == viewport) {viewports = viewports . next; return;}
		Viewport v = viewports;
		while (v . next != null) {if (v . next == viewport) {v . next = v . next . next; return;} v = v . next;}
	}
	public void save (String file_name) {
		try {
			FileWriter tc = new FileWriter (file_name);
			tc . write ("[auto_atoms]\n\n");
			tc . write ("[Erase]\n");
			if (viewports != null) viewports . save (tc);
			tc . write ("[wait 100]\n");
			tc . write ("[Clean]\n");
			tc . write ("[exit]\n\n");
			tc . close ();
		} catch (Exception ex) {return;}
		clean = true;
	}
	public void erase () {
		if (viewports != null) viewports . erase (); viewports = null;
		clean = true;
	}
	public void init (PrologRoot root, PrologDirectory directory) {
		this . root = root; this . directory = directory;
		if (location_atom == null && directory != null) {
			location_atom = directory . searchAtom ("Location");
			size_atom = directory . searchAtom ("Size");
			position_atom = directory . searchAtom ("Position");
			scaling_atom = directory . searchAtom ("Scaling");
			repaint_atom = directory . searchAtom ("Repaint");
			mode_atom = directory . searchAtom ("Mode");
		}
	}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("Viewport")) return new viewport_class (this);
		if (name . equals ("Erase")) return new erase_class (this);
		if (name . equals ("Clean")) return new clean_class (this);
		if (name . equals ("Clean?")) return new is_clean_class (this);
		if (name . equals ("SaveBoard")) return new save_board_class (this);
		return null;
	}
}

