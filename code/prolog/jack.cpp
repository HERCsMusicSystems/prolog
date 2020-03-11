
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
PrologRoot * proot = 0;
PrologDirectory * prolog_directory = 0;
PrologAtom * income_midi_atom = 0;
PrologAtom * keyoff_atom = 0, * keyon_atom = 0, * polyaftertouch_atom = 0, * control_atom = 0, * programchange_atom = 0, * aftertouch_atom = 0, * pitch_atom = 0,
* sysex_atom = 0, * quarterframe_atom = 0, * songpositionpointer_atom = 0, * songselect_atom = 0, * uF4_atom = 0, * uF5_atom = 0, * tunerequest_atom = 0,
* timingclock_atom = 0, * uF9_atom = 0, * START_atom = 0, * CONTINUE_atom = 0, * STOP_atom = 0, * uFD_atom = 0, * activesensing_atom = 0, * systemreset_atom = 0;

void prolog_midi_callback (int size, unsigned char * data) {
	PrologAtom * atom;
	switch (data [0] >> 4) {
	case 0x8: atom = keyoff_atom; break;
	case 0x9: atom = keyon_atom; break;
	case 0xa: atom = polyaftertouch_atom; break;
	case 0xb: atom = control_atom; break;
	case 0xc: atom = programchange_atom; break;
	case 0xd: atom = aftertouch_atom; break;
	case 0xe: atom = pitch_atom; break;
	case 0xf: {
		switch (data [0]) {
		case 0xf0: atom = sysex_atom; break;
		case 0xf1: atom = quarterframe_atom; break;
		case 0xf2: atom = songpositionpointer_atom; break;
		case 0xf3: atom = songselect_atom; break;
		case 0xf4: atom = uF4_atom; break;
		case 0xf5: atom = uF5_atom; break;
		case 0xf6: atom = tunerequest_atom; break;
		case 0xf7: atom = sysex_atom; break;
		case 0xf8: atom = timingclock_atom; break;
		case 0xf9: atom = uF9_atom; break;
		case 0xfa: atom = START_atom; break;
		case 0xfb: atom = CONTINUE_atom; break;
		case 0xfc: atom = STOP_atom; break;
		case 0xfd: atom = uFD_atom; break;
		case 0xfe: atom = activesensing_atom; break;
		case 0xff: atom = systemreset_atom; break;
		}
		break;
	}
	default: atom = income_midi_atom; break;
	}
	PrologElement * el = proot -> pair (proot -> atom (atom), proot -> earth ());
	PrologElement * query = proot -> pair (proot -> atom (income_midi_atom), el);
	el = el -> getRight ();
	if (data [0] < 0xf0) {el -> setPair (proot -> integer (data [0] & 0xf), proot -> earth ()); el = el -> getRight ();}
	for (int ind = 1; ind < size; ind ++) {if (data [ind] < 128) {el -> setPair (proot -> integer (data [ind]), proot -> earth ()); el = el -> getRight ();}}
	query = proot -> pair (query, proot -> earth ());
	query = proot -> pair (proot -> head (0), query);
	proot -> resolution (query);
	delete query;
};

struct midi {
public:
	int time;
	int b1, b2, b3;
	int * sysex;
	int size;
	midi * next;
};

class midi_line {
public:
	midi * root;
	midi * top;
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
	void insert_midi (int b1) {
		midi * md = new midi;
		md -> time = proot -> get_system_time ();
		md -> b1 = b1; md -> size = 1;
		md -> next = 0; md -> sysex = 0;
		if (top == 0) top = root = md; else top = top -> next = md;
	}
	void insert_midi (int b1, int b2) {
		midi * md = new midi;
		md -> time = proot -> get_system_time ();
		md -> b1 = b1; md -> b2 = b2; md -> size = 2;
		md -> next = 0; md -> sysex = 0;
		if (top == 0) top = root = md; else top = top -> next = md;
	};
	void insert_midi (int b1, int b2, int b3) {
		midi * md = new midi;
		md -> time = proot -> get_system_time ();
		md -> b1 = b1; md -> b2 = b2; md -> b3 = b3; md -> size = 3;
		md -> next = 0; md -> sysex = 0;
		if (top == 0) top = root = md; else top = top -> next = md;
	};
	void insert_midi (int * sysex, int size) {
		midi * md = new midi;
		md -> time = proot -> get_system_time ();
		md -> sysex = sysex; md -> size = size;
		md -> next = 0;
		if (top == 0) top = root = md; else top = top -> next = md;
	}
	midi_line (void) {root = 0; top = 0;};
	~ midi_line (void) {erase_line ();};
};

