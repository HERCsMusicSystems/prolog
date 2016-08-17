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

import javafx . scene . paint . Color;
import javafx . scene . canvas . GraphicsContext;

public class Token extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public PrologAtom atom;
	public String text = "";
	public Point rounding = new Point (0.0, 0.0);
	public Rect location = new Rect (new Point (0.0, 0.0), new Point (61.0, 61.0));
	public double rotation = 0.0;
	public Point scaling = new Point (1.0, 1.0);
	public int side = 0;
	public Rect indexing = new Rect (new Point (0.0, 0.0), new Point (4.0, 4.0));
	public boolean indexed = true;
	public Colour foreground;
	public Colour background;
	public boolean selected = false;
	public boolean locked = false;
	public Token next;
	public Color fgcc () {return Color . color (foreground . red, foreground . green, foreground . blue, foreground . alpha);}
	public Color bgcc () {return Color . color (background . red, background . green, background . blue, background . alpha);}
	public Point actionLocation = new Point (0.0, 0.0);
	public boolean doubleAction () {return false;}
	public boolean moveAction () {location . position = actionLocation; return true;}
	public boolean releaseAction () {return false;}
	public boolean can_insert () {return false;}
	public boolean hitTest (Point position) {
		if (locked) return selected = false;
		return selected = location . contains (position . add (location . size . half ()));
	}
	public void repaint () {}
	public void token_draw (GraphicsContext gc, Viewport v) {gc . save (); draw (gc, v); gc . restore ();}
	public void draw (GraphicsContext gc, Viewport v) {}
	public boolean moveOnGrid (Token token, Point position) {return false;}
	public int randomise_side () {return side;}
	private void set_2 (PrologElement var, double a, double b) {
		var . setPair (); var . getLeft () . setDouble (a); var = var . getRight ();
		var . setPair (); var . getLeft () . setDouble (b);
	}
	private void set_4 (PrologElement var, double a, double b, double c, double d) {
		var . setPair (); var . getLeft () . setDouble (a); var = var . getRight ();
		var . setPair (); var . getLeft () . setDouble (b); var = var . getRight ();
		var . setPair (); var . getLeft () . setDouble (c); var = var . getRight ();
		var . setPair (); var . getLeft () . setDouble (d);
	}
	private void set_4i (PrologElement var, Rect p) {
		var . setPair (); var . getLeft () . setInteger ((int) p . position . x); var = var . getRight ();
		var . setPair (); var . getLeft () . setInteger ((int) p . position . y); var = var . getRight ();
		var . setPair (); var . getLeft () . setInteger ((int) p . size . x); var = var . getRight ();
		var . setPair (); var . getLeft () . setInteger ((int) p . size . y);
	}
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		// CLOSE
		if (parameters . isEarth ()) {atom . setMachine (null); fxg . clean = false; programmatic_close (); return true;}
		PrologAtom at = null, grid = null;
		PrologElement a = null, b = null, c = null, d = null, var = null;
		String t = null;
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isAtom ()) {if (at == null) at = el . getAtom (); else grid = el . getAtom ();}
			else if (el . isNumber ()) {if (a == null) a = el; else if (b == null) b = el; else if (c == null) c = el; else d = el;}
			else if (el . isVar ()) var = el;
			else if (el . isText ()) t = "" + el . getText ();
			parameters = parameters . getRight ();
		}
		if (parameters . isVar ()) var = parameters;
		if (at == null) return false;
		// LOCATION
		if (at == fxg . location_atom) {
			if (var != null) {set_4 (var, location . position . x, location . position . y, location . size . x, location . size . y); return true;}
			if (b == null) return false;
			location . position = new Point (a . getNumber (), b . getNumber ());
			fxg . clean = false;
			if (c == null) return true;
			double cc = c . getNumber ();
			location . size = new Point (cc, d == null ? cc : d . getNumber ());
			sizeChanged ();
			return true;
		}
		// SIZE
		if (at == fxg . size_atom) {
			if (var != null) {set_2 (var, location . size . x, location . size . y); return true;}
			if (a == null) return false;
			double aa = a . getNumber ();
			location . size = new Point (aa, b == null ? aa : b . getNumber ());
			sizeChanged (); fxg . clean = false;
			return true;
		}
		// POSITION
		if (at == fxg . position_atom) {
			if (var != null) {Point position = getPosition (); set_2 (var, position . x, position . y); return true;}
			if (b == null) return false;
			fxg . clean = false;
			Point position = new Point (a . getNumber (), b . getNumber ());
			if (grid != null) {
				PrologNativeCode machine = grid . getMachine ();
				if (machine == null) return false;
				if (! (machine instanceof Token)) return false;
				Token token = (Token) machine;
				if (moveOnGrid (token, position)) return true;
				return token . moveOnGrid (this, position);
			}
			setPosition (position);
			return true;
		}
		// SCALING
		if (at == fxg . scaling_atom) {
			if (var != null) {set_2 (var, scaling . x, scaling . y); return true;}
			if (a == null) return false;
			double aa = a . getNumber ();
			scaling = new Point (aa, b == null ? aa : b . getNumber ());
			fxg . clean = false;
			return true;
		}
		// ROTATION
		if (at == fxg . rotation_atom) {
			if (var != null) {var . setPair (); var . getLeft () . setDouble (rotation); return true;}
			if (a == null) return false;
			rotation = a . getNumber ();
			fxg . clean = false;
			return true;
		}
		// SIDE
		if (at == fxg . side_atom) {
			if (var != null) {var . setInteger (side); return true;}
			if (a == null || ! a . isInteger ()) return false;
			int ind = a . getInteger ();
			if (ind < 0 || ind >= numberOfSides ()) return false;
			side = ind; sideChanged (); fxg . clean = false;
			return true;
		}
		// SIDES
		if (at == fxg . sides_atom) {
			if (var != null) {var . setInteger (numberOfSides ()); return true;}
			if (a == null || ! a . isInteger ()) return false;
			return setSides (a . getInteger ());
		}
		// INDEXING
		if (at == fxg . indexing_atom) {
			if (var != null) {set_4i (var, indexing); return true;}
			fxg . clean = false;
			if (a != null) indexing . position . x = a . getNumber (); else {indexed = ! indexed; return false;}
			if (b != null) indexing . position . y = b . getNumber ();
			if (c != null) indexing . size . x = c . getNumber ();
			if (d != null) indexing . size . y = d . getNumber ();
			return true;
		}
		if (at == fxg . indexed_atom) return indexed;
		// ROUNDING
		if (at == fxg . rounding_atom) {
			if (var != null) {set_2 (var, rounding . x, rounding . y); return true;}
			if (a == null) return false;
			double aa = a . getNumber ();
			rounding = new Point (aa, b == null ? aa : b . getNumber ());
			fxg . clean = false;
			return true;
		}
		// FOREGROUND / BACKGROUND
		if (at == fxg . foreground_atom || at == fxg . background_atom) {
			if (var != null) {Colour cl = at == fxg . foreground_atom ? foreground : background; set_4 (var, cl . red, cl . green, cl . blue, cl . alpha); return true;}
			if (c == null) return false;
			Colour cl = new Colour (a . getNumber (), b . getNumber (), c . getNumber (), d == null ? 1.0 : d . getNumber ());
			if (at == fxg . foreground_atom) foreground = cl; else background = cl;
			fxg . clean = false;
			return true;
		}
		// LOCKING
		if (at == fxg . lock_atom) {locked = true; fxg . clean = false; return true;}
		if (at == fxg . unlock_atom) {locked = false; fxg . clean = false; return true;}
		if (at == fxg . locked_atom) return locked;
		// SELECTING
		if (at == fxg . select_atom) {selected = true; fxg . clean = false; return true;}
		if (at == fxg . deselect_atom) {selected = false; fxg . clean = false; return true;}
		if (at == fxg . selected_atom) return selected;
		// ROLLING
		if (at == fxg . roll_atom) {
			int ret = randomise_side ();
			if (var != null) var . setInteger (ret);
			fxg . clean = false;
			return true;
		}
		if (at == fxg . text_atom) {
			if (var != null) {var . setText (text); return true;}
			if (t == null) return false;
			text = t;
			fxg . clean = false;
			return true;
		}
		// REPAINT
		if (at == fxg . repaint_atom) {repaint (); return true;}
		return false;
	}
	/*
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (atom -> getAtom () == select_deck_atom) {if (! token -> can_insert ()) return false; board -> deck = token; return true;}
		if (atom -> getAtom () == shuffle_atom) {return token -> shuffle ();}
		if (atom -> getAtom () == insert_atom) {
			if (parameters -> isEarth ()) {
				if (board -> deck == 0) return false;
				return board -> transfer_token_to_deck (board -> deck, token);
			}
			if (! parameters -> isPair ()) return false;
			PrologElement * deck_element = parameters -> getLeft (); if (! deck_element -> isAtom ()) return false;
			PrologAtom * deck_atom = deck_element -> getAtom (); if (deck_atom == 0) return false;
			PrologNativeCode * deck_machine = deck_atom -> getMachine (); if (deck_machine == 0) return false;
			if (! deck_machine -> isTypeOf (token_actions :: name ())) return false;
			boarder_token * deck_token = ((token_actions *) deck_machine) -> token; if (deck_token == 0) return false;
			if (deck_token -> can_insert ()) return board -> transfer_token_to_deck (deck_token, token);
			if (token -> can_insert ()) return board -> transfer_token_to_deck (token, deck_token);
			return false;
		}
		if (atom -> getAtom () == release_atom) {
			boarder_token * btp = board -> release_token_from_deck (token);
			if (btp == 0) return false;
			if (parameters -> isPair ()) parameters = parameters -> getLeft ();
			if (parameters -> isVar ()) parameters -> setAtom (btp -> atom);
			return true;
		}
		if (atom -> getAtom () == release_random_atom) {
			boarder_token * btp = board -> release_random_token_from_deck (token);
			if (btp == 0) return false;
			if (parameters -> isPair ()) parameters = parameters -> getLeft ();
			if (parameters -> isVar ()) parameters -> setAtom (btp -> atom);
			return true;
		}
		if (atom -> getAtom () == is_deck_atom) {return token -> can_insert ();}
		return false;
	}
	*/
	public Token insert (Token token) {return null;}
	public void erase () {programmatic_close (); atom . setMachine (null); if (next != null) next . erase ();}
	public void programmatic_close () {fxg . remove_token (this);}
	public void save (FileWriter tc) {if (next != null) next . save (tc);}
	public void setPosition (Point position) {location . position = new Point (position);}
	public Point getPosition () {return new Point (location . position . x, location . position . y);}
	public void sizeChanged () {}
	public void sideChanged () {}
	public boolean setSides (int ind) {return false;}
	public int numberOfSides () {return 1;}
	public Token (PrologFXGStudio fxg, PrologAtom atom, Colour foreground, Colour background, Token next) {
		this . fxg = fxg;
		this . atom = atom;
		this . foreground = new Colour (foreground);
		this . background = new Colour (background);
		this . next = next;
	}
}
