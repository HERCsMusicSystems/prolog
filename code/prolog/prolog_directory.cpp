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

PrologDirectory :: PrologDirectory (char * name, PrologDirectory * root) TRACKING (13) {
	directoryName = create_text (name);
//	string_copy (this -> directoryName, name);
	next = root;
	firstAtom = NULL;
	service_class = NULL;
}

PrologDirectory :: PrologDirectory (char * name, PrologDirectory * root, PrologServiceClass * service_class) TRACKING (13) {
	directoryName = create_text (name);
//	string_copy (this -> directoryName, name);
	next = root;
	firstAtom = NULL;
	this -> service_class = service_class;
}

PrologDirectory :: ~ PrologDirectory (void) {
	if (firstAtom != NULL) firstAtom -> removeAtoms (); //delete firstAtom;
	if (service_class != NULL) delete service_class;
	delete_text (directoryName);
	if (next != NULL) delete next;
}

PrologServiceClass * PrologDirectory :: getServiceClass (void) {return service_class;}
PrologServiceClass * PrologDirectory :: getServiceClass (char * class_name) {
	if (strcmp (directoryName, class_name) == 0) return service_class;
	if (next == NULL) return NULL;
	return next -> getServiceClass (class_name);
}

char * PrologDirectory :: name (void) {return directoryName;}

int PrologDirectory :: names (char * area, int ind) {
	ind = area_cat (area, ind, directoryName);
	PrologDirectory * sub = next;
	while (sub != NULL) {
		ind = area_cat (area, ind, " ");
		ind = area_cat (area, ind, sub -> name ());
		sub = sub -> next;
	}
	return ind;
}

PrologAtom * PrologDirectory :: searchAtom (char * search) {
	if (firstAtom == NULL) return NULL;
	return firstAtom -> search (search);
}

PrologAtom * PrologDirectory :: searchPrivateAtom (char * search) {
	if (firstAtom == NULL) return NULL;
	return firstAtom -> searchPrivate (search);
}

PrologAtom * PrologDirectory :: createAtom (char * name) {
	firstAtom = new PrologAtom (name, firstAtom);
	return firstAtom;
}

bool PrologDirectory :: removeAtom (PrologAtom * atom) {
	if (firstAtom == NULL) {
		if (next != NULL) return next -> removeAtom (atom);
		return false;
	}
	PrologAtom * to_delete;
	if (firstAtom == atom) {
		to_delete = firstAtom;
		firstAtom = firstAtom -> next;
		to_delete -> next = NULL;
		to_delete -> removeAtoms ();
		//delete to_delete;
		return true;
	}
	PrologAtom * ap = firstAtom;
	to_delete = ap -> next;
	while (to_delete != NULL) {
		if (to_delete == atom) {
			ap -> next = to_delete -> next;
			to_delete -> next = NULL;
			to_delete -> removeAtoms ();
			//delete to_delete;
			return true;
		}
		ap = to_delete;
		to_delete = ap -> next;
	}
	if (next == NULL) return false;
	return next -> removeAtom (atom);
}

int PrologDirectory :: list (char * area, int ind) {
	if (firstAtom == NULL) return ind;
	return firstAtom -> names (area, ind);
}

PrologDirectory * PrologDirectory :: close (void) {
	if (next == NULL) return this;
	PrologDirectory * sub = next;
	next = next -> next;
	sub -> next = this;
	return sub;
}

PrologDirectory * PrologDirectory :: duplicate (PrologDirectory * root) {
	PrologDirectory * dir = new PrologDirectory (directoryName, root, service_class);
	dir -> firstAtom = firstAtom;
	return dir;
}
