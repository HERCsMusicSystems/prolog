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

#include "prolog_midi.h"
#include <string.h>

class prolog_midi_reader : public midi_reader {
public:
	PrologRoot * root;
	PrologDirectory * midi_dir;
	PrologAtom * income_midi_atom;
	PrologMidiServiceClass * servo;
	void call (PrologElement * query);
	PrologElement * build_call (PrologAtom * atom, int channel, int ind);
	PrologElement * build_call (PrologAtom * atom, int channel, int ind, int sub);
	virtual void midi_keyoff (int channel, int key);
	virtual void midi_keyoffv (int channel, int key, int velocity);
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
	prolog_midi_reader (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologMidiServiceClass * servo);
	~ prolog_midi_reader (void);
};

void prolog_midi_reader :: call (PrologElement * query) {
	if (income_midi_atom == 0) {delete query; return;}
	query = root -> pair (root -> atom (income_midi_atom), query);
	query = root -> pair (query, root -> earth ());
	query = root -> pair (root -> head (NULL), query);
	root -> resolution (query);
	delete query;
}
PrologElement * prolog_midi_reader :: build_call (PrologAtom * atom, int channel, int ind) {
	return root -> pair (root -> atom (atom), root -> pair (root -> integer (channel + (midi_channel_extension != 0x7f ? midi_channel_extension << 4 : 0)), root -> pair (root -> integer (ind), root -> earth ())));
}
PrologElement * prolog_midi_reader :: build_call (PrologAtom * atom, int channel, int ind, int sub) {
	return root -> pair (root -> atom (atom), root -> pair (root -> integer (channel + (midi_channel_extension != 0x7f ? midi_channel_extension << 4 : 0)), root -> pair (root -> integer (ind), root -> pair (root -> integer (sub), root -> earth ()))));
}
void prolog_midi_reader :: midi_keyoff (int channel, int key) {call (build_call (servo -> keyoff_atom, channel, key));}
void prolog_midi_reader :: midi_keyoffv (int channel, int key, int velocity) {call (build_call (servo -> keyoff_atom, channel, key, velocity));}
void prolog_midi_reader :: midi_keyon (int channel, int key, int velocity) {
	call (build_call (servo -> keyon_atom, channel, key, velocity));
}
void prolog_midi_reader :: midi_pat (int channel, int key, int value) {call (build_call (servo -> polyaftertouch_atom, channel, key, value));}
void prolog_midi_reader :: midi_control (int channel, int controller, int value) {call (build_call (servo -> control_atom, channel, controller, value));}
void prolog_midi_reader :: midi_programchange (int channel, int program) {call (build_call (servo -> programchange_atom, channel, program));}
void prolog_midi_reader :: midi_cat (int channel, int value) {call (build_call (servo -> aftertouch_atom, channel, value));}
void prolog_midi_reader :: midi_pitchbend (int channel, int v1, int v2) {call (build_call (servo -> pitch_atom, channel, v1, v2));}
void prolog_midi_reader :: midi_system_exclusive (midi_stream * line) {
	// prolog command sysex filter
	line -> mark ();
	PrologAtom * atom = servo -> SYSEX_atom;
	int extension_position = 1;
	if (line -> check_system_exclusive ()) {atom = servo -> sysex_atom; extension_position = 0;}
	else line -> restore ();
	PrologElement * query = root -> pair (root -> atom (atom), root -> earth ());
	PrologElement * el = query -> getRight ();
	int ind = line -> get ();
	while (ind < 247) {
		if (extension_position >= 0) {
			if (extension_position == 0 && 0 < midi_channel_extension && midi_channel_extension < 127) ind += midi_channel_extension << 8;
			extension_position--;
		}
		el -> setPair (root -> integer (ind), root -> earth ());
		el = el -> getRight ();
		ind = line -> get ();
	}
	call (query);
}
void prolog_midi_reader :: midi_timing_clock (void) {call (root -> pair (root -> atom (servo -> timingclock_atom), root -> earth ()));}
void prolog_midi_reader :: midi_start (void) {call (root -> pair (root -> atom (servo -> start_atom), root -> earth ()));}
void prolog_midi_reader :: midi_continue (void) {call (root -> pair (root -> atom (servo -> continue_atom), root -> earth ()));}
void prolog_midi_reader :: midi_stop (void) {call (root -> pair (root -> atom (servo -> stop_atom), root -> earth ()));}
void prolog_midi_reader :: midi_active_sensing (void) {call (root -> pair (root -> atom (servo -> activesensing_atom), root -> earth ()));}
prolog_midi_reader :: prolog_midi_reader (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologMidiServiceClass * servo) {
	this -> root = root;
	midi_dir = directory;
	income_midi_atom = atom;
	if (income_midi_atom) {COLLECTOR_REFERENCE_INC (income_midi_atom);}
	this -> servo = servo;
}
prolog_midi_reader :: ~ prolog_midi_reader (void) {if (income_midi_atom) income_midi_atom -> removeAtom (); income_midi_atom = 0;}

static char * midi_internal_line_name = "MidiInternalLine";

class InternalMidiLine : public buffered_midi_stream {
public:
	prolog_midi_reader * reader;
	virtual void internal_ready (void) {if (reader != 0) reader -> read (this);}
	InternalMidiLine (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologMidiServiceClass * servo, int size)
		: buffered_midi_stream (size) {reader = atom == 0 ? 0 : new prolog_midi_reader (root, directory, atom, servo);}
	virtual ~ InternalMidiLine (void) {if (reader) delete reader; reader = 0;}
};

#include <fcntl.h>

#ifdef LINUX_OPERATING_SYSTEM
#include <unistd.h>
#endif

#ifdef WINDOWS_OPERATING_SYSTEM
int read (int ind, unsigned char * data, int size) {return -1;}
int open (char * name, unsigned long int switches) {return 0;}
int close (int id) {return 0;}
void usleep (unsigned long int delay) {Sleep (delay / 1000);}
#endif

static void * midi_runner (void * parameter) {
	SourceMidiLine * line = (SourceMidiLine *) parameter;
	line -> reader -> read (line);
	line -> running = false;
	printf ("RUNNER STOPPED.\n");
	return 0;
}

class PrologMidiSourceCode;

void SourceMidiLine :: internal_close_message (void) {
	midi_stream :: internal_close_message ();
	reader -> read (this);
}

bool SourceMidiLine :: message_waiting (void) {
	unsigned char v1;
	int res = read (midi_input_id, & v1, 1);
	prefetch = v1;
	return res >= 0;
}

int SourceMidiLine :: internal_get (void) {
	if (prefetch >= 0) {int ret = prefetch; prefetch = -1; return ret;}
	unsigned char v1;
	int res = read (midi_input_id, & v1, 1);
	return v1;
}

int SourceMidiLine :: internal_get_command (void) {
	if (prefetch < 0x80) return command;
	command = prefetch;
	prefetch = -1;
	return command;
}

SourceMidiLine :: SourceMidiLine (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologMidiServiceClass * servo, int midi_input_id) {
	reader = new prolog_midi_reader (root, directory, atom, servo);
	this -> midi_input_id = midi_input_id;
	command = 0;
	prefetch = -1;
	running = true;
	pthread_t thread;
	pthread_create (& thread, 0, midi_runner, this);
	pthread_detach (thread);
}