midi_line line1;
midi_line line2;
midi_line * line = & line1;

int global_time = 0;

int Callback (jack_nframes_t nframes, void * args) {
	void * ib = jack_port_get_buffer (jack_midi_in, nframes);
	void * ob = jack_port_get_buffer (jack_midi_out, nframes);
	// CRITICAL //
	pthread_mutex_lock (& mutex);
	int new_time = proot -> get_system_time ();
	int previous_time = global_time;
	global_time = new_time;
	double delta = (double) nframes / (double) (new_time - previous_time);
	midi_line * myline = line;
	line = line == & line1 ? & line2 : & line1;
	pthread_mutex_unlock (& mutex);
	//////////////
	jack_midi_clear_buffer (ob);
	midi * md = myline -> root;
	while (md != 0) {
		int shift = (int) ((double) (md -> time - previous_time) * delta);
		if (shift >= nframes) shift = nframes - 1;
		if (md -> sysex == 0) {
			unsigned char * b = jack_midi_event_reserve (ob, shift , md -> size);
			b [0] = md -> b1;
			if (md -> size > 1) b [1] = md -> b2;
			if (md -> size > 2) b [2] = md -> b3;
		} else {
			unsigned char * b = jack_midi_event_reserve (ob, shift, md -> size);
			for (int ind = 0; ind < md -> size; ind ++) b [ind] = md -> sysex [ind];
		}
		md = md -> next;
	}
	myline -> erase_line ();
	jack_nframes_t events = jack_midi_get_event_count (ib);
	jack_midi_event_t event;
	for (int ind = 0; ind < events; ind ++) {
		jack_midi_event_get (& event, ib, ind);
		prolog_midi_callback (event . size, event . buffer);
		// printf ("midi [%0X ", event . time);
		// for (int sub = 0; sub < event . size; sub ++) printf (" %0X", event . buffer [sub]);
		// printf ("]\n");
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

class keyoff_class : public PrologNativeCode {
public:
	int shift;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * channel = 0;
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) {if (channel == 0) channel = el; else if (key == 0) key = el; else velocity = el;}
			parameters = parameters -> getRight ();
		}
		if (key == 0) {
			if (shift != 0x80) return false;
			// CRITICAL //////////////////////////////////////////////////////////////
			pthread_mutex_lock (& mutex);                                           //
			line -> insert_midi (0xb0 | (channel -> getInteger () & 0xf), 0x7b, 0); //
			pthread_mutex_unlock (& mutex);                                         //
			//////////////////////////////////////////////////////////////////////////
			return true;
		}
		int b1 = (channel -> getInteger () & 0xf) | shift;
		int b2 = key -> getInteger () & 0x7f;
		int b3 = velocity == 0 ? 100 : velocity -> getInteger () & 0xff;
		// CRITICAL ////////////////////////
		pthread_mutex_lock (& mutex);     //
		line -> insert_midi (b1, b2, b3); //
		pthread_mutex_unlock (& mutex);   //
		////////////////////////////////////
		return true;
	};
	keyoff_class (int shift) {this -> shift = shift;};
};

class programchange_class : public PrologNativeCode {
public:
	int shift;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * channel = 0;
		PrologElement * program = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) {if (channel == 0) channel = el; else program = el;}
			parameters = parameters -> getRight ();
		}
		if (program == 0) return false;
		int b1 = (channel -> getInteger () & 0xf) | shift;
		int b2 = program -> getInteger () & 0x7f;
		// CRITICAL //////////////////////
		pthread_mutex_lock (& mutex);   //
		line -> insert_midi (b1, b2);   //
		pthread_mutex_unlock (& mutex); //
		//////////////////////////////////
		return true;
	};
	programchange_class (int shift) {this -> shift = shift;};
};

