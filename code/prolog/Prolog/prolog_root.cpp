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

#include "prolog_studio.h"
#include <string.h>

PrologRoot :: PrologRoot (void) TRACKING (5) {
	resource_loader = NULL;
	service_loader = NULL;
	main_query = NULL;
	set_standard_captions ();
	auto_atoms = false;
	preprocessor = NULL;
	command = NULL;
	search_directories = NULL;
	args = NULL;
}
PrologElement * PrologRoot :: head (void * obj) {
	PrologElement * ret = new PrologElement ();
	ret -> setHead (obj);
	return ret;
}
PrologElement * PrologRoot :: integer (int i) {
	PrologElement * ret = new PrologElement ();
	ret -> setInteger (i);
	return ret;
}
PrologElement * PrologRoot :: Double (double d) {
	PrologElement * ret = new PrologElement ();
	ret -> setDouble (d);
	return ret;
}

bool PrologRoot :: Private (char * name) {
	if (root == NULL) return false;
	PrologAtom * atom = root -> searchPrivateAtom (name); //search (name);
	if (atom == NULL) return false;
	atom -> Private ();
	return true;
}

bool PrologRoot :: Protect (char * name) {
	if (root == NULL) return false;
	PrologAtom * atom = root -> searchPrivateAtom (name); //search (name);
	if (atom == NULL) return false;
	atom -> Protect ();
	return true;
}

PrologElement * PrologRoot :: getClausePointer (PrologAtom * atom , int position) {
	if (atom == NULL) return NULL;
	if (atom -> firstClause == NULL) return NULL;
	PrologElement * ret = atom -> firstClause;
	while (position > 0) {
		ret = ret -> getLeft ();
		if (ret == NULL) return NULL;
		ret = ret -> getLeft ();
		if (ret == NULL) return NULL;
		ret = (PrologElement *) ret -> getHead ();
		if (ret == NULL) return NULL;
		position--;
	}
	return ret;
}

PrologElement * PrologRoot :: getNextClausePointer (PrologElement * clause) {
	PrologElement * ret = clause -> getLeft ();
	if (ret == NULL) return NULL;
	ret = ret -> getLeft ();
	if (ret == NULL) return NULL;
	ret = (PrologElement *) ret -> getHead ();
	if (ret == NULL) return NULL;
	return ret;
}

int PrologRoot :: listAtom (char * name, char * area, int ind) {
	PrologAtom * atom = search (name);
	if (atom == NULL) {
		ind = area_cat (area, ind, name);
		ind = area_cat (area, ind, " not found.");
		ind = area_cat (area, ind, new_line_caption);
		return ind;
	}
	PrologElement * el = atom -> firstClause;
	while (el != NULL) {
		ind = area_cat (area, ind, left_caption);
		ind = area_cat (area, ind, left_caption);
		ind = area_cat (area, ind, name);
		ind = getRightCaption (el -> getLeft () -> getRight (), area, ind);
		ind = area_cat (area, ind, right_caption);
		ind = getRightCaption (el -> getRight (), area, ind);
		ind = area_cat (area, ind, right_caption);
		ind = area_cat (area, ind, new_line_caption);
		el = getNextClausePointer (el);
	}
	return ind;
}

int PrologRoot :: attachClause (PrologElement * clause) {
	PrologAtom * atom = NULL;
	PrologElement * sub = NULL;
	if (! clause -> isPair ()) return 1;
	sub = clause -> getLeft ();
	if (! sub -> isPair ()) return 2;
	sub = sub -> getLeft ();
	if (! sub -> isAtom ()) return 3;
	atom = sub -> getAtom ();
	if (atom -> Protected) return 4;
	sub -> setHead (NULL);
	if (atom -> firstClause == NULL) {
		atom -> firstClause = clause;
		return 0;
	}
	PrologElement * head = atom -> firstClause;
	if (! head -> isPair ()) return 5;
	head = head -> getLeft ();
	if (! head -> isPair ()) return 6;
	head = head -> getLeft ();
	if (! head -> isHead ()) return 7;
	while (head -> getHead () != NULL) {
		head = (PrologElement *) head -> getHead ();
		if (! head -> isPair ()) return 8;
		head = head -> getLeft ();
		if (! head -> isPair ()) return 9;
		head = head -> getLeft ();
		if (! head -> isHead ()) return 10;
	}
	head -> setHead (clause);
	return 0;
}

