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

import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . stage . Modality;
import javafx . stage . WindowEvent;
import javafx . event . EventHandler;
import javafx . scene . Scene;
import javafx . scene . Group;
import javafx . scene . paint . Color;
import javafx . scene . canvas . Canvas;
import javafx . scene . canvas . GraphicsContext;
import javafx . beans . value . ChangeListener;
import javafx . beans . value . ObservableValue;

public class Viewport extends Token {
	public String viewport_name;
	public Point screen_position = new Point (0.0, 0.0);
	enum edit_modes {
		move, select,
		create_rectangle, create_circle,
		create_tetrahedron, create_cube, create_dice,
		create_octahedron, create_deltahedron, create_deltahedron_10,
		create_dodecahedron, create_icosahedron,
		create_text, create_grid, create_deck,
		edit_size, edit_indexing, edit_rotation, edit_side, edit_scaling, edit_ordering,
		edit, shuffle
	};
	public Stage viewport;
	public GraphicsContext gc;
	public Canvas canvas;
	public void repaint () {
		canvas . setWidth (location . size . x); canvas . setHeight (location . size . y);
		gc . clearRect (0, 0, canvas . getWidth (), canvas . getHeight ()); fxg . draw (gc, this);
	}
	public void build () {
		viewport = new Stage ();
		viewport . setTitle (viewport_name);
		viewport . initModality (Modality . NONE);
		viewport . initOwner (null);
		//=========
		Group g = new Group ();
		canvas = new Canvas (location . size . x, location . size . y);
		gc = canvas . getGraphicsContext2D ();
		repaint ();
		g . getChildren () . add (canvas);
		viewport . setScene (new Scene (g));
		//=============
		//StackPane pane = new StackPane ();
		//Scene viewport_scene = new Scene (pane, location . size . x, location . size . y);
		//viewport . setScene (viewport_scene);
		//==============
		viewport . setX (location . position . x); viewport . setY (location . position . y);
		viewport . widthProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {
				location . size . x = (double) current;
				Platform . runLater (new Runnable () {public void run () {repaint ();}});
			}
		});
		viewport . heightProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {
				location . size . y = (double) current;
				Platform . runLater (new Runnable () {public void run () {repaint ();}});
			}
		});
		viewport . xProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {location . position . x = (double) current;}
		});
		viewport . yProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {location . position . y = (double) current;}
		});
		viewport . setOnCloseRequest (new EventHandler <WindowEvent> () {public void handle (WindowEvent e) {close ();}});
		viewport . show ();
	}
	public void change_viewport_name () {viewport . setTitle (viewport_name + " [" + edit_modes . values () [side] . name () + "]");}
	public void setPosition (double x, double y) {screen_position . x = x; screen_position . y = y;}
	public void setWindowSize (double w, double h) {viewport . setWidth (location . size . x = w); viewport . setHeight (location . size . y = h);}
	public void setWindowLocation (double x, double y, double w, double h) {
		viewport . setX (location . position . x = x);
		viewport . setY (location . position . y = y);
		viewport . setWidth (location . size . x = w);
		viewport . setHeight (location . size . y = h);
	}
	public void erase () {close (); Platform . runLater (new Runnable () {public void run () {viewport . close ();}}); super . erase ();}
	public void close () {fxg . clean = false; atom . setMachine (null); fxg . remove_viewport (this);}
	public void save (FileWriter tc) {
		super . save (tc);
		try {
			tc . write ("[Viewport " + atom . name () + " \"" + viewport_name + "\" "
				+ location . position . x + " " + location . position . y + " " + location . size . x + " " + location . size . y + "]\n");
			if (! screen_position . eq (new Point (0.0, 0.0))) tc . write ("[" + atom . name () + " Position " + screen_position . x + " " + screen_position . y + "]\n");
			if (scaling != 1.0) tc . write ("[" + atom . name () + " Scaling " + scaling + "]\n");
			if (side != edit_modes . move . ordinal ()) tc . write ("[" + atom . name () + " Mode " + side + "]\n");
			tc . write ("\n");
		} catch (Exception ex) {}
	}
	public void sideChanged () {Platform . runLater (new Runnable () {public void run () {change_viewport_name ();}});}
	public int numberOfSides () {return edit_modes . values () . length;}
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {close (); Platform . runLater (new Runnable () {public void run () {viewport . close ();}}); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement atom = parameters . getLeft (); parameters = parameters . getRight ();
		if (! atom . isAtom ()) return false;
		if (atom . getAtom () == fxg . side_atom) {
			if (parameters . isVar ()) {parameters . setInteger (side); return true;}
			if (! parameters . isPair ()) return false; parameters = parameters . getLeft ();
			if (! parameters . isInteger ()) return false;
			int ind = parameters . getInteger ();
			if (ind < 0 || ind >= numberOfSides ()) return false;
			side = ind; sideChanged (); return true;
		}
		if (atom . getAtom () == fxg . location_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (location . position . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . position . y); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . size . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . size . y);
				return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement x = parameters . getLeft (); if (! x . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement y = parameters . getLeft (); if (! y . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement w = parameters . getLeft (); if (! w . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement h = parameters . getLeft (); if (! h . isNumber ()) return false;
			Platform . runLater (new Runnable () {public void run () {setWindowLocation (x . getNumber (), y . getNumber (), w . getNumber (), h . getNumber ());}});
			fxg . clean = false;
			return true;
		}
		if (atom . getAtom () == fxg . position_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (screen_position . x);
				parameters = parameters . getRight (); parameters . setPair ();
				parameters . getLeft () . setDouble (screen_position . y);
				return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement x = parameters . getLeft (); if (! x . isNumber ()) return false; parameters = parameters . getRight ();
			if (! parameters . isPair ()) return false;
			PrologElement y = parameters . getLeft (); if (! y . isNumber ()) return false; parameters = parameters . getRight ();
			Platform . runLater (new Runnable () {public void run () {setPosition (x . getNumber (), y . getNumber ());}});
			fxg . clean = false;
			return true;
		}
		if (atom . getAtom () == fxg . size_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (location . size . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (location . size . y);
				return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement w = parameters . getLeft (); if (! w . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement h = parameters . getLeft (); if (! h . isNumber ()) return false;
			Platform . runLater (new Runnable () {public void run () {setWindowSize (w . getNumber (), h . getNumber ());}});
			fxg . clean = false;
			return true;
		}
		if (atom . getAtom () == fxg . scaling_atom) {
			if (parameters . isVar ()) {parameters . setPair (); parameters . getLeft () . setDouble (scaling); return true;}
			if (! parameters . isPair ()) return false;
			PrologElement el = parameters . getLeft (); if (! el . isNumber ()) return false;
			scaling = el . getNumber ();
			fxg . clean = false;
			return true;
		}
		//if (atom . getAtom () == fxg . repaint_atom) {....}
		return false;
	}
	public Viewport (PrologFXGStudio fxg, PrologAtom atom, Colour foreground, Colour background, String viewport_name, Rect location, Token next) {
		super (fxg, atom, foreground, background);
		this . viewport_name = viewport_name;
		this . location = new Rect (new Point (0.0, 0.0), location . size);
		this . screen_position = new Point (location . position);
		Platform . runLater (new Runnable () {public void run () {build ();}});
	}
}
