
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

jack_client_t * jack_client = 0;
jack_port_t * jack_midi_in = 0;
jack_port_t * jack_midi_out = 0;
pthread_mutex_t mutex;
PrologRoot * prolog_root = 0;
PrologDirectory * prolog_directory = 0;

struct midi {
public:
	int time;
	int b1, b2, b3;
	int * sysex;
	int sysex_size;
	midi * next;
};

class midi_line {
public:
	midi * root;
	midi * top;
	char * name;
	void erase_line (void) {
		midi * line = root;
		while (line != 0) {
			if (line -> sysex) delete [] line -> sysex;
			midi * next = line -> next;
			delete line;
			line = next;
		}
		root = 0; top = 0;
	};
	void insert_midi (int b1, int b2, int b3) {
		midi * md = new midi;
		md -> time = prolog_root -> get_system_time ();
		md -> b1 = b1; md -> b2 = b2; md -> b3 = b3;
		md -> next = 0; md -> sysex = 0; md -> sysex_size = 0;
		if (top == 0) top = root = md;
		else top = top -> next = md;
	};
	void insert_midi (int * sysex, int size) {
		midi * md = new midi;
		md -> time = prolog_root -> get_system_time ();
		md -> sysex = sysex; md -> sysex_size = size;
		md -> next = 0;
		if (top == 0) top = root = md;
		else top = top -> next = md;
	}
	midi_line (void) {root = 0; top = 0;};
	~ midi_line (void) {erase_line ();};
};

midi_line line1;
midi_line line2;
midi_line * line = & line1;

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
		jack_port_unregister (jack_client, jack_midi_out);
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

int global_time = 0;

int Callback (jack_nframes_t nframes, void * args) {
	void * ib = jack_port_get_buffer (jack_midi_in, nframes);
	void * ob = jack_port_get_buffer (jack_midi_out, nframes);
	pthread_mutex_lock (& mutex);
	int new_time = prolog_root -> get_system_time ();
	double delta = (double) nframes / (double) (new_time - global_time);
	jack_midi_clear_buffer (ob);
	midi * md = line -> root;
	while (md != 0) {
		unsigned char * b = jack_midi_event_reserve (ob, (int) ((double) (md -> time - global_time) * delta) , 3);
		b [0] = md -> b1;
		b [1] = md -> b2;
		b [2] = md -> b3;
		md = md -> next;
	}
	line -> erase_line ();
	line = line == & line1 ? & line2 : & line1;
	global_time = new_time;
	pthread_mutex_unlock (& mutex);
	jack_nframes_t events = jack_midi_get_event_count (ib);
	jack_midi_event_t event;
	for (int ind = 0; ind < events; ind ++) {
		jack_midi_event_get (& event, ib, ind);
		printf ("midi [%0X ", event . time);
		for (int sub = 0; sub < event . size; sub ++) printf (" %0X", event . buffer [sub]);
		printf ("]\n");
	}
	return 0;
};

bool activate (char * client, char * in, char * out) {
	if (jack_client != 0) return false;
	jack_client = jack_client_open (client, JackNullOption, 0);
	jack_midi_out = jack_port_register (jack_client, out, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
	jack_midi_in = jack_port_register (jack_client, in, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	jack_set_process_callback (jack_client, Callback, 0);
	jack_activate (jack_client);
	return true;
};

bool deactivate (void) {
	if (jack_client == 0) return false;
	jack_port_unregister (jack_client, jack_midi_in); jack_midi_in = 0;
	jack_port_unregister (jack_client, jack_midi_out); jack_midi_out = 0;
	jack_client_close (jack_client); jack_client = 0;
	return true;
};

class activate_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return deactivate ();
		PrologElement * client = 0;
		PrologElement * in = 0;
		PrologElement * out = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) {if (client == 0) client = el; else if (in == 0) in = el; else out = el;}
			parameters = parameters -> getRight ();
		}
		if (client == 0) return false;
		return activate (client -> getText (), in == 0 ? (char *) "in" : in -> getText (), out == 0 ? (char *) "out" : out -> getText ());
	};
};

class keyon_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * channel = 0;
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) {if (channel == 0) channel = el; else {if (key == 0) key = el; else velocity = el;}}
			parameters = parameters -> getRight ();
		}
		if (velocity == 0) return false;
		int b1 = channel -> getInteger () + 0x90;
		int b2 = key -> getInteger ();
		int b3 = velocity -> getInteger ();
		pthread_mutex_lock (& mutex);
		printf ("%s %i %i %i\n", line -> name, b1, key -> getInteger (), b3);
		//line -> insert_midi (0x90 + channel -> getInteger (), key -> getInteger (), velocity -> getInteger ());
		line -> insert_midi (b1, b2, b3);
		pthread_mutex_unlock (& mutex);
		return true;
	};
};

class jack_service : public PrologServiceClass {
public:
	void init (PrologRoot * root, PrologDirectory * directory) {
		prolog_root = root;
		prolog_directory = directory;
		PrologString * args = root -> args;
		while (args != 0) {printf ("ARGS [%s]\n", args -> text); args = args -> next;}
//		char * * port_names = (char * *) jack_get_ports (jack_client, 0, 0, 0);
//		while (* port_names != 0) {printf ("PORTS [%s]\n", * port_names); port_names ++;}
//		printf ("callback registering [%i]\n", jack_set_process_callback (jack_client, Callback, 0));
//		printf ("activate [%i]\n", jack_activate (jack_client));
		global_time = root -> get_system_time ();
	}
	PrologNativeCode * getNativeCode (char * name) {
		if (strcmp (name, "port") == 0) return new port_class ();
		if (strcmp (name, "activate") == 0) return new activate_class ();
		if (strcmp (name, "keyon") == 0) return new keyon_class ();
		return 0;
	}
	jack_service (void) {
		mutex = PTHREAD_MUTEX_INITIALIZER;
		line1 . name = "Left";
		line2 . name = "Right";
	}
	~ jack_service (void) {
		deactivate ();
		pthread_mutex_destroy (& mutex);
	}
};

PrologServiceClass * create_service_class (void) {return new jack_service ();}

