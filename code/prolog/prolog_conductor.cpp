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

#include "prolog_conductor.h"
#include <string.h>

///////////////
// TRANSPORT //
///////////////

static bool wt_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) return transport -> wait (1);
	if (parameters -> isVar ()) {parameters -> setInteger (transport -> getTick ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (parameters -> isVar ()) {parameters -> setInteger (transport -> getTick ()); return true;}
	if (! parameters -> isInteger ()) return false;
	return transport -> wait (parameters -> getInteger ());
}

static bool beat_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) return t -> waitBeat (1);
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBeat ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBeat ()); return true;}
	if (! parameters -> isInteger ()) {
		if (! parameters -> isDouble ()) return false;
		return t -> waitBeat (parameters -> getDouble ());
	}
	return t -> waitBeat (parameters -> getInteger ());
}

static bool bar_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) return t -> waitBar (1);
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBar ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (parameters -> isVar ()) {parameters -> setInteger (t -> getBar ()); return true;}
	if (! parameters -> isInteger ()) return false;
	return t -> waitBar (parameters -> getInteger ());
}

static bool signal_tick_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) {transport -> signal (); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	int sentinel = parameters -> getInteger ();
	for (int ind = 0; ind < sentinel; ind++) transport -> signal ();
	return true;
}

static bool signal_beat_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) {transport -> signalBeat (); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	int sentinel = parameters -> getInteger ();
	for (int ind = 0; ind < sentinel; ind++) transport -> signalBeat ();
	return true;
}

static bool signal_bar_function (PrologElement * parameters, PrologTransport * transport) {
	if (parameters -> isEarth ()) {transport -> signalBar (); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	int sentinel = parameters -> getInteger ();
	for (int ind = 0; ind < sentinel; ind++) transport -> signalBar ();
	return true;
}

static bool tempo_function (PrologElement * parameters, PrologTransport * transport) {
	PrologElement * right = 0;
	if (parameters -> isPair ()) {right = parameters -> getRight (); parameters = parameters -> getLeft ();}
	if (parameters -> isVar ()) {
		if (right != 0 && right -> isPair ()) {
			right -> getLeft () -> setInteger (transport -> getBeatSeconds ());
			parameters -> setInteger (transport -> getBeatsPerSeconds ());
		}
		else parameters -> setDouble (transport -> getBeatsPerMinute ());
		return true;
	}
	if (parameters -> isInteger ()) {
		if (right != 0 && right -> isPair () && right -> getLeft () -> isInteger ()) {
			transport -> tempo (parameters -> getInteger (), right -> getLeft () -> getInteger ());
		}
		else transport -> tempo ((double) parameters -> getInteger ());
		return true;
	}
	if (parameters -> isDouble ()) {transport -> tempo (parameters -> getDouble ()); return true;}
	return false;
}

static bool tempo_division_function (PrologElement * parameters, PrologTransport * t) {
	if (! parameters -> isPair ()) return false;
	PrologElement * beats = parameters -> getLeft ();
	if (beats -> isVar ()) {
		beats -> setInteger (t -> getBeatsPerBar ());
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) return true;
		parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		parameters -> setInteger (t -> getTicksPerBeat ());
		return true;
	}
	if (! beats -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> division (beats -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	t -> division (beats -> getInteger (), parameters -> getInteger ());
	return true;
}

static bool atempo_function (PrologTransport * t) {t -> atempo (); return true;}

static bool accel_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) {t -> accelerando (); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * steps = parameters -> getLeft ();
	if (! steps -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> accelerando (steps -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * ticks = parameters -> getLeft ();
	if (! ticks -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> accelerando (steps -> getInteger (), ticks -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * sentinel = parameters -> getLeft ();
	if (! sentinel -> isInteger ()) return false;
	t -> accelerando (steps -> getInteger (), ticks -> getInteger (), sentinel -> getInteger ());
	return true;
}

static bool rit_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isEarth ()) {t -> ritardando (); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * steps = parameters -> getLeft ();
	if (! steps -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> ritardando (steps -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * ticks = parameters -> getLeft ();
	if (! ticks -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (parameters -> isEarth ()) {t -> ritardando (steps -> getInteger (), ticks -> getInteger ()); return true;}
	if (! parameters -> isPair ()) return false;
	PrologElement * sentinel = parameters -> getLeft ();
	if (! sentinel -> isInteger ()) return false;
	t -> ritardando (steps -> getInteger (), ticks -> getInteger (), sentinel -> getInteger ());
	return true;
}

static bool metrum_function (PrologElement * parameters, PrologTransport * t) {
	if (! parameters -> isPair ()) return false;
	PrologElement * top = parameters -> getLeft ();
	if (top -> isVar ()) {
		top -> setInteger (t -> getBeatsPerBar ());
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		int div = t -> getTicksPerBeat ();
		if (div != 0) div = 96 / div;
		parameters -> setInteger (div);
		return true;
	}
	if (! top -> isInteger ()) return false;
	parameters = parameters -> getRight ();
	if (! parameters -> isPair ()) return false;
	parameters = parameters -> getLeft ();
	if (! parameters -> isInteger ()) return false;
	t -> metrum (top -> getInteger (), parameters -> getInteger ());
	return true;
}

static bool reset_function (PrologElement * parameters, PrologTransport * t) {
	if (parameters -> isPair ()) {
		PrologElement * beats = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) {
			if (! beats -> isInteger ()) return false;
			PrologElement * ticks = parameters -> getLeft ();
			if (! ticks -> isInteger ()) return false;
			t -> reset (beats -> getInteger (), ticks -> getInteger ());
			return true;
		}
		if (beats -> isDouble ()) {t -> reset (beats -> getDouble ()); return true;}
		if (beats -> isInteger ()) {t -> reset (beats -> getInteger ()); return true;}
		return false;
	}
	t -> reset ();
	return true;
};

class start : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return t -> start ();}
	start (PrologTransport * t) {this -> t = t;}
};

class pause_class : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return t -> pause ();}
	pause_class (PrologTransport * t) {this -> t = t;}
};

class stop : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return t -> stop ();}
	stop (PrologTransport * t) {this -> t = t;}
};

