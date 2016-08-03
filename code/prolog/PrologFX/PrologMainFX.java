
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
import javafx . scene . control . TextArea;
import javafx . scene . canvas . Canvas;
import javafx . scene . canvas . GraphicsContext;
import javafx . scene . paint . Color;
import javafx . event . ActionEvent;
import javafx . geometry . Insets;
import javafx . geometry . Rectangle2D;

class emitter extends java . io . OutputStream {
	private TextArea area;
	private String text = "";
	public void write (int ind) {
		text += (char) ind;
		if (ind == 10) {area . appendText (text); text = "";}
		//area . appendText ("" + (char) ind);
	}
	emitter (TextArea area) {this . area = area;}
}

public class PrologMainFX extends Application {
	public static PrologRoot main_root = null;
	public static java . io . PrintStream oout = null;
	public static Stage stage = null;
	public static Canvas canvas = null;
	public static GraphicsContext gc = null;
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
		TextArea area = new TextArea ();
		main_root . insertCommander (new java . io . PrintStream (new emitter (area)));
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
		root . add (area, 0, 0, 3, 1);
		root . add (command_label, 0, 1);
		root . add (command, 1, 1);
		root . add (exit, 2, 1);
		Rectangle2D bounds = Screen . getPrimary () . getVisualBounds ();
		canvas = new Canvas (bounds . getWidth (), bounds . getHeight () * 0.5);
		gc = canvas . getGraphicsContext2D ();
		root . add (canvas, 0, 2, 3, 1);
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