SourceMidiLine :: ~ SourceMidiLine (void) {if (reader) delete reader; reader = 0; printf ("SOURCE LINE DELETED.\n");}

char * PrologMidiNativeCode :: name (void) {return midi_internal_line_name;}
char * PrologMidiNativeCode :: codeName (void) {return name ();}
bool PrologMidiNativeCode :: isTypeOf (char * code_name) {return name () == code_name ? true : PrologNativeCode :: isTypeOf (code_name);}

class MidiLineType : public PrologNativeCode {public: char * codeName (void) {return PrologMidiNativeCode :: name ();}};

class PrologMidiLineNativeCode : public PrologMidiNativeCode {
public:
	PrologAtom * atom;
	midi_stream * line;
	PrologMidiServiceClass * servo;
	virtual midi_stream * getLine (void) {return line;}
	virtual bool connectThru (PrologMidiNativeCode * code) {line -> connect_thru (code != 0 ? code -> getLine () : 0); return true;}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (! el -> isAtom ()) return false;
		PrologNativeCode * machine = el -> getAtom () -> getMachine ();
		if (machine != 0 && machine -> isTypeOf (PrologMidiNativeCode :: name ())) return connectThru ((PrologMidiNativeCode *) machine);
		if (el -> getAtom () == servo -> midi_manufacturers_id_atom) {
			if (parameters -> isEarth ()) {line -> set_manufacturers_id (); return true;}
			if (! parameters -> isPair ()) return false;
			PrologElement * ind = parameters -> getLeft (); if (! ind -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (parameters -> isEarth ()) {line -> set_manufacturers_id (ind -> getInteger ()); return true;}
			if (! parameters -> isPair ()) return false;
			PrologElement * sub = parameters -> getLeft (); if (! sub -> isInteger ()) return false;
			line -> set_manufacturers_id (ind -> getInteger (), sub -> getInteger ());
			return true;
		}
		if (el -> getAtom () == servo -> midi_product_id_atom || el -> getAtom () == servo -> midi_product_version_atom) {
			char id [4] = {-1, -1, -1, -1}; int ind = 0;
			while (parameters -> isPair () && ind < 4) {
				PrologElement * e = parameters -> getLeft ();
				if (e -> isInteger ()) id [ind++] = (char) e -> getInteger ();
				if (e -> isText ()) {
					char * text = e -> getText ();
					while (* text != '\0' && ind < 4) id [ind++] = * text++;
				}
				parameters = parameters -> getRight ();
			}
			if (el -> getAtom () == servo -> midi_product_id_atom) line -> set_product_id (id [0], id [1], id [2], id [3]);
			if (el -> getAtom () == servo -> midi_product_version_atom) line -> set_product_version (id [0], id [1], id [2], id [3]);
			return true;
		}
		if (el -> getAtom () == servo -> connect_thru_atom) {
			if (parameters -> isEarth ()) {connectThru (0); return true;}
			if (! parameters -> isPair ()) return false;
			parameters = parameters -> getLeft ();
			if (! parameters -> isAtom ()) return false;
			machine = parameters -> getAtom () -> getMachine ();
			if (machine == 0 || (! machine -> isTypeOf (PrologMidiNativeCode :: name ()))) return false;
			return connectThru ((PrologMidiNativeCode *) machine);
		}
		if (el -> getAtom () == servo -> default_destination_atom) {
			if (! parameters -> isEarth ()) return false;
			servo -> default_destination = getLine ();
			return true;
		}
		return false;
	}
	PrologMidiLineNativeCode (PrologAtom * atom, PrologRoot * root, PrologDirectory * directory, PrologAtom * income_midi, PrologMidiServiceClass * servo, int size) {
		this -> atom = atom;
		this -> servo = servo;
		line = new InternalMidiLine (root, directory, income_midi, servo, size);
	}
	~ PrologMidiLineNativeCode (void) {if (line) delete line; line = 0;}
};

class PrologMidiSourceCode : public PrologNativeCode {
public:
	PrologAtom * atom;
	SourceMidiLine * line;
	int midi_input_id;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> setMachine (0);
			close (midi_input_id);
			while (line -> running) usleep (10000);
			delete this;
			return true;
		}
		return false;
	}
	PrologMidiSourceCode (PrologRoot * root, PrologDirectory * directory, PrologMidiServiceClass * servo, PrologAtom * atom, PrologAtom * income, int midi_input_id) {
		this -> atom = atom;
		this -> midi_input_id = midi_input_id;
		line = new SourceMidiLine (root, directory, income, servo, midi_input_id);
	}
	~ PrologMidiSourceCode (void) {if (line) delete line; line = 0; printf ("SOURCE CODE DELETED.\n");}
};

class ConnectThru : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * source = parameters -> getLeft (); if (! source -> isAtom ()) return false; parameters = parameters -> getRight ();
		PrologNativeCode * machine = source -> getAtom () -> getMachine ();
		if (machine == 0 || (! machine -> isTypeOf (PrologMidiNativeCode :: name ()))) return false;
		PrologMidiNativeCode * source_code = (PrologMidiNativeCode *)  machine;
		if (parameters -> isEarth ()) {source_code -> connectThru (0); return false;}
		if (! parameters -> isPair ()) return false;
		PrologElement * destination = parameters -> getLeft (); if (! destination -> isAtom ()) return false;
		machine = destination -> getAtom () -> getMachine ();
		if (machine == 0 || (! machine -> isTypeOf (PrologMidiNativeCode :: name ()))) return false;
		PrologMidiNativeCode * destination_code = (PrologMidiNativeCode *) machine;
		return source_code -> connectThru (destination_code);
	}
};

class DefaultDestination : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (servo == 0) return false;
		if (parameters -> isEarth ()) {servo -> default_destination = 0; return true;}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isAtom ()) return false;
		PrologNativeCode * code = parameters -> getAtom () -> getMachine ();
		if (code == 0 || (! code -> isTypeOf (PrologMidiNativeCode :: name ()))) return false;
		servo -> default_destination = ((PrologMidiNativeCode *) code) -> getLine ();
		return true;
	}
	DefaultDestination (PrologMidiServiceClass * servo) {this -> servo = servo;}
};

#define FIND_MIDI_DESTINATION \
		midi_stream * destination = servo -> default_destination;\
		if (parameters -> isPair ()) {\
			PrologElement * el = parameters -> getLeft ();\
			if (el -> isAtom ()) {\
				PrologNativeCode * machine = el -> getAtom () -> getMachine ();\
				if (machine == 0) return false;\
				if (! machine -> isTypeOf (PrologMidiNativeCode :: name ())) return false;\
				destination = ((PrologMidiNativeCode *) machine) -> getLine ();\
				parameters = parameters -> getRight ();\
			}\
		}\
		if (destination == 0) return false