int PrologRoot :: attachClause (PrologElement * clause, int position) {
	PrologAtom * atom = NULL;
	PrologElement * sub = NULL;
	if (! clause -> isPair ()) return 1;
	sub = clause -> getLeft ();
	if (! sub -> isPair ()) return 2;
	sub = sub -> getLeft ();
	if (! sub -> isAtom ()) return 3;
	atom = sub -> getAtom ();
	if (atom -> Protected) return 4;
	if ((atom -> firstClause == NULL) || (position <= 0)) {
		sub -> setHead (atom -> firstClause);
		atom -> firstClause = clause;
		return 0;
	}
	position--;
	PrologElement * head = atom -> firstClause;
	if (! head -> isPair ()) return 5;
	head = head -> getLeft ();
	if (! head -> isPair ()) return 6;
	head = head -> getLeft ();
	if (! head -> isHead ()) return 7;
	while ((head -> getHead () != NULL) && (position > 0)) {
		position--;
		head = (PrologElement *) head -> getHead ();
		if (! head -> isPair ()) return 8;
		head = head -> getLeft ();
		if (! head -> isPair ()) return 9;
		head = head -> getLeft ();
		if (! head -> isHead ()) return 10;
	}
	sub -> setHead (head -> getHead ());
	head -> setHead (clause);
	return 0;
}

extern PrologServiceClass * load_plugin_service_class (char * directory, char * name);
PrologServiceClass * PrologRoot :: loadServiceClass (char * name) {
	PrologServiceClass * service_class = NULL;
	if (strcmp (name, "prolog.studio") == 0) service_class = new PrologStudio ();
	if (service_loader != NULL && service_class == NULL) service_class = service_loader -> load (name);
	if (service_class == NULL) service_class = load_plugin_service_class ("", name);
	PrologString * search_directory = search_directories;
	while (search_directory != NULL && service_class == NULL) {
		service_class = load_plugin_service_class (search_directory -> text, name);
		search_directory = search_directory -> next;
	}
	return service_class;
}

void PrologRoot :: message (char * head) {
	if (command == NULL) {printf ("%s []\n", head); return;}
	command -> print (head);
	command -> print (" []\n");
}

void PrologRoot :: message (char * head, int ind) {
	if (command == NULL) {printf ("%s [%i]\n", head, ind); return;}
	PROLOG_STRING text;
	sprintf (text, "%s [%i]\n", head, ind);
	command -> print (text);
}

void PrologRoot :: message (char * head, int ind, int sub) {
	if (command == NULL) {printf ("%s [%i %i]\n", head, ind, sub); return;}
	PROLOG_STRING text;
	sprintf (text, "%s [%i %i]\n", head, ind, sub);
	command -> print (text);
}

void PrologRoot :: message (char * head, char * variable) {
	if (command == NULL) {printf ("%s [%s]\n", head, variable); return;}
	command -> print (head); command -> print (" ["); command -> print (variable); command -> print ("]\n");
}

void PrologRoot :: print (char * text) {
	if (command == NULL) printf ("%s", text);
	else command -> print (text);
}

