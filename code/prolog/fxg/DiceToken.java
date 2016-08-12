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

import javafx . scene . canvas . *;
import javafx . scene . text . *;
import javafx . scene . paint . *;
import javafx . geometry . *;

public class DiceToken extends Token {
	public int sides = 6, shift = 1, multiplier = 1;
	public int numberOfSides () {return sides < 1 ? 6 : sides;}
	public int randomise_side () {rotation = Math . random () * 24.0; side = (int) (Math . random () * (sides < 1 ? 6 : sides)); return side * multiplier + shift;}
	public void draw_dice (GraphicsContext gc, Viewport v) {
		Point half = location . size . half ();
		Point dot = location . size . times (0.28);
		Point circ = location . size . times (0.25);
		Point radius = circ . half ();
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
		switch (side) {
		case 0: gc . fillOval (- radius . x, - radius . y, circ . x, circ . y); break;
		case 1:
			gc . fillOval (dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			break;
		case 2:
			gc . fillOval (- radius . x, - radius . y, circ . x, circ . y);
			gc . fillOval (dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			break;
		case 3:
			gc . fillOval (dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			break;
		case 4:
			gc . fillOval (- radius . x, - radius . y, circ . x, circ . y);
			gc . fillOval (dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			break;
		case 5:
			gc . fillOval (dot . x - radius . x, - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, - radius . y, circ . x, circ . y);
			gc . fillOval (dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (dot . x - radius . x, dot . y - radius . y, circ . x, circ . y);
			gc . fillOval (- dot . x - radius . x, - dot . y - radius . y, circ . x, circ . y);
			break;
		default: break;
		}
	}
	public void draw_cube (GraphicsContext gc, Viewport v) {
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
		gc . setFont (new Font (location . size . y * 0.5));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, 0.0);
	}
	public void draw_tetrahedron (GraphicsContext gc, Viewport v) {
		Point half = location . size . half ();
		Color fgcc = fgcc ();
		double hour = Math . PI / 6.0;
		double [] xx = {half . x * Math . cos (hour * 1.0), half . x * Math . cos (hour * 5.0), half . x * Math . cos (hour * 9.0)};
		double [] yy = {half . y * Math . sin (hour * 1.0), half . y * Math . sin (hour * 5.0), half . y * Math . sin (hour * 9.0)};
		if (background . alpha > 0.0) {
			gc . setFill (bgcc ());
			gc . fillPolygon (xx, yy, 3);
		}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc);
			gc . strokePolygon (xx, yy, 3);
		}
		gc . setFont (new Font (location . size . y * 0.5));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, half . y * -0.125);
	}
	public void draw (GraphicsContext gc, Viewport v) {
		gc . scale (v . scaling . x, v . scaling . y);
		gc . translate (- v . location . position . x, - v . location . position . y);
		gc . translate (location . position . x, location . position . y);
		gc . scale (scaling . x, scaling . y);
		if (rotation != 0.0) gc . rotate (rotation * 15.0);
		switch (sides) {
		case 0: draw_dice (gc, v); break;
		case 4: draw_tetrahedron (gc, v); break;
		case 6: draw_cube (gc, v); break;
		default: draw_cube (gc, v); break;
		}
	}
	public DiceToken (PrologFXGStudio fxg, PrologAtom atom, int sides, int shift, int multiplier, Token next) {
		super (fxg, atom, fxg . dice_foreground (sides, multiplier), fxg . dice_background (sides, multiplier), next);
		this . sides = sides; this . shift = shift; this . multiplier = multiplier;
		this . rounding = new Point (16.0, 16.0);
	}
}
