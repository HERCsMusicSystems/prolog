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
	prolog_midi_reader (PrologRoot * root, PrologAtom * atom, PrologMidiServiceClass * servo);
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
	if (line -> check_system_exclusive ()) atom = servo -> sysex_atom;
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
void prolog_midi_reader :: midi_timing_clock (void) {call (root -> pair (root -> atom (servo -> timingclock_atom), root -> earth ()));}
void prolog_midi_reader :: midi_start (void) {call (root -> pair (root -> atom (servo -> start_atom), root -> earth ()));}
void prolog_midi_reader :: midi_continue (void) {call (root -> pair (root -> atom (servo -> continue_atom), root -> earth ()));}
void prolog_midi_reader :: midi_stop (void) {call (root -> pair (root -> atom (servo -> stop_atom), root -> earth ()));}
void prolog_midi_reader :: midi_active_sensing (void) {call (root -> pair (root -> atom (servo -> activesensing_atom), root -> earth ()));}
prolog_midi_reader :: prolog_midi_reader (PrologRoot * root, PrologAtom * atom, PrologMidiServiceClass * servo) {
	this -> root = root;
	midi_dir = 0;
	income_midi_atom = atom;
	if (income_midi_atom) {COLLECTOR_REFERENCE_INC (income_midi_atom);}
	this -> servo = servo;
	if (root == 0) return;
	midi_dir = root -> searchDirectory ("midi");
}
prolog_midi_reader :: ~ prolog_midi_reader (void) {if (income_midi_atom) income_midi_atom -> removeAtom (); income_midi_atom = 0;}

static char * midi_internal_line_name = "MidiInternalLine";

class InternalMidiLine : public buffered_midi_stream {
public:
	prolog_midi_reader * reader;
	virtual void internal_close_message (void) {
		buffered_midi_stream :: internal_close_message ();
		reader -> read (this);
	}
	InternalMidiLine (PrologRoot * root, PrologAtom * atom, PrologMidiServiceClass * servo) : buffered_midi_stream (512) {reader = new prolog_midi_reader (root, atom, servo);}
	virtual ~ InternalMidiLine (void) {if (reader) delete reader; reader = 0;}
};

typedef void * (* runner_procedure) (void * parameter);
static void beginthread (runner_procedure runner, void * root) {
	pthread_t threader;
	pthread_attr_t attr;
	pthread_attr_init (& attr);
	pthread_attr_setstacksize (& attr, 120 * 1024);
	pthread_attr_setdetachstate (& attr, PTHREAD_CREATE_DETACHED);
	pthread_create (& threader, & attr, runner, root);
	pthread_attr_destroy (& attr);
}

#include <unistd.h>
#include <fcntl.h>
static int midi_input_id = -1;

static void * midi_runner (void * parameter) {
	unsigned char v1;
	while (true) {
		int res = read (midi_input_id, & v1, 1);
		if (v1 != 254) printf ("read [%i %i]\n", res, v1);
	}
}

class SourceMidiLine : public midi_stream {
public:
	prolog_midi_reader * reader;
	virtual void internal_close_message (void) {
		midi_stream :: internal_close_message ();
		reader -> read (this);
	}
	SourceMidiLine (PrologRoot * root, PrologAtom * atom, PrologMidiServiceClass * servo, char * file_name) {
		reader = new prolog_midi_reader (root, atom, servo);
		midi_input_id = open (file_name, O_RDONLY);
		printf ("open [%i]\n", midi_input_id);
		//beginthread (midi_runner, this);
	}
	virtual ~ SourceMidiLine (void) {if (reader) delete reader; reader = 0;}
};

class PrologMidiNativeCode : public PrologNativeCode {
public:
	PrologAtom * atom;
	midi_stream * line;
	static char * name (void) {return midi_internal_line_name;}
	char * codeName (void) {return midi_internal_line_name;}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		return false;
	}
	PrologMidiNativeCode (PrologAtom * atom, PrologRoot * root, PrologAtom * income_midi, PrologMidiServiceClass * servo) {
		this -> atom = atom;
		line = new InternalMidiLine (root, income_midi, servo);
	}
	~ PrologMidiNativeCode (void) {if (line) delete line; line = 0;}
};

