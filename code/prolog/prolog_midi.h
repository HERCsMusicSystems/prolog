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
#include "midi_stream.h"

class prolog_midi_reader;
class PrologMidiNativeCode;
class PrologMidiServiceClass;

class SourceMidiLine : public midi_stream {
public:
	prolog_midi_reader * reader;
	int midi_input_id;
	int command;
	int prefetch;
	bool running;
	virtual void internal_close_message (void);
	virtual bool message_waiting (void);
	virtual int internal_get (void);
	virtual int internal_get_command (void);
	SourceMidiLine (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologMidiServiceClass * servo, int midi_input_id);
	virtual ~ SourceMidiLine (void);
};

class PrologMidiNativeCode : public PrologNativeCode {
public:
	static char * name (void);
	virtual char * codeName (void);
	virtual bool isTypeOf (char * code_name);
	virtual bool connectThru (PrologMidiNativeCode * destination) = 0;
	virtual midi_stream * getLine (void) = 0;
};

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
	PrologAtom * midi_manufacturers_id_atom, * midi_product_id_atom, * midi_product_version_atom;
	PrologAtom * connect_thru_atom, * default_destination_atom;
	midi_stream * default_destination;
	int diatonic (PrologAtom * atom);
	int chromatic (PrologAtom * atom);
	PrologAtom * note (int diatonic, int chromatic);
	void set_atoms (void);
	virtual void init (PrologRoot * root, PrologDirectory * directory);
	virtual PrologNativeCode * getNativeCode (char * name);
	PrologMidiServiceClass (void);
	~ PrologMidiServiceClass (void);
};

#endif

