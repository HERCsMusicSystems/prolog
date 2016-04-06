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

#include <string.h>

#include "prolog_neural.h"
#include "prolog_conductor.h"
#include "prolog_midi.h"
#include "prolog_xml.h"
#include "prolog_distribution.h"
#include "prolog_control.h"
#include "prolog_http.h"

#ifdef WINDOWS_OPERATING_SYSTEM
#include "prolog_windows_console.h"
#include "studio_resource.h"
class resource_loader_class : public PrologResourceLoader {
public:
	char * load (char * name) {
		HRSRC resource = NULL;
		if (strcmp (name, "studio.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STUDIO_PRC), RT_RCDATA);
		if (strcmp (name, "conductor.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (CONDUCTOR_PRC), RT_RCDATA);
		if (strcmp (name, "midi.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (MIDI_PRC), RT_RCDATA);
		if (strcmp (name, "http.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (HTTP_PRC), RT_RCDATA);
		if (strcmp (name, "store.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STORE_PRC), RT_RCDATA);
		if (strcmp (name, "f1.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (F1_PRC), RT_RCDATA);
		if (strcmp (name, "help.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (HELP_PRC), RT_RCDATA);
		if (strcmp (name, "record.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (RECORD_PRC), RT_RCDATA);
		if (strcmp (name, "neural.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (NEURAL_PRC), RT_RCDATA);
		if (strcmp (name, "keyboard.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (KEYBOARD_PRC), RT_RCDATA);
		if (strcmp (name, "sql.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (SQL_PRC), RT_RCDATA);
		if (strcmp (name, "test.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (TEST_PRC), RT_RCDATA);
		if (strcmp (name, "xml.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (XML_PRC), RT_RCDATA);
		if (strcmp (name, "distribution.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (DISTRIBUTION_PRC), RT_RCDATA);
		if (strcmp (name, "control.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (CONTROL_PRC), RT_RCDATA);
		if (! resource) return NULL;
		HGLOBAL loader = LoadResource (NULL, resource);
		if (! loader) return NULL;
		return (char *) LockResource (loader);
	}
} resource_loader;
#endif

#ifdef LINUX_OPERATING_SYSTEM
#include "prolog_linux_console.h"
extern char resource_studio;
extern char resource_conductor;
extern char resource_midi;
extern char resource_http;
extern char resource_store;
extern char resource_f1;
extern char resource_help;
extern char resource_record;
extern char resource_neural;
extern char resource_keyboard;
extern char resource_sql;
extern char resource_test;
extern char resource_xml;
extern char resource_distribution;
extern char resource_control;
class resource_loader_class : public PrologResourceLoader {
public:
	char * load (char * name) {
		char * ret = 0;
		if (strcmp (name, "studio.prc") == 0) ret = & resource_studio;
		if (strcmp (name, "conductor.prc") == 0) ret = & resource_conductor;
		if (strcmp (name, "midi.prc") == 0) ret = & resource_midi;
		if (strcmp (name, "http.prc") == 0) ret = & resource_http;
		if (strcmp (name, "store.prc") == 0) ret = & resource_store;
		if (strcmp (name, "f1.prc") == 0) ret = & resource_f1;
		if (strcmp (name, "help.prc") == 0) ret = & resource_help;
		if (strcmp (name, "record.prc") == 0) ret = & resource_record;
		if (strcmp (name, "neural.prc") == 0) ret = & resource_neural;
		if (strcmp (name, "keyboard.prc") == 0) ret = & resource_keyboard;
		if (strcmp (name, "sql.prc") == 0) ret = & resource_sql;
		if (strcmp (name, "test.prc") == 0) ret = & resource_test;
		if (strcmp (name, "xml.prc") == 0) ret = & resource_xml;
		if (strcmp (name, "distribution.prc") == 0) ret = & resource_distribution;
		if (strcmp (name, "control.prc") == 0) ret = & resource_control;
		return ret;
	}
} resource_loader;
#endif

class service_class_loader_class : public PrologServiceClassLoader {
public:
	PrologServiceClass * load (char * name) {
		if (strcmp (name, "prolog.conductor") == 0) return new PrologConductorServiceClass ();
		if (strcmp (name, "prolog.midi") == 0) return new PrologMidiServiceClass ();
		if (strcmp (name, "prolog.http") == 0) return new PrologHttpServiceClass ();
		if (strcmp (name, "prolog.neural") == 0) return new PrologNeuralServiceClass ();
		if (strcmp (name, "prolog.xml") == 0) return new PrologXMLServiceClass ();
		if (strcmp (name, "prolog.distribution") == 0) return new PrologDistributionServiceClass ();
		if (strcmp (name, "prolog.control") == 0) return new PrologControlServiceClass ();
		return NULL;
	}
} service_class_loader;

int main (int args, char * argv []) {
	PrologRoot * root = new PrologRoot ();
	root -> get_search_directories_from_environment ("STUDIO_HOME");
	root -> setResourceLoader (& resource_loader);
	root -> setServiceClassLoader (& service_class_loader);
	root -> set_uap32_captions ();
	char * name = 0;
	for (int ind = 1; ind < args; ind++) {
		if (name == 0) name = argv [ind];
		else root -> addArg (argv [ind]);
	}

	#ifdef LINUX_OPERATING_SYSTEM
	PrologCommand * console = new PrologLinuxConsole ();
	#endif
	#ifdef WINDOWS_OPERATING_SYSTEM
	PrologCommand * console = new PrologWindowsConsole ();
	#endif

	root -> insertCommander (console);
	int ctrl;
	if (name == 0) {root -> auto_atoms = true; ctrl = root -> resolution ();}
	else ctrl = root -> resolution (name);

	delete root;
	delete console;
	if (object_left ()) drop_object_counter ();
#ifdef _DEBUG
	getchar ();
#endif
	return 0;
}

