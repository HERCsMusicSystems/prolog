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

#ifndef _PROLOG_STUDIO_
#define _PROLOG_STUDIO_

#include "prolog.h"

class HERCs_PROLOG_SDK_EXPORT prolog_midi_reader;

class standard_in_reader : public PrologReader {
protected:
	void message (char * text);
	void message_v (char * text, char * variable);
	int move_z (void);
public:
	standard_in_reader (PrologRoot * root);
};

class PrologNoise TRACK {
public:
	int a, c, m, v, range;
	int get (void);
	int get (int min, int max);
	void resolution (int bits);
	void control (int v);
	void control (int a, int c);
	PrologNoise (void);
};

class prolog_midi_reader : public midi_reader {
public:
	PrologDirectory * studio_dir;
	PrologRoot * root;
	PrologAtom * income_midi_atom;
	PrologAtom * keyoff_atom;
	PrologAtom * keyon_atom;
	PrologAtom * polyaftertouch_atom;
	PrologAtom * control_atom;
	PrologAtom * programchange_atom;
	PrologAtom * aftertouch_atom;
	PrologAtom * pitch_atom;
	PrologAtom * sysex_atom;
	PrologAtom * SYSEX_atom;
	PrologAtom * timingclock_atom;
	PrologAtom * start_atom;
	PrologAtom * continue_atom;
	PrologAtom * stop_atom;
	PrologAtom * activesensing_atom;
	void call (PrologElement * query);
	PrologElement * build_call (PrologAtom * atom, int channel, int ind);
	PrologElement * build_call (PrologAtom * atom, int channel, int ind, int sub);
	virtual void midi_keyoff (int channel, int key);
	virtual void midi_keyon (int channel, int key, int velocity);
	virtual void midi_pat (int channel, int key, int value);
	virtual void midi_control (int channel, int controller, int value);
	virtual void midi_programchange (int channel, int program);
	virtual void midi_cat (int channel, int value);
	virtual void midi_pitchbend (int channel, int v1, int v2);
	virtual void midi_system_exclusive (midi_stream * line);
	virtual void midi_timing_clock (void);
	virtual void midi_start (void);
	virtual void midi_continue (void);
	virtual void midi_stop (void);
	virtual void midi_active_sensing (void);
	virtual bool is_ready (void);
	prolog_midi_reader (PrologRoot * root);
};

class PrologStudio : public PrologServiceClass {
private:
	PrologRoot * root;
	standard_in_reader * stdr;
	PrologNoise * n;
	PrologTransportPool * transport;
	PrologTransport * t;
public:
	PrologDirectory * dir;
	PrologAtom * c, * cb, * cbb, * cx, * cxx;
	PrologAtom * d, * db, * dbb, * dx, * dxx;
	PrologAtom * e, * eb, * ebb, * ex, * exx;
	PrologAtom * f, * fb, * fbb, * fx, * fxx;
	PrologAtom * g, * gb, * gbb, * gx, * gxx;
	PrologAtom * a, * ab, * abb, * ax, * axx;
	PrologAtom * b, * bb, * bbb, * bx, * bxx;
	void set_atoms (void);
	int diatonic (PrologAtom * atom);
	int chromatic (PrologAtom * atom);
	PrologAtom * note (int diatonic, int chromatic);
public:
	virtual void init (PrologRoot * root);
	virtual PrologNativeCode * getNativeCode (char * name);
	virtual ~ PrologStudio (void);
};

#endif

