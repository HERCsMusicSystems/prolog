
package PrologFX;

import Prolog . *;

import javafx . application . Application;
import javafx . application . Platform;
import javafx . stage . Stage;
import javafx . stage . Modality;
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
	public void start (Stage stage) {
		Button exit = new Button ();
		exit . setText ("_Exit");
		exit . setOnAction ((ActionEvent event) -> {Platform . exit ();});
		Tooltip . install (exit, new Tooltip ("exit control"));
		Label command_label = new Label ("Command:");
		TextArea area = new TextArea ();
		main_root . insertCommander (new java . io . PrintStream (new emitter (area)));
		TextField command = new TextField ();
		command . setOnAction ((ActionEvent event) -> {
			oout . print (command . getText () + "\n");
			//area . appendText (command . getText () + "\n");
			command . setText ("");
		});
		Platform . runLater (new Runnable () {public void run () {command . requestFocus ();}});
		Button extra = new Button ();
		extra . setText ("Extra");
		extra . setOnAction ((ActionEvent event) -> {
			area . appendText ("extras....");
			Stage dialog = new Stage ();
			dialog . initModality (Modality . NONE);
			dialog . initOwner (null);
			StackPane pane = new StackPane ();
			Scene dialog_scene = new Scene (pane, 200, 200);
			dialog . setScene (dialog_scene);
			dialog . show ();
		});
		//===============================
		GridPane root = new GridPane ();
		//root . setGridLinesVisible (true);
		root . setHgap (4);
		root . setVgap (4);
		root . setPadding (new Insets (4));
		root . add (area, 0, 0, 4, 1);
		root . add (command_label, 0, 1);
		root . add (command, 1, 1);
		root . add (exit, 2, 1);
		root . add (extra, 3, 1);
		//===============================
		Scene scene = new Scene (root, 360, 200);
		stage . setTitle ("sonda");
		stage . setScene (scene);
		stage . show ();
	}
	public static void main (String [] args, PrologRoot root, java . io . PrintStream ps) {
		main_root = root;
		oout = ps;
		launch (args);
	}
	public static void stop_fx () {Platform . exit ();}
}

