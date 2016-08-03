
package PrologFX;

import Prolog . *;

import javafx . application . Application;
import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . stage . Modality;
import javafx . stage . Screen;
import javafx . scene . Scene;
import javafx . scene . layout . StackPane;
import javafx . scene . layout . HBox;
import javafx . scene . layout . GridPane;
import javafx . scene . control . Button;
import javafx . scene . control . Tooltip;
import javafx . scene . control . Label;
import javafx . scene . control . TextField;
import javafx . scene . control . ListView;
import javafx . scene . canvas . Canvas;
import javafx . scene . canvas . GraphicsContext;
import javafx . scene . paint . Color;
import javafx . scene . text . TextAlignment;
import javafx . event . ActionEvent;
import javafx . geometry . Insets;
import javafx . geometry . Rectangle2D;
import javafx . geometry . VPos;

class emitter extends java . io . OutputStream {
	private String text = "";
	public void write (int ind) {
		text += (char) ind;
		if (ind == 10) {
			PrologMainFX . text += text;
			text = "";
			Platform . runLater (new Runnable () {public void run () {PrologMainFX . repaint ();}});
		}
	}
}

public class PrologMainFX extends Application {
	public static PrologRoot main_root = null;
	public static java . io . PrintStream oout = null;
	public static Stage stage = null;
	public static Canvas canvas = null;
	public static GraphicsContext gc = null;
	public static String text = "";
	public static void repaint () {
		gc . setTextBaseline(VPos . TOP);
		gc . setFont (new javafx . scene . text . Font ("Monospaced", 20));
		gc . setFill (new Color (0.0, 0.0, 1.0, 1.0));
		gc . fillText(text, 0.0, 0.0);
	}
	public void start (Stage stage) {
		this . stage = stage;
		if (main_root == null) {
			main_root = new PrologRoot ();
			main_root . set_uap32_captions ();
			main_root . resolution ("fx");
		}
		Button exit = new Button ();
		exit . setText ("_Exit");
		exit . setOnAction ((ActionEvent event) -> {Platform . exit ();});
		Tooltip . install (exit, new Tooltip ("exit control"));
		Label command_label = new Label ("Command:");
		main_root . insertCommander (new java . io . PrintStream (new emitter ()));
		TextField command = new TextField ();
		command . setPrefWidth (400);
		command . setOnAction ((ActionEvent event) -> {
			oout . print (command . getText () + "\n");
			//area . appendText (command . getText () + "\n");
			command . setText ("");
		});
		Platform . runLater (new Runnable () {public void run () {command . requestFocus ();}});
		//===============================
		GridPane root = new GridPane ();
		//root . setGridLinesVisible (true);
		root . setHgap (4);
		root . setVgap (4);
		root . setPadding (new Insets (4));
		root . add (command_label, 0, 0);
		root . add (command, 1, 0);
		root . add (exit, 2, 0);
		Rectangle2D bounds = Screen . getPrimary () . getVisualBounds ();
		canvas = new Canvas (bounds . getWidth (), bounds . getHeight () * 0.5);
		gc = canvas . getGraphicsContext2D ();
		for (String name : javafx . scene . text . Font . getFamilies ()) {
			System . out . println (name);
		}
		repaint ();
		root . add (canvas, 0, 1, 3, 1);
		//===============================
		Scene scene = new Scene (root, bounds . getWidth (), bounds . getHeight ());
		stage . setTitle ("sonda");
		stage . setScene (scene);
		stage . show ();
	}
	public static void main (String [] args, PrologRoot root, java . io . PrintStream ps) {
		oout = ps;
		if (main_root == null) {
			main_root = root;
			launch (args);
		}
	}
	public static void stop_fx () {Platform . exit ();}
}

