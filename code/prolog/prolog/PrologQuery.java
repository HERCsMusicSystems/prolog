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

public class PrologQuery {
	PrologQuery stack;
	PrologQuery context;
	PrologQuery fail_target;
	boolean original;
	PrologElement query;
	public PrologQuery (PrologQuery stack, PrologQuery context, PrologElement query) {
		this . stack = stack;
		this . context = context;
		this . fail_target = null;
		this . original = true;
		this . query = query;
	}
	public PrologQuery (PrologQuery stack, PrologQuery context, PrologQuery fail_target, boolean original, PrologElement query) {
		this . stack = stack;
		this . context = context;
		this . fail_target = fail_target;
		this . original = original;
		this . query = query;
	}
	public PrologQuery (PrologElement query) {
		this . stack = null;
		this . context = null;
		this . fail_target = null;
		this . original = true;
		this . query = query;
	}
	public void drop_stack_to_fail_target () {
		PrologQuery q1 = stack;
		while (q1 != null && q1 != fail_target) q1 = q1 . stack;
		stack = fail_target;
	}
};
