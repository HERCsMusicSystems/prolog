///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

package Prolog;

public class PrologAtom {
	public String atomName = null;
	public PrologAtom next = null;
	public boolean Privated = false;
	public boolean Protected = false;
	public PrologElement firstClause = null;
	public PrologNativeCode machine = null;
	public PrologAtom () {atomName = "<" + this . toString () + "#" + System . currentTimeMillis () + ">";}
	public PrologAtom (String name) {atomName = "" + name;}
	public PrologAtom (String name, PrologAtom root) {atomName = "" + name; next = root;}
	public String name () {return atomName;}
	public String names () {
		String area = "" + atomName;
		PrologAtom sub = next;
		while (sub != null) {area += " " + sub . name (); sub = sub . next;}
		return area;
	}
	public PrologAtom search (String name) {
		PrologAtom ret = this;
		while (ret . Privated || ! ret . atomName . equals (name)) {
			ret = ret . next;
			if (ret == null) return null;
		}
		return ret;
	}
	public PrologAtom searchPrivate (String name) {
		PrologAtom ret = this;
		while (! ret . atomName . equals (name)) {
			ret = ret . next;
			if (ret == null) return null;
		}
		return ret;
	}
	public void Protect () {Protected = true;}
	public void Private () {Privated = true;}
	public void unProtect () {Protected = false;}
	public void unPrivate () {Privated = false;}
	public boolean setMachine (PrologNativeCode obj) {
		if (firstClause != null || (Protected && obj != null)) return false;
		Protected = (obj != null);
		machine = obj;
		return true;
	}
	public boolean isTypeOf (PrologAtom atom) {
		if (machine == null) return false;
		PrologNativeCode machine_type = atom . getMachine ();
		if (machine_type == null) return false;
		return machine . isTypeOf (machine_type . codeName ());
	}
	public boolean isTypeOf (PrologNativeCode code) {
		if (machine == null) return false;
		if (code == null) return false;
		return machine . isTypeOf (code . codeName ());
	}
	public String machineType () {return machine == null ? null : machine . codeName ();}
	public PrologNativeCode getMachine () {return machine;}
}

