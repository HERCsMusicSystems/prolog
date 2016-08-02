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
	public boolean main;
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
		fxg . viewports = new Viewport (fxg, atom . getAtom (),
				fxg . default_viewport_foreground, fxg . default_viewport_background,
				name != null ? name . getText () : atom . getAtom () . name (),
				location, main, fxg . viewports);
		if (! atom . getAtom () . setMachine (fxg . viewports)) return false;
		return true;
	}
	public viewport_class (PrologFXGStudio fxg, boolean main) {this . fxg = fxg; this . main = main;}
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

class repaint_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {fxg . repaint (); return true;}
	public repaint_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class create_rectangle_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement atom = parameters . getLeft ();
		if (atom . isVar ()) atom . setAtom (new PrologAtom ());
		if (! atom . isAtom ()) return false;
		if (atom . getAtom () . getMachine () != null) return false;
		Token machine = new RectangleToken (fxg, atom . getAtom (), fxg . default_rectangle_foreground, fxg . default_rectangle_background, null);
		if (! atom . getAtom () . setMachine (machine)) return false;
		fxg . insert_token (machine);
		fxg . clean = false;
		return true;
	}
	public create_rectangle_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

public class PrologFXGStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public PrologAtom location_atom, size_atom, position_atom, scaling_atom, repaint_atom, rotation_atom, rounding_atom;
	public PrologAtom side_atom, sides_atom;
	public PrologAtom foreground_atom, background_atom;
	public boolean clean = true;
	public Token viewports;
	public Token tokens;
	public Token deck;
	public Colour default_viewport_foreground = new Colour (0.0, 0.0, 0.0), default_viewport_background = new Colour (0.0, 0.0, 1.0);
	public Colour default_rectangle_foreground = new Colour (1.0, 1.0, 0.0), default_rectangle_background = new Colour (0.0, 0.0, 1.0);
	public void repaint () {
		Token v = viewports;
		while (v != null) {v . repaint (); v = v . next;}
	}
	public void draw (javafx . scene . canvas . GraphicsContext gc, Viewport v) {
		Token t = tokens;
		while (t != null) {t . token_draw (gc, v); t = t . next;}
	}
	public Token insert_token (Token token) {
		if (deck != null) {
			if (! deck . can_insert ()) return null;
			return deck . insert (token);
		}
		token . next = tokens;
		return tokens = token;
	}
	public void remove_token (Token token) {
		if (tokens == token) {tokens = tokens . next; token . next = null; return;}
		Token t = tokens;
		while (t . next != null) {
			if (t . next == token) {t . next = token . next; token . next = null; return;}
			t = t . next;
		}
	}
	public void remove_viewport (Token viewport) {
		if (viewports == null) return;
		if (viewports == viewport) {viewports = viewports . next; return;}
		Token v = viewports;
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
		if (tokens != null) tokens . erase (); tokens = null;
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
			rotation_atom = directory . searchAtom ("Rotation");
			rounding_atom = directory . searchAtom ("Rounding");
			side_atom = directory . searchAtom ("Side");
			sides_atom = directory . searchAtom ("Sides");
			foreground_atom = directory . searchAtom ("ForegroundColour");
			background_atom = directory . searchAtom ("BackgroundColour");
		}
	}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("Viewport")) return new viewport_class (this, false);
		if (name . equals ("MainViewport")) return new viewport_class (this, true);
		if (name . equals ("Erase")) return new erase_class (this);
		if (name . equals ("Clean")) return new clean_class (this);
		if (name . equals ("Clean?")) return new is_clean_class (this);
		if (name . equals ("SaveBoard")) return new save_board_class (this);
		if (name . equals ("Repaint")) return new repaint_class (this);
		if (name . equals ("CreateRectangle")) return new create_rectangle_class (this);
		return null;
	}
}

