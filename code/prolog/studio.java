
import java . io . InputStream;

import Prolog . *;

public class studio {
	public static void main (String [] args) {
		PrologRoot root = new PrologRoot ();
		String name = null;
		for (int ind = 0; ind < args . length; ind++) {
			if (name == null) name = "" + args [ind];
			else root . addArg (args [ind]);
		}
		if (name == null) {root . auto_atoms = true; root . resolution ();}
		else {
			if (name . indexOf (".prc") < 0 && name . indexOf (".prb") < 0) name += ".prc";
			root . resolution (name);
		}
	}
}
/*	public studio () {
		InputStream prcgtk = this . getClass () . getResourceAsStream ("studio.prc");
		int ch;
		try {
			while ((ch = prcgtk . read ()) >= 0) {
				System . out . print ((char) ch);
			}
			prcgtk . close ();
			System . out . println ("CLOSED");
		} catch (Exception ex) {}
	}
}*/

