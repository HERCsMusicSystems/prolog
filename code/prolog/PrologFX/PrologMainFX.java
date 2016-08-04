
package PrologFX;

import Prolog . *;

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
	public static String call_arguments [] = null;
	public void start (Stage stage) {
		System . out . println ("START");
		this . stage = stage;
		build_uap_system ();
		root . insertCommander (new java . io . PrintStream (new emitter ()));
		TextField command = new TextField ();
		command . setOnAction ((ActionEvent event) -> {oout . print (command . getText () + "\n"); command . setText ("");});
		Platform . runLater (new Runnable () {public void run () {command . requestFocus ();}});
		//===============================
		//================================
		AnchorPane anchor = new AnchorPane ();
		Rectangle2D bounds = Screen . getPrimary () . getVisualBounds ();
		canvas = new Canvas (100, 100);//bound . getWidth (), bounds . getHeight());
		gc = canvas . getGraphicsContext2D ();
		feedback = new Label ();
		AnchorPane . setLeftAnchor (feedback, 4.0);
		AnchorPane . setTopAnchor (feedback, 4.0);
		AnchorPane . setLeftAnchor (canvas, 0.0);
		AnchorPane . setRightAnchor (canvas, 0.0);
		AnchorPane . setTopAnchor (canvas, 0.0);
		AnchorPane . setBottomAnchor (canvas, 0.0);
		AnchorPane . setRightAnchor (command, 0.0);
		AnchorPane . setLeftAnchor (command, 0.0);
		AnchorPane . setBottomAnchor (command, 0.0);
		anchor . getChildren () . addAll (canvas, command, feedback);
		//===============================
		Scene scene = new Scene (anchor, 200, 200); //bounds . getWidth (), bounds . getHeight ());
		stage . setTitle ("sonda");
		stage . setScene (scene);
		stage . show ();
	}
	public static void build_uap_system () {
		if (root != null) return;
		root = new PrologRoot ();
		root . set_uap32_captions ();
		root . resolution ("fx");
	}
	public static void main (String [] args) {
		System . out . println ("MAIN solo");
		call_arguments = args;
		build_uap_system ();
		System . out . println ("MAIN completed.");
	}
	public static void activate (PrologRoot uap_system, java . io . PrintStream out) {
		root = uap_system;
		oout = out;
		System . out . println ("ACTIVATING....");
		if (call_arguments == null) launch (new String [0]);
		else launch (call_arguments);
	}
	public static void stop_fx () {Platform . exit ();}
}

