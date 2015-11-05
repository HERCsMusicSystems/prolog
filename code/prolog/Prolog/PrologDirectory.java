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

public class PrologDirectory {
	public String directoryName = null;
	public PrologDirectory next = null;
	public PrologAtom firstAtom = null;
	public PrologServiceClass service_class = null;
	public PrologDirectory (String name, PrologDirectory root) {directoryName = "" + name; next = root;}
	public PrologDirectory (String name, PrologDirectory root, PrologServiceClass service) {directoryName = "" + name; next = root; service_class = service;}
	public PrologServiceClass getServiceClass () {return service_class;}
	public PrologServiceClass getServiceClass (String class_name) {
		if (directoryName . equals (class_name)) return service_class;
		if (next == null) return null;
		return next . getServiceClass (class_name);
	}
	public String name () {return directoryName;}
	public String names () {
		String area = "" + directoryName;
		PrologDirectory sub = next;
		while (sub != null) {area += " " + sub . name (); sub = sub . next;}
		return area;
	}
	public PrologAtom searchAtom (String search) {if (firstAtom == null) return null; return firstAtom . search (search);}
	public PrologAtom searchPrivateAtom (String search) {if (firstAtom == null) return null; return firstAtom . searchPrivate (search);}
	public PrologAtom createAtom (String name) {firstAtom = new PrologAtom (name, firstAtom); return firstAtom;}
	public boolean removeAtom (PrologAtom atom) {
		if (firstAtom == null) {if (next != null) return next . removeAtom (atom); return false;}
		PrologAtom to_delete;
		if (firstAtom == atom) {to_delete = firstAtom; firstAtom = firstAtom . next; to_delete . next = null; return true;}
		PrologAtom ap = firstAtom;
		to_delete = ap . next;
		while (to_delete != null) {
			if (to_delete == atom) {ap . next = to_delete . next; to_delete . next = null; return true;}
			ap = to_delete; to_delete = ap . next;
		}
		if (next == null) return false;
		return next . removeAtom (atom);
	}
	public String list () {if (firstAtom == null) return ""; return firstAtom . names ();}
	public PrologDirectory close () {if (next == null) return this; PrologDirectory sub = next; next = next . next; sub . next = this; return sub;}
	public PrologDirectory duplicate (PrologDirectory root) {
		PrologDirectory dir = new PrologDirectory (directoryName, root, service_class);
		dir . firstAtom = firstAtom;
		return dir;
	}
};