class wt : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return wt_function (parameters, t);}
	wt (PrologTransport * t) {this -> t = t;}
};

class beat : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return beat_function (parameters, t);}
	beat (PrologTransport * t) {this -> t = t;}
};

class bar : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return bar_function (parameters, t);}
	bar (PrologTransport * t) {this -> t = t;}
};

class signal_tick : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return signal_tick_function (parameters, t);}
	signal_tick (PrologTransport * t) {this -> t = t;}
};

class signal_beat : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return signal_beat_function (parameters, t);}
	signal_beat (PrologTransport * t) {this -> t = t;}
};

class signal_bar : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return signal_bar_function (parameters, t);}
	signal_bar (PrologTransport * t) {this -> t = t;}
};

class reset : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * reoslution) {return reset_function (parameters, t);}
	reset (PrologTransport * t) {this -> t = t;}
};

class tempo : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return tempo_function (parameters, t);}
	tempo (PrologTransport * t) {this -> t = t;}
};

class tempo_division : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return tempo_division_function (parameters, t);}
	tempo_division (PrologTransport * t) {this -> t = t;}
};

class atempo : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return atempo_function (t);}
	atempo (PrologTransport * t) {this -> t = t;}
};

class accel : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return accel_function (parameters, t);}
	accel (PrologTransport * t) {this -> t = t;}
};

class rit : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return rit_function (parameters, t);}
	rit (PrologTransport * t) {this -> t = t;}
};

class metrum : public PrologNativeCode {
public:
	PrologTransport * t;
	bool code (PrologElement * parameters, PrologResolution * resolution) {return metrum_function (parameters, t);}
	metrum (PrologTransport * t) {this -> t = t;}
};

///////////////
// CONDUCTOR //
///////////////