class PrologMidiSourceCode : public PrologNativeCode {
public:
	PrologAtom * atom;
	midi_stream * line;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		return false;
	}
	PrologMidiSourceCode (PrologRoot * root, PrologMidiServiceClass * servo, PrologAtom * atom, PrologAtom * income, char * file_name) {
		this -> atom = atom;
		line = new SourceMidiLine (root, income, servo, file_name);
	}
	~ PrologMidiSourceCode (void) {if (line) delete line; line = 0;}
};

bool short_message_processor (int required, int command, PrologElement * parameters, PrologMidiServiceClass * servo, int ctrl = -1, bool extended = false) {
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
		if (required > 1) return false;
		destination -> line -> insert (command);
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
	command += destination -> line -> chex (channel);
	if (parameters -> isEarth ()) {
		if (required > 2) return false;
		if (extended) {
			if (ctrl < 0) return false;
			msb &= 0x3fff;
			int lsb = msb & 0x7f;
			msb >>= 7;
			destination -> line -> insert (command, ctrl + 32, lsb);
			destination -> line -> insert (command, ctrl, msb);
		} else {
			if (ctrl >= 0) destination -> line -> insert (command, ctrl, msb);
			else destination -> line -> insert (command, msb);
		}
		return true;
	}
	if (! parameters -> isPair ()) return false;
	el = parameters -> getLeft ();
	if (! el -> isInteger ()) return false;
	int lsb = el -> getInteger ();
	if (required > 3) return false;
	if (extended) {
		int mlsb = lsb & 0x3fff;
		int llsb = mlsb & 0x7f;
		mlsb >>= 7;
		destination -> line -> insert (command, msb + 32, llsb);
		destination -> line -> insert (command, msb, mlsb);
	} else destination -> line -> insert (command, msb, lsb);
	return true;
}

class CreateLine : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * line_atom = parameters -> getLeft ();
		if (line_atom -> isVar ()) line_atom -> setAtom (new PrologAtom ());
		if (! line_atom -> isAtom ()) return false;
		PrologAtom * income_midi = 0;
		if (parameters -> isPair ()) {
			parameters = parameters -> getRight ();
			if (parameters -> isPair ()) {
				PrologElement * el = parameters -> getLeft ();
				if (el -> isAtom ()) income_midi = el -> getAtom ();
			}
		}
		PrologMidiNativeCode * line = new PrologMidiNativeCode (line_atom -> getAtom (), root, income_midi, servo);
		if (line_atom -> getAtom () -> setMachine (line)) return true;
		delete line;
		return false;
	}
	CreateLine (PrologRoot * root, PrologMidiServiceClass * servo) {this -> root = root; this -> servo = servo;}
};

class CreateSource : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologMidiServiceClass * servo;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
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
		PrologMidiSourceCode * source = new PrologMidiSourceCode (root, servo, line, income, file_name);
		if (line -> setMachine (source)) return true;
		delete source;
		return false;
	}
	CreateSource (PrologRoot * root, PrologMidiServiceClass * servo) {this -> root = root; this -> servo = servo;}
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

class control_command : public MidiShortCommand {
public: control_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 3, 176) {}};

class attack_command : public MidiShortCommand {
public: attack_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 73) {}};

class release_command : public MidiShortCommand {
public: release_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 72) {}};

class cutoff_command : public MidiShortCommand {
public: cutoff_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 74) {}};

class resonance_command : public MidiShortCommand {
public: resonance_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 71) {}};

class portatime_command : public MidiShortCommand {
public: portatime_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 5) {}};

class volume_command : public MidiShortCommand {
public: volume_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 7) {}};

class reverb_command : public MidiShortCommand {
public: reverb_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 91) {}};

class chorus_command : public MidiShortCommand {
public: chorus_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 93) {}};

class foot_command : public MidiShortCommand {
public: foot_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 4) {}};

class breath_command : public MidiShortCommand {
public: breath_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 2) {}};

class pan_command : public MidiShortCommand {
public: pan_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 10) {}};

class modulation_command : public MidiShortCommand {
public: modulation_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 1) {}};

