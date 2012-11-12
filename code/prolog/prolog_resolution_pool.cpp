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

PrologResolutionPool :: PrologResolutionPool (PrologRoot * root) TRACKING (6) {
	head = NULL;
	main_thread = NULL;
	this -> root = root;
	signal_line = root -> getMidiOutput ();
	signals = 0;
}

PrologResolutionPool :: ~ PrologResolutionPool (void) {
	drop_resolutions ();
}

void PrologResolutionPool :: drop_resolutions (void) {
	if (head == NULL) return;
	PrologResolution * r1 = head;
	PrologResolution * r2;
	do {
		r2 = r1 -> next;
		drop_stack (r1 -> q_root, NULL);
		r1 -> removeQuery ();
		delete r1;
		r1 = r2;
	} while (r1 != head);
}

void PrologResolutionPool :: insert (PrologElement * query) {
	head = new PrologResolution (root, this, head);
	if (main_thread == NULL) main_thread = head;
	head -> query (query);
}

void PrologResolutionPool :: insertMain (PrologElement * query) {
	head = new PrologResolution (root, this, head);
	main_thread = head;
	head -> query (query);
}

void PrologResolutionPool :: shift (void) {head = head -> next;}

void PrologResolutionPool :: main_shift (void) {
	if (main_thread == NULL) return;
	while (head != main_thread) head = head -> next;
}

int PrologResolutionPool :: move (void) {
	// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query or not present,
	//          4 = continue
	PrologResolution * remover;
	PrologResolution * r;
	int ctrl = head -> move ();
	do {
		if (ctrl < 4) {
			if (head == main_thread) {
				r = head;
				while (r -> next != head) r = r -> next;
				remover = head;
				r -> next = head -> next;
				head = (head == r ? NULL : r);
				delete remover;
				main_thread = NULL;
				return ctrl;
			}
			r = head;
			while (r -> next != head) r = r -> next;
			r -> next = head -> next;
			head -> removeQuery ();
			delete head;
			head = r -> next;

		} else {
			head = head -> next;
		}
		if (head == main_thread) {
			while (signals-- > 0) {if (signal_line) signal_line -> insert (0xf8);}
			signals = root -> moveTransport ();
			return 4;
		}
		ctrl = head -> move ();
	} while (true);
}