class pitch_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * channel = 0;
		PrologElement * ll = 0;
		PrologElement * hh = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) {if (channel == 0) channel = el; else if (ll == 0) ll = el; else hh = el;}
			parameters = parameters -> getRight ();
		}
		if (ll == 0) return false;
		int b1 = (channel -> getInteger () & 0xf) | 0xe0;
		int b2 = ll -> getInteger ();
		int b3;
		if (hh == 0) {b3 = b2; b2 = 0;}
		else b3 = hh -> getInteger ();
		if (b3 > 127) b3 = b2 = 127;
		b2 &= 0x7f; b3 &= 0x7f;
		// CRITICAL ////////////////////////
		pthread_mutex_lock (& mutex);     //
		line -> insert_midi (b1, b2, b3); //
		pthread_mutex_unlock (& mutex);   //
		////////////////////////////////////
		return true;
	};
};

class sysex_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * el = parameters;
		int counter = 0;
		while (el -> isPair ()) {PrologElement * ell = el -> getLeft (); if (ell -> isInteger ()) counter ++; el = el -> getRight ();}
		if (counter < 1) return false;
		int * data = new int [counter + 2];
		counter = 1; data [0] = 0xf0;
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (el -> isInteger ()) data [counter ++] = el -> getInteger ();
			parameters = parameters -> getRight ();
		}
		data [counter ++] = 0xf7;
		// CRITICAL ///////////////////////////
		pthread_mutex_lock (& mutex);        //
		line -> insert_midi (data, counter); //
		pthread_mutex_unlock (& mutex);      //
		///////////////////////////////////////
		return true;
	};
};

class realtime_class_0 : public PrologNativeCode {
public:
	int msg;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		// CRITICAL ////////////////////////
		pthread_mutex_lock (& mutex);     //
		line -> insert_midi (msg);        //
		pthread_mutex_unlock (& mutex);   //
		////////////////////////////////////
		return true;
	};
	realtime_class_0 (int msg) {this -> msg = msg;};
};

class realtime_class_1 : public PrologNativeCode {
public:
	int msg;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft (); if (! parameters -> isInteger ()) return false;
		int dd = parameters -> getInteger () & 0x7f;
		// CRITICAL ////////////////////////
		pthread_mutex_lock (& mutex);     //
		line -> insert_midi (msg, dd);    //
		pthread_mutex_unlock (& mutex);   //
		////////////////////////////////////
		return true;
	};
	realtime_class_1 (int msg) {this -> msg = msg;};
};

class realtime_class_2 : public PrologNativeCode {
public:
	int msg;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * ll = 0, * hh = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) {if (ll == 0) ll = el; else hh = el;}
			parameters = parameters -> getRight ();
		}
		if (hh == 0) return false;
		int lll = ll -> getInteger () & 0x7f;
		int hhh = hh -> getInteger () & 0x7f;
		// CRITICAL ///////////////////////////
		pthread_mutex_lock (& mutex);        //
		line -> insert_midi (msg, lll, hhh); //
		pthread_mutex_unlock (& mutex);      //
		///////////////////////////////////////
		return true;
	};
	realtime_class_2 (int msg) {this -> msg = msg;};
};