class CONTROL_command : public MidiShortCommand {
public: CONTROL_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 3, 176, true) {}};

class ATTACK_command : public MidiShortCommand {
public: ATTACK_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 73, true) {}};

class RELEASE_command : public MidiShortCommand {
public: RELEASE_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 72, true) {}};

class CUTOFF_command : public MidiShortCommand {
public: CUTOFF_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 74, true) {}};

class RESONANCE_command : public MidiShortCommand {
public: RESONANCE_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 71, true) {}};

class PORTATIME_command : public MidiShortCommand {
public: PORTATIME_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 5, true) {}};

class VOLUME_command : public MidiShortCommand {
public: VOLUME_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 7, true) {}};

class REVERB_command : public MidiShortCommand {
public: REVERB_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 91, true) {}};

class CHORUS_command : public MidiShortCommand {
public: CHORUS_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 93, true) {}};

class FOOT_command : public MidiShortCommand {
public: FOOT_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 4, true) {}};

class BREATH_command : public MidiShortCommand {
public: BREATH_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 2, true) {}};

class PAN_command : public MidiShortCommand {
public: PAN_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 10, true) {}};

class MODULATION_command : public MidiShortCommand {
public: MODULATION_command (PrologMidiServiceClass * servo) : MidiShortCommand (servo, 2, 176, 1, true) {}};

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
	dir = root -> searchDirectory ("midi");
	if (dir == NULL) return;
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
}

PrologNativeCode * PrologMidiServiceClass :: getNativeCode (char * name) {
	set_atoms ();
	if (strcmp (name, "createLine") == 0) return new CreateLine (root, this);
	if (strcmp (name, "createSource") == 0) return new CreateSource (root, this);
	if (strcmp (name, "createDestination") == 0) return new CreateDestination ();
	if (strcmp (name, "control") == 0) return new control_command (this);
	if (strcmp (name, "attack") == 0) return new attack_command (this);
	if (strcmp (name, "release") == 0) return new release_command (this);
	if (strcmp (name, "cutoff") == 0) return new cutoff_command (this);
	if (strcmp (name, "resonance") == 0) return new resonance_command (this);
	if (strcmp (name, "portatime") == 0) return new portatime_command (this);
	if (strcmp (name, "volume") == 0) return new volume_command (this);
	if (strcmp (name, "reverb") == 0) return new reverb_command (this);
	if (strcmp (name, "chorus") == 0) return new chorus_command (this);
	if (strcmp (name, "foot") == 0) return new foot_command (this);
	if (strcmp (name, "breath") == 0) return new breath_command (this);
	if (strcmp (name, "pan") == 0) return new pan_command (this);
	if (strcmp (name, "modulation") == 0) return new modulation_command (this);
	if (strcmp (name, "CONTROL") == 0) return new CONTROL_command (this);
	if (strcmp (name, "ATTACK") == 0) return new ATTACK_command (this);
	if (strcmp (name, "RELEASE") == 0) return new RELEASE_command (this);
	if (strcmp (name, "CUTOFF") == 0) return new CUTOFF_command (this);
	if (strcmp (name, "RESONANCE") == 0) return new RESONANCE_command (this);
	if (strcmp (name, "PORTATIME") == 0) return new PORTATIME_command (this);
	if (strcmp (name, "VOLUME") == 0) return new VOLUME_command (this);
	if (strcmp (name, "REVERB") == 0) return new REVERB_command (this);
	if (strcmp (name, "CHORUS") == 0) return new CHORUS_command (this);
	if (strcmp (name, "FOOT") == 0) return new FOOT_command (this);
	if (strcmp (name, "BREATH") == 0) return new BREATH_command (this);
	if (strcmp (name, "PAN") == 0) return new PAN_command (this);
	if (strcmp (name, "MODULATION") == 0) return new MODULATION_command (this);
	return NULL;
}

void PrologMidiServiceClass :: init (PrologRoot * root) {this -> root = root; default_source = default_destination = 0;}
PrologMidiServiceClass :: PrologMidiServiceClass (void) {this -> root = NULL; dir = 0;}
PrologMidiServiceClass :: ~ PrologMidiServiceClass (void) {}

