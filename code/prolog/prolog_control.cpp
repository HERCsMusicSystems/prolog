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
	if (callback != 0) {
		COLLECTOR_REFERENCE_INC (callback);
	}
	this -> freq = freq;
	delay = -1.0;
	if (freq < 0.0) return;
	if (callback == 0) return;
	#ifdef LINUX_OPERATING_SYSTEM
	delay = freq > 0.0 ? 1000000.0 / freq : 0.0;
	#endif
	#ifdef WINDOWS_OPERATING_SYSTEM
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
		joystick_code * jc = new joystick_code (path != 0 ? path -> getText () : 0, root, atom -> getAtom (), callback != 0 ? callback -> getAtom () : 0, freq);
		if (atom -> getAtom () -> setMachine (jc)) return true;
		delete jc;
		return false;
	}
	create_joystick (PrologRoot * root) {this -> root = root;}
};

void PrologControlServiceClass :: init (PrologRoot * root) {this -> root = root;}

PrologNativeCode * PrologControlServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "joystick") == 0) return new create_joystick (root);
	return 0;
}