class sysex : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool generic_sysex;
	bool checksum;
	int correction (int v) {return ((v >> 8) << 4) | (v & 0xf);}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		PrologElement * v1 = 0;
		PrologElement * v2 = 0;
		if (parameters -> isPair ()) {v1 = parameters -> getLeft (); parameters = parameters -> getRight ();}
		if (parameters -> isPair ()) {v2 = parameters -> getLeft (); parameters = parameters -> getRight ();}
		destination -> lock ();
		if (generic_sysex) {
			if (v2 != 0 && v2 -> isInteger ()) destination -> chex (correction (v2 -> getInteger ()));
			destination -> open_generic_system_exclusive ();
		} else {
			if (v1 != 0 && v1 -> isInteger ()) destination -> chex (correction (v1 -> getInteger ()));
			destination -> open_system_exclusive ();
		}
		if (v1 != 0) {if (v1 -> isInteger ()) destination -> insert (v1 -> getInteger () & 0x7f); if (v1 -> isText ()) destination -> insert (v1 -> getText ());}
		if (v2 != 0) {if (v2 -> isInteger ()) destination -> insert (v2 -> getInteger () & 0x7f); if (v2 -> isText ()) destination -> insert (v2 -> getText ());}
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) destination -> insert (el -> getInteger ());
			if (el -> isText ()) destination -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		if (checksum) destination -> insert_checksum ();
		destination -> close_system_exclusive ();
		destination -> unlock ();
		destination -> ready ();
		return true;
	}
	sysex (PrologMidiServiceClass * servo, bool generic_sysex, bool checksum) {this -> servo = servo; this -> generic_sysex = generic_sysex; this -> checksum = checksum;}
};

class chex : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	int chexer (midi_stream * line, int channel, int offset) {line -> lock (); int chx = offset + line -> chex (channel); line -> unlock (); return chx;}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		if (parameters -> isVar ()) {parameters -> setInteger (destination -> get_channel_extension ()); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * channel = parameters -> getLeft ();
		if (channel -> isVar ()) {channel -> setInteger (destination -> get_channel_extension ()); return true;}
		if (! channel -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isVar ()) {parameters -> setInteger (chexer (destination, channel -> getInteger (), 0)); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * offset = parameters -> getLeft ();
		if (offset -> isVar ()) {offset -> setInteger (chexer (destination, channel -> getInteger (), 0)); return true;}
		if (! offset -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setInteger (chexer (destination, channel -> getInteger (), offset -> getInteger ()));
		return true;
	}
	chex (PrologMidiServiceClass * servo) {this -> servo = servo;}
};

class chexer : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * v = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (v -> isInteger ()) {
			int iv = v -> getInteger ();
			parameters -> setPair (); parameters -> getLeft () -> setInteger (iv >> 8); parameters = parameters -> getRight ();
			parameters -> setPair (); parameters -> getLeft () -> setInteger (iv & 0xf0); parameters = parameters -> getRight ();
			parameters -> setPair (); parameters -> getLeft () -> setInteger (iv & 0xf);
			return true;
		}
		if (! v -> isVar ()) return false;
		if (! parameters -> isPair ()) return false;
		int iv = 0;
		PrologElement * el = parameters -> getLeft (); if (! el -> isInteger ()) return false; iv = el -> getInteger () << 8; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft (); if (! el -> isInteger ()) return false; iv += el -> getInteger () & 0xf0; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft (); if (! el -> isInteger ()) return false; iv += el -> getInteger () & 0xf;
		v -> setInteger (iv);
		return true;
	}
};

class keyoff_command : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		if (! parameters -> isPair ()) return false;
		PrologElement * channel = parameters -> getLeft (); if (! channel -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {destination -> insert_control (channel -> getInteger (), 123, 0); destination -> ready (); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * keyel = parameters -> getLeft (); parameters = parameters -> getRight ();
		int key = 0;
		if (keyel -> isInteger ()) key = keyel -> getInteger ();
		else if (keyel -> isPair ()) {
			PrologElement * note = keyel -> getLeft (); keyel = keyel -> getRight (); if (! keyel -> isPair ()) return false;
			PrologElement * octave = keyel -> getLeft (); if (! note -> isAtom ()) return false; if (! octave -> isInteger ()) return false;
			key = 48 + octave -> getInteger () * 12 + servo -> chromatic (note -> getAtom ());
		} else return false;
		if (parameters -> isEarth ()) {destination -> insert_keyoff (channel -> getInteger (), key); destination -> ready (); return true;}
		if (! parameters -> isPair ()) return false;
		PrologElement * velocity = parameters -> getLeft (); if (! velocity -> isInteger ()) return false;
		destination -> insert_keyoff (channel -> getInteger (), key, velocity -> getInteger ());
		destination -> ready ();
		return true;
	}
	keyoff_command (PrologMidiServiceClass * servo) {this -> servo = servo;}
};

class pitch_command : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool extended;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		if (! parameters -> isPair ()) return false;
		PrologElement * channel = parameters -> getLeft (); if (! channel -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * msb = parameters -> getLeft (); if (! msb -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			if (extended) {
				int mmsb = msb -> getInteger () & 0x3fff;
				int llsb = mmsb & 0x7f;
				mmsb >>= 7;
				destination -> insert_pitchbend (channel -> getInteger (), llsb, mmsb);
			} else destination -> insert_pitchbend (channel -> getInteger (), msb -> getInteger ());
			destination -> ready ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * lsb = parameters -> getLeft (); if (! lsb -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {destination -> insert_pitchbend (channel -> getInteger (), msb -> getInteger (), lsb -> getInteger ()); destination -> ready (); return true;}
		return false;
	}
	pitch_command (PrologMidiServiceClass * servo, bool extended) {this -> servo = servo; this -> extended = extended;}
};

class programchange_command : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		if (! parameters -> isPair ()) return false;
		PrologElement * channel = parameters -> getLeft (); if (! channel -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * msb = parameters -> getLeft (); parameters = parameters -> getRight ();
		//[[programchange *channel *msb *lsb *program] [add *channel 64 *selector] [sysex *selector 84 *lsb *program *msb]]
		if (msb -> isText ()) {
			if (! parameters -> isPair ()) return false;
			PrologElement * lsb = parameters -> getLeft (); if (! lsb -> isInteger ()) return false; parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			PrologElement * program = parameters -> getLeft (); if (! program -> isInteger ()) return false;
			destination -> lock ();
			int selector = destination -> chex (channel -> getInteger ());
			destination -> open_system_exclusive ();
			destination -> insert (64 + selector);
			destination -> insert (67);
			destination -> insert (lsb -> getInteger ());
			destination -> insert (program -> getInteger ());
			destination -> insert (msb -> getText ());
			destination -> close_system_exclusive ();
			destination -> unlock ();
			destination -> ready ();
			return true;
		}
		if (! msb -> isInteger ()) return false;
		if (! parameters -> isEarth ()) return false;
		destination -> insert_programchange (channel -> getInteger (), msb -> getInteger ());
		destination -> ready ();
		return true;
	}
	programchange_command (PrologMidiServiceClass * servo) {this -> servo = servo;}
};

