///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2013 Robert P. Wolf                       //
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

#ifndef _PROLOG_MIDI_SERVICE_CLASS_
#define _PROLOG_MIDI_SERVICE_CLASS_

#include "prolog.h"

class PrologMidiNativeCode;

class PrologMidiServiceClass : public PrologServiceClass {
private:
	PrologRoot * root;
	PrologDirectory * dir;
	PrologAtom * c, * cb, * cbb, * cx, * cxx;
	PrologAtom * d, * db, * dbb, * dx, * dxx;
	PrologAtom * e, * eb, * ebb, * ex, * exx;
	PrologAtom * f, * fb, * fbb, * fx, * fxx;
	PrologAtom * g, * gb, * gbb, * gx, * gxx;
	PrologAtom * a, * ab, * abb, * ax, * axx;
	PrologAtom * b, * bb, * bbb, * bx, * bxx;
public:
	PrologAtom * keyoff_atom, * keyon_atom, * polyaftertouch_atom, * control_atom, * programchange_atom, * aftertouch_atom, * pitch_atom;
	PrologAtom * sysex_atom, * SYSEX_atom, * timingclock_atom, * start_atom, * continue_atom, * stop_atom, * activesensing_atom;
	PrologMidiNativeCode * default_source;
	PrologMidiNativeCode * default_destination;
	int diatonic (PrologAtom * atom);
	int chromatic (PrologAtom * atom);
	void set_atoms (void);
	virtual void init (PrologRoot * root);
	virtual PrologNativeCode * getNativeCode (char * name);
	PrologMidiServiceClass (void);
	~ PrologMidiServiceClass (void);
};

#endif

