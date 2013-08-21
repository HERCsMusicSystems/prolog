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
#include "midi_stream.h"
#include <string.h>

static bool midi_process (midi_stream * line, PrologElement * parameters) {
	printf ("I am here!\n");
	return true;
}

static char * midi_internal_line_name = "MidiInternalLine";
char * PrologMidiNativeCode :: name (void) {return midi_internal_line_name;}
char * PrologMidiNativeCode :: codeName (void) {return midi_internal_line_name;}
void PrologMidiNativeCode :: insert_one (int command) {printf ("command [%i]\n", command);}
void PrologMidiNativeCode :: insert_two (int command, int channel, int msb) {printf ("command [%i %i]\n", command + channel, msb);}
void PrologMidiNativeCode :: insert_three (int command, int channel, int msb, int lsb) {printf ("command [%i %i %i]\n", command + channel, msb, lsb);}

class MidiInternalLine : public PrologMidiNativeCode {
public:
	PrologAtom * atom;
	buffered_midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		return midi_process (line, parameters);
	}
	MidiInternalLine (PrologAtom * atom) {this -> atom = atom; line = new buffered_midi_stream (512);}
	~ MidiInternalLine (void) {if (line) delete line; line = 0;}
};

bool short_message_processor (int command, PrologElement * parameters, PrologMidiServiceClass * servo) {
	PrologMidiNativeCode * destination = servo -> default_destination;
	if (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) {
			PrologNativeCode * machine = el -> getAtom () -> getMachine ();
			if (machine == 0) return false;
			if (machine -> codeName () != PrologMidiNativeCode :: name ()) return false;
			destination = (PrologMidiNativeCode *) machine;
			parameters = parameters -> getRight ();
		}
	}
	if (destination == 0) return false;
	if (parameters -> isEarth ()) {
		destination -> insert_one (command);
		return true;
	}
	if (! parameters -> isPair ()) return false;
	PrologElement * el = parameters -> getLeft ();
	if (! el -> isInteger ()) return false;
	int channel = el -> getInteger ();
	parameters = parameters -> getRight ();
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
		destination -> insert_two (command, channel, msb);
		return true;
	}
	if (! parameters -> isPair ()) return false;
	el = parameters -> getLeft ();
	if (! el -> isInteger ()) return false;
	int lsb = el -> getInteger ();
	destination -> insert_three (command, channel, msb, lsb);
	return true;
}

class CreateLine : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * line_atom = parameters -> getLeft ();
		if (line_atom -> isVar ()) line_atom -> setAtom (new PrologAtom ());
		if (! line_atom -> isAtom ()) return false;
		MidiInternalLine * line = new MidiInternalLine (line_atom -> getAtom ());
		if (line_atom -> getAtom () -> setMachine (line)) return true;
		delete line;
		return false;
	}
	CreateLine (void) {}
};

class control : public PrologNativeCode {
public:
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return short_message_processor (176, parameters, servo);}
	control (PrologMidiServiceClass * servo) {this -> servo = servo;}
};

