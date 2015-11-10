///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2014 Robert P. Wolf                       //
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

#include "prolog_control.h"
#include "pthread.h"

#ifdef WINDOWS_OPERATING_SYSTEM
#include "joystick_windows.h"
static void usleep (int delay) {Sleep (delay);}
#endif
#ifdef LINUX_OPERATING_SYSTEM
#include "joystick_linux.h"
#include <string.h>
#include <unistd.h>
#endif

#include <fcntl.h>

/*********************************************************

[joystick "dev/js0" js cb 0.0] => opens joystick with ultra callback
	"dev/js0" => default, can be omitted
	0.0 => continuous reading
	-1.0 => no auto callback
	cb => causes auto callback with default 0.0
[js] => closes joystick

*********************************************************/

class prolog_joystick : public joystick {
public:
	PrologRoot * root;
	PrologAtom * callback;
	pthread_t thread;
	double freq;
	double delay;
	bool should_continue;
	void call_axis (int ind, double value) {
		PrologElement * query = root -> pair (root -> atom (callback),
							root -> pair (root -> Double ((double) ind),
							root -> pair (root -> Double (value),
							root -> earth ())));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void call_button (int ind, bool value) {
		PrologElement * query = root -> pair (root -> atom (callback),
							root -> pair (root -> integer (ind),
							root -> pair (root -> integer (value ? 1 : 0),
							root -> earth ())));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
public:
	void axis (int ind, double value) {if (callback != 0) call_axis (ind, value);}
	void button (int ind, bool value) {if (callback != 0) call_button (ind, value);}
	prolog_joystick (char * path, PrologRoot * root, PrologAtom * callback, double freq);
	~ prolog_joystick (void);
};

static void * joystick_runner (void * parameters) {
	prolog_joystick * js = (prolog_joystick *) parameters;
	js -> should_continue = true;
	while (js -> should_continue) {
		js -> move (js -> delay);
	}
	js -> should_continue = true;
	return 0;
}

prolog_joystick :: prolog_joystick (char * path, PrologRoot * root, PrologAtom * callback, double freq) : joystick (path) {
	should_continue = false;
	this -> root = root;
	this -> callback = callback;
	if (callback != 0) {COLLECTOR_REFERENCE_INC (callback);}
	this -> freq = freq;
	delay = -1.0;
	if (freq < 0.0) return;
	if (callback == 0) return;
	#ifdef LINUX_OPERATING_SYSTEM
	delay = freq > 0.0 ? 1000000.0 / freq : 0.0;
	if (fd < 0) return;
	#endif
	#ifdef WINDOWS_OPERATING_SYSTEM
	if (joystick_not_found ()) return;
	delay = freq > 0.0 ? 1000.0 / freq : 0.0;
	#endif
	pthread_create (& thread, 0, joystick_runner, this);
	pthread_detach (thread);
}

prolog_joystick :: ~ prolog_joystick (void) {
	if (should_continue) {
		should_continue = false;
		while (! should_continue) usleep ((int) delay);
		should_continue = false;
	}
	if (callback != 0) callback -> removeAtom (); callback = 0;
}

class serial_code : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * callback;
	pthread_t thread;
	bool should_continue;
#ifdef WIN32
	HANDLE fd;
#else
	int fd;
#endif
	bool code (PrologElement * parameters, PrologResolution * resolution);
	bool port_not_found (void);
	serial_code (char * location, PrologRoot * root, PrologAtom * atom, PrologAtom * callback);
	~ serial_code (void);
};

#ifdef WIN32
static int tmread (HANDLE fd) {
	int ind = 0;
	ReadFile (fd, & ind, 1, NULL, NULL);
	return ind;
}
#else
static int tmread (int fd) {
	fd_set readset;
	FD_ZERO (& readset);
	FD_SET (fd, & readset);
	timeval timeout;
	timeout . tv_sec = 1;
	timeout . tv_usec = 0;
	if (select (fd + 1, & readset, 0, 0, & timeout) > 0) {int v = 0; read (fd, & v, 1); return v;}
	return -1;
}
#endif

static void * serial_runner (void * parameters) {
	serial_code * code = (serial_code *) parameters;
	PrologRoot * root = code -> root;
	code -> should_continue = true;
	while (code -> should_continue) {
		int ind = tmread (code -> fd);
		if (ind >= 0) {
			PrologElement * clause = root -> pair (root -> atom (code -> callback), root -> pair (root -> integer (ind), root -> earth ()));
			clause = root -> pair (root -> head (0), root -> pair (clause, root -> earth ()));
			root -> resolution (clause);
			delete clause;
		}
	}
	code -> should_continue = true;
	return 0;
}

bool serial_code :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isInteger ()) {
			int ind = el -> getInteger () & 0xff;
#ifdef WIN32
			DWORD written;
			WriteFile (fd, & ind, 1, & written, NULL);
#else
			write (fd, & ind, 1);
#endif
		}
		if (el -> isText ()) {
			char * text = el -> getText ();
#ifdef WIN32
			DWORD written;
			WriteFile (fd, text, strlen (text), & written, NULL);
#else
			write (fd, text, strlen (text));
#endif
		}
		if (el -> isVar ()) {
			int ind = tmread (fd);
			el -> setInteger (ind);
		}
		parameters = parameters -> getRight ();
	}
	return true;
}
bool serial_code :: port_not_found (void) {
#ifdef WIN32
	return fd == INVALID_HANDLE_VALUE;
#else
	return fd < 0;
#endif
}
serial_code :: serial_code (char * location, PrologRoot * root, PrologAtom * atom, PrologAtom * callback) {
	this -> should_continue = false;
	this -> root = root;
	this -> atom = atom;
	this -> callback = callback;
	if (callback != 0) {COLLECTOR_REFERENCE_INC (callback);}
#ifdef WIN32
	fd = CreateFile (location, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#else
	fd = open (location, O_RDWR | O_NONBLOCK);
#endif
	if (callback == 0) return;
	if (port_not_found ()) return;
	pthread_create (& thread, 0, serial_runner, this);
	pthread_detach (thread);
}
serial_code :: ~ serial_code (void) {
	if (should_continue) {should_continue = false; while (! should_continue) usleep (100);}
	if (callback != 0) callback -> removeAtom (); callback = 0;
	if (! port_not_found ()) {
#ifdef WIN32
		CloseHandle (fd);
#else
		close (fd);
#endif
	}
}

class joystick_code : public PrologNativeCode {
public:
	prolog_joystick js;
	PrologAtom * atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> setMachine (0);
			delete this;
			return true;
		}
		if (js . delay < 0.0) {
			js . move ();
		}
		if (parameters -> isVar ()) {
			parameters -> setPair ();
			PrologElement * axes = parameters -> getLeft ();
			axes -> setPair (); axes -> getLeft () -> setDouble (js . x); axes = axes -> getRight ();
			axes -> setPair (); axes -> getLeft () -> setDouble (js . y); axes = axes -> getRight ();
			axes -> setPair (); axes -> getLeft () -> setDouble (js . z); axes = axes -> getRight ();
			axes -> setPair (); axes -> getLeft () -> setDouble (js . throttle); axes = axes -> getRight ();
			axes -> setPair (); axes -> getLeft () -> setDouble (js . hx); axes = axes -> getRight ();
			axes -> setPair (); axes -> getLeft () -> setDouble (js . hy);
			PrologElement * buttons = parameters -> getRight ();
			buttons -> setPair (); buttons = buttons -> getLeft ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . fire != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . autofire != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . h1 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . h2 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . h3 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . h4 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . e1 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . e2 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . e3 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . e4 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . e5 != 0); buttons = buttons -> getRight ();
			buttons -> setPair (); buttons -> getLeft () -> setInteger (js . e6 != 0);
		}
		return true;
	}
	joystick_code (char * path, PrologRoot * root, PrologAtom * atom, PrologAtom * callback, double freq) : js (path, root, callback, freq) {
		this -> atom = atom;
	}
};

