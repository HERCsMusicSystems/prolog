///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2004 Robert P. Wolf                       //
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

#include "prolog.h"

PrologElement :: PrologElement (void) TRACKING (12) {
	text = 0;
	floating_point = 0.0;
	integer = 0;
	left = 0;
	right = 0;
	atom = 0;
	head = 0;
	type = 0;
}

PrologElement :: PrologElement (PrologAtom * atom) TRACKING (12) {
	text = 0;
	floating_point = 0.0;
	integer = 0;
	left = 0;
	right = 0;
	this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
	head = 0;
	type = 3;
}

PrologElement :: PrologElement (char * text) TRACKING (12) {
	this -> text = create_text (text);
	floating_point = 0.0;
	integer = 0;
	left = 0;
	right = 0;
	atom = 0;
	head = 0;
	type = 6;
}

PrologElement :: PrologElement (void * head) TRACKING (12) {
	text = 0;
	floating_point = 0.0;
	integer = 0;
	left = 0;
	right = 0;
	atom = 0;
	this -> head = head;
	type = 7;
}

PrologElement :: PrologElement (int integer) TRACKING (12) {
	text = 0;
	floating_point = 0.0;
	this -> integer = integer;
	left = 0;
	right = 0;
	atom = 0;
	head = 0;
	type = 8;
}

PrologElement :: PrologElement (double floating_point) TRACKING (12) {
	text = 0;
	this -> floating_point = floating_point;
	integer = 0;
	left = 0;
	right = 0;
	atom = 0;
	head = 0;
	type = 9;
}

#define DESTROYER\
	if (text) {delete_text (text); text = 0;}\
	while (right) {PrologElement * el = right -> right; right -> right = 0; delete right; right = el;}\
	if (left) {delete left; left = 0;}\
	if (atom) {atom -> removeAtom ();\
	atom = 0;}
#define LEFT_DESTROYER if (left) {delete left; left = NULL;}
#define RIGHT_DESTROYER if (right) {delete right; right = NULL;}

PrologElement :: ~ PrologElement (void) {DESTROYER;}

bool PrologElement :: isEarth (void) {return type == 0;}
bool PrologElement :: isPair (void) {return type == 1;}
bool PrologElement :: isVar (void) {return type == 2;}
bool PrologElement :: isAtom (void) {return type == 3;}
bool PrologElement :: isSlash (void) {return type == 4;}
bool PrologElement :: isFail (void) {return type == 5;}
bool PrologElement :: isText (void) {return type == 6;}
bool PrologElement :: isHead (void) {return type == 7;}
bool PrologElement :: isInteger (void) {return type == 8;}
bool PrologElement :: isDouble (void) {return type == 9;}
bool PrologElement :: isNumber (void) {return type == 8 || type == 9;}

void PrologElement :: setEarth (void) {DESTROYER; type = 0;}
void PrologElement :: setPair (PrologElement * l, PrologElement * r) {
	DESTROYER;
	type = 1;
	left = l;
	right = r;
}
void PrologElement :: setLeft (PrologElement * l) {LEFT_DESTROYER; left = l;}
void PrologElement :: setRight (PrologElement * r) {RIGHT_DESTROYER; right = r;}
void PrologElement :: setPair (void) {
	DESTROYER;
	type = 1;
	left = new PrologElement ();
	right = new PrologElement ();
}
void PrologElement :: setVar (int i) {DESTROYER; type = 2; integer = i;}
void PrologElement :: setAtom (PrologAtom * a) {
	DESTROYER;
	if (a == NULL) {type = -1; return;}
	type = 3;
	atom = a;
	COLLECTOR_REFERENCE_INC (atom)
//	atom -> reference_counter++;
}
void PrologElement :: setSlash (void) {DESTROYER; type = 4;}
void PrologElement :: setFail (void) {DESTROYER; type = 5;}
void PrologElement :: setText (char * t) {DESTROYER; type = 6; text = create_text (t);} //string_copy (text, t);}
void PrologElement :: setHead (void * o) {DESTROYER; type = 7; head = o;}
void PrologElement :: setInteger (int i) {DESTROYER; type = 8; integer = i;}
void PrologElement :: setDouble (double d) {DESTROYER; type = 9; floating_point = d;}

PrologElement * PrologElement :: getLeft (void) {return left;}
PrologElement * PrologElement :: getRight (void) {return right;}
int PrologElement :: getVar (void) {return integer;}
PrologAtom * PrologElement :: getAtom (void) {return atom;}
char * PrologElement :: getText (void) {return text;}
void * PrologElement :: getHead (void) {return head;}
int PrologElement :: getInteger (void) {return integer;}
double PrologElement :: getDouble (void) {return floating_point;}
double PrologElement :: getNumber (void) {return type == 8 ? (double) integer : floating_point;}

int PrologElement :: getType (void) {return type;}

PrologElement * PrologElement :: duplicate (void) {
	PrologElement * ret = new PrologElement ();
	ret -> type = type;
	switch (type) {
	case 1:
		ret -> left = left -> duplicate ();
		ret -> right = right -> duplicate ();
		return ret;
	case 2: case 8: ret -> integer = integer; return ret;
	case 3: ret -> atom = atom; COLLECTOR_REFERENCE_INC (atom) return ret;
//	case 3: ret -> atom = atom; atom -> reference_counter++; return ret;
	case 6: ret -> text = create_text (text); return ret;
//	case 6: string_copy (ret -> text, text); return ret;
	case 7: ret -> head = head; return ret;
	case 9: ret -> floating_point = floating_point; return ret;
	default: return ret;
	}
}

void PrologElement :: duplicate (PrologElement * e) {
	switch (e -> type) {
	case 0: setEarth (); break;
	case 1: setPair (e -> getLeft () -> duplicate (), e -> getRight () -> duplicate ()); break;
	case 2: setVar (e -> getVar ()); break;
	case 3: setAtom (e -> getAtom ()); break;
	case 4: setSlash (); break;
	case 5: setFail (); break;
	case 6: setText (e -> getText ()); break;
	case 7: setHead (e -> getHead ()); break;
	case 8: setInteger (e -> getInteger ()); break;
	case 9: setDouble (e -> getDouble ()); break;
	default: break;
	}
}

void PrologElement :: drop_one_right (void) {
	PrologElement * to_delete = right;
	right = right -> right;
	to_delete -> right = NULL;
	delete to_delete;
}