//[[egcopy_index *channel *from *to] [sum *channel 64 *ch] [sysex *ch 66 *from *to]]
//[[egcopy_freq *channel *from *to] [sum *channel 64 *ch] [sum *from 16 *f] [sum *to 16 *t] [sysex *ch 66 *f *t]]
//[[egcopy_amp *channel *from *to] [sum *channel 64 *ch] [sum *from 32 *f] [sum *to 32 *t] [sysex *ch 66 *f *t]]
//[[egcopy_pan *channel *from *to] [sum *channel 64 *ch] [sum *from 48 *f] [sum *to 48 *t] [sysex *ch 66 *f *t]]
//[[egcopy : *command] [egcopy_amp : *command]]

class egcopy : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	int selector;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		if (! parameters -> isPair ()) return false; PrologElement * channel = parameters -> getLeft (); if (! channel -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false; PrologElement * from = parameters -> getLeft (); if (! from -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false; PrologElement * to = parameters -> getLeft (); if (! to -> isInteger ()) return false; parameters = parameters -> getRight ();
		destination -> lock ();
		int chx = destination -> chex (channel -> getInteger ());
		destination -> open_system_exclusive ();
		destination -> insert (64 + chx);
		destination -> insert (72);
		destination -> insert (from -> getInteger () + selector);
		destination -> insert (to -> getInteger () + selector);
		destination -> close_system_exclusive ();
		destination -> unlock ();
		destination -> ready ();
		return true;
	}
	egcopy (PrologMidiServiceClass * servo, int selector) {this -> servo = servo; this -> selector = selector;}
};

class bank_command : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		if (! parameters -> isPair ()) return false;
		PrologElement * channel = parameters -> getLeft (); if (! channel -> isInteger ()) return false; parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * msb = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			//[[bank *channel *value] [control *channel 0 *value]]
			if (msb -> isInteger ()) {destination -> insert_control (channel -> getInteger (), 0, msb -> getInteger ()); destination -> ready (); return true;}
			//[[bank *channel *msb] [add *channel 64 *selector] [sysex *selector 82 *msb]]
			if (msb -> isText ()) {
				destination -> lock ();
				int ch = destination -> chex (channel -> getInteger ());
				destination -> open_system_exclusive ();
				destination -> insert (64 + ch); destination -> insert (65); destination -> insert (msb -> getText ());
				destination -> close_system_exclusive ();
				destination -> unlock ();
				destination -> ready ();
				return true;
			}
			return false;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * lsb = parameters -> getLeft (); if (! lsb -> isInteger ()) return false;
		//[[bank *channel *msb *lsb] [control *channel 0 *msb] [control *channel 32 *lsb]]
		if (msb -> isInteger ()) {destination -> insert_control (channel -> getInteger (), 0, msb -> getInteger (), lsb -> getInteger ()); destination -> ready (); return true;}
		//[[bank *channel *msb *lsb] [add *channel 64 *selector] [sysex *selector 83 *lsb *msb]]
		if (msb -> isText ()) {
			destination -> lock ();
			int ch = destination -> chex (channel -> getInteger ());
			destination -> open_system_exclusive ();
			destination -> insert (64 + ch); destination -> insert (66); destination -> insert (lsb -> getInteger ()); destination -> insert (msb -> getText ());
			destination -> close_system_exclusive ();
			destination -> unlock ();
			destination -> ready ();
			return true;
		}
		return false;
	}
	bank_command (PrologMidiServiceClass * servo) {this -> servo = servo;}
};

class nrpn_rpn_command : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool rpn;
	bool extended;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		int numbers [5];
		int ind = 0;
		while (parameters -> isPair ()) {
			if (ind >= 5) return false;
			PrologElement * el = parameters -> getLeft ();
			if (! el -> isInteger ()) return false;
			numbers [ind++] = el -> getInteger ();
			parameters = parameters -> getRight ();
		}
		if (rpn) {
			switch (ind) {
			case 1: destination -> insert_rpn (numbers [0]); break;
			case 2: destination -> insert_rpn (numbers [0], numbers [1]); break;
			case 3: destination -> insert_rpn (numbers [0], numbers [1], numbers [2]); break;
			case 4: destination -> insert_rpn (numbers [0], numbers [1], numbers [2], numbers [3]); break;
			default: return false; break;
			}
			destination -> ready ();
			return true;
		}
		switch (ind) {
		case 1: destination -> insert_nrpn (numbers [0]); break;
		case 2:
			if (extended) destination -> insert_nrpn_14 (numbers [0], numbers [1]);
			else destination -> insert_nrpn (numbers [0], numbers [1]);
			break;
		case 3: destination -> insert_nrpn (numbers [0], numbers [1], numbers [2]); break;
		case 4:
			if (extended) destination -> insert_nrpn_14 (numbers [0], numbers [1], numbers [2], numbers [3]);
			else destination -> insert_nrpn (numbers [0], numbers [1], numbers [2], numbers [3]);
			break;
		case 5: destination -> insert_nrpn (numbers [0], numbers [1], numbers [2], numbers [3], numbers [4]); break;
		default: return false; break;
		}
		destination -> ready ();
		return true;
	}
	nrpn_rpn_command (PrologMidiServiceClass * servo, bool rpn = false, bool extended = false) {this -> servo = servo; this -> rpn = rpn; this -> extended = extended;}
};

class midi_message_command : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		FIND_MIDI_DESTINATION;
		destination -> lock ();
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) destination -> insert (el -> getInteger ());
			if (el -> isText ()) destination -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		destination -> close_message ();
		destination -> unlock ();
		destination -> ready ();
		return true;
	}
	midi_message_command (PrologMidiServiceClass * servo) {this -> servo = servo;}
};



bool short_message_processor (int required, int command, PrologElement * parameters, PrologMidiServiceClass * servo, int ctrl = -1, bool extended = false) {
	FIND_MIDI_DESTINATION;
	if (parameters -> isEarth ()) {
		if (required > 0) return false;
		destination -> insert_channel_command (command); destination -> ready ();
		return true;
	}
	if (! parameters -> isPair ()) return false;
	PrologElement * el = parameters -> getLeft ();
	if (! el -> isInteger ()) return false;
	int channel = el -> getInteger ();
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {
		if (required != 1) return false;
		destination -> insert_control (channel, command, ctrl);
		destination -> ready ();
		return true;
	}
	if (! parameters -> isPair ()) return false;
	el = parameters -> getLeft ();
	int msb = 0;
	if (el -> isInteger ()) msb = el -> getInteger ();
	else if (el -> isPair ()) {
		PrologElement * note = el -> getLeft ();
		el = el -> getRight ();
		if (! el -> isPair ()) return false;
		PrologElement * octave = el -> getLeft ();
		if (! note -> isAtom ()) return false;
		if (! octave -> isInteger ()) return false;
		msb = 48 + octave -> getInteger () * 12 + servo -> chromatic (note -> getAtom ());
	} else return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {
		if (required != 2) return false;
		if (extended) {
			if (ctrl < 0) return false;
			msb &= 0x3fff;
			int lsb = msb & 0x7f;
			msb >>= 7;
			destination -> insert_control (channel, ctrl, msb, lsb);
		} else {
			if (ctrl >= 0) destination -> insert_control (channel, ctrl, msb);
			else destination -> insert_cat (channel, msb);
		}
		destination -> ready ();
		return true;
	}
	if (! parameters -> isPair ()) return false;
	el = parameters -> getLeft ();
	if (! el -> isInteger ()) return false;
	int lsb = el -> getInteger ();
	if (required != 3 || ! parameters -> getRight () -> isEarth ()) return false;
	if (extended) {
		int mlsb = lsb & 0x3fff;
		int llsb = mlsb & 0x7f;
		mlsb >>= 7;
		destination -> insert_control (channel, msb, mlsb, llsb);
	} else {
		switch (command) {
		case 144: destination -> insert_keyon (channel, msb, lsb); break;
		case 160: destination -> insert_pat (channel, msb, lsb); break;
		case 176: destination -> insert_control (channel, msb, lsb); break;
		default: break;
		}
	}
	destination -> ready ();
	return true;
}

