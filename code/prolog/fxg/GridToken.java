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

import javafx . scene . canvas . *;
import javafx . scene . text . *;
import javafx . scene . paint . *;
import javafx . geometry . *;

public class GridToken extends Token {
	public int numberOfSides () {return 5;}
	public boolean moveOnGrid (Token token, Point position) {
		Point gsc = new Point (1.0, 1.0);
		token . indexing . position = new Point (position);
		position = position . sub (indexing . position);
		switch ((int) side) {
			case 1: if ((int) position . x % 2 != 0) position . y += 0.5; gsc = new Point (0.75, 0.866025404); break;
			case 2: if ((int) position . x % 2 == 0) position . y += 0.5; gsc = new Point (0.75, 0.866025404); break;
			case 3: if ((int) position . y % 2 != 0) position . x += 0.5; gsc = new Point (0.866025404, 0.75); break;
			case 4: if ((int) position . y % 2 == 0) position . x += 0.5; gsc = new Point (0.866025404, 0.75); break;
			default: break;
		}
		token . location . position = location . position . add (position . times (location . size . times (gsc)));
		return true;
	}
	public void draw_square_grid (GraphicsContext gc, Viewport v) {
		Point half = location . size . half () . times (-1.0);
		Color fgcc = fgcc ();
		gc . setStroke (fgcc);
		for (int ind = 0; ind <= indexing . size . x; ind++)
			gc . strokeLine (
					half . x + ind * location . size . x,
					half . y,
					half . x + ind * location . size . x,
					half . y + location . size . y * indexing . size . y);
		for (int ind = 0; ind <= indexing . size . y; ind++)
			gc . strokeLine (
					half . x,
					half . y + ind * location . size . y,
					half . x + location . size . x * indexing . size . x,
					half . y + ind * location . size . y);
		if (! indexed) return;
		gc . setFont (new Font (12));
		gc . setFill (fgcc);
		gc . setTextBaseline (VPos . TOP);
		for (int x = 0; x < indexing . size . x; x++) {
			for (int y = 0; y < indexing . size . y; y++) {
				String command = String . format ("%02d%02d", (int) indexing . position . x + x, (int) indexing . position . y + y);
				gc . fillText (command, 2 + half . x + x * location . size . x, 1 + half . y + y * location . size . y);
			}
		}
	}
	public void draw_vertical_hex_grid (GraphicsContext gc, Viewport v, boolean initial) {
		Point half = location . size . half ();
		Point quarter = location . size . times (0.25);
		Point three = location . size . times (0.75);
		Point p = half . times (-1.0);
		Color fgcc = fgcc ();
		gc . setStroke (fgcc);
		gc . setLineWidth (1.5);
		double H = half . y * 0.866025404;
		double vertical_shift = initial ? half . y : half . y - H;
		for (int x = 0; x < indexing . size . x; x++) {
			double xx = p . x + location . size . x * (0.25 + (double) x * 0.75);
			double yy = p . y + vertical_shift;
			double xxhx = xx + half . x;
			double xxtx = xx + three . x;
			if (vertical_shift < half . y && x < indexing . size . x - 1) gc . strokeLine (xxhx, yy, xxtx, yy + H);
			for (int y = 0; y < indexing . size . y; y++) {
				gc . strokeLine (xxhx, yy, xx, yy);
				double xxmq = xx - quarter . x, yyH = yy + H;
				gc . strokeLine (xx, yy, xxmq, yyH);
				gc . strokeLine (xxmq, yyH, xx, yyH + H);
				if (x == indexing . size . x - 1) {
					gc . strokeLine (xxhx, yy, xxtx, yyH);
					gc . strokeLine (xxtx, yyH, xxhx, yyH + H);
				}
				yy += H + H;
			}
			gc . strokeLine (xx, yy, xxhx, yy);
			if (vertical_shift == half . y && x < indexing . size . x - 1) gc . strokeLine (xxhx, yy, xxtx, yy - H);
			vertical_shift = vertical_shift == half . y ? half . y - H : half . y;
		}
		if (! indexed) return;
		gc . setFont (new Font (12));
		gc . setFill (fgcc);
		gc . setTextBaseline (VPos . TOP);
		vertical_shift = initial ? half . y : half . y - H;
		for (int x = 0; x < indexing . size . x; x++) {
			for (int y = 0; y < indexing . size . y; y++) {
				String command = String . format ("%02d%02d", (int) indexing . position . x + x, (int) indexing . position . y + y);
				gc . fillText (command, 1 + p . x + quarter . x + three . x * x, p . y + H * 2 * y + vertical_shift);
			}
			vertical_shift = vertical_shift == half . y ? half . y - H : half . y;
		}
	}
	public void draw_horizontal_hex_grid (GraphicsContext gc, Viewport v, boolean initial) {
		Point half = location . size . half ();
		Point quarter = location . size . times (0.25);
		Point three = location . size . times (0.75);
		Point p = half . times (-1.0);
		Color fgcc = fgcc ();
		gc . setStroke (fgcc);
		gc . setLineWidth (1.5);
		double W = half . x * 0.866025404;
		double horizontal_shift = initial ? half . x : half . x - W;
		for (int y = 0; y < indexing . size . y; y++) {
			double yy = p . y + location . size . y * (0.25 + (double) y * 0.75);
			double xx = p . x + horizontal_shift;
			double yyhy = yy + half . y;
			double yyty = yy + three . y;
			if (horizontal_shift < half . x && y < indexing . size . y - 1) gc . strokeLine (xx, yyhy, xx + W, yyty);
			for (int x = 0; x < indexing . size . x; x++) {
				gc . strokeLine (xx, yyhy, xx, yy);
				double yymq = yy - quarter . y, xxW = xx + W;
				gc . strokeLine (xx, yy, xxW, yymq);
				gc . strokeLine (xxW, yymq, xxW + W, yy);
				if (y == indexing . size . y - 1) {
					gc . strokeLine (xx, yyhy, xxW, yyty);
					gc . strokeLine (xxW, yyty, xxW + W, yyhy);
				}
				xx += W + W;
			}
			gc . strokeLine (xx, yy, xx, yyhy);
			if (horizontal_shift == half . x && y < indexing . size . y - 1) gc . strokeLine (xx, yyhy, xx - W, yyty);
			horizontal_shift = horizontal_shift == half . x ? half . x - W : half . x;
		}
		if (! indexed) return;
		gc . setFont (new Font (12));
		gc . setFill (fgcc);
		gc . setTextBaseline (VPos . TOP);
		horizontal_shift = initial ? half . x : half . x - W;
		for (int y = 0; y < indexing . size . y; y++) {
			for (int x = 0; x < indexing . size . x; x++) {
				String command = String . format ("%02d%02d", (int) indexing . position . x + x, (int) indexing . position . y + y);
				gc . fillText (command, 1 + p . x + W * 2 * x + horizontal_shift, p . y + quarter . y + three . y * y);
			}
			horizontal_shift = horizontal_shift == half . x ? half . x - W : half . x;
		}
	}
	public void draw (GraphicsContext gc, Viewport v) {
		gc . scale (v . scaling . x, v . scaling . y);
		gc . translate (- v . location . position . x, - v . location . position . y);
		gc . translate (location . position . x, location . position . y);
		gc . scale (scaling . x, scaling . y);
		if (rotation != 0.0) gc . rotate (rotation * 15.0);
		switch ((int) side) {
		case 0: draw_square_grid (gc, v); break;
		case 1: draw_vertical_hex_grid (gc, v, false); break;
		case 2: draw_vertical_hex_grid (gc, v, true); break;
		case 3: draw_horizontal_hex_grid (gc, v, false); break;
		case 4: draw_horizontal_hex_grid (gc, v, true); break;
		default: draw_square_grid (gc, v); break;
		}
	}
	public void save (FileWriter tc) {
		try {
			tc . write ("[Grid " + atom . name () + "]\n");
			save_location_and_size (tc);
			save_scaling (tc);
			save_rotation (tc);
			save_side (tc);
			save_indexing (tc);
			save_foreground (tc, fxg . default_picture_foreground);
			save_lock (tc);
			tc . write ("\n");
		} catch (Exception ex) {}
	}
	public GridToken (PrologFXGStudio fxg, PrologAtom atom, Colour foreground, Colour background, Token next) {super (fxg, atom, foreground, background, next);}
}
