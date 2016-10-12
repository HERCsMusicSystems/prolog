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

import prolog . *;
import prolog . geometry . *;

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
	public boolean hitTest (Point position) {
		if (locked) return selected = false;
		Point size = location . size . times (scaling);
		return selected = new Rect (location . position, size) . contains (position . add (size . half ()));
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
	public void erase () {programmatic_close (); atom . setMachine (null); if (next != null) next . erase ();}
	public void programmatic_close () {fxg . remove_token (this);}
	public void save_stack (FileWriter tc) {if (next != null) next . save_stack (tc); save (tc);}
	public void save (FileWriter tc) {}
	public void save_location (FileWriter tc) {
		if (! location . position . eq (new Point (0.0, 0.0)))
			try {tc . write ("[" + atom . name () + " Location " + location . position . x + " " + location . position . y + "]\n");} catch (Exception ex) {}
	}
	public void save_location_and_size (FileWriter tc) {
		try {
			tc . write ("[" + atom . name () + " Location " + location . position . x + " " + location . position . y + " "
			+ location . size . x + " " + location . size . y + "]\n");
		} catch (Exception e) {}
	}
	public void save_scaling (FileWriter tc) {
		if (! scaling . eq (new Point (1.0, 1.0)))
			try {tc . write ("[" + atom . name () + " Scaling " + scaling . x + " " + scaling . y + "]\n");} catch (Exception ex) {}
	}
	public void save_rotation (FileWriter tc) {
		if (rotation != 0.0) try {tc . write ("[" + atom . name () + " Rotation " + rotation + "]\n");} catch (Exception ex) {}
	}
	public void save_rounding (FileWriter tc) {
		if (! rounding . eq (new Point (0.0, 0.0)))
			try {tc . write ("[" + atom . name () + " Rounding " + rounding . x + " " + rounding . y + "]\n");} catch (Exception ex) {}
	}
	public void save_index (FileWriter tc) {
		if (! indexing . position . eq (new Point (0.0, 0.0)))
			try {
				tc . write ("[" + atom . name () + " Indexing " + indexing . position . x + " " + indexing . position . y + "]\n");
			} catch (Exception ex) {}
	}
	public void save_indexing (FileWriter tc) {
		try {
			tc . write ("[" + atom . name () + " Indexing " + indexing . position . x + " " + indexing . position . y + " "
			+ indexing . size . x + " " + indexing . size . y + "]\n");
			if (! indexed) tc . write ("[" + atom . name () + " Indexing]\n");
		} catch (Exception ex) {}
	}
	public void save_lock (FileWriter tc) {if (locked) try {tc . write ("[" + atom . name () + " Lock]\n");} catch (Exception ex) {}}
	public void save_foreground (FileWriter tc, Colour c) {
		if (! foreground . eq (c))
			try {
				tc . write ("[" + atom . name () + " ForegroundColour " + foreground . red + " " + foreground . green + " " + foreground . blue);
				if (foreground . alpha != c . alpha) tc . write (" " + foreground . alpha);
				tc . write ("]\n");
			} catch (Exception ex) {}
	}
	public void save_background (FileWriter tc, Colour c) {
		if (! background . eq (c))
			try {
				tc . write ("[" + atom . name () + " BackgroundColour " + background . red + " " + background . green + " " + background . blue);
				if (background . alpha != c . alpha) tc . write (" " + background . alpha);
				tc . write ("]\n");
			} catch (Exception ex) {}
	}
	public void save_side (FileWriter tc) {if (side != 0) try {tc . write ("[" + atom . name () + " Side " + side + "]\n");} catch (Exception ex) {}}
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
