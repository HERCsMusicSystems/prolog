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

public class PrologElement {
	public int type;
	public String text = null;
	public PrologAtom atom;
	public Object head;
	public int integer;
	public double floating_point;
	public PrologElement left;
	public PrologElement right;
	public PrologElement () {}
	public PrologElement (PrologAtom atom) {this . atom = atom; type = 3;}
	public PrologElement (String text) {this . text = text; type = 6;}
	public PrologElement (Object head) {this . head = head; type = 7;}
	public PrologElement (int integer) {this . integer = integer; type = 8;}
	public PrologElement (double floating_point) {this . floating_point = floating_point; type = 9;}
	public boolean isEarth () {return type == 0;}
	public boolean isPair () {return type == 1;}
	public boolean isVar () {return type == 2;}
	public boolean isAtom () {return type == 3;}
	public boolean isSlash () {return type == 4;}
	public boolean isFail () {return type == 5;}
	public boolean isText () {return type == 6;}
	public boolean isHead () {return type == 7;}
	public boolean isInteger () {return type == 8;}
	public boolean isDouble () {return type == 9;}
	public boolean isNumber () {return type == 8 || type == 9;}
	public void setEarth () {type = 0;}
	public void setPair (PrologElement l, PrologElement r) {type = 1; left = l; right = r;}
	public void setLeft (PrologElement l) {left = l;}
	public void setRight (PrologElement r) {right = r;}
	public void setPair () {type = 1; left = new PrologElement (); right = new PrologElement ();}
	public void setVar (int i) {type = 2; integer = i;}
	public void setAtom (PrologAtom a) {if (a == null) {type = -1; return;} type = 3; atom = a;}
	public void setSlash () {type = 4;}
	public void setFail () {type = 5;}
	public void setText (String t) {type = 6; text = "" + t;}
	public void setHead (Object o) {type = 7; head = o;}
	public void setInteger (int i) {type = 8; integer = i;}
	public void setDouble (double d) {type = 9; floating_point = d;}
	public PrologElement getLeft () {return left;}
	public PrologElement getRight () {return right;}
	public int getVar () {return integer;}
	public PrologAtom getAtom () {return atom;}
	public String getText () {return text;}
	public Object getHead () {return head;}
	public int getInteger () {return integer;}
	public double getDouble () {return floating_point;}
	public double getNumber () {return type == 8 ? (double) integer : floating_point;}
	public int getType () {return type;}
	public PrologElement duplicate () {
		PrologElement ret = new PrologElement ();
		ret . type = type;
		switch (type) {
		case 1:
			ret . left = left . duplicate ();
			ret . right = right . duplicate ();
			return ret;
		case 2: case 8: ret . integer = integer; return ret;
		case 3: ret . atom = atom; return ret;
		case 6: ret . text = "" + text; return ret;
		case 7: ret . head = head; return ret;
		case 9: ret . floating_point = floating_point; return ret;
		default: return ret;
		}
	}
	public void duplicate (PrologElement e) {
		switch (e . type) {
		case 0: setEarth (); break;
		case 1: setPair (e . getLeft () . duplicate (), e . getRight () . duplicate ()); break;
		case 2: setVar (e . getVar ()); break;
		case 3: setAtom (e . getAtom ()); break;
		case 4: setSlash (); break;
		case 5: setFail (); break;
		case 6: setText (e . getText ()); break;
		case 7: setHead (e . getHead ()); break;
		case 8: setInteger (e . getInteger ()); break;
		case 9: setDouble (e . getDouble ()); break;
		default: break;
		}
	}
	public void drop_one_right () {right = right . right;}
}

