
package PrologFX;

import Prolog . *;

import javafx . application . Application;
import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . scene . Scene;
import javafx . scene . layout . StackPane;
import javafx . scene . layout . HBox;
import javafx . scene . layout . GridPane;
import javafx . scene . control . Button;
import javafx . scene . control . Tooltip;
import javafx . scene . control . Label;
import javafx . scene . control . TextField;
import javafx . scene . control . ListView;
import javafx . scene . control . TextArea;
import javafx . event . ActionEvent;
import javafx . geometry . Insets;

class emitter extends java . io . OutputStream {
	private TextArea area;
	public void write (int ind) {area . appendText ("" + (char) ind);}
	emitter (TextArea area) {this . area = area;}
}

class Swallower extends java . io . InputStream {
	private TextArea area;
	public String command = "    [list]" + (char) 13 + (char) 10;
	public int readd () {try {int ind = System . in . read (); area . appendText ("" + (char) ind); return ind;} catch (Exception ex) {return 0;}}
	public int read () {
		if (command . length () > 0) {
			int ind = command . charAt (0);
			command = command . substring (1);
			System . out . println ("<" + (char) ind + ">");
			return ind;
		}
		return -1;
	}
	Swallower (TextArea area) {this . area = area;}
}

public class PrologMainFX extends Application {
	public static PrologRoot main_root = null;
	public void start (Stage stage) {
		Button exit = new Button ();
		exit . setText ("_Exit");
		exit . setOnAction ((ActionEvent event) -> {Platform . exit ();});
		Tooltip . install (exit, new Tooltip ("exit control"));
		Label command_label = new Label ("Command:");
		TextArea area = new TextArea ();
		main_root . insertCommander (new java . io . PrintStream (new emitter (area)));
		Swallower swallower = new Swallower (area);
		main_root . insertReader (swallower);
		TextField command = new TextField ();
		command . setOnAction ((ActionEvent event) -> {area . appendText (command . getText () + "\n"); System . out . println (command . getText ()); command . setText ("");});
		Platform . runLater (new Runnable () {public void run () {command . requestFocus ();}});
		//===============================
		GridPane root = new GridPane ();
		//root . setGridLinesVisible (true);
		root . setHgap (4);
		root . setVgap (4);
		root . setPadding (new Insets (4));
		root . add (area, 0, 0, 3, 1);
		root . add (command_label, 0, 1);
		root . add (command, 1, 1);
		root . add (exit, 2, 1);
		//===============================
		Scene scene = new Scene (root, 300, 200);
		stage . setTitle ("sonda");
		stage . setScene (scene);
		stage . show ();
	}
	public static void main (String [] args, PrologRoot root) {
		main_root = root;
		launch (args);
	}
	public static void stop_fx () {Platform . exit ();}
}

