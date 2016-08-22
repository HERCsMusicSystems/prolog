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

import java . util . ArrayList;

import javafx . scene . canvas . *;
import javafx . scene . image . *;

public class PictureToken extends Token {
	public ArrayList <Image> images = new ArrayList <Image> ();
	public int numberOfSides () {return images . size () + 1;}
	public int randomise_side () {return side = (int) (Math . random () * images . size ());}
	public void sideChanged () {if (side >= images . size ()) return; Image i = images . get (side); location . size = new Point (i . getWidth (), i . getHeight ());}
	public void draw (GraphicsContext gc, Viewport v) {
		gc . scale (v . scaling . x, v . scaling . y);
		gc . translate (- v . location . position . x, - v . location . position . y);
		gc . translate (location . position . x, location . position . y);
		gc . scale (scaling . x, scaling . y);
		if (rotation != 0.0) gc . rotate (rotation * 15.0);
		Point half = location . size . half ();
		if (side < images . size ()) {gc . drawImage (images . get (side), - half . x, - half . y); return;}
		if (background . alpha > 0.0) {
			gc . setFill (bgcc ());
			gc . fillRoundRect (- half . x, - half . y, location . size . x, location . size . y, rounding . x, rounding . y);
		}
		if (! foreground . eq (background)) {
			gc . setStroke (fgcc ());
			gc . strokeRoundRect (- half . x, - half . y, location . size . x, location . size . y, rounding . x, rounding . y);
		}
	}
	public PictureToken (PrologFXGStudio fxg, PrologAtom atom, ArrayList <String> pictures, Colour foreground, Colour background, Token next) {
		super (fxg, atom, foreground, background, next);
		for (String name : pictures) images . add (new Image (name));
		sideChanged ();
	}
}