void PrologRoot :: print_character (int i) {
	if (i >= 0) {
		if (command == 0) {
			if (i <= 0xff) putchar (i);
			else if (i <= 0xffff) {putchar (i >> 8); putchar (i & 0xff);}
			else if (i <= 0xffffff) {putchar (i >> 16); putchar ((i >> 8) & 0xff); putchar (i & 0xff);}
			else {putchar (i >> 24); putchar ((i >> 16) & 0xff); putchar ((i >> 8) & 0xff); putchar (i & 0xff);}
		} else {
			if (i <= 0xff) command -> print (i);
			else if (i <= 0xffff) {command -> print (i >> 8); command -> print (i & 0xff);}
			else if (i <= 0xffffff) {command -> print (i >> 16); command -> print ((i >> 8) & 0xff); command -> print (i & 0xff);}
			else {command -> print (i >> 24); command -> print ((i >> 16) & 0xff); command -> print ((i >> 8) & 0xff); command -> print (i & 0xff);}
		}
		return;
	}
	i = -i;
	if (i <= 0x7f) {if (command == 0) putchar (i); else command -> print (i); return;}
	if (i <= 0x7ff) {
		int msb = 0xc0 | (i >> 6);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (msb); putchar (lsb);}
		else {command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0xffff) {
		int hsb = 0xe0 | (i >> 12);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0x1fffff) {
		int xlsb = 0xf0 | (i >> 18);
		int hsb = 0x80 | ((i >> 12) & 0x3f);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (xlsb); putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (xlsb); command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0x3ffffff) {
		int xmsb = 0xf8 | (i >> 26);
		int xlsb = 0x80 | ((i >> 18) & 0x3f);
		int hsb = 0x80 | ((i >> 12) & 0x3f);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (xmsb); putchar (xlsb); putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (xmsb); command -> print (xlsb); command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0x7fffffff) {
		int xhsb = 0xfc | (i >> 31);
		int xmsb = 0x80 | ((i >> 24) & 0x3f);
		int xlsb = 0x80 | ((i >> 18) & 0x3f);
		int hsb = 0x80 | ((i >> 12) & 0x3f);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (xhsb); putchar (xmsb); putchar (xlsb); putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (xhsb); command -> print (xmsb); command -> print (xlsb); command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
}

int PrologRoot :: get_character (void) {
	if (command == NULL) return getchar ();
	return command -> get ();
}

void PrologRoot :: setResourceLoader (PrologResourceLoader * resource_loader) {this -> resource_loader = resource_loader;}
void PrologRoot :: setServiceClassLoader (PrologServiceClassLoader * service_loader) {this -> service_loader = service_loader;}

int PrologRoot :: resolution (PrologElement * query) {
	// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query
	// query is a clause, which is a list anyway
	// the head of the query is a return interface and can be any term
	// the rest of the query are calls
	// after successfull processing only the interface
	// is left from the original query matched with the result
	// after failure the query remains unchanged
	// query egzample: [[*x *y *z] [c1 *x] [c2 *y] [c3 *z]]
//	if (active_main) {
//		print ("Instructions dropped: ");
//		AREA area;
//		getValue (NULL, area, 0);
//		print (area);
//		print (new_line_caption);
//		return 4;
//	}
	PrologResolution resolution (this);
	return resolution . resolution (query);
}

bool PrologRoot :: resolutionHead (char * directory) {
	if (strstr (directory, ".prb") != 0) {
		PrologLoader loader (this);
		if (! loader . load ("studio.prc")) return false;
		if (main_query != 0) removeMainQuery ();
		auto_atoms = true;
		main_query = pair (pair (atom ("batch"), pair (text (directory), earth ())), earth ());
		main_query = pair (head (0), main_query);
		return true;
	}
	if (strcmp (directory, "") == 0) return resolutionHead ();
	PrologLoader * loader = new PrologLoader (this);
	bool ret = loader -> load (directory);
	delete loader;
	return ret;
}

bool PrologRoot :: resolutionHead (void) {
	PrologLoader * loader = new PrologLoader (this);
	if (! loader -> load ("studio.prc")) {delete loader; return false;}
	delete loader;
	if (main_query != NULL) {
		AREA area;
		getValue (main_query, area, 0);
		message ("Instructions dropped <studio.prc>", area);
		removeMainQuery ();
	}
	main_query = pair (pair (atom ("command"), earth ()), earth ());
	main_query = pair (head (NULL), main_query);
	return true;
}

int PrologRoot :: resolution (char * directory) {
	if (! resolutionHead (directory)) return 4;
	int ctrl = resolution (main_query);
	removeMainQuery ();
	return ctrl;
}

int PrologRoot :: resolution (void) {
	if (! resolutionHead ()) return 4;
	int ctrl = resolution (main_query);
	removeMainQuery ();
	return ctrl;
}

void PrologRoot :: removeMainQuery (void) {
	if (main_query != NULL) delete main_query;
	main_query = NULL;
}

