///////////////////////////////////////
// Copyright (C) 2010 Robert P. Wolf //
//        ALL RIGHTS RESERVED        //
///////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "notes.h"

class note_calculator : public PrologNativeCode {
public:
	PrologAtom * c, * cb, * cbb, * cx, * cxx;
	PrologAtom * d, * db, * dbb, * dx, * dxx;
	PrologAtom * e, * eb, * ebb, * ex, * exx;
	PrologAtom * f, * fb, * fbb, * fx, * fxx;
	PrologAtom * g, * gb, * gbb, * gx, * gxx;
	PrologAtom * a, * ab, * abb, * ax, * axx;
	PrologAtom * b, * bb, * bbb, * bx, * bxx;
	int diatonic (PrologAtom * atom) {
		if (atom == c || atom == cb || atom == cbb || atom == cx || atom == cxx) return 0;
		if (atom == d || atom == db || atom == dbb || atom == dx || atom == dxx) return 1;
		if (atom == e || atom == eb || atom == ebb || atom == ex || atom == exx) return 2;
		if (atom == f || atom == fb || atom == fbb || atom == fx || atom == fxx) return 3;
		if (atom == g || atom == gb || atom == gbb || atom == gx || atom == gxx) return 4;
		if (atom == a || atom == ab || atom == abb || atom == ax || atom == axx) return 5;
		if (atom == b || atom == bb || atom == bbb || atom == bx || atom == bxx) return 6;
		return -127;
	}
	int chromatic (PrologAtom * atom) {
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
	PrologAtom * note (int diatonic, int chromatic) {
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
	note_calculator (PrologRoot * root) {
		c = cb = cbb = cx = cxx = NULL;
		d = db = dbb = dx = dxx = NULL;
		e = eb = ebb = ex = exx = NULL;
		f = fb = fbb = fx = fxx = NULL;
		g = gb = gbb = gx = gxx = NULL;
		a = ab = abb = ax = axx = NULL;
		b = bb = bbb = bx = bxx = NULL;
		PrologDirectory * dir = root -> searchDirectory ("notes");
		if (dir == NULL) return;
		c = dir -> searchAtom ("C"); cb = dir -> searchAtom ("Cb"); cbb = dir -> searchAtom ("Cbb"); cx = dir -> searchAtom ("C#"); cxx = dir -> searchAtom ("Cx");
		d = dir -> searchAtom ("D"); db = dir -> searchAtom ("Db"); dbb = dir -> searchAtom ("Dbb"); dx = dir -> searchAtom ("D#"); dxx = dir -> searchAtom ("Dx");
		e = dir -> searchAtom ("E"); eb = dir -> searchAtom ("Eb"); ebb = dir -> searchAtom ("Ebb"); ex = dir -> searchAtom ("E#"); exx = dir -> searchAtom ("Ex");
		f = dir -> searchAtom ("F"); fb = dir -> searchAtom ("Fb"); fbb = dir -> searchAtom ("Fbb"); fx = dir -> searchAtom ("F#"); fxx = dir -> searchAtom ("Fx");
		g = dir -> searchAtom ("G"); gb = dir -> searchAtom ("Gb"); gbb = dir -> searchAtom ("Gbb"); gx = dir -> searchAtom ("G#"); gxx = dir -> searchAtom ("Gx");
		a = dir -> searchAtom ("A"); ab = dir -> searchAtom ("Ab"); abb = dir -> searchAtom ("Abb"); ax = dir -> searchAtom ("A#"); axx = dir -> searchAtom ("Ax");
		b = dir -> searchAtom ("B"); bb = dir -> searchAtom ("Bb"); bbb = dir -> searchAtom ("Bbb"); bx = dir -> searchAtom ("B#"); bxx = dir -> searchAtom ("Bx");
	}
};

class interval_calculator : public note_calculator {
public:
	PrologRoot * root;
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
			int diatonic_left = diatonic (note_left -> getAtom ());
			if (diatonic_left < 0) return false;
			int chromatic_left = chromatic (note_left -> getAtom ());
			left_note = left_note -> getRight ();
			if (! left_note -> isPair ()) return false;
			PrologElement * octave_left = left_note -> getLeft ();
			if (! octave_left -> isInteger ()) return false;
			if (right_note -> isPair ()) {
				diatonic_left += 28 + octave_left -> getInteger () * 7;
				chromatic_left += 48 + octave_left -> getInteger () * 12;
				PrologElement * note_right = right_note -> getLeft ();
				if (! note_right -> isAtom ()) return false;
				int diatonic_right = diatonic (note_right -> getAtom ());
				if (diatonic_right < 0) return false;
				int chromatic_right = chromatic (note_right -> getAtom ());
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
				PrologAtom * ret = note (diatonic_right, chromatic_right);
				if (ret == NULL) return false;
				right_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_right), root -> earth ()));
				return true;
			}
			return false;
		}
		if (! right_note -> isPair ()) return false;
		PrologElement * note_right = right_note -> getLeft ();
		if (! note_right -> isAtom ()) return false;
		int diatonic_right = diatonic (note_right -> getAtom ());
		if (diatonic_right < 0) return false;
		int chromatic_right = chromatic (note_right -> getAtom ());
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
		PrologAtom * ret = note (diatonic_left, chromatic_left);
		if (ret == NULL) return false;
		left_note -> setPair (root -> atom (ret), root -> pair (root -> integer (octave_left), root -> earth ()));
		return true;
	}
	interval_calculator (PrologRoot * root) : note_calculator (root) {this -> root = root;}
};

void NotesServiceClass :: init (PrologRoot * root) {this -> root = root;}
PrologNativeCode * NotesServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "INTERVAL") == 0) return new interval_calculator (root);
	return NULL;
}

