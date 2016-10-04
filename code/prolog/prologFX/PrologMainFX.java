
package PrologFX;

import Prolog . *;
import Prolog . geometry . *;

import javafx . application . *;
import javafx . stage . *;
import javafx . scene . Scene;
import javafx . scene . layout . *;
import javafx . scene . control . *;
import javafx . scene . canvas . *;
import javafx . scene . paint . *;
import javafx . scene . text . *;
import javafx . event . ActionEvent;
import javafx . geometry . *;
import javafx . beans . value . *;

class emitter extends java . io . OutputStream {
	private String text = "";
	public void write (int ind) {
		text += (char) ind;
		if (ind == 10) {
			if (text . length () > 400) text = text . substring (text . length () - 400);
			Platform . runLater (new Runnable () {public void run () {PrologMainFX . feedback . setText (text);}});
		}
	}
}

public class PrologMainFX extends Application {
	public static PrologRoot root = null;
	public static java . io . PrintStream oout = null;
	public static Stage stage = null;
	public static Canvas canvas = null;
	public static Label feedback = null;
	public static GraphicsContext gc = null;
	public static String call_arguments [] = new String [0];
	public static Point initial_size = new Point (512.0, 512.0);
	public static AnchorPane anchor = null;
	public static TextArea area = null;
	public static String file_name = null;
	public void call_repaint () {
		if (root == null) return;
		PrologAtom atom = root . search ("fx_resize_callback"); if (atom == null) return;
		PrologElement query = root . pair (root . atom (atom), root . pair (root . Double (stage . getWidth ()), root . pair (root . Double (stage . getHeight ()), root . earth ())));
		query = root . pair (query, root . earth ());
		query = root . pair (root . head (null), query);
		root . resolution (query);
	}
	public void start (Stage stage) {
		System . out . println ("START");
		this . stage = stage;
		Rectangle2D bounds = Screen . getPrimary () . getVisualBounds ();
		if (initial_size . eq (new Point (0.0, 0.0))) initial_size = new Point (bounds . getWidth (), bounds . getHeight ());
		build_uap_system ();
		root . insertCommander (new java . io . PrintStream (new emitter ()));
		TextField kn = new TextField ();
		TextField command = new TextField ();
		command . setOnAction ((ActionEvent event) -> {
			String cmd = command . getText ();
			if (cmd . charAt (0) != '[' && cmd . charAt (0) != '(') cmd = "[" + cmd + "]";
			oout . print (cmd + "\n"); command . setText ("");
		});
		Platform . runLater (new Runnable () {public void run () {command . requestFocus ();}});
		Button keyboard = new Button ("KB");
		keyboard . setOnAction ((ActionEvent event) -> {Platform . runLater (new Runnable () {public void run () {command . requestFocus ();}});});
		area = new TextArea ();
		//===============================
		//================================
		anchor = new AnchorPane ();
		canvas = new Canvas (initial_size . x, initial_size . y);
		gc = canvas . getGraphicsContext2D ();
		gc . setFill (Color . BLACK);
		gc . fillRect (0.0, 0.0, initial_size . x, initial_size . y);
		feedback = new Label (); feedback . setTextFill (Color . YELLOW);
		AnchorPane . setLeftAnchor (feedback, 4.0);
		AnchorPane . setBottomAnchor (feedback, (double) Math . round (initial_size . y * 0.75));
		AnchorPane . setLeftAnchor (canvas, 0.0);
		AnchorPane . setRightAnchor (canvas, 0.0);
		AnchorPane . setTopAnchor (canvas, 0.0);
		AnchorPane . setBottomAnchor (canvas, 0.0);
		AnchorPane . setRightAnchor (command, 0.0);
		AnchorPane . setLeftAnchor (command, 0.0);
		AnchorPane . setTopAnchor (command, 0.0);
		AnchorPane . setTopAnchor (area, 30.0);
		AnchorPane . setLeftAnchor (area, 10.0);
		AnchorPane . setRightAnchor (area, 10.0);
		AnchorPane . setBottomAnchor (area, 10.0);
		AnchorPane . setBottomAnchor (keyboard, 1.0);
		AnchorPane . setRightAnchor (keyboard, 1.0);
		anchor . getChildren () . addAll (canvas, feedback, command, keyboard);
		//===============================
		Scene scene = new Scene (anchor, initial_size . x, initial_size . y);
		stage . setTitle ("sonda");
		stage . setScene (scene);
		stage . show ();
		stage . heightProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {
				Platform . runLater (new Runnable () {public void run () {call_repaint ();}});
			}
		});
		stage . widthProperty () . addListener (new ChangeListener <Number> () {
			public void changed (ObservableValue <? extends Number> o, Number old, Number current) {
				Platform . runLater (new Runnable () {public void run () {call_repaint ();}});
			}
		});
	}
	public static void fxedit_command (String name) {
		if (name == null) {
			file_name = null;
			Platform . runLater (new Runnable () {public void run () {anchor . getChildren () . addAll (area);}});
		} else {
			try {
				java . io . FileReader fr = new java . io . FileReader (root . ccd (name));
				String content = "";
				int ch;
				while ((ch = fr . read ()) >= 0) content += (char) ch;
				Platform . runLater (new Runnable () {public void run () {anchor . getChildren () . addAll (area);}});
				area . setText (content);
				file_name = "" + name;
			} catch (Exception ex) {}
		}
	}
	public static boolean fxsave_command (String name) {
		if (name == null) name = file_name;
		if (name == null) return false;
		try {
			java . io . FileWriter fw = new java . io . FileWriter (root . ccd (name));
			fw . write (area . getText ());
			fw . close ();
			return true;
		} catch (Exception ex) {}
		return false;
	}
	public static void build_uap_system () {
		if (root != null) return;
		root = new PrologRoot ();
		root . set_uap32_captions ();
		root . resolution ("fx");
	}
	public static void main (String [] args) {
		initial_size = new Point (0.0, 0.0);
		System . out . println ("MAIN solo");
		call_arguments = args;
		build_uap_system ();
		System . out . println ("MAIN completed.");
	}
	public static void activate (PrologRoot uap_system, java . io . PrintStream out) {
		root = uap_system;
		oout = out;
		System . out . println ("ACTIVATING....");
		if (stage == null) launch (call_arguments);
	}
	public static void stop_fx () {Platform . exit ();}
}

