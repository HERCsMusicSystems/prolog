///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2004 Robert P. Wolf                       //
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

#include "prolog.h"

void PrologQuery :: drop_stack_to_fail_target (void) {
	PrologQuery * q1 = stack;
	PrologQuery * q2;
	while (q1 != NULL && q1 != fail_target) {
		q2 = q1 -> stack;
		delete q1;
		q1 = q2;
	}
	stack = fail_target;
}

PrologQuery :: PrologQuery (PrologQuery * stack, PrologQuery * context, PrologElement * query) TRACKING (10) {
	this -> stack = stack;
	this -> context = context;
	this -> fail_target = NULL;
	this -> original = true;
	this -> query = query;
}

PrologQuery :: PrologQuery (PrologQuery * stack, PrologQuery * context, PrologQuery * fail_target, bool original, PrologElement * query) TRACKING (10) {
	this -> stack = stack;
	this -> context = context;
	this -> fail_target = fail_target;
	this -> original = original;
	this -> query = query;
}

PrologQuery :: PrologQuery (PrologElement * query) TRACKING (10) {
	this -> stack = NULL;
	this -> context = NULL;
	this -> fail_target = NULL;
	this -> original = true;
	this -> query = query;
}

PrologQuery :: ~ PrologQuery (void) {
	delete query;
}
