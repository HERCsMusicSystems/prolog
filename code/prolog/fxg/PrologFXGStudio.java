
package fxg;

import Prolog . *;
import Prolog . geometry . *;

import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . stage . Modality;
import javafx . scene . Scene;
import javafx . scene . layout . StackPane;

class boarder_viewport {
	public PrologAtom atom;
	public String viewport_name;
	public Stage viewport;
	public Rect location;
	public void build (String name, Rect location) {
		this . location = new Rect (location);
		viewport = new Stage ();
		viewport . setTitle (viewport_name);
		viewport . initModality (Modality . NONE);
		viewport . initOwner (null);
		StackPane pane = new StackPane ();
		Scene viewport_scene = new Scene (pane, location . size . x, location . size . y);
		viewport . setScene (viewport_scene);
		viewport . setX (location . position . x); viewport . setY (location . position . y);
		viewport . show ();
	}
	public void setBoarderPosition (double x, double y) {viewport . setX (location . position . x); viewport . setY (location . position . y);}
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
		return true;
	}
	public viewport_action (PrologFXGStudio fxg) {this . fxg = fxg;}
}
/*
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (atom -> getAtom () == mode_atom ) {
			if (parameters -> isVar ()) {parameters -> setInteger ((int) viewport -> edit_mode); return true;}
			if (! parameters -> isPair ()) return false; parameters = parameters -> getLeft ();
			if (! parameters -> isInteger ()) return false;
			viewport -> edit_mode = (boarder_viewport :: edit_modes) parameters -> getInteger ();
			g_idle_add ((GSourceFunc) ChangeViewportNameIdleCode, viewport);
			return true;
		}
		if (atom -> getAtom () == location_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> location . position . x);
				parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> location . position . y);
				parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> location . size . x);
				parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> location . size . y);
				return true;
			}
			if (! parameters -> isPair ()) return false;
			PrologElement * x = parameters -> getLeft (); if (! x -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			PrologElement * y = parameters -> getLeft (); if (! y -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			PrologElement * width = parameters -> getLeft (); if (! width -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			PrologElement * height = parameters -> getLeft (); if (! height -> isInteger ()) return false; parameters = parameters -> getRight ();
			viewport -> setWindowLocation (rect (x -> getInteger (), y -> getInteger (), width -> getInteger (), height -> getInteger ()));
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == position_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> board_position . x);
				parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> board_position . y);
				return true;
			}
			if (! parameters -> isPair ()) return false;
			PrologElement * x = parameters -> getLeft (); if (! x -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			PrologElement * y = parameters -> getLeft (); if (! y -> isInteger ()) return false; parameters = parameters -> getRight ();
			viewport -> setBoardPosition (point (x -> getInteger (), y -> getInteger ()));
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == size_atom) {
			if (parameters -> isVar ()) {
				parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> location . size . x);
				parameters = parameters -> getRight (); parameters -> setPair ();
				parameters -> getLeft () -> setInteger ((int) viewport -> location . size . y);
				return true;
			}
			if (! parameters -> isPair ()) return false;
			PrologElement * width = parameters -> getLeft (); if (! width -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			PrologElement * height = parameters -> getLeft (); if (! height -> isInteger ()) return false; parameters = parameters -> getRight ();
			viewport -> setWindowSize (point (width -> getInteger (), height -> getInteger ()));
			boarder_clean = false;
			return true;
		}
		if (atom -> getAtom () == scaling_atom) {
			if (parameters -> isVar ()) {parameters -> setPair (); parameters -> getLeft () -> setDouble (viewport -> scaling); return true;}
			if (! parameters -> isPair ()) return false;
			PrologElement * scaling = parameters -> getLeft ();
			if (scaling -> isDouble ()) {viewport -> scaling = scaling -> getDouble (); boarder_clean = false; return true;}
			if (scaling -> isInteger ()) {viewport -> scaling = (int) scaling -> getInteger (); boarder_clean = false; return true;}
			return false;
		}
		if (atom -> getAtom () == repaint_atom) {gtk_widget_queue_draw (viewport -> window); return true;}
		return false;
	}
};
*/
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

