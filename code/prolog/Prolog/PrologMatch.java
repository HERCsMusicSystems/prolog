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

public class PrologMatch {
	public int var_number;
	public PrologVariable var;
	public PrologMatch next;
	public PrologMatch (int var_number, PrologVariable var, PrologMatch next) {this . var_number = var_number; this . var = var; this . next = next;}
	public PrologMatch (int var_number, PrologMatch next, PrologResolution resolution) {this . var_number = var_number; var = resolution . newVariable (); this . next = next;}
	public PrologMatch (int var_number, PrologElement term, boolean location, PrologMatch next, PrologResolution resolution) {
		this . var_number = var_number; var = resolution . newVariable (term, location); this . next = next;
	}
	public PrologMatch getVar (int i) {
		PrologMatch pointer = this;
		while (pointer != null) {if (pointer . var_number == i) return pointer; pointer = pointer . next;}
		return null;
	}
	public void connect_var (PrologVariable v1, PrologVariable v2) {PrologMatch mv = this; while (mv != null) {if (mv . var == v1) mv . var = v2; mv = mv . next;}}
}
