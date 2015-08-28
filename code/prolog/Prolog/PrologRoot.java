
package Prolog;

public class PrologRoot {
	public boolean auto_atoms = false;
	public void resolution () {System . out . println ("Default resolution: studio");}
	public void resolution (String name) {System . out . println ("Named resolution: " + name);}
	public void addArg (String arg) {System . out . println ("Argument added: " + arg);}
}

