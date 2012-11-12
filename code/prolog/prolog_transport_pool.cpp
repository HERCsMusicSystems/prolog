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
//#include "windows.h"

PrologTransportPool :: PrologTransportPool (void) TRACKING (2) {
//	sampling_rate = 1000;
	seconds = 1;
	miliseconds = 0;
	active = false;
	transport = NULL;
	horizontal = 1000;
	tempo ();
}

PrologTransportPool :: PrologTransportPool (int horizontal) TRACKING (2) {
//	sampling_rate = 1000;
	seconds = 1;
	miliseconds = 0;
	active = false;
	transport = NULL;
	this -> horizontal = horizontal;
	tempo ();
}

PrologTransportPool :: PrologTransportPool (int horizontal, int seconds) TRACKING (2) {
//	sampling_rate = 1000;
	this -> seconds = seconds;
	miliseconds = 0;
	active = false;
	transport = NULL;
	this -> horizontal = horizontal;
	tempo ();
}
/*
PrologTransportPool :: PrologTransportPool (int sampling_rate, int horizontal) : object_tracker (2) {
	this -> sampling_rate = sampling_rate;
	seconds = 1;
	miliseconds = 0;
	active = false;
	transport = NULL;
	this -> horizontal = horizontal;
	tempo ();
}

PrologTransportPool :: PrologTransportPool (int sampling_rate, int horizontal, int seconds) : object_tracker (2) {
	this -> sampling_rate = sampling_rate;
	this -> seconds = seconds;
	miliseconds = 0;
	active = false;
	transport = NULL;
	this -> horizontal = horizontal;
	tempo ();
}
*/
PrologTransportPool :: ~ PrologTransportPool (void) {
	if (transport != NULL) delete transport;
}

PrologTransport * PrologTransportPool :: insert (void) {
	transport = new PrologTransport (transport);
	return transport;
}

bool PrologTransportPool :: drop (PrologTransport * t) {
	if (t == NULL) return false;
	if (transport == NULL) return false;
	PrologTransport * to_delete = transport;
	if (transport == t) {
		transport = transport -> next;
		to_delete -> next = NULL;
		delete to_delete;
		return true;
	}
	PrologTransport * sub = transport;
	while (sub -> next != NULL) {
		if (sub -> next == t) {
			to_delete = sub -> next;
			sub -> next = sub -> next -> next;
			to_delete -> next = NULL;
			delete to_delete;
			return true;
		}
		sub = sub -> next;
	}
	return false;
}

int PrologTransportPool :: move (void) {
	if (d >= 0) {
		d += d1;
		miliseconds += delta_miliseconds_1;
		if (transport != NULL) return transport -> move (delta_miliseconds_1);
	} else {
		d += d2;
		miliseconds += delta_miliseconds_2;
		if (transport != NULL) return transport -> move (delta_miliseconds_2);
	}
	return 0;
	// wait and signal code
//	Sleep (d >= 0 ? delta_miliseconds_1 : delta_miliseconds_2);
}

//void PrologTransportPool :: wait (PrologRoot * root) {
//	root -> wait (d >= 0 ? delta_miliseconds_1 : delta_miliseconds_2);
//}

int PrologTransportPool :: getCurrentDelay (void) {
	return (d >= 0 ? delta_miliseconds_1 : delta_miliseconds_2);
}

int PrologTransportPool :: getMiliseconds (void) {return miliseconds;}

void PrologTransportPool :: tempo (void) {
	int dx = horizontal * seconds;
//	int dy = sampling_rate * seconds;
	int dy = 1000 * seconds;
	delta_miliseconds_2 = dy / dx;
	delta_miliseconds_1 = delta_miliseconds_2 + 1;
	dy -= delta_miliseconds_2 * dx;
	d2 = dy + dy;
	d = d2 - dx;
	d1 = d - dx;
}