class CreateLine : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologDirectory * directory;
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * line_atom = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (line_atom -> isVar ()) line_atom -> setAtom (new PrologAtom ());
		if (! line_atom -> isAtom ()) return false;
		PrologAtom * income_midi = 0;
		int size = 2048;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) income_midi = el -> getAtom ();
			if (el -> isInteger ()) size = el -> getInteger ();
			parameters = parameters -> getRight ();
		}
		if (line_atom -> getAtom () -> getMachine () != 0) return false;
		PrologMidiLineNativeCode * line = new PrologMidiLineNativeCode (line_atom -> getAtom (), root, directory, income_midi, servo, size);
		if (line_atom -> getAtom () -> setMachine (line)) {if (servo -> default_destination == 0) servo -> default_destination = line -> getLine (); return true;}
		delete line;
		return false;
	}
	CreateLine (PrologRoot * root, PrologDirectory * directory, PrologMidiServiceClass * servo) {
		this -> root = root;
		this -> directory = directory;
		this -> servo = servo;
	}
};

class CreateSource : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologDirectory * directory;
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (root == 0 || servo == 0) return false;
		PrologAtom * line = 0;
		PrologAtom * income = 0;
		char * file_name = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isVar ()) el -> setAtom (new PrologAtom ());
			if (el -> isAtom ()) {
				if (line != 0) income = el -> getAtom ();
				else line = el -> getAtom ();
			}
			if (el -> isText ()) file_name = el -> getText ();
			parameters = parameters -> getRight ();
		}
		if (file_name == 0 || line == 0) return false;
		int midi_input_id = open (file_name, O_RDONLY);
		if (midi_input_id < 0) return false;
		if (line -> getMachine () != 0) return false;
		PrologMidiSourceCode * source = new PrologMidiSourceCode (root, directory, servo, line, income, midi_input_id);
		if (line -> setMachine (source)) return true;
		delete source;
		return false;
	}
	CreateSource (PrologRoot * root, PrologDirectory * directory, PrologMidiServiceClass * servo) {
		this -> root = root;
		this -> directory = directory;
		this -> servo = servo;
	}
};

class CreateDestination : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		return false;
	}
};

class MidiShortCommand : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	int required;
	int command;
	int ctrl;
	bool extended;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return short_message_processor (required, command, parameters, servo, ctrl, extended);}
	MidiShortCommand (PrologMidiServiceClass * servo, int required, int command, int ctrl = -1, bool extended = false) {
		this -> servo = servo;
		this -> required = required;
		this -> command = command;
		this -> ctrl = ctrl;
		this -> extended = extended;
	}
};

class interval_processor : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologMidiServiceClass * servo;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * left_note = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * right_note = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * interval = parameters -> getLeft ();
		if (left_note -> isPair ()) {
			PrologElement * note_left = left_note -> getLeft ();
			if (! note_left -> isAtom ()) return false;
			int diatonic_left = servo -> diatonic (note_left -> getAtom ());
			if (diatonic_left < 0) return false;
			int chromatic_left = servo -> chromatic (note_left -> getAtom ());
			left_note = left_note -> getRight ();
			if (! left_note -> isPair ()) return false;
			PrologElement * octave_left = left_note -> getLeft ();
			if (! octave_left -> isInteger ()) return false;
			if (right_note -> isPair ()) {
				diatonic_left += 28 + octave_left -> getInteger () * 7;
				chromatic_left += 48 + octave_left -> getInteger () * 12;
				PrologElement * note_right = right_note -> getLeft ();
				if (! note_right -> isAtom ()) return false;
				int diatonic_right = servo -> diatonic (note_right -> getAtom ());
				if (diatonic_right < 0) return false;
				int chromatic_right = servo -> chromatic (note_right -> getAtom ());
				right_note = right_note -> getRight ();
				if (! right_note -> isPair ()) return false;
				PrologElement * octave_right = right_note -> getLeft ();
				if (! octave_right -> isInteger ()) return false;
				diatonic_right += 28 + octave_right -> getInteger () * 7;
				chromatic_right += 48 + octave_right -> getInteger () * 12;
				interval -> setPair (root -> integer (diatonic_right - diatonic_left), root -> pair (root -> integer (chromatic_right - chromatic_left), root -> earth ()));
				return true;
			}
			if (interval -> isPair ()) {
				PrologElement * diatonic_interval = interval -> getLeft ();
				if (! diatonic_interval -> isInteger ()) return false;
				interval = interval -> getRight ();
				if (! interval -> isPair ()) return false;
				PrologElement * chromatic_interval = interval -> getLeft ();
				if (! chromatic_interval -> isInteger ()) return false;
				int diatonic_right = diatonic_left + diatonic_interval -> getInteger ();
				int chromatic_right = chromatic_left + chromatic_interval -> getInteger ();
				int octave_right = octave_left -> getInteger ();
				while (diatonic_right < 0) {diatonic_right += 7; chromatic_right += 12; octave_right--;}
				while (diatonic_right > 6) {diatonic_right -= 7; chromatic_right -= 12; octave_right++;}
				PrologAtom * ret = servo -> note (diatonic_right, chromatic_right);
				if (ret == NULL) return false;
				right_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_right), root -> earth ()));
				return true;
			}
			return false;
		}
		if (! right_note -> isPair ()) return false;
		PrologElement * note_right = right_note -> getLeft ();
		if (! note_right -> isAtom ()) return false;
		int diatonic_right = servo -> diatonic (note_right -> getAtom ());
		if (diatonic_right < 0) return false;
		int chromatic_right = servo -> chromatic (note_right -> getAtom ());
		right_note = right_note -> getRight ();
		if (! right_note -> isPair ()) return false;
		PrologElement * octave_right = right_note -> getLeft ();
		if (! octave_right -> isInteger ()) return false;
		if (! interval -> isPair ()) return false;
		PrologElement * diatonic_interval = interval -> getLeft ();
		if (! diatonic_interval -> isInteger ()) return false;
		interval = interval -> getRight ();
		if (! interval -> isPair ()) return false;
		PrologElement * chromatic_interval = interval -> getLeft ();
		if (! chromatic_interval -> isInteger ()) return false;
		int diatonic_left = diatonic_right - diatonic_interval -> getInteger ();
		int chromatic_left = chromatic_right - chromatic_interval -> getInteger ();
		int octave_left = octave_right -> getInteger ();
		while (diatonic_left < 0) {diatonic_left += 7; chromatic_left += 12; octave_left--;}
		while (diatonic_left > 6) {diatonic_left -= 7; chromatic_left -= 12; octave_left++;}
		PrologAtom * ret = servo -> note (diatonic_left, chromatic_left);
		if (ret == NULL) return false;
		left_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_left), root -> earth ()));
		return true;
	}
	interval_processor (PrologRoot * root, PrologMidiServiceClass * servo) {this -> root = root; this -> servo = servo;}
};

