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
	public boolean main;
	public void repaint () {
		canvas . setWidth (location . size . x = viewport . getWidth ()); canvas . setHeight (location . size . y = viewport . getHeight ());
		gc . clearRect (0, 0, canvas . getWidth (), canvas . getHeight ()); fxg . draw (gc, this);
	}
	public void build () {
		viewport = new Stage ();
		viewport . setTitle (viewport_name);
		viewport . initModality (Modality . NONE);
		viewport . initOwner (null);
		viewport . setWidth (location . size . x); viewport . setHeight (location . size . y);
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
		viewport . setX (screen_position . x); viewport . setY (screen_position . y);
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
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {screen_position . x = (double) current;}
		});
		viewport . yProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {screen_position . y = (double) current;}
		});
		viewport . setOnCloseRequest (new EventHandler <WindowEvent> () {public void handle (WindowEvent e) {ui_close ();}});
		viewport . show ();
	}
	public void change_viewport_name () {viewport . setTitle (viewport_name + " [" + edit_modes . values () [side] . name () + "]");}
	public void setPosition (double x, double y) {
		screen_position = new Point (x, y);
		Platform . runLater (new Runnable () {public void run () {viewport . setX (screen_position . x); viewport . setY (screen_position . y);}});
	}
	public Point getPosition () {return new Point (screen_position . x, screen_position . y);}
	public void sizeChanged () {
		Platform . runLater (new Runnable () {public void run () {viewport . setWidth (location . size . x); viewport . setHeight (location . size . y);}});
	}
	public void programmatic_close () {fxg . remove_viewport (this); Platform . runLater (new Runnable () {public void run () {viewport . close ();}});}
	public void ui_close () {fxg . remove_viewport (this); atom . setMachine (null); fxg . clean = false;}
	public void save (FileWriter tc) {
		super . save (tc);
		try {
			tc . write ("[Viewport " + atom . name () + " \"" + viewport_name + "\" "
				+ location . position . x + " " + location . position . y + " " + location . size . x + " " + location . size . y + "]\n");
			if (! screen_position . eq (new Point (0.0, 0.0))) tc . write ("[" + atom . name () + " Position " + screen_position . x + " " + screen_position . y + "]\n");
			if (! scaling . eq (new Point (1.0, 1.0))) tc . write ("[" + atom . name () + " Scaling " + scaling . x + " " + scaling . y + "]\n");
			if (side != edit_modes . move . ordinal ()) tc . write ("[" + atom . name () + " Mode " + side + "]\n");
			tc . write ("\n");
		} catch (Exception ex) {}
	}
	public void sideChanged () {Platform . runLater (new Runnable () {public void run () {change_viewport_name ();}});}
	public int numberOfSides () {return edit_modes . values () . length;}
	public Viewport (PrologFXGStudio fxg, PrologAtom atom, Colour foreground, Colour background, String viewport_name, Rect location, boolean main, Token next) {
		super (fxg, atom, foreground, background, next);
		this . viewport_name = viewport_name;
		this . screen_position = new Point (location . position);
		this . main = main;
		if (main) {
			this . viewport = PrologFX . PrologMainFX . stage;
			canvas = PrologFX . PrologMainFX . canvas;
			gc = PrologFX . PrologMainFX . gc;
			this . location = new Rect (new Point (0.0, 0.0), new Point (canvas . getWidth (), canvas . getHeight ()));
			Platform . runLater (new Runnable () {public void run () {repaint ();}});
		} else {
			this . location = new Rect (new Point (0.0, 0.0), location . size);
			Platform . runLater (new Runnable () {public void run () {build ();}});
		}
	}
}
