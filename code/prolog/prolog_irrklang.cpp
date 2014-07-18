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

#include "prolog_irrklang.h"

class irrklang_control_class : public PrologNativeCode {
public:
	PrologAtom * atom;
	PrologAtom * irrpause, * irrplay, * irrstop;
	irrklang :: ISound * ctrl;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			ctrl -> stop ();
			atom -> setMachine (0);
			delete this;
			return true;
		}
		PrologElement * command = 0;
		PrologElement * volume = 0;
		PrologElement * frequency = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) command = el;
			if (el -> isDouble ()) {
				if (volume == 0) volume = el;
				else frequency = el;
			}
			parameters = parameters -> getRight ();
		}
		if (volume != 0) ctrl -> setVolume ((irrklang :: ik_f32) volume -> getDouble ());
		if (frequency != 0) ctrl -> setPlaybackSpeed ((irrklang :: ik_f32) frequency -> getDouble ());
		if (command != 0) {
			PrologAtom * atom = command -> getAtom ();
			if (atom == irrpause) ctrl -> setIsPaused (true);
			if (atom == irrplay) ctrl -> setIsPaused (false);
			if (atom == irrstop) ctrl -> stop ();
		}
		return true;
	}
	irrklang_control_class (PrologDirectory * dir, PrologAtom * atom, irrklang :: ISound * ctrl) {
		this -> atom = atom;
		this -> ctrl = ctrl;
		irrpause = irrplay = irrstop = 0;
		if (dir != 0) {
			irrpause = dir -> searchAtom ("irrpause");
			irrplay = dir -> searchAtom ("irrplay");
			irrstop = dir -> searchAtom ("irrstop");
		}
	}
	~ irrklang_control_class (void) {if (ctrl != 0) ctrl -> drop ();}
};

class irrklang_source_class : public PrologNativeCode {
public:
	PrologDirectory * dir;
	PrologAtom * irrshot, * irrloop;
	PrologAtom * atom;
	irrklang :: ISoundEngine * irrklang_engine;
	irrklang :: ISoundSource * irrklang_source;
	double default_frequency;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			atom -> setMachine (0);
			delete this;
			return true;
		}
		PrologElement * command = 0;
		PrologElement * ctrl = 0;
		PrologElement * volume = 0;
		PrologElement * frequency = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isVar ()) {el -> setAtom (new PrologAtom ()); ctrl = el;}
			if (el -> isAtom ()) {
				if (command == 0) command = el;
				else ctrl = el;
				if (ctrl != 0) {
					PrologAtom * atom = ctrl -> getAtom ();
					if (atom == irrshot || atom == irrloop) {PrologElement * ex = ctrl; ctrl = command; command = ex;}
				}
			}
			if (el -> isDouble ()) {
				if (volume == 0) volume = el;
				else frequency = el;
			}
			parameters = parameters -> getRight ();
		}
		if (frequency != 0) default_frequency = frequency -> getDouble ();
		if (volume != 0) irrklang_source -> setDefaultVolume ((irrklang :: ik_f32) volume -> getDouble ());
		irrklang :: ISound * sound_control = 0;
		if (command != 0) {
			PrologAtom * atom = command -> getAtom ();
			if (atom == irrshot) {
				sound_control = irrklang_engine -> play2D (irrklang_source, false, true);
			}
			if (atom == irrloop) {
				sound_control = irrklang_engine -> play2D (irrklang_source, true, true);
			}
		}
		if (sound_control != 0) {
			if (ctrl != 0) {
				if (ctrl -> getAtom () -> getMachine () != 0) return false;
				irrklang_control_class * icc = new irrklang_control_class (dir, ctrl -> getAtom (), sound_control);
				if (! ctrl -> getAtom () -> setMachine (icc)) {delete icc; return false;}
			}
			if (default_frequency != 1.0) sound_control -> setPlaybackSpeed ((irrklang :: ik_f32) default_frequency);
			sound_control -> setIsPaused (false);
		}
		return true;
	}
	irrklang_source_class (PrologDirectory * dir, PrologAtom * atom, irrklang :: ISoundEngine * irrklang_engine, irrklang :: ISoundSource * irrklang_source) {
		this -> dir = dir;
		this -> atom = atom;
		this -> irrklang_engine = irrklang_engine;
		this -> irrklang_source = irrklang_source;
		irrshot = irrloop = 0;
		default_frequency = 1.0;
		if (dir != 0) {
			irrshot = dir -> searchAtom ("irrshot");
			irrloop = dir -> searchAtom ("irrloop");
		}
	}
};

class irrklang_class : public PrologNativeCode {
public:
	PrologDirectory * dir;
	irrklang :: ISoundEngine * irrklang_engine;
	irrklang :: ISoundSource * irrklang_source;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * atom = 0;
		PrologElement * path = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) atom = el;
			if (el -> isText ()) path = el;
			parameters = parameters -> getRight ();
		}
		if (irrklang_engine == 0) return false;
		if (atom == 0) return false;
		if (path == 0) return false;
		irrklang_source = irrklang_engine -> getSoundSource (path -> getText ());
		if (irrklang_source == 0) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		irrklang_source_class * source = new irrklang_source_class (dir, atom -> getAtom (), irrklang_engine, irrklang_source);
		if (atom -> getAtom () -> setMachine (source)) return true;
		delete source;
		return false;
	}
	irrklang_class (PrologDirectory * dir, irrklang :: ISoundEngine * irrklang_engine) {this -> dir = dir; this -> irrklang_engine = irrklang_engine; irrklang_source = 0;}
};

void PrologIrrKlangServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
	this -> dir = directory;
}

PrologNativeCode * PrologIrrKlangServiceClass :: getNativeCode (char * name) {
	if (dir == 0) return 0;
	if (strcmp (name, "irrklang") == 0) return new irrklang_class (dir, irrklang_engine);
	return 0;
}

PrologIrrKlangServiceClass :: PrologIrrKlangServiceClass (void) {
	root = 0;
	dir = 0;
	irrklang_engine = irrklang :: createIrrKlangDevice ();
}

PrologIrrKlangServiceClass :: ~ PrologIrrKlangServiceClass (void) {
	irrklang_engine -> stopAllSounds ();
	irrklang_engine -> removeAllSoundSources ();
	irrklang_engine -> drop ();
}
