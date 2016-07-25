
package fxg;

import Prolog . *;
import Prolog . geometry . *;

import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . stage . Modality;
import javafx . scene . Scene;
import javafx . scene . layout . StackPane;
import javafx . beans . value . ChangeListener;
import javafx . beans . value . ObservableValue;

class boarder_viewport {
	public PrologAtom atom;
	public String viewport_name;
	public Stage viewport;
	public Rect location;
	public double scaling = 1.0;
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
	public void build (String name, Rect loc) {
		this . location = new Rect (loc);
		viewport = new Stage ();
		viewport . setTitle (viewport_name);
		viewport . initModality (Modality . NONE);
		viewport . initOwner (null);
		StackPane pane = new StackPane ();
		Scene viewport_scene = new Scene (pane, location . size . x, location . size . y);
		viewport . setScene (viewport_scene);
		viewport . setX (location . position . x); viewport . setY (location . position . y);
		viewport . widthProperty () . addListener (new ChangeListener<Number> () {
			public void changed (ObservableValue<? extends Number> o, Number old, Number current) {location . size . x = (double) current;}
		});
		viewport . heightProperty () . addListener (new ChangeListener<Number> () {
			public void changed (ObservableValue<? extends Number> o, Number old, Number current) {location . size . y = (double) current;}
		});
		viewport . xProperty () . addListener (new ChangeListener<Number> () {
			public void changed (ObservableValue<? extends Number> o, Number old, Number current) {location . position . x = (double) current;}
		});
		viewport . yProperty () . addListener (new ChangeListener<Number> () {
			public void changed (ObservableValue<? extends Number> o, Number old, Number current) {location . position . y = (double) current;}
		});
		viewport . show ();
	}
	public void setBoarderPosition (double x, double y) {viewport . setX (location . position . x = x); viewport . setY (location . position . y = y);}
	public void setWindowSize (double w, double h) {viewport . setWidth (location . size . x = w); viewport . setHeight (location . size . y = h);}
	public void setWindowLocation (double x, double y, double w, double h) {
		viewport . setX (location . position . x = x);
		viewport . setY (location . position . y = y);
		viewport . setWidth (location . size . x = w);
		viewport . setHeight (location . size . y = h);
	}
	public void change_viewport_name () {viewport . setTitle (viewport_name + " [" + edit_mode . name () + "]");}
	public boarder_viewport (PrologAtom atom, String viewport_name) {this . atom = atom; this . viewport_name = viewport_name;}
}

class viewport_action extends PrologNativeCode {
	public PrologFXGStudio fxg;
	public PrologAtom location_atom, size_atom, position_atom, scaling_atom, repaint_atom, mode_atom;
	public boarder_viewport viewport;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {fxg . clean = false; viewport . atom . setMachine (null); fxg . remove_viewport (viewport); return true;}
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
				parameters . setPair (); parameters . getLeft () . setDouble (viewport . location . position . x);
				parameters = parameters . getRight (); parameters . setPair ();
				parameters . getLeft () . setDouble (viewport . location . position . y);
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
		machine . viewport = fxg . insert_viewport (atom . getAtom (), viewport_name);
		Platform . runLater (new Runnable () {public void run () {
			machine . viewport . build (viewport_name, new Rect (locations [0], locations [1], locations [2], locations [3]));
		}});
		return true;
	}
	public viewport_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

/*////////////
		viewport_action * machine = new viewport_action (directory);
		if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
		PROLOG_STRING viewport_name;
		if (name == 0) prolog_string_copy (viewport_name, "VIEWPORT");
		else prolog_string_copy (viewport_name, name -> getText ());
		machine -> viewport = board -> insert_viewport (atom -> getAtom (), viewport_name, rect (locations));
		//CreateViewportIdleCode (machine -> viewport);
		g_idle_add ((GSourceFunc) CreateViewportIdleCode, machine -> viewport);
		return true;
	}
	viewport (PrologDirectory * directory) {this -> directory = directory;}
};

*////////////


public class PrologFXGStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public PrologAtom location_atom, size_atom, position_atom, scaling_atom, repaint_atom, mode_atom;
	public void remove_viewport (boarder_viewport viewport) {}
	public boarder_viewport insert_viewport (PrologAtom atom, String name) {return new boarder_viewport (atom, name);}
	public boolean clean = true;
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
		return null;
	}
}

