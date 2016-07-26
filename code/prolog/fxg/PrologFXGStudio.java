
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
import javafx . scene . canvas . Canvas;
import javafx . scene . canvas . GraphicsContext;
import javafx . scene . paint . Color;
import javafx . scene . layout . StackPane;
import javafx . beans . value . ChangeListener;
import javafx . beans . value . ObservableValue;

class boarder_viewport {
	public viewport_action action;
	public PrologAtom atom;
	public String viewport_name;
	public Stage viewport;
	public Rect location;
	public Point board_position = new Point (0.0, 0.0);
	public double scaling = 1.0;
	public boarder_viewport next;
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
	public edit_modes edit_mode = edit_modes . move;
	public void build () {
		viewport = new Stage ();
		viewport . setTitle (viewport_name);
		viewport . initModality (Modality . NONE);
		viewport . initOwner (null);
		//=========
		Group g = new Group ();
		Canvas c = new Canvas (location . size . x, location . size . y);
		GraphicsContext gc = c . getGraphicsContext2D ();
		gc . setFill (Color . GREEN);
		gc . setStroke (Color . BLUE);
		gc . setLineWidth (5);
		gc . strokeLine (40, 10, 10, 40);
		g . getChildren () . add (c);
		viewport . setScene (new Scene (g));
		//=============
		//StackPane pane = new StackPane ();
		//Scene viewport_scene = new Scene (pane, location . size . x, location . size . y);
		//viewport . setScene (viewport_scene);
		//==============
		viewport . setX (location . position . x); viewport . setY (location . position . y);
		viewport . widthProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {location . size . x = (double) current;}
		});
		viewport . heightProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {location . size . y = (double) current;}
		});
		viewport . xProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {location . position . x = (double) current;}
		});
		viewport . yProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {location . position . y = (double) current;}
		});
		viewport . setOnCloseRequest (new EventHandler <WindowEvent> () {public void handle (WindowEvent e) {action . close ();}});
		viewport . show ();
	}
	public void close () {viewport . close ();}
	public void save (FileWriter tc) throws java . io . IOException {
		if (next != null) next . save (tc);
		tc . write ("[Viewport " + atom . name () + " \"" + viewport_name + "\" "
			+ location . position . x + " " + location . position . y + " " + location . size . x + " " + location . size . y + "]\n");
		if (! board_position . eq (new Point (0.0, 0.0))) tc . write ("[" + atom . name () + " Position " + board_position . x + " " + board_position . y + "]\n");
		if (scaling != 1.0) tc . write ("[" + atom . name () + " Scaling " + scaling + "]\n");
		if (edit_mode != edit_modes . move) tc . write ("[" + atom . name () + " Mode " + edit_mode . ordinal () + "]\n");
		tc . write ("\n");
	}
	public void erase () {action . close (); Platform . runLater (new Runnable () {public void run () {viewport . close ();}}); if (next == null) return; next . erase ();}
	public void setBoarderPosition (double x, double y) {board_position . x = x; board_position . y = y;}
	public void setWindowSize (double w, double h) {viewport . setWidth (location . size . x = w); viewport . setHeight (location . size . y = h);}
	public void setWindowLocation (double x, double y, double w, double h) {
		viewport . setX (location . position . x = x);
		viewport . setY (location . position . y = y);
		viewport . setWidth (location . size . x = w);
		viewport . setHeight (location . size . y = h);
	}
	public void change_viewport_name () {viewport . setTitle (viewport_name + " [" + edit_mode . name () + "]");}
	public boarder_viewport (viewport_action action, PrologAtom atom, String viewport_name, Rect location, boarder_viewport next) {
		this . action = action;
		this . atom = atom;
		this . viewport_name = viewport_name;
		this . location = new Rect (location);
		this . next = next;
		Platform . runLater (new Runnable () {public void run () {build ();}});
	}
}

