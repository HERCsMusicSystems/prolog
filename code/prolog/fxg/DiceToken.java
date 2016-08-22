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
	public boolean doubleAction () {if (selected) {randomise_side (); return true;} return false;}
	public int randomise_side () {
		rotation += Math . random () * 3.0 + 1.0;
		rotation = (double) ((int) rotation % 24);
		side = (int) (Math . random () * (sides < 1 ? 6 : sides));
		return side * multiplier + shift;
	}
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
		if (background . alpha > 0.0) {gc . setFill (bgcc ()); gc . fillPolygon (xx, yy, 3);}
		if (! foreground . eq (background)) {gc . setStroke (fgcc); gc . strokePolygon (xx, yy, 3);}
		gc . setFont (new Font (location . size . y * 0.5));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, half . y * -0.125);
	}
	public void draw_octahedron (GraphicsContext gc, Viewport v) {
		Point half = location . size . half ();
		Color fgcc = fgcc ();
		double hour = Math . PI / 6.0;
		double step = hour + hour;
		double [] xx = new double [6];
		double [] yy = new double [6];
		for (int ind = 0; ind < 6; ind++) {
			xx [ind] = half . x * Math . cos (hour);
			yy [ind] = half . y * Math . sin (hour);
			hour += step;
		}
		if (background . alpha > 0.0) {gc . setFill (bgcc ()); gc . fillPolygon (xx, yy, 6);}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc); gc . strokePolygon (xx, yy, 6);
			xx [1] = xx [2]; xx [2] = xx [4];
			yy [1] = yy [2]; yy [2] = yy [4];
			gc . strokePolygon (xx, yy, 3);
		}
		gc . setFont (new Font (location . size . y * 0.5));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, half . y * -0.125);
	}
	public void draw_deltahedron (GraphicsContext gc, Viewport v) {
		double across = location . size . y * 3.0 / 4.0;
		double gridside = across / 1.11178594;
		double half_angle = 0.45227844665387872478834364742025;
		double skirt_angle = 1.1185178801410178944429780442195;
		double angled_across_angle = 0.5 * (half_angle + skirt_angle);
		double angled_across = gridside / Math . cos (angled_across_angle - half_angle);
		double skirt_side = gridside * Math . sin (2.0 * 0.45227844665387872478834364742025);
		Point start = new Point (0.0, - location . size . y * 0.375);
		Point p1 = start . add (new Point (skirt_side * Math . sin (skirt_angle), skirt_side * Math . cos (skirt_angle)));
		Point p2 = start . add (new Point (angled_across * Math . sin (angled_across_angle), angled_across * Math . cos (angled_across_angle)));
		Point p3 = start . add (new Point (0.0, across));
		Point p4 = start . add (new Point (angled_across * Math . sin (- angled_across_angle), angled_across * Math . cos (- angled_across_angle)));
		Point p5 = start . add (new Point (skirt_side * Math . sin (- skirt_angle), skirt_side * Math . cos (- skirt_angle)));
		Point p6 = start . add (new Point (gridside * Math . sin (half_angle), gridside * Math . cos (half_angle)));
		Point p7 = start . add (new Point (gridside * Math . sin (- half_angle), gridside * Math . cos (- half_angle)));
		Color fgcc = fgcc ();
		double [] xx = {start . x, p1 . x, p2 . x, p3 . x, p4 . x, p5 . x};
		double [] yy = {start . y, p1 . y, p2 . y, p3 . y, p4 . y, p5 . y};
		if (background . alpha > 0.0) {gc . setFill (bgcc ()); gc . fillPolygon (xx, yy, 6);}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc); gc . strokePolygon (xx, yy, 6);
			gc . strokeLine (start . x, start . y, p6 . x, p6 . y);
			gc . strokeLine (start . x, start . y, p7 . x, p7 . y);
		}
		gc . setFont (new Font (location . size . y * 0.3));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, location . size . y * 0.0625);
	}
	public void draw_dodecahedron (GraphicsContext gc, Viewport v) {
		Point half = location . size . half ();
		Point inner = location . size . times (0.30901699437494742410229341718283);
		double step = Math . PI / 5.0;
		double hour = step * 1.5;
		double [] xx = new double [10];
		double [] yy = new double [10];
		for (int ind = 0; ind < 10; ind++) {
			xx [ind] = half . x * Math . cos (hour);
			yy [ind] = half . y * Math . sin (hour);
			hour += step;
		}
		double [] xi = new double [5];
		double [] yi = new double [5];
		hour = step * 1.5;
		for (int ind = 0; ind < 5; ind++) {
			xi [ind] = inner . x * Math . cos (hour);
			yi [ind] = inner . y * Math . sin (hour);
			hour += step + step;
		}
		Color fgcc = fgcc ();
		if (background . alpha > 0.0) {gc . setFill (bgcc ()); gc . fillPolygon (xx, yy, 10);}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc); gc . strokePolygon (xx, yy, 10);
			gc . strokePolygon (xi, yi, 5);
			for (int ind = 0; ind < 5; ind++) gc . strokeLine (xi [ind], yi [ind], xx [ind + ind], yy [ind + ind]);
		}
		gc . setFont (new Font (location . size . y * 0.3));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, half . y * -0.0625);
	}
	public void draw_icosahedron (GraphicsContext gc, Viewport v) {
		Point half = location . size . half ();
		Point inner = location . size . times (0.30901699437494742410229341718283);
		double hour = Math . PI / 6.0;
		double step = hour + hour;
		double [] xx = new double [6];
		double [] yy = new double [6];
		for (int ind = 0; ind < 6; ind++) {
			xx [ind] = half . x * Math . cos (hour);
			yy [ind] = half . y * Math . sin (hour);
			hour += step;
		}
		double [] xi = new double [3];
		double [] yi = new double [3];
		hour = step * 0.5;
		for (int ind = 0; ind < 3; ind++) {
			xi [ind] = inner . x * Math . cos (hour);
			yi [ind] = inner . y * Math . sin (hour);
			hour += step + step;
		}
		Color fgcc = fgcc ();
		if (background . alpha > 0.0) {gc . setFill (bgcc ()); gc . fillPolygon (xx, yy, 6);}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc); gc . strokePolygon (xx, yy, 6);
			gc . strokePolygon (xi, yi, 3);
			gc . strokeLine (xi [0], yi [0], xx [5], yy [5]);
			gc . strokeLine (xi [0], yi [0], xx [0], yy [0]);
			gc . strokeLine (xi [0], yi [0], xx [1], yy [1]);
			gc . strokeLine (xi [1], yi [1], xx [1], yy [1]);
			gc . strokeLine (xi [1], yi [1], xx [2], yy [2]);
			gc . strokeLine (xi [1], yi [1], xx [3], yy [3]);
			gc . strokeLine (xi [2], yi [2], xx [3], yy [3]);
			gc . strokeLine (xi [2], yi [2], xx [4], yy [4]);
			gc . strokeLine (xi [2], yi [2], xx [5], yy [5]);
		}
		gc . setFont (new Font (location . size . y * 0.3));
		gc . setTextAlign (TextAlignment . CENTER);
		gc . setTextBaseline (VPos . CENTER);
		gc . setFill (fgcc);
		gc . fillText ("" + (side * multiplier + shift), 0.0, half . y * -0.0625);
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
		case 8: draw_octahedron (gc, v); break;
		case 10: draw_deltahedron (gc, v); break;
		case 12: draw_dodecahedron (gc, v); break;
		default: draw_icosahedron (gc, v); break;
		}
	}
	public DiceToken (PrologFXGStudio fxg, PrologAtom atom, int sides, int shift, int multiplier, Token next) {
		super (fxg, atom, fxg . dice_foreground (sides, multiplier), fxg . dice_background (sides, multiplier), next);
		this . sides = sides; this . shift = shift; this . multiplier = multiplier;
		this . rounding = new Point (16.0, 16.0);
	}
}
