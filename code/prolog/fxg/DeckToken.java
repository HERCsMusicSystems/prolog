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
import java . util . ArrayList;
import java . util . Collections;

import javafx . scene . canvas . *;
import javafx . scene . paint . *;
import javafx . scene . text . *;
import javafx . geometry . *;

public class DeckToken extends Token {
	public Token tokens = null;
	public boolean shuffled = false;
	public int content_size () {
		int ret = 0;
		Token tk = tokens;
		while (tk != null) {tk = tk . next; ret++;}
		return ret;
	}
	public boolean doubleAction () {if (fxg . currentDeck == this) {randomise_side (); return true;} return false;}
	public int randomise_side () {
		ArrayList <Token> list = new ArrayList <Token> ();
		while (tokens != null) {list . add (tokens); tokens = tokens . next;}
		Collections . shuffle (list);
		for (Token tk : list) {tk . next = tokens; tokens = tk;}
		shuffled = true;
		return 0;
	}
	public int randomise_sided () {
		int ind = content_size ();
		if (ind < 2) return 0;
		Token tk = null;
		while (ind > 0) {
			int sub = (int) (Math . random () * (double) ind);
			ind--;
			if (sub == 0) {Token tkk = tokens; tokens = tokens . next; tkk . next = tk; tk = tkk;}
			else {
				Token tkk = tokens;
				while (sub > 1) {tkk = tkk . next; sub--;}
			}
		}
		tokens = tk;
		shuffled = true;
		return 0;
	}
	public void insert_token (Token token) {shuffled = false; token . selected = false; token . next = tokens; tokens = token;}
	public Token release_token (Point p) {
		shuffled = false;
		if (tokens == null) return null;
		Token ret = tokens;
		tokens = ret . next;
		ret . next = null;
		ret . location . position = new Point (p);
		return ret;
	}
	public void draw (GraphicsContext gc, Viewport v) {
		gc . scale (v . scaling . x, v . scaling . y);
		gc . translate (- v . location . position . x, - v . location . position . y);
		gc . translate (location . position . x, location . position . y);
		gc . scale (scaling . x, scaling . y);
		if (rotation != 0.0) gc . rotate (rotation * 15.0);
		Point half = location . size . half ();
		Color fgcc = fgcc ();
		if (background . alpha > 0.0) {
			gc . setFill (bgcc ());
			gc . fillRoundRect (- half . x, - half . y, location . size . x, location . size . y, rounding . x, rounding . y);
		}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc);
			gc . strokeRoundRect (- half . x, - half . y, location . size . x, location . size . y, rounding . x, rounding . y);
		}
		gc . setFill (fgcc);
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . fillText (text + " (" + (shuffled ? "shuffled " : "") + content_size () + ")", 0.0, 0.0);
	}
	public void erase () {if (tokens != null) tokens . erase (); tokens = null; super . erase ();}
	public void save (FileWriter tc) {
		try {
			tc . write ("[Deck " + atom . name () + " \"" + text . replace ("\"", "\\\"") + "\"]\n");
			save_location_and_size (tc);
			save_scaling (tc);
			save_rotation (tc);
			save_rounding (tc);
			save_index (tc);
			save_foreground (tc, fxg . default_deck_foreground);
			save_background (tc, fxg . default_deck_background);
			save_lock (tc);
			if (locked && tokens != null) {
				tc . write ("[" + atom . name () + " Select]\n\n");
				tokens . save_stack (tc);
				tc . write ("[" + atom . name () + " Deselect]\n");
			}
			tc . write ("\n");
		} catch (Exception ex) {}
	}
	public DeckToken (PrologFXGStudio fxg, PrologAtom atom, String text, Token next) {
		super (fxg, atom, fxg . default_deck_foreground, fxg . default_deck_background, next);
		this . text = "" + text;
	}
}