class jack_service : public PrologServiceClass {
public:
	void init (PrologRoot * root, PrologDirectory * directory) {
		proot = root;
		prolog_directory = directory;
		income_midi_atom = directory -> searchAtom ("income_midi");
		keyoff_atom = directory -> searchAtom ("keyoff");
		keyon_atom = directory -> searchAtom ("keyon");
		polyaftertouch_atom = directory -> searchAtom ("polyaftertouch");
		control_atom = directory -> searchAtom ("control");
		programchange_atom = directory -> searchAtom ("programchange");
		aftertouch_atom = directory -> searchAtom ("aftertouch");
		pitch_atom = directory -> searchAtom ("pitch");
		sysex_atom = directory -> searchAtom ("sysex");
		quarterframe_atom = directory -> searchAtom ("quarterframe");
		songpositionpointer_atom = directory -> searchAtom ("songpositionpointer");
		songselect_atom = directory -> searchAtom ("songselect");
		uF4_atom = directory -> searchAtom ("uF4");
		uF5_atom = directory -> searchAtom ("uF5");
		tunerequest_atom = directory -> searchAtom ("tunerequest");
		timingclock_atom = directory -> searchAtom ("timingclock");
		uF9_atom = directory -> searchAtom ("uF9");
		START_atom = directory -> searchAtom ("START");
		CONTINUE_atom = directory -> searchAtom ("CONTINUE");
		STOP_atom = directory -> searchAtom ("STOP");
		uFD_atom = directory -> searchAtom ("uFD");
		activesensing_atom = directory -> searchAtom ("activesensing");
		systemreset_atom = directory -> searchAtom ("systemreset");
		PrologString * args = root -> args;
		while (args != 0) {printf ("ARGS [%s]\n", args -> text); args = args -> next;}
//		char * * port_names = (char * *) jack_get_ports (jack_client, 0, 0, 0);
//		while (* port_names != 0) {printf ("PORTS [%s]\n", * port_names); port_names ++;}
//		printf ("callback registering [%i]\n", jack_set_process_callback (jack_client, Callback, 0));
//		printf ("activate [%i]\n", jack_activate (jack_client));
		global_time = root -> get_system_time ();
	}
	PrologNativeCode * getNativeCode (char * name) {
		if (strcmp (name, "activate") == 0) return new activate_class ();
		if (strcmp (name, "keyoff") == 0) return new keyoff_class (0x80);
		if (strcmp (name, "keyon") == 0) return new keyoff_class (0x90);
		if (strcmp (name, "polyaftertouch") == 0) return new keyoff_class (0xa0);
		if (strcmp (name, "control") == 0) return new keyoff_class (0xb0);
		if (strcmp (name, "programchange") == 0) return new programchange_class (0xc0);
		if (strcmp (name, "aftertouch") == 0) return new programchange_class (0xd0);
		if (strcmp (name, "pitch") == 0) return new pitch_class ();
		if (strcmp (name, "sysex") == 0) return new sysex_class ();
		if (strcmp (name, "quarterframe") == 0) return new realtime_class_1 (0xf1);
		if (strcmp (name, "songpositionpointer") == 0) return new realtime_class_2 (0xf2);
		if (strcmp (name, "songselect") == 0) return new realtime_class_1 (0xf3);
		if (strcmp (name, "uF4") == 0) return new realtime_class_0 (0xf4);
		if (strcmp (name, "uF5") == 0) return new realtime_class_0 (0xf5);
		if (strcmp (name, "tunerequest") == 0) return new realtime_class_0 (0xf6);
		if (strcmp (name, "timingclock") == 0) return new realtime_class_0 (0xf8);
		if (strcmp (name, "uF9") == 0) return new realtime_class_0 (0xf9);
		if (strcmp (name, "START") == 0) return new realtime_class_0 (0xfa);
		if (strcmp (name, "CONTINUE") == 0) return new realtime_class_0 (0xfb);
		if (strcmp (name, "STOP") == 0) return new realtime_class_0 (0xfc);
		if (strcmp (name, "uFD") == 0) return new realtime_class_0 (0xfd);
		if (strcmp (name, "activesensing") == 0) return new realtime_class_0 (0xfe);
		if (strcmp (name, "systemreset") == 0) return new realtime_class_0 (0xff);
		return 0;
	}
	jack_service (void) {
		mutex = PTHREAD_MUTEX_INITIALIZER;
	}
	~ jack_service (void) {
		deactivate ();
		pthread_mutex_destroy (& mutex);
	}
};

PrologServiceClass * create_service_class (void) {return new jack_service ();}

