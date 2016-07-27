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

import javafx . scene . paint . Color;
import javafx . scene . canvas . GraphicsContext;

public class Token extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public PrologAtom atom;
	public Point rounding = new Point (0.0, 0.0);
	public Rect location = new Rect (new Point (10.0, 10.0), new Point (60.0, 60.0));
	public double rotation = 0.0;
	public Colour foreground;
	public Colour background;
	public Token next;
	public Color fgcc () {return Color . color (foreground . red, foreground . green, foreground . blue, foreground . alpha);}
	public Color bgcc () {return Color . color (background . red, background . green, background . blue, background . alpha);}
	public boolean can_insert () {return false;}
	public void token_draw (GraphicsContext gc, Viewport v) {gc . save (); draw (gc, v); gc . restore ();}
	public void draw (GraphicsContext gc, Viewport v) {}
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {atom . setMachine (null); fxg . remove_token (this); fxg . clean = false; return true;}
		if (! parameters . isPair ()) return false;
		PrologElement atom = parameters . getLeft (); parameters = parameters . getRight ();
		if (! atom . isAtom ()) return false;
		if (atom . getAtom () == fxg . location_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (location . position . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . position . y); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . size . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . size . y); return true;
			}
			if (! parameters . isPair ()) return false; PrologElement x = parameters . getLeft (); if (! x . isNumber ()) return false; parameters = parameters . getRight ();
			if (! parameters . isPair ()) return false; PrologElement y = parameters . getLeft (); if (! y . isNumber ()) return false; parameters = parameters . getRight ();
			if (! parameters . isPair ()) return false; PrologElement w = parameters . getLeft (); if (! w . isNumber ()) return false; parameters = parameters . getRight ();
			if (parameters . isEarth ()) {
				location = new Rect (new Point (x . getNumber (), y . getNumber ()), new Point (w . getNumber (), w . getNumber ())); fxg . clean = false; return true;
			}
			if (! parameters . isPair ()) return false; PrologElement h = parameters . getLeft (); if (! h . isNumber ()) return false;
			location = new Rect (new Point (x . getNumber (), y . getNumber ()), new Point (w . getNumber (), h . getNumber ())); fxg . clean = false; return true;
		}
		if (atom . getAtom () == fxg . rotation_atom) {
			if (parameters . isVar ()) {parameters . setPair (); parameters . getLeft () . setDouble (rotation); return true;}
			if (! parameters . isPair ()) return false;
			PrologElement r = parameters . getLeft (); if (! r . isNumber ()) return false; rotation = r . getNumber (); return true;
		}
		if (atom . getAtom () == fxg . rounding_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (rounding . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (rounding . y); return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement x = parameters . getLeft (); if (! x . isNumber ()) return false;
			parameters = parameters . getRight (); if (parameters . isEarth ()) {rounding . x = rounding . y = x . getNumber (); return true;}
			if (! parameters . isPair ()) return false;
			PrologElement y = parameters . getLeft (); if (! y . isNumber ()) return false;
			rounding . x = x . getNumber (); rounding . y = y . getNumber (); return true;
		}
		return true;
	}
	/*
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (atom -> getAtom () == position_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair ();
				rect token_location = token -> get_location ();
				parameters -> getLeft () -> setInteger ((int) token_location . position . x); parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) token_location . position . y);
				return true;
			}
			if (! parameters -> isPair ()) return false; PrologElement * x = parameters -> getLeft ();
			if (x -> isAtom ()) {
				PrologNativeCode * machine = x -> getAtom () -> getMachine ();
				if (machine == 0) return false;
				if (! machine -> isTypeOf (token_actions :: name ())) return false;
				parameters = parameters -> getRight ();
				if (! parameters -> isPair ()) return false; x = parameters -> getLeft (); if (! x -> isInteger ()) return false; parameters = parameters -> getRight ();
				if (! parameters -> isPair ()) return false; PrologElement * y = parameters -> getLeft (); if (! y -> isInteger ()) return false;
				if (token -> moveOnGrid (((token_actions *) machine) -> token, point (x -> getInteger (), y -> getInteger ()))) return true;
				return ((token_actions *) machine) -> token -> moveOnGrid (token, point (x -> getInteger (), y -> getInteger ()));
			}
			if (! x -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * y = parameters -> getLeft (); if (! y -> isInteger ()) return false; parameters = parameters -> getRight ();
			token -> set_position (point (x -> getInteger (), y -> getInteger ()));
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == size_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair ();
				rect token_location = token -> get_location ();
				parameters -> getLeft () -> setInteger ((int) token_location . size . x); parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) token_location . size . y);
				return true;
			}
			if (! parameters -> isPair ()) return false; PrologElement * width = parameters -> getLeft (); if (! width -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (parameters -> isEarth ()) {token -> set_size (point (width -> getInteger (), width -> getInteger ())); boarder_clean = false; return true;}
			if (! parameters -> isPair ()) return false; PrologElement * height = parameters -> getLeft (); if (! height -> isInteger ()) return false; parameters = parameters -> getRight ();
			token -> set_size (point (width -> getInteger (), height -> getInteger ()));
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == background_colour_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> background_colour . red)); parameters = parameters -> getRight ();
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> background_colour . green)); parameters = parameters -> getRight ();
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> background_colour . blue)); parameters = parameters -> getRight ();
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> background_colour . alpha)); parameters = parameters -> getRight ();
				return true;
			}
			if (! parameters -> isPair ()) return false; PrologElement * red = parameters -> getLeft (); if (! red -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * green = parameters -> getLeft (); if (! green -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * blue = parameters -> getLeft (); if (! blue -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				PrologElement * alpha = parameters -> getLeft (); if (! alpha -> isInteger ()) return false;
				token -> background_colour = colour (red -> getInteger (), green -> getInteger (), blue -> getInteger (), alpha -> getInteger ());
			} else token -> background_colour = colour (red -> getInteger (), green -> getInteger (), blue -> getInteger ());
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == foreground_colour_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> foreground_colour . red)); parameters = parameters -> getRight ();
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> foreground_colour . green)); parameters = parameters -> getRight ();
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> foreground_colour . blue)); parameters = parameters -> getRight ();
				parameters -> setPair (); parameters -> getLeft () -> setInteger (colour_to_int (token -> foreground_colour . alpha)); parameters = parameters -> getRight ();
				return true;
			}
			if (! parameters -> isPair ()) return false; PrologElement * red = parameters -> getLeft (); if (! red -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * green = parameters -> getLeft (); if (! green -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * blue = parameters -> getLeft (); if (! blue -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				PrologElement * alpha = parameters -> getLeft (); if (! alpha -> isInteger ()) return false;
				token -> foreground_colour = colour (red -> getInteger (), green -> getInteger (), blue -> getInteger (), alpha -> getInteger ());
			} else token -> foreground_colour = colour (red -> getInteger (), green -> getInteger (), blue -> getInteger ());
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == scaling_atom) {
			if (parameters -> isVar ()) {parameters -> setPair (); parameters -> getLeft () -> setDouble (token -> scaling); return true;}
			if (! parameters -> isPair ()) return false;
			PrologElement * scaling = parameters -> getLeft ();
			if (scaling -> isDouble ()) {token -> scaling = scaling -> getDouble (); boarder_clean = false; return true;}
			if (scaling -> isInteger ()) {token -> scaling = (double) scaling -> getInteger (); boarder_clean = false; return true;}
			return false;
		}
		if (atom -> getAtom () == rotation_atom) {
			if (parameters -> isVar ()) {parameters -> setPair (); parameters -> getLeft () -> setDouble (token -> rotation); return true;}
			if (! parameters -> isPair ()) return false;
			PrologElement * rotation = parameters -> getLeft ();
			if (rotation -> isDouble ()) {token -> rotation = rotation -> getDouble (); boarder_clean = false; return true;}
			if (rotation -> isInteger ()) {token -> rotation = (double) rotation -> getInteger (); boarder_clean = false; return true;}
			return false;
		}
		if (atom -> getAtom () == side_atom) {
			if (parameters -> isVar ()) {parameters -> setPair (); parameters -> getLeft () -> setInteger (token -> side); return true;}
			if (! parameters -> isPair ()) return false;
			PrologElement * side = parameters -> getLeft ();
			if (! side -> isInteger ()) return false;
			token -> side = side -> getInteger ();
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == indexing_atom) {
			if (parameters -> isEarth ()) {token -> no_indexing = false; boarder_clean = false; return true;}
			if (parameters -> isVar ()) {
				parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) token -> indexing . position . x); parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) token -> indexing . position . y); parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) token -> indexing . size . x); parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) token -> indexing . size . y);
				return true;
			}
			if (! parameters -> isPair ()) return false; PrologElement * x = parameters -> getLeft (); if (! x -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * y = parameters -> getLeft (); if (! y -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * width = parameters -> getLeft (); if (! width -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false; PrologElement * height = parameters -> getLeft (); if (! height -> isInteger ()) return false; parameters = parameters -> getRight ();
			token -> indexing = rect (x -> getInteger (), y -> getInteger (), width -> getInteger (), height -> getInteger ());
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == no_indexing_atom) {token -> no_indexing = true; boarder_clean = false; return true;}
		if (atom -> getAtom () == indexed_atom) return ! token -> no_indexing;
		if (atom -> getAtom () == roll_atom) {
			int ret = token -> randomise_side ();
			boarder_clean = false;
			if (parameters -> isEarth ()) return true;
			if (parameters -> isPair ()) parameters = parameters -> getLeft ();
			if (parameters -> isVar ()) {parameters -> setInteger (ret); return true;}
			return false;
		}
		if (atom -> getAtom () == lock_atom) {token -> locked = true; boarder_clean = false; return true;}
		if (atom -> getAtom () == unlock_atom) {token -> locked = false; boarder_clean = false; return true;}
		if (atom -> getAtom () == is_locked_atom) {return token -> locked;}
		if (atom -> getAtom () == select_atom) {token -> selected = true; return true;}
		if (atom -> getAtom () == deselect_atom) {token -> selected = false; return true;}
		if (atom -> getAtom () == is_selected_atom) {return token -> selected;}
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
		if (atom -> getAtom () == sides_atom) {
			if (parameters -> isPair ()) parameters = parameters -> getLeft ();
			if (parameters -> isVar ()) {parameters -> setInteger (token -> get_sides ()); return true;}
			if (! parameters -> isInteger ()) return false;
			return token -> set_sides (parameters -> getInteger ());
		}
		if (atom -> getAtom () == text_atom) {
			if (parameters -> isPair ()) parameters = parameters -> getLeft ();
			if (parameters -> isVar ()) {parameters -> setText (token -> get_text ()); return true;}
			if (! parameters -> isText ()) return false;
			return token -> set_text (parameters -> getText ());
		}
		if (atom -> getAtom () == is_deck_atom) {return token -> can_insert ();}
		return false;
	}
	*/
	public Token insert (Token token) {return null;}
	public Token (PrologFXGStudio fxg, PrologAtom atom, Colour foreground, Colour background) {
		this . fxg = fxg;
		this . atom = atom;
		this . foreground = new Colour (foreground);
		this . background = new Colour (background);
	}
}
