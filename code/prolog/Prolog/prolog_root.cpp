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

