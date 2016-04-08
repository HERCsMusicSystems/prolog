
import java . io . InputStream;

import Prolog . *;

public class studio {
	public static void main (String [] args) {
		PrologRoot root = new PrologRoot ();
		root . get_search_directories_from_environment ("STUDIO_HOME");
		root . set_uap32_captions ();
		String name = null;
		for (int ind = 0; ind < args . length; ind++) {
			if (name == null) name = "" + args [ind];
			else root . addArg (args [ind]);
		}
		// root . insertCommander (System . out); default value in PrologRoot
		if (name == null) {root . auto_atoms = true; root . resolution ();}
		else root . resolution (name);
	}
}