class conductor : public PrologNativeCode {
public:
	PrologAtom * atom;
	PrologTransport transport;
	PrologAtom * wt_atom;
	PrologAtom * beat_atom;
	PrologAtom * bar_atom;
	PrologAtom * signal_atom;
	PrologAtom * signal_beat_atom;
	PrologAtom * signal_bar_atom;
	PrologAtom * reset_atom;
	PrologAtom * tempo_atom;
	PrologAtom * atempo_atom;
	PrologAtom * accel_atom;
	PrologAtom * rit_atom;
	PrologAtom * metrum_atom;
	PrologAtom * division_atom;
	PrologAtom * start_atom;
	PrologAtom * pause_atom;
	PrologAtom * stop_atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> setMachine (0);
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * atom = parameters -> getLeft ();
		if (! atom -> isAtom ()) return false;
		PrologAtom * a = atom -> getAtom ();
		PrologElement * original = parameters;
		parameters = parameters -> getRight ();
		if (a == start_atom) return transport . start ();
		if (a == pause_atom) return transport . pause ();
		if (a == stop_atom) return transport . stop ();
		if (a == reset_atom) return reset_function (parameters, & transport);
		if (a == signal_atom) return signal_tick_function (parameters, & transport);
		if (a == signal_beat_atom) return signal_beat_function (parameters, & transport);
		if (a == signal_bar_atom) return signal_bar_function (parameters, & transport);
		if (a == wt_atom) return wt_function (parameters, & transport);
		if (a == beat_atom) return beat_function (parameters, & transport);
		if (a == bar_atom) return bar_function (parameters, & transport);
		if (a == tempo_atom) return tempo_function (parameters, & transport);
		if (a == atempo_atom) return atempo_function (& transport);
		if (a == accel_atom) return accel_function (parameters, & transport);
		if (a == rit_atom) return rit_function (parameters, & transport);
		if (a == division_atom) return tempo_division_function (parameters, & transport);
		if (a == metrum_atom) return metrum_function (parameters, & transport);
		return false;
	}
	conductor (PrologAtom * atom, PrologAtom * wt_atom, PrologAtom * beat_atom, PrologAtom * bar_atom, PrologAtom * signal_atom, PrologAtom * signal_beat_atom, PrologAtom * signal_bar_atom, PrologAtom * reset_atom, PrologAtom * tempo_atom, PrologAtom * atempo_atom, PrologAtom * accel_atom, PrologAtom * rit_atom, PrologAtom * metrum_atom, PrologAtom * division_atom, PrologAtom * start_atom, PrologAtom * pause_atom, PrologAtom * stop_atom) {
		this -> atom = atom;
		this -> wt_atom = wt_atom;
		this -> beat_atom = beat_atom;
		this -> bar_atom = bar_atom;
		this -> signal_atom = signal_atom;
		this -> signal_beat_atom = signal_beat_atom;
		this -> signal_bar_atom = signal_bar_atom;
		this -> reset_atom = reset_atom;
		this -> tempo_atom = tempo_atom;
		this -> atempo_atom = atempo_atom;
		this -> accel_atom = accel_atom;
		this -> rit_atom = rit_atom;
		this -> metrum_atom = metrum_atom;
		this -> division_atom = division_atom;
		this -> start_atom = start_atom;
		this -> pause_atom = pause_atom;
		this -> stop_atom = stop_atom;
	}
};

