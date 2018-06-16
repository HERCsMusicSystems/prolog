
import java . io . InputStream;

import prolog . *;

public class studio {
	public static void main (String [] args) {
		PrologRoot root = new PrologRoot ();
		root . get_search_directories_from_environment ("JAVA_STUDIO_HOME");
		root . set_uap32_captions ();
		String name = null;
		for (int ind = 0; ind < args . length; ind++) {
			if (name == null) name = "" + args [ind];
			else root . addArg (args [ind]);
		}
		// root . insertCommander (System . out); default value in PrologRoot
		root . resolution (name);
		System . exit (root . exit_code);
	}
}
