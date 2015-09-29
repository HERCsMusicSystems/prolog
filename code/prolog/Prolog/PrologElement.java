
package Prolog;

public class PrologElement {
	public int type;
	public int head;
	public PrologElement getLeft () {return null;}
	public PrologElement getRight () {return null;}
	public double getDouble () {return 0.0;}
	public int getInteger () {return 0;}
	public String getText () {return "";}
	public PrologAtom getAtom () {return null;}
	public void setPair (PrologElement left, PrologElement right) {}
	public void setVar (int num) {}
	public void setAtom (PrologAtom atom) {}
	public void setSlash () {}
	public void setFail () {}
	public void setText (String text) {}
	public void setHead (Object obj) {}
}