class create_joystick : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * path = 0;
		PrologElement * atom = 0;
		PrologElement * callback = 0;
		double freq = 0.0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) path = el;
			if (el -> isVar ()) el -> setAtom (new PrologAtom ());
			if (el -> isAtom ()) {
				if (atom == 0) atom = el;
				else callback = el;
			}
			if (el -> isInteger ()) freq = (double) el -> getInteger ();
			if (el -> isDouble ()) freq = el -> getDouble ();
			parameters = parameters -> getRight ();
		}
		if (atom == 0) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		char * joystick_location;
		if (path != 0) joystick_location = path -> getText ();
		else joystick_location = "/dev/input/js0";
		joystick_code * jc = new joystick_code (joystick_location, root, atom -> getAtom (), callback != 0 ? callback -> getAtom () : 0, freq);
		if (jc -> js . joystick_not_found ()) {delete jc; return false;}
		if (atom -> getAtom () -> setMachine (jc)) return true;
		delete jc;
		return false;
	}
	create_joystick (PrologRoot * root) {this -> root = root;}
};

class create_serial : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * path = 0;
		PrologElement * atom = 0;
		PrologElement * callback = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) path = el;
			if (el -> isVar ()) el -> setAtom (new PrologAtom ());
			if (el -> isAtom ()) {if (atom == 0) atom = el; else callback = el;}
			parameters = parameters -> getRight ();
		}
		if (atom == 0) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		char * serial_location;
		if (path != 0) serial_location = path -> getText ();
		else serial_location = "/dev/ttyACM0";
		serial_code * sc = new serial_code (serial_location, root, atom -> getAtom (), callback != 0 ? callback -> getAtom () : 0);
		if (sc -> port_not_found ()) {delete sc; return false;}
		if (atom -> getAtom () -> setMachine (sc)) return true;
		delete sc;
		return false;
	}
	create_serial (PrologRoot * root) {this -> root = root;}
};

void PrologControlServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {this -> root = root;}

PrologNativeCode * PrologControlServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "joystick") == 0) return new create_joystick (root);
	if (strcmp (name, "serial") == 0) return new create_serial (root);
	return 0;
}