class conductor_maker : public PrologNativeCode {
public:
	PrologAtom * wt_atom;
	PrologAtom * beat_atom;
	PrologAtom * bar_atom;
	PrologAtom * signal_atom;
	PrologAtom * signal_beat_atom;
	PrologAtom * signal_bar_atom;
	PrologAtom * reset_atom;
	PrologAtom * tempo_atom;
	PrologAtom * atempo_atom;
	PrologAtom * accel_atom;
	PrologAtom * rit_atom;
	PrologAtom * metrum_atom;
	PrologAtom * division_atom;
	PrologAtom * start_atom;
	PrologAtom * pause_atom;
	PrologAtom * stop_atom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (wt_atom == NULL || beat_atom == NULL || bar_atom == NULL
			|| signal_atom == NULL || signal_beat_atom == NULL || signal_bar_atom == NULL
			|| reset_atom == NULL || tempo_atom == NULL || atempo_atom == NULL
			|| accel_atom == NULL || rit_atom == NULL
			|| metrum_atom == NULL || division_atom == NULL
			|| start_atom == NULL || pause_atom == NULL || stop_atom == NULL) return false;
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (ea -> isVar ()) {
			atom = new PrologAtom ();
			ea -> setAtom (atom);
		} else {
			if (! ea -> isAtom ()) return false;
			atom = ea -> getAtom ();
		}
		if (atom -> getMachine () != 0) return false;
		conductor * c = new conductor (atom, wt_atom, beat_atom, bar_atom, signal_atom, signal_beat_atom, signal_bar_atom, reset_atom, tempo_atom, atempo_atom, accel_atom, rit_atom, metrum_atom, division_atom, start_atom, pause_atom, stop_atom);
		if (atom -> setMachine (c)) return true;
		delete c;
		return false;
	}
	conductor_maker (PrologDirectory * dir) {
		wt_atom = beat_atom = bar_atom = signal_atom = signal_beat_atom = signal_bar_atom = reset_atom = tempo_atom = atempo_atom
		= accel_atom = rit_atom = metrum_atom = division_atom = start_atom = pause_atom = stop_atom = 0;
		if (dir == 0) return;
		wt_atom = dir -> searchAtom ("wt");
		beat_atom = dir -> searchAtom ("beat");
		bar_atom = dir -> searchAtom ("bar");
		signal_atom = dir -> searchAtom ("signal_tick");
		signal_beat_atom = dir -> searchAtom ("signal_beat");
		signal_bar_atom = dir -> searchAtom ("signal_bar");
		reset_atom = dir -> searchAtom ("reset");
		tempo_atom = dir -> searchAtom ("tempo");
		atempo_atom = dir -> searchAtom ("atempo");
		accel_atom = dir -> searchAtom ("accel");
		rit_atom = dir -> searchAtom ("rit");
		metrum_atom = dir -> searchAtom ("metrum");
		division_atom = dir -> searchAtom ("division");
		start_atom = dir -> searchAtom ("start");
		pause_atom = dir -> searchAtom ("pause");
		stop_atom = dir -> searchAtom ("stop");
	}
};

PrologNativeCode * PrologConductorServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "start") == 0) return new start (& t);
	if (strcmp (name, "pause") == 0) return new pause_class (& t);
	if (strcmp (name, "stop") == 0) return new stop (& t);
	if (strcmp (name, "wt") == 0) return new wt (& t);
	if (strcmp (name, "beat") == 0) return new beat (& t);
	if (strcmp (name, "bar") == 0) return new bar (& t);
	if (strcmp (name, "signal_tick") == 0) return new signal_tick (& t);
	if (strcmp (name, "signal_beat") == 0) return new signal_beat (& t);
	if (strcmp (name, "signal_bar") == 0) return new signal_bar (& t);
	if (strcmp (name, "reset") == 0) return new reset (& t);
	if (strcmp (name, "tempo") == 0) return new tempo (& t);
	if (strcmp (name, "atempo") == 0) return new atempo (& t);
	if (strcmp (name, "accel") == 0) return new accel (& t);
	if (strcmp (name, "rit") == 0) return new rit (& t);
	if (strcmp (name, "tempo_division") == 0) return new tempo_division (& t);
	if (strcmp (name, "metrum") == 0) return new metrum (& t);
	if (strcmp (name, "conductor") == 0) return new conductor_maker (directory);
	return NULL;
}

void PrologConductorServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {this -> directory = directory;}
PrologConductorServiceClass :: PrologConductorServiceClass (void) {this -> directory = NULL;}
PrologConductorServiceClass :: ~ PrologConductorServiceClass (void) {}

