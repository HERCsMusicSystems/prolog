
#include <string.h>
#include "prolog.h"
#include "jack/jack.h"
#include "jack/midiport.h"

#ifdef LINUX_OPERATING_SYSTEM
extern "C" {
	extern char * get_module_code (void);
	extern PrologServiceClass * create_service_class (void);
}
extern char resource_jack;
char * get_module_code (void) {return & resource_jack;}
#endif

jack_client_t * jack_client;

class port_code : public PrologNativeCode {
public:
	PrologAtom * atom;
	jack_port_t * jack_midi_in;
	jack_port_t * jack_midi_out;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		return true;
	};
	port_code (PrologAtom * atom, char * input_name, char * output_name) {
		this -> atom = atom;
		jack_midi_out = jack_port_register (jack_client, input_name, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
		jack_midi_in = jack_port_register (jack_client, output_name, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	}
	~ port_code (void) {
		jack_port_unregister (jack_client, jack_midi_in);
		printf ("Port closed\n");
	}
};

class port_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * symbol = 0;
		PrologElement * input_name = 0;
		PrologElement * output_name = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) symbol = el;
			if (el -> isVar ()) symbol = el;
			if (el -> isText ()) {if (input_name == 0) input_name = el; else output_name = el;}
			parameters = parameters -> getRight ();
		}
		if (symbol == 0 || input_name == 0 || output_name == 0) return false;
		if (symbol -> isVar ()) symbol -> setAtom (new PrologAtom ());
		PrologAtom * atom = symbol -> getAtom ();
		if (atom -> getMachine () != 0) return false;
		port_code * code = new port_code (atom, input_name -> getText (), output_name -> getText ());
		if (atom -> setMachine (code)) return true;
		delete code;
		return false;
	};
};

class jack_service : public PrologServiceClass {
public:
	PrologNativeCode * getNativeCode (char * name) {
		if (strcmp (name, "port") == 0) return new port_class ();
		return 0;
	}
	jack_service (void) {
		jack_client = jack_client_open ("HRCS", JackNullOption, 0);
		printf ("Service init....\n");
	}
	~ jack_service (void) {
		printf ("Service stop..[%i]..\n", jack_client_close (jack_client));
	}
};

PrologServiceClass * create_service_class (void) {return new jack_service ();}
