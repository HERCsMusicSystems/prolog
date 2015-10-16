
import java . io . InputStream;

import Prolog . *;

public class studio {
//	private static class service_class_loader_class extends PrologServiceClassLoader {
//		public PrologServiceClass load (String name) {
//			return null;
//		}
//	}
	public static void main (String [] args) {
		PrologRoot root = new PrologRoot ();
		root . get_search_directories_from_environment ("STUDIO_HOME");
//		root . setServiceClassLoader (new service_class_loader_class ());
		root . set_uap32_captions ();
		String name = null;
		for (int ind = 0; ind < args . length; ind++) {
			if (name == null) name = "" + args [ind];
			else root . addArg (args [ind]);
		}
		// root . insertCommander (System . out); default value in PrologRoot
		if (name == null) {root . auto_atoms = true; root . resolution ();}
		else {
			if (name . indexOf (".prc") < 0 && name . indexOf (".prb") < 0) name += ".prc";
			root . resolution (name);
		}
	}
}