/*

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

class midi_short_msg : public PrologNativeCode {
public:
	midi_stream * line;
	PrologStudio * studio;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		int msg, channel, msb, lsb;
		PrologElement * el;
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		msg = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			midi_wait ();
			line -> insert (msg);
			midi_signal ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		channel = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (el -> isInteger ()) msb = el -> getInteger ();
		else if (el -> isPair ()) {
			PrologElement * note = el -> getLeft ();
			el = el -> getRight ();
			if (! el -> isPair ()) return false;
			PrologElement * octave = el -> getLeft ();
			if (! note -> isAtom ()) return false;
			if (! octave -> isInteger ()) return false;
			msb = 48 + octave -> getInteger () * 12 + studio -> chromatic (note -> getAtom ());
			if (msb < 0 || msb > 127) return false;
		} else return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			midi_wait ();
			line -> insert (msg + line -> chex (channel), msb);
			midi_signal ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		lsb = el -> getInteger ();
		midi_wait ();
		line -> insert (msg + line -> chex (channel), msb, lsb);
		midi_signal ();
		return true;
	}
	midi_short_msg (midi_stream * line, PrologStudio * studio) {this -> line = line; this -> studio = studio;}
};

class MIDI_SHORT_MSG : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		int msg, channel, ctrl, msb, lsb;
		PrologElement * el;
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		msg = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		channel = el -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		ctrl = el -> getInteger ();
		if (ctrl >= 96) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		el = parameters -> getLeft ();
		if (! el -> isInteger ()) return false;
		msb = el -> getInteger ();
		msb &= 0x3fff;
		lsb = msb & 0x7f;
		msb >>= 7;
		midi_wait ();
		msg += line -> chex (channel);
		line -> insert (msg, ctrl + 32, lsb);
		line -> insert (msg, ctrl, msb);
		midi_signal ();
		return true;
	}
	MIDI_SHORT_MSG (midi_stream * line) {this -> line = line;}
};

class midi_long_msg : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * el;
		midi_wait ();
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger ()) return false;
			line -> insert (el -> getInteger ());
			parameters = parameters -> getRight ();
		}
		line -> close_message ();
		midi_signal ();
		return true;
	}
	midi_long_msg (midi_stream * line) {this -> line = line;}
};

class sysex : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {line -> close_system_exclusive (); midi_signal (); return false;}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	sysex (midi_stream * line) {this -> line = line;}
};

class SYSEX : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_generic_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {line -> close_system_exclusive (); midi_signal (); return false;}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	SYSEX (midi_stream * line) {this -> line = line;}
};

class sysexch : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {
				line -> insert_checksum ();
				line -> close_system_exclusive ();
				midi_signal ();
				return false;
			}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> insert_checksum ();
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	sysexch (midi_stream * line) {this -> line = line;}
};

class SYSEXCH : public PrologNativeCode {
public:
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_wait ();
		line -> open_generic_system_exclusive ();
		PrologElement * el;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (! el -> isInteger () && ! el -> isText ()) {
				line -> insert_checksum ();
				line -> close_system_exclusive ();
				midi_signal ();
				return false;
			}
			if (el -> isInteger ()) line -> insert (el -> getInteger ());
			if (el -> isText ()) line -> insert (el -> getText ());
			parameters = parameters -> getRight ();
		}
		line -> insert_checksum ();
		line -> close_system_exclusive ();
		midi_signal ();
		return true;
	}
	SYSEXCH (midi_stream * line) {this -> line = line;}
};

class midi_manufacturers_id : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_stream * line_in = root -> getMidiInput ();
		midi_stream * line_out = root -> getMidiOutput ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		if (parameters -> isEarth ()) {
			if (line_in != NULL) line_in -> set_manufacturers_id ();
			if (line_out != NULL) line_out -> set_manufacturers_id ();
			if (service != NULL) service -> changeManufacturersId ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * m1 = parameters -> getLeft ();
		if (! m1 -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			if (line_in != NULL) line_in -> set_manufacturers_id (m1 -> getInteger ());
			if (line_out != NULL) line_out -> set_manufacturers_id (m1 -> getInteger ());
			if (service != NULL) service -> changeManufacturersId (m1 -> getInteger ());
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * m2 = parameters -> getLeft ();
		if (! m2 -> isInteger ()) return false;
		if (line_in != NULL) line_in -> set_manufacturers_id (m1 -> getInteger (), m2 -> getInteger ());
		if (line_out != NULL) line_out -> set_manufacturers_id (m1 -> getInteger (), m2 -> getInteger ());
		if (service != NULL) service -> changeManufacturersId (m1 -> getInteger (), m2 -> getInteger ());
		return true;
	}
	midi_manufacturers_id (PrologRoot * root) {this -> root = root;}
};

class midi_product_id : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_stream * line_in = root -> getMidiInput ();
		midi_stream * line_out = root -> getMidiOutput ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		char id1 = -1, id2 = -1, id3 = -1, id4 = -1;
		if (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (! e -> isInteger ()) return false;
			id1 = (char) e -> getInteger ();
			parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				e = parameters -> getLeft ();
				if (! e -> isInteger ()) return false;
				id2 = (char) e -> getInteger ();
				parameters = parameters -> getRight ();
				if (parameters -> isPair ()) {
					e = parameters -> getLeft ();
					if (! e -> isInteger ()) return false;
					id3 = (char) e -> getInteger ();
					parameters = parameters -> getRight ();
					if (parameters -> isPair ()) {
						e = parameters -> getLeft ();
						if (! e -> isInteger ()) return false;
						id4 = (char) e -> getInteger ();
					}
				}
			}
		}
		if (line_in != NULL) line_in -> set_product_id (id1, id2, id3, id4);
		if (line_out != NULL) line_out -> set_product_id (id1, id2, id3, id4);
		if (service != NULL) service -> changeProductId (id1, id2, id3, id4);
		return true;
	}
	midi_product_id (PrologRoot * root) {this -> root = root;}
};

class midi_product_version : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		midi_stream * line_in = root -> getMidiInput ();
		midi_stream * line_out = root -> getMidiOutput ();
		PrologMidiPortServiceClass * service = root -> getMidiPortServiceClass ();
		char id1 = -1, id2 = -1, id3 = -1, id4 = -1;
		if (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (! e -> isInteger ()) return false;
			id1 = (char) e -> getInteger ();
			parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				e = parameters -> getLeft ();
				if (! e -> isInteger ()) return false;
				id2 = (char) e -> getInteger ();
				parameters = parameters -> getRight ();
				if (parameters -> isPair ()) {
					e = parameters -> getLeft ();
					if (! e -> isInteger ()) return false;
					id3 = (char) e -> getInteger ();
					parameters = parameters -> getRight ();
					if (parameters -> isPair ()) {
						e = parameters -> getLeft ();
						if (! e -> isInteger ()) return false;
						id4 = (char) e -> getInteger ();
					}
				}
			}
		}
		if (line_in != NULL) line_in -> set_product_version (id1, id2, id3, id4);
		if (line_out != NULL) line_out -> set_product_version (id1, id2, id3, id4);
		if (service != NULL) service -> changeProductVersion (id1, id2, id3, id4);
		return true;
		return true;
	}
	midi_product_version (PrologRoot * root) {this -> root = root;}
};

void prolog_midi_reader :: call (PrologElement * query) {
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
void prolog_midi_reader :: midi_keyoff (int channel, int key) {call (build_call (keyoff_atom, channel, key));}
void prolog_midi_reader :: midi_keyon (int channel, int key, int velocity) {
	call (build_call (keyon_atom, channel, key, velocity));
}
void prolog_midi_reader :: midi_pat (int channel, int key, int value) {call (build_call (polyaftertouch_atom, channel, key, value));}
void prolog_midi_reader :: midi_control (int channel, int controller, int value) {call (build_call (control_atom, channel, controller, value));}
void prolog_midi_reader :: midi_programchange (int channel, int program) {call (build_call (programchange_atom, channel, program));}
void prolog_midi_reader :: midi_cat (int channel, int value) {call (build_call (aftertouch_atom, channel, value));}
void prolog_midi_reader :: midi_pitchbend (int channel, int v1, int v2) {call (build_call (pitch_atom, channel, v1, v2));}
void prolog_midi_reader :: midi_system_exclusive (midi_stream * line) {
	// prolog command sysex filter
	line -> mark ();
	PrologAtom * atom = SYSEX_atom;
	if (line -> check_system_exclusive ()) atom = sysex_atom;
	else line -> restore ();
	PrologElement * query = root -> pair (root -> atom (atom), root -> earth ());
	PrologElement * el = query -> getRight ();
	int ind = line -> get ();
	while (ind < 247) {
		el -> setPair (root -> integer (ind), root -> earth ());
		el = el -> getRight ();
		ind = line -> get ();
	}
	call (query);
}
void prolog_midi_reader :: midi_timing_clock (void) {call (root -> pair (root -> atom (timingclock_atom), root -> earth ()));}
void prolog_midi_reader :: midi_start (void) {call (root -> pair (root -> atom (start_atom), root -> earth ()));}
void prolog_midi_reader :: midi_continue (void) {call (root -> pair (root -> atom (continue_atom), root -> earth ()));}
void prolog_midi_reader :: midi_stop (void) {call (root -> pair (root -> atom (stop_atom), root -> earth ()));}
void prolog_midi_reader :: midi_active_sensing (void) {call (root -> pair (root -> atom (activesensing_atom), root -> earth ()));}
bool prolog_midi_reader :: is_ready (void) {
	if (studio_dir != NULL) return true;
	studio_dir = root -> searchDirectory ("studio");
	if (studio_dir == NULL) return false;
	income_midi_atom = studio_dir -> searchAtom ("income_midi");
	keyoff_atom = studio_dir -> searchAtom ("keyoff");
	keyon_atom = studio_dir -> searchAtom ("keyon");
	polyaftertouch_atom = studio_dir -> searchAtom ("polyaftertouch");
	control_atom = studio_dir -> searchAtom ("control");
	programchange_atom = studio_dir -> searchAtom ("programchange");
	aftertouch_atom = studio_dir -> searchAtom ("aftertouch");
	pitch_atom = studio_dir -> searchAtom ("pitch");
	sysex_atom = studio_dir -> searchAtom ("sysex");
	SYSEX_atom = studio_dir -> searchAtom ("SYSEX");
	timingclock_atom = studio_dir -> searchAtom ("timingclock");
	start_atom = studio_dir -> searchAtom ("START");
	continue_atom = studio_dir -> searchAtom ("CONTINUE");
	stop_atom = studio_dir -> searchAtom ("STOP");
	activesensing_atom = studio_dir -> searchAtom ("activesensing");
	return true;
}
prolog_midi_reader :: prolog_midi_reader (PrologRoot * root) {
	this -> root = root;
	studio_dir = NULL;
	income_midi_atom = NULL;
	keyoff_atom = NULL;
	keyon_atom = NULL;
	polyaftertouch_atom = NULL;
	control_atom = NULL;
	programchange_atom = NULL;
	aftertouch_atom = NULL;
	pitch_atom = NULL;
	sysex_atom = NULL;
	SYSEX_atom = NULL;
	timingclock_atom = NULL;
	start_atom = NULL;
	continue_atom = NULL;
	stop_atom = NULL;
	activesensing_atom = NULL;
	is_ready ();
}

class interval_processor : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologStudio * studio;
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
			int diatonic_left = studio -> diatonic (note_left -> getAtom ());
			if (diatonic_left < 0) return false;
			int chromatic_left = studio -> chromatic (note_left -> getAtom ());
			left_note = left_note -> getRight ();
			if (! left_note -> isPair ()) return false;
			PrologElement * octave_left = left_note -> getLeft ();
			if (! octave_left -> isInteger ()) return false;
			if (right_note -> isPair ()) {
				diatonic_left += 28 + octave_left -> getInteger () * 7;
				chromatic_left += 48 + octave_left -> getInteger () * 12;
				PrologElement * note_right = right_note -> getLeft ();
				if (! note_right -> isAtom ()) return false;
				int diatonic_right = studio -> diatonic (note_right -> getAtom ());
				if (diatonic_right < 0) return false;
				int chromatic_right = studio -> chromatic (note_right -> getAtom ());
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
				PrologAtom * ret = studio -> note (diatonic_right, chromatic_right);
				if (ret == NULL) return false;
				right_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_right), root -> earth ()));
				return true;
			}
			return false;
		}
		if (! right_note -> isPair ()) return false;
		PrologElement * note_right = right_note -> getLeft ();
		if (! note_right -> isAtom ()) return false;
		int diatonic_right = studio -> diatonic (note_right -> getAtom ());
		if (diatonic_right < 0) return false;
		int chromatic_right = studio -> chromatic (note_right -> getAtom ());
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
		PrologAtom * ret = studio -> note (diatonic_left, chromatic_left);
		if (ret == NULL) return false;
		left_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_left), root -> earth ()));
		return true;
	}
	interval_processor (PrologRoot * root, PrologStudio * studio) {this -> root = root; this -> studio = studio;}
};

PrologAtom * PrologStudio :: note (int diatonic, int chromatic) {
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
	if (dir != NULL) return;
	dir = root -> searchDirectory ("studio");
	if (dir == NULL) return;
	c = dir -> searchAtom ("C"); cb = dir -> searchAtom ("Cb"); cbb = dir -> searchAtom ("Cbb"); cx = dir -> searchAtom ("C#"); cxx = dir -> searchAtom ("Cx");
	d = dir -> searchAtom ("D"); db = dir -> searchAtom ("Db"); dbb = dir -> searchAtom ("Dbb"); dx = dir -> searchAtom ("D#"); dxx = dir -> searchAtom ("Dx");
	e = dir -> searchAtom ("E"); eb = dir -> searchAtom ("Eb"); ebb = dir -> searchAtom ("Ebb"); ex = dir -> searchAtom ("E#"); exx = dir -> searchAtom ("Ex");
	f = dir -> searchAtom ("F"); fb = dir -> searchAtom ("Fb"); fbb = dir -> searchAtom ("Fbb"); fx = dir -> searchAtom ("F#"); fxx = dir -> searchAtom ("Fx");
	g = dir -> searchAtom ("G"); gb = dir -> searchAtom ("Gb"); gbb = dir -> searchAtom ("Gbb"); gx = dir -> searchAtom ("G#"); gxx = dir -> searchAtom ("Gx");
	a = dir -> searchAtom ("A"); ab = dir -> searchAtom ("Ab"); abb = dir -> searchAtom ("Abb"); ax = dir -> searchAtom ("A#"); axx = dir -> searchAtom ("Ax");
	b = dir -> searchAtom ("B"); bb = dir -> searchAtom ("Bb"); bbb = dir -> searchAtom ("Bbb"); bx = dir -> searchAtom ("B#"); bxx = dir -> searchAtom ("Bx");
}

PrologNativeCode * PrologMidiServiceClass :: getNativeCode (char * name) {
	set_atoms ();
	if (strcmp (name, "createLine") == 0) return new CreateLine ();
	if (strcmp (name, "control") == 0) return new control (this);
	return NULL;
}

void PrologMidiServiceClass :: init (PrologRoot * root) {this -> root = root; default_source = default_destination = 0;}
PrologMidiServiceClass :: PrologMidiServiceClass (void) {this -> root = NULL; dir = 0;}
PrologMidiServiceClass :: ~ PrologMidiServiceClass (void) {}

