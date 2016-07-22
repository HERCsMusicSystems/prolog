
package fxg;

import Prolog . *;

import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . stage . Modality;
import javafx . scene . Scene;
import javafx . scene . layout . StackPane;

class viewport_action extends PrologNativeCode {
	public PrologDirectory directory;
	viewport_action (PrologDirectory directory) {this . directory = directory;}
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
		viewport_action machine = new viewport_action (fxg . directory);
		if (! atom . getAtom () . setMachine (machine)) return false;
		final String viewport_name = name != null ? name . getText () : atom . getAtom () . name ();
		Platform . runLater (new Runnable () {public void run () {
			Stage viewport = new Stage ();
			viewport . setTitle (viewport_name);
			viewport . initModality (Modality . NONE);
			viewport . initOwner (null);
			StackPane pane = new StackPane ();
			Scene viewport_scene = new Scene (pane, locations [2], locations [3]);
			viewport . setScene (viewport_scene);
			viewport . setX (locations [0]); viewport . setY (locations [1]);
			viewport . show ();
		}});
		return true;
	}
	public viewport_class (PrologFXGStudio fxg) {this . fxg = fxg;}
}

/*////////////
		if (atom == 0) return false;
		if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
		if (! atom -> isAtom ()) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
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
	public void init (PrologRoot root, PrologDirectory directory) {this . root = root; this . directory = directory;}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("Viewport")) return new viewport_class (this);
		return null;
	}
}