class viewport_action extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public PrologAtom location_atom, size_atom, position_atom, scaling_atom, repaint_atom, mode_atom;
	public boarder_viewport viewport;
	public void close () {fxg . clean = false; viewport . atom . setMachine (null); fxg . remove_viewport (viewport);}
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {close (); Platform . runLater (new Runnable () {public void run () {viewport . close ();}}); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement atom = parameters . getLeft (); parameters = parameters . getRight ();
		if (! atom . isAtom ()) return false;
		if (atom . getAtom () == fxg . mode_atom) {
			if (parameters . isVar ()) {parameters . setInteger (viewport . edit_mode . ordinal ()); return true;}
			if (! parameters . isPair ()) return false; parameters = parameters . getLeft ();
			if (! parameters . isInteger ()) return false;
			viewport . edit_mode = boarder_viewport . edit_modes . values () [parameters . getInteger ()];
			Platform . runLater (new Runnable () {public void run () {viewport . change_viewport_name ();}});
			return true;
		}
		if (atom . getAtom () == fxg . location_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . position . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . position . y); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . size . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . size . y);
				return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement x = parameters . getLeft (); if (! x . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement y = parameters . getLeft (); if (! y . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement w = parameters . getLeft (); if (! w . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement h = parameters . getLeft (); if (! h . isNumber ()) return false;
			Platform . runLater (new Runnable () {public void run () {viewport . setWindowLocation (x . getNumber (), y . getNumber (), w . getNumber (), h . getNumber ());}});
			fxg . clean = false;
			return true;
		}
		if (atom . getAtom () == fxg . position_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . board_position . x);
				parameters = parameters . getRight (); parameters . setPair ();
				parameters . getLeft () . setDouble (viewport . board_position . y);
				return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement x = parameters . getLeft (); if (! x . isNumber ()) return false; parameters = parameters . getRight ();
			if (! parameters . isPair ()) return false;
			PrologElement y = parameters . getLeft (); if (! y . isNumber ()) return false; parameters = parameters . getRight ();
			Platform . runLater (new Runnable () {public void run () {viewport . setBoarderPosition (x . getNumber (), y . getNumber ());}});
			fxg . clean = false;
			return true;
		}
		if (atom . getAtom () == fxg . size_atom) {
			if (parameters . isVar ()) {
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . size . x); parameters = parameters . getRight ();
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . size . y);
				return true;
			}
			if (! parameters . isPair ()) return false;
			PrologElement w = parameters . getLeft (); if (! w . isNumber ()) return false; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
			PrologElement h = parameters . getLeft (); if (! h . isNumber ()) return false;
			Platform . runLater (new Runnable () {public void run () {viewport . setWindowSize (w . getNumber (), h . getNumber ());}});
			fxg . clean = false;
			return true;
		}
		if (atom . getAtom () == fxg . scaling_atom) {
			if (parameters . isVar ()) {parameters . setPair (); parameters . getLeft () . setDouble (viewport . scaling); return true;}
			if (! parameters . isPair ()) return false;
			PrologElement scaling = parameters . getLeft (); if (! scaling . isNumber ()) return false;
			viewport . scaling = scaling . getNumber ();
			fxg . clean = false;
			return true;
		}
		//if (atom . getAtom () == fxg . repaint_atom) {....}
		return false;
	}
	public viewport_action (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class viewport_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologElement name = null;
		PrologElement atom = null;
		double locations [] = {100.0, 100.0, 160.0, 90.0};
		int location_index = 0;
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isAtom ()) atom = el;
			if (el . isVar ()) atom = el;
			if (el . isText ()) name = el;
			if (el . isNumber () && location_index < 4) locations [location_index++] = el . getNumber ();
			parameters = parameters . getRight ();
		}
		if (atom == null) return false;
		if (atom . isVar ()) atom . setAtom (new PrologAtom ());
		if (! atom . isAtom ()) return false;
		if (atom . getAtom () . getMachine () != null) return false;
		viewport_action machine = new viewport_action (fxg);
		if (! atom . getAtom () . setMachine (machine)) return false;
		final String viewport_name = name != null ? name . getText () : atom . getAtom () . name ();
		machine . viewport = fxg . insert_viewport (machine, atom . getAtom (), viewport_name, new Rect (locations [0], locations [1], locations [2], locations [3]));
		return true;
	}
	public viewport_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class erase_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {fxg . erase (); return true;}
	public erase_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class clean_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {fxg . clean = true; return true;}
	public clean_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class is_clean_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {return fxg . clean;}
	public is_clean_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

class save_board_class extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false; parameters = parameters . getLeft (); if (! parameters . isText ()) return false;
		fxg . save (parameters . getText ());
		return true;
	}
	public save_board_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

public class PrologFXGStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public PrologAtom location_atom, size_atom, position_atom, scaling_atom, repaint_atom, mode_atom;
	public void remove_viewport (boarder_viewport viewport) {
		if (viewports == null) return;
		if (viewports == viewport) {viewports = viewports . next; return;}
		boarder_viewport v = viewports;
		while (v . next != null) {if (v . next == viewport) {v . next = v . next . next; return;} v = v . next;}
	}
	public boarder_viewport insert_viewport (viewport_action action, PrologAtom atom, String name, Rect location) {
		return viewports = new boarder_viewport (action, atom, name, location, viewports);
	}
	public boolean clean = true;
	public boarder_viewport viewports;
	public void save (String file_name) {
		try {
			FileWriter tc = new FileWriter (file_name);
			tc . write ("[auto_atoms]\n\n");
			tc . write ("[Erase]\n");
			if (viewports != null) viewports . save (tc);
			tc . write ("[wait 100]\n");
			tc . write ("[Clean]\n");
			tc . write ("[exit]\n\n");
			tc . close ();
		} catch (Exception ex) {return;}
		clean = true;
	}
	public void erase () {
		if (viewports != null) viewports . erase (); viewports = null;
		clean = true;
	}
	public void init (PrologRoot root, PrologDirectory directory) {
		this . root = root; this . directory = directory;
		if (location_atom == null && directory != null) {
			location_atom = directory . searchAtom ("Location");
			size_atom = directory . searchAtom ("Size");
			position_atom = directory . searchAtom ("Position");
			scaling_atom = directory . searchAtom ("Scaling");
			repaint_atom = directory . searchAtom ("Repaint");
			mode_atom = directory . searchAtom ("Mode");
		}
	}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("Viewport")) return new viewport_class (this);
		if (name . equals ("Erase")) return new erase_class (this);
		if (name . equals ("Clean")) return new clean_class (this);
		if (name . equals ("Clean?")) return new is_clean_class (this);
		if (name . equals ("SaveBoard")) return new save_board_class (this);
		return null;
	}
}

