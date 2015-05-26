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

#include <string.h>
#include "prolog.h"

PrologAtom :: PrologAtom (void) TRACKING (14) {
	Privated = false;
	Protected = false;
	firstClause = NULL;
	machine = NULL;
	PROLOG_STRING name;
	time_t time_stamp = time (0);
	sprintf (name, "<Atom=%p#%p>", (void *) this, (void *) time_stamp);
	atomName = create_text (name);
	next = NULL;
	COLLECTOR_RESET (0)
//	reference_counter = 0;
}

PrologAtom :: PrologAtom (char * name) TRACKING (14) {
	Privated = false;
	Protected = false;
	firstClause = NULL;
	machine = NULL;
	atomName = create_text (name);
	next = NULL;
	COLLECTOR_RESET (0)
//	reference_counter = 0;
}

PrologAtom :: PrologAtom (char * name, PrologAtom * root) TRACKING (14) {
	Privated = false;
	Protected = false;
	firstClause = NULL;
	machine = NULL;
	atomName = create_text (name);
	next = root;
	COLLECTOR_RESET (1)
//	reference_counter = 1;
}

static void delete_clause (PrologElement * clause) {
	while (clause != 0) {
		PrologElement * head = (PrologElement *) clause -> getLeft () -> getLeft () -> getHead ();
		delete clause;
		clause = head;
	}
}

bool PrologAtom :: removeAtom (void) {
	if (NOT_COLLECTOR_DEC) return false;
//	if (--reference_counter != 0) return false;
	if (next != NULL) return false;
	if (firstClause != NULL) delete_clause (firstClause);
	if (machine != NULL) delete machine;
	delete this;
	return true;
}

void PrologAtom :: removeAtoms (void) {
	if (next != NULL) next -> removeAtoms ();
	next = NULL;
	if (firstClause != NULL) delete_clause (firstClause);
	firstClause = NULL;
	if (machine != NULL) delete machine;
	machine = NULL;
	if (COLLECTOR_DEC) delete this;
//	if (--reference_counter == 0) delete this;
}

PrologAtom :: ~ PrologAtom (void) {
	COLLECTOR_DESTROY
	delete_text (atomName);
}

char * PrologAtom :: name (void) {return atomName;}

int PrologAtom :: names (char * area, int ind) {
	ind = area_cat (area, ind, atomName);
	PrologAtom * sub = next;
	while (sub != NULL) {
		ind = area_cat (area, ind, " ");
		ind = area_cat (area, ind, sub -> name ());
		sub = sub -> next;
	}
	return ind;
}

PrologAtom * PrologAtom :: search (char * name) {
	PrologAtom * ret = this;
	while (ret -> Privated || strcmp (ret -> atomName, name) != 0) {
		ret = ret -> next;
		if (ret == NULL) return NULL;
	}
	return ret;
}

PrologAtom * PrologAtom :: searchPrivate (char * name) {
	PrologAtom * ret = this;
	while (strcmp (ret -> atomName, name) != 0) {
		ret = ret -> next;
		if (ret == NULL) return NULL;
	}
	return ret;
}

void PrologAtom :: Protect (void) {Protected = true;}
void PrologAtom :: Private (void) {Privated = true;}
void PrologAtom :: unProtect (void) {Protected = false;}
void PrologAtom :: unPrivate (void) {Privated = false;}

bool PrologAtom :: setMachine (PrologNativeCode * obj) {
	if (firstClause != 0 || (Protected && obj != 0)) return false;
	Protected = (obj != 0);
	machine = obj;
	return true;
}

bool PrologAtom :: isTypeOf (PrologAtom * atom) {
	if (machine == 0) return false;
	PrologNativeCode * machine_type = atom -> getMachine ();
	if (machine_type == 0) return false;
	return machine -> isTypeOf (machine_type -> codeName ());
}

bool PrologAtom :: isTypeOf (PrologNativeCode * code) {
	if (machine == 0) return false;
	if (code == 0) return false;
	return machine -> isTypeOf (code -> codeName ());
}

char * PrologAtom :: machineType (void) {return machine == 0 ? 0 : machine -> codeName ();}

PrologNativeCode * PrologAtom :: getMachine (void) {return machine;}