PrologAtom * PrologMidiServiceClass :: note (int diatonic, int chromatic) {
	switch (diatonic) {
	case 0:
		switch (chromatic) {
		case -2: return cbb;
		case -1: return cb;
		case 0: return c;
		case 1: return cx;
		case 2: return cxx;
		default: return NULL;
		} return NULL;
	case 1:
		switch (chromatic) {
		case 0: return dbb;
		case 1: return db;
		case 2: return d;
		case 3: return dx;
		case 4: return dxx;
		default: return NULL;
		} return NULL;
	case 2:
		switch (chromatic) {
		case 2: return ebb;
		case 3: return eb;
		case 4: return e;
		case 5: return ex;
		case 6: return exx;
		default: return NULL;
		} return NULL;
	case 3:
		switch (chromatic) {
		case 3: return fbb;
		case 4: return fb;
		case 5: return f;
		case 6: return fx;
		case 7: return fxx;
		default: return NULL;
		} return NULL;
	case 4:
		switch (chromatic) {
		case 5: return gbb;
		case 6: return gb;
		case 7: return g;
		case 8: return gx;
		case 9: return gxx;
		default: return NULL;
		} return NULL;
	case 5:
		switch (chromatic) {
		case 7: return abb;
		case 8: return ab;
		case 9: return a;
		case 10: return ax;
		case 11: return axx;
		default: return NULL;
		} return NULL;
	case 6:
		switch (chromatic) {
		case 9: return bbb;
		case 10: return bb;
		case 11: return b;
		case 12: return bx;
		case 13: return bxx;
		default: return NULL;
		} return NULL;
	default: return NULL;
	}
	return NULL;
}

class DCMOD : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e_diatonic = parameters -> getLeft ();
		if (! e_diatonic -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_chromatic = parameters -> getLeft ();
		if (! e_chromatic -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_octave = parameters -> getLeft ();
		if (! e_octave -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_diatonic_shift = parameters -> getLeft ();
		if (! e_diatonic_shift -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e_chromatic_shift = parameters -> getLeft ();
		if (! e_chromatic_shift -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		int diatonic = e_diatonic -> getInteger () + e_diatonic_shift -> getInteger ();
		int chromatic = e_chromatic -> getInteger () + e_chromatic_shift -> getInteger ();
		int octave = e_octave -> getInteger ();
		while (chromatic < 0) {chromatic += 12; diatonic += 7; octave--;}
		while (chromatic > 11) {chromatic -= 12; diatonic -= 7; octave++;}
		if (! parameters -> isPair ()) return false;
		parameters -> getLeft () -> setInteger (diatonic);
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters -> getLeft () -> setInteger (chromatic);
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters -> getLeft () -> setInteger (octave);
		return true;
	}
};


/*

////////////////
// MIDI Ports //
////////////////

class midi_in_info : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		int inputs = service -> getNumberOfInputs ();
		int active = service -> getInputPort ();
		PROLOG_STRING command;
		bool exporting;
		if (parameters -> isEarth ()) {
			sprintf (command, "MIDI Inputs [%i]\n", inputs);
			root -> print (command);
			exporting = false;
		} else exporting = true;
		parameters -> setEarth ();
		for (int ind = 0; ind < inputs; ind++) {
			if (exporting) {
				sprintf (command, "%s%s", service -> getInputInfo (ind), active == ind ? " [ACTIVE]" : "");
				parameters -> setPair ();
				parameters -> getLeft () -> setText (command);
				parameters = parameters -> getRight ();
			} else {
				sprintf (command, "	%i %s=> %s\n", ind, active == ind ? "[ACTIVE] " : "", service -> getInputInfo (ind));
				root -> print (command);
			}
		}
		return true;
	}
	midi_in_info (PrologRoot * root) {this -> root = root;}
};

class midi_out_info : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		int outputs = service -> getNumberOfOutputs ();
		int active = service -> getOutputPort ();
		PROLOG_STRING command;
		bool exporting;
		if (parameters -> isEarth ()) {
			sprintf (command, "MIDI Outputs [%i]\n", outputs);
			root -> print (command);
			exporting = false;
		} else exporting = true;
		parameters -> setEarth ();
		for (int ind = 0; ind < outputs; ind++) {
			if (exporting) {
				sprintf (command, "%s%s", service -> getOutputInfo (ind), active == ind ? " [ACTIVE]" : "");
				parameters -> setPair ();
				parameters -> getLeft () -> setText (command);
				parameters = parameters -> getRight ();
			} else {
				sprintf (command, "	%i %s=> %s\n", ind, active == ind ? "[ACTIVE] " : "", service -> getOutputInfo (ind));
				root -> print (command);
			}
		}
		return true;
	}
	midi_out_info (PrologRoot * root) {this -> root = root;}
};

class midi_in_port : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		if (parameters -> isText ()) return service -> setInputPort (parameters -> getText ());
		if (! parameters -> isInteger ()) return false;
		return service -> setInputPort (parameters -> getInteger ());
	}
	midi_in_port (PrologRoot * root) {this -> root = root;}
};

class midi_out_port : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (service == NULL) return true;
		if (parameters -> isText ()) return service -> setOutputPort (parameters -> getText ());
		if (! parameters -> isInteger ()) return false;
		return service -> setOutputPort (parameters -> getInteger ());
	}
	midi_out_port (PrologRoot * root) {this -> root = root;}
};

//////////
// MIDI //
//////////

class void_midi_message : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		root -> print ("MIDI interfaces not connected.");
		root -> print (root -> new_line_caption);
		return true;
	}
	void_midi_message (PrologRoot * root) {this -> root = root;}
};

static pthread_mutex_t midi_mutex;
static void midi_wait (void) {pthread_mutex_lock (& midi_mutex);}
static void midi_signal (void) {pthread_mutex_unlock (& midi_mutex);}

class PrologStudio : public PrologServiceClass {
private:
	PrologRoot * root;
	standard_in_reader stdr;
	PrologNoise n;
public:
	PrologDirectory * dir;
///
	void set_atoms (void);
	int diatonic (PrologAtom * atom);
	int chromatic (PrologAtom * atom);
	PrologAtom * note (int diatonic, int chromatic);
public:
	virtual void init (PrologRoot * root);
	virtual PrologNativeCode * getNativeCode (char * name);
	virtual ~ PrologStudio (void);
};

void PrologStudio :: init (PrologRoot * root) {
	this -> root = root;
	stdr . setRoot (root);
	c = cb = cbb = cx = cxx = NULL;
	d = db = dbb = dx = dxx = NULL;
	e = eb = ebb = ex = exx = NULL;
	f = fb = fbb = fx = fxx = NULL;
	g = gb = gbb = gx = gxx = NULL;
	a = ab = abb = ax = axx = NULL;
	b = bb = bbb = bx = bxx = NULL;
	midi_mutex = PTHREAD_MUTEX_INITIALIZER;
}

PrologStudio :: ~ PrologStudio (void) {
	pthread_mutex_destroy (& midi_mutex);
}

PrologNativeCode * PrologStudio :: getNativeCode (char * name) {
	set_atoms ();

	
	
	if (strcmp (name, "midi_in_info") == 0) return new midi_in_info (root);
	if (strcmp (name, "midi_out_info") == 0) return new midi_out_info (root);
	if (strcmp (name, "midi_in_port") == 0) return new midi_in_port (root);
	if (strcmp (name, "midi_out_port") == 0) return new midi_out_port (root);
	if (strcmp (name, "DCMOD") == 0) return new DCMOD ();
	if (strcmp (name, "INTERVAL") == 0) return new interval_processor (root, this);
	if (strcmp (name, "midi_manufacturers_id") == 0) return new midi_manufacturers_id (root);
	if (strcmp (name, "midi_product_id") == 0) return new midi_product_id (root);
	if (strcmp (name, "midi_product_version") == 0) return new midi_product_version (root);
	midi_stream * line = root -> getMidiOutput ();
	if (line) {
		if (strcmp (name, "midi_short_msg") == 0) return new midi_short_msg (line, this);
		if (strcmp (name, "MIDI_SHORT_MSG") == 0) return new MIDI_SHORT_MSG (line);
		if (strcmp (name, "midi_long_msg") == 0) return new midi_long_msg (line);
		if (strcmp (name, "sysex") == 0) return new sysex (line);
		if (strcmp (name, "sysexch") == 0) return new sysexch (line);
		if (strcmp (name, "SYSEX") == 0) return new SYSEX (line);
		if (strcmp (name, "SYSEXCH") == 0) return new SYSEXCH (line);
	} else {
		if (strcmp (name, "midi_short_msg") == 0) return new void_midi_message (root);
		if (strcmp (name, "MIDI_SHORT_MSG") == 0) return new void_midi_message (root);
		if (strcmp (name, "midi_long_msg") == 0) return new void_midi_message (root);
		if (strcmp (name, "sysex") == 0) return new void_midi_message (root);
		if (strcmp (name, "sysexch") == 0) return new void_midi_message (root);
		if (strcmp (name, "SYSEX") == 0) return new void_midi_message (root);
		if (strcmp (name, "SYSEXCH") == 0) return new void_midi_message (root);
	}

*/

int PrologMidiServiceClass :: diatonic (PrologAtom * atom) {
	if (atom == c || atom == cb || atom == cbb || atom == cx || atom == cxx) return 0;
	if (atom == d || atom == db || atom == dbb || atom == dx || atom == dxx) return 1;
	if (atom == e || atom == eb || atom == ebb || atom == ex || atom == exx) return 2;
	if (atom == f || atom == fb || atom == fbb || atom == fx || atom == fxx) return 3;
	if (atom == g || atom == gb || atom == gbb || atom == gx || atom == gxx) return 4;
	if (atom == a || atom == ab || atom == abb || atom == ax || atom == axx) return 5;
	if (atom == b || atom == bb || atom == bbb || atom == bx || atom == bxx) return 6;
	return -127;
}

int PrologMidiServiceClass :: chromatic (PrologAtom * atom) {
	if (atom == cbb) return -2;
	if (atom == cb) return -1;
	if (atom == c || atom == dbb) return 0;
	if (atom == cx || atom == db) return 1;
	if (atom == cxx || atom == d || atom == ebb) return 2;
	if (atom == dx || atom == eb || atom == fbb) return 3;
	if (atom == dxx || atom == e || atom == fb) return 4;
	if (atom == ex || atom == f || atom == gbb) return 5;
	if (atom == exx || atom == fx || atom == gb) return 6;
	if (atom == fxx || atom == g || atom == abb) return 7;
	if (atom == gx || atom == ab) return 8;
	if (atom == gxx || atom == a || atom == bbb) return 9;
	if (atom == ax || atom == bb || atom == cbb) return 10;
	if (atom == axx || atom == b || atom == cb) return 11;
	if (atom == bx) return 12;
	if (atom == bxx) return 13;
	return 0;
}

void PrologMidiServiceClass :: set_atoms (void) {
	if (keyon_atom != NULL) return;
	c = dir -> searchAtom ("C"); cb = dir -> searchAtom ("Cb"); cbb = dir -> searchAtom ("Cbb"); cx = dir -> searchAtom ("C#"); cxx = dir -> searchAtom ("Cx");
	d = dir -> searchAtom ("D"); db = dir -> searchAtom ("Db"); dbb = dir -> searchAtom ("Dbb"); dx = dir -> searchAtom ("D#"); dxx = dir -> searchAtom ("Dx");
	e = dir -> searchAtom ("E"); eb = dir -> searchAtom ("Eb"); ebb = dir -> searchAtom ("Ebb"); ex = dir -> searchAtom ("E#"); exx = dir -> searchAtom ("Ex");
	f = dir -> searchAtom ("F"); fb = dir -> searchAtom ("Fb"); fbb = dir -> searchAtom ("Fbb"); fx = dir -> searchAtom ("F#"); fxx = dir -> searchAtom ("Fx");
	g = dir -> searchAtom ("G"); gb = dir -> searchAtom ("Gb"); gbb = dir -> searchAtom ("Gbb"); gx = dir -> searchAtom ("G#"); gxx = dir -> searchAtom ("Gx");
	a = dir -> searchAtom ("A"); ab = dir -> searchAtom ("Ab"); abb = dir -> searchAtom ("Abb"); ax = dir -> searchAtom ("A#"); axx = dir -> searchAtom ("Ax");
	b = dir -> searchAtom ("B"); bb = dir -> searchAtom ("Bb"); bbb = dir -> searchAtom ("Bbb"); bx = dir -> searchAtom ("B#"); bxx = dir -> searchAtom ("Bx");
	keyoff_atom = dir -> searchAtom ("keyoff");
	keyon_atom = dir -> searchAtom ("keyon");
	polyaftertouch_atom = dir -> searchAtom ("polyaftertouch");
	control_atom = dir -> searchAtom ("control");
	programchange_atom = dir -> searchAtom ("programchange");
	aftertouch_atom = dir -> searchAtom ("aftertouch");
	pitch_atom = dir -> searchAtom ("pitch");
	sysex_atom = dir -> searchAtom ("sysex");
	SYSEX_atom = dir -> searchAtom ("SYSEX");
	timingclock_atom = dir -> searchAtom ("timingclock");
	start_atom = dir -> searchAtom ("START");
	continue_atom = dir -> searchAtom ("CONTINUE");
	stop_atom = dir -> searchAtom ("STOP");
	activesensing_atom = dir -> searchAtom ("activesensing");
	midi_manufacturers_id_atom = dir -> searchAtom ("midi_manufacturers_id");
	midi_product_id_atom = dir -> searchAtom ("midi_product_id");
	midi_product_version_atom = dir -> searchAtom ("midi_product_version");
	connect_thru_atom = dir -> searchAtom ("connectThru");
	default_destination_atom = dir -> searchAtom ("defaultDestination");
}

PrologNativeCode * PrologMidiServiceClass :: getNativeCode (char * name) {
	set_atoms ();
	if (strcmp (name, "createLine") == 0) return new CreateLine (root, dir, this);
	if (strcmp (name, "createSource") == 0) return new CreateSource (root, dir, this);
	if (strcmp (name, "createDestination") == 0) return new CreateDestination ();
	if (strcmp (name, "connectThru") == 0) return new ConnectThru ();
	if (strcmp (name, "defaultDestination") == 0) return new DefaultDestination (this);
	if (strcmp (name, "MidiLineType") == 0) return new MidiLineType ();
	if (strcmp (name, "midi_message") == 0) return new midi_message_command (this);
	if (strcmp (name, "keyoff") == 0) return new keyoff_command (this);
	if (strcmp (name, "keyon") == 0) return new MidiShortCommand (this, 3, 144);
	if (strcmp (name, "polyaftertouch") == 0) return new MidiShortCommand (this, 3, 160);
	if (strcmp (name, "aftertouch") == 0) return new MidiShortCommand (this, 2, 208);
	if (strcmp (name, "programchange") == 0) return new programchange_command (this);
	if (strcmp (name, "pitch") == 0) return new pitch_command (this, false);
	if (strcmp (name, "bank") == 0) return new bank_command (this);
	if (strcmp (name, "egcopy_index") == 0) return new egcopy (this, 0);
	if (strcmp (name, "egcopy_freq") == 0) return new egcopy (this, 16);
	if (strcmp (name, "egcopy_amp") == 0) return new egcopy (this, 32);
	if (strcmp (name, "egcopy_pan") == 0) return new egcopy (this, 48);
	if (strcmp (name, "egcopy") == 0) return new egcopy (this, 32);
	if (strcmp (name, "control") == 0) return new MidiShortCommand (this, 3, 176);
	if (strcmp (name, "banklsb") == 0) return new MidiShortCommand (this, 2, 176, 32);
	if (strcmp (name, "attack") == 0) return new MidiShortCommand (this, 2, 176, 73);
	if (strcmp (name, "release") == 0) return new MidiShortCommand (this, 2, 176, 72);
	if (strcmp (name, "cutoff") == 0) return new MidiShortCommand (this, 2, 176, 74);
	if (strcmp (name, "resonance") == 0) return new MidiShortCommand (this, 2, 176, 71);
	if (strcmp (name, "portatime") == 0) return new MidiShortCommand (this, 2, 176, 5);
	if (strcmp (name, "volume") == 0) return new MidiShortCommand (this, 2, 176, 7);
	if (strcmp (name, "reverb") == 0) return new MidiShortCommand (this, 2, 176, 91);
	if (strcmp (name, "chorus") == 0) return new MidiShortCommand (this, 2, 176, 93);
	if (strcmp (name, "foot") == 0) return new MidiShortCommand (this, 2, 176, 4);
	if (strcmp (name, "breath") == 0) return new MidiShortCommand (this, 2, 176, 2);
	if (strcmp (name, "pan") == 0) return new MidiShortCommand (this, 2, 176, 10);
	if (strcmp (name, "modulation") == 0) return new MidiShortCommand (this, 2, 176, 1);
	if (strcmp (name, "nrpn") == 0) return new nrpn_rpn_command (this);
	if (strcmp (name, "rpn") == 0) return new nrpn_rpn_command (this, true);
	if (strcmp (name, "mono") == 0) return new MidiShortCommand (this, 1, 126, 0);
	if (strcmp (name, "poly") == 0) return new MidiShortCommand (this, 1, 127, 0);
	if (strcmp (name, "portaon") == 0) return new MidiShortCommand (this, 1, 65, 127);
	if (strcmp (name, "portaoff") == 0) return new MidiShortCommand (this, 1, 65, 0);
	if (strcmp (name, "holdon") == 0) return new MidiShortCommand (this, 1, 64, 127);
	if (strcmp (name, "holdoff") == 0) return new MidiShortCommand (this, 1, 64, 0);
	if (strcmp (name, "sysex") == 0) return new sysex (this, false, false);
	if (strcmp (name, "sysexch") == 0) return new sysex (this, false, true);
	if (strcmp (name, "SYSEX") == 0) return new sysex (this, true, false);
	if (strcmp (name, "SYSEXCH") == 0) return new sysex (this, true, true);
	if (strcmp (name, "chex") == 0) return new chex (this);
	if (strcmp (name, "chexer") == 0) return new chexer ();
	if (strcmp (name, "timingclock") == 0) return new MidiShortCommand (this, 0, 248);
	if (strcmp (name, "START") == 0) return new MidiShortCommand (this, 0, 250);
	if (strcmp (name, "STOP") == 0) return new MidiShortCommand (this, 0, 252);
	if (strcmp (name, "CONTINUE") == 0) return new MidiShortCommand (this, 0, 251);
	if (strcmp (name, "activesensing") == 0) return new MidiShortCommand (this, 0, 254);
	if (strcmp (name, "PITCH") == 0) return new pitch_command (this, true);
	if (strcmp (name, "CONTROL") == 0) return new MidiShortCommand (this, 3, 176, -1, true);
	if (strcmp (name, "ATTACK") == 0) return new MidiShortCommand (this, 2, 176, 73, true);
	if (strcmp (name, "RELEASE") == 0) return new MidiShortCommand (this, 2, 176, 72, true);
	if (strcmp (name, "CUTOFF") == 0) return new MidiShortCommand (this, 2, 176, 74, true);
	if (strcmp (name, "RESONANCE") == 0) return new MidiShortCommand (this, 2, 176, 71, true);
	if (strcmp (name, "PORTATIME") == 0) return new MidiShortCommand (this, 2, 176, 5, true);
	if (strcmp (name, "VOLUME") == 0) return new MidiShortCommand (this, 2, 176, 7, true);
	if (strcmp (name, "REVERB") == 0) return new MidiShortCommand (this, 2, 176, 91, true);
	if (strcmp (name, "CHORUS") == 0) return new MidiShortCommand (this, 2, 176, 93, true);
	if (strcmp (name, "FOOT") == 0) return new MidiShortCommand (this, 2, 176, 4, true);
	if (strcmp (name, "BREATH") == 0) return new MidiShortCommand (this, 2, 176, 2, true);
	if (strcmp (name, "PAN") == 0) return new MidiShortCommand (this, 2, 176, 10, true);
	if (strcmp (name, "MODULATION") == 0) return new MidiShortCommand (this, 2, 176, 1, true);
	if (strcmp (name, "NRPN") == 0) return new nrpn_rpn_command (this, false, true);
	if (strcmp (name, "DCMOD") == 0) return new DCMOD ();
	if (strcmp (name, "INTERVAL") == 0) return new interval_processor (root, this);
	return NULL;
}

void PrologMidiServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
	default_destination = 0;
	this -> dir = directory;
	keyon_atom = 0;
}
PrologMidiServiceClass :: PrologMidiServiceClass (void) {this -> root = NULL; dir = 0;}
PrologMidiServiceClass :: ~ PrologMidiServiceClass (void) {}

