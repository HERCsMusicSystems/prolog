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
#include "prolog_studio.h"
#include "prolog_http.h"

#include <string.h>

#ifdef WINDOWS_OPERATING_SYSTEM
#include <process.h>
#define RUNNER_RETURN void
#define RUNNER_PARAMETER void *
#define RETURN
#endif

#ifdef LINUX_OPERATING_SYSTEM
#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/times.h>
#include <fcntl.h>
#define RUNNER_RETURN static void *
#define RUNNER_PARAMETER void *
#define RETURN return 0;
#endif

#ifdef LINUX_OPERATING_SYSTEM
#ifdef MAC_OPERATING_SYSTEM
#include "mac_midi.h"
mac_midi_service midi_service ("STUDIO");
#else
#include "linux_midi.h"
linux_midi_service midi_service;
#endif
#endif

#ifdef WINDOWS_OPERATING_SYSTEM
#include "windows_midi.h"
windows_midi_service midi_service;
#endif

prolog_midi_reader * midi_reader = NULL;

volatile bool running = false;
RUNNER_RETURN transport_runner (RUNNER_PARAMETER root) {
	running = true;
	while (running) {
		((PrologRoot *) root) -> moveTransport ();
		#ifdef WINDOWS_OPERATING_SYSTEM
		Sleep (20);
		#endif
		#ifdef LINUX_OPERATING_SYSTEM
		usleep (20000);
		#endif
	}
	running = true;
	RETURN
}

#ifdef LINUX_OPERATING_SYSTEM
typedef void * (* runner_procedure) (RUNNER_PARAMETER);
void beginthread (runner_procedure runner, int value, PrologRoot * root) {
	pthread_t threader;
	pthread_attr_t attr;
	pthread_attr_init (& attr);
	pthread_attr_setstacksize (& attr, 120 * 1024);
	pthread_attr_setdetachstate (& attr, PTHREAD_CREATE_DETACHED);
	pthread_create (& threader, & attr, runner, root);
	pthread_attr_destroy (& attr);
}
#endif
#ifdef WINDOWS_OPERATING_SYSTEM
#define beginthread(procedure, value, pointer) _beginthread (procedure, value, pointer)
#endif


#ifdef INTERNAL_RESOURCES
#include "neural.h"
//#ifdef LINUX_OPERATING_SYSTEM
//#include "prolog_mysql.h"
//#endif
#ifdef WINDOWS_OPERATING_SYSTEM
#include "resource.h"
class resource_loader_class : public PrologResourceLoader {
public:
	char * load (char * name) {
		HRSRC resource = NULL;
		if (strcmp (name, "studio") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STUDIO_PRC), RT_RCDATA);
		if (strcmp (name, "store") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STORE_PRC), RT_RCDATA);
		if (strcmp (name, "help") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (HELP_PRC), RT_RCDATA);
		if (strcmp (name, "keyboard") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (KEYBOARD_PRC), RT_RCDATA);
		if (strcmp (name, "record") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (RECORD_PRC), RT_RCDATA);
		if (strcmp (name, "scala_reader") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (SCALA_READER_PRC), RT_RCDATA);
		if (strcmp (name, "neural") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (NEURAL_PRC), RT_RCDATA);
		if (strcmp (name, "f1") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (F1_PRC), RT_RCDATA);
		if (strcmp (name, "sql") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (SQL_PRC), RT_RCDATA);
		if (strcmp (name, "studio.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STUDIO_PRC), RT_RCDATA);
		if (strcmp (name, "store.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STORE_PRC), RT_RCDATA);
		if (strcmp (name, "help.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (HELP_PRC), RT_RCDATA);
		if (strcmp (name, "keyboard.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (KEYBOARD_PRC), RT_RCDATA);
		if (strcmp (name, "record.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (RECORD_PRC), RT_RCDATA);
		if (strcmp (name, "scala_reader.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (SCALA_READER_PRC), RT_RCDATA);
		if (strcmp (name, "neural.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (NEURAL_PRC), RT_RCDATA);
		if (strcmp (name, "f1.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (F1_PRC), RT_RCDATA);
		if (strcmp (name, "sql.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (SQL_PRC), RT_RCDATA);
		if (! resource) return NULL;
		HGLOBAL loader = LoadResource (NULL, resource);
		if (! loader) return NULL;
		return (char *) LockResource (loader);
	}
} resource_loader;
#endif

#ifdef LINUX_OPERATING_SYSTEM
extern char resource_0 [];
extern char resource_1 [];
extern char resource_2 [];
extern char resource_3 [];
extern char resource_4 [];
extern char resource_5 [];
extern char resource_6 [];
extern char resource_7 [];
extern char resource_8 [];
class resource_loader_class : public PrologResourceLoader {
public:
	char * load (char * name) {
		char * ret = NULL;
		if (strcmp (name, "studio") == 0) ret = resource_0;
		if (strcmp (name, "http") == 0) ret = resource_1;
		if (strcmp (name, "store") == 0) ret = resource_2;
		if (strcmp (name, "f1") == 0) ret = resource_3;
		if (strcmp (name, "help") == 0) ret = resource_4;
		if (strcmp (name, "record") == 0) ret = resource_5;
		if (strcmp (name, "neural") == 0) ret = resource_6;
		if (strcmp (name, "keyboard") == 0) ret = resource_7;
		if (strcmp (name, "sql") == 0) ret = resource_8;
		if (strcmp (name, "studio.prc") == 0) ret = resource_0;
		if (strcmp (name, "http.prc") == 0) ret = resource_1;
		if (strcmp (name, "store.prc") == 0) ret = resource_2;
		if (strcmp (name, "f1.prc") == 0) ret = resource_3;
		if (strcmp (name, "help.prc") == 0) ret = resource_4;
		if (strcmp (name, "record.prc") == 0) ret = resource_5;
		if (strcmp (name, "neural.prc") == 0) ret = resource_6;
		if (strcmp (name, "keyboard.prc") == 0) ret = resource_7;
		if (strcmp (name, "sql.prc") == 0) ret = resource_8;
		return ret;
	}
} resource_loader;
#endif

class service_class_loader_class : public PrologServiceClassLoader {
public:
	PrologServiceClass * load (char * name) {
		if (strcmp (name, "neural") == 0) return new neural_service ();
		if (strcmp (name, "prolog.http") == 0) return new PrologHttp ();
//#ifdef LINUX_OPERATING_SYSTEM
//#ifndef MAC_OPERATING_SYSTEM
//		if (strcmp (name, "prolog_mysql") == 0) return new MySQLServiceClass ();
//#endif
//#endif
		return NULL;
	}
} service_class_loader;

#endif

int main (int args, char * argv []) {
	PrologRoot * root = new PrologRoot ();
	root -> get_search_directories_from_environment ("PRC_MODULE_SEARCH_PATHS");
	#ifdef INTERNAL_RESOURCES
	root -> setResourceLoader (& resource_loader);
	root -> setServiceClassLoader (& service_class_loader);
	#endif
	root -> set_uap32_captions ();
	int threads_type = 2;
	PROLOG_STRING name;
	strcpy (name, "");
	for (int ind = 1; ind < args; ind++) {
		if (strcmp (argv [ind], "*gt") == 0) threads_type = 0;
		else if (strcmp (argv [ind], "*ot") == 0) threads_type = 1;
		else if (strcmp (argv [ind], "*nt") == 0) threads_type = 2;
		else if (strlen (name) < 1) strcpy (name, argv [ind]);
		else root -> addArg (argv [ind]);
	}
	//*
	switch (threads_type) {
	case 0: root -> greenThreads (50); break;
	case 1: root -> opaqueThreads (50); break;
	case 2: root -> nativeThreads (50); break;
	default: break;
	}

	#ifdef LINUX_OPERATING_SYSTEM
	PrologLinuxConsole * console = threads_type == 2 ? new PrologLinuxConsole () : new PrologLinuxConsole (10);
	#endif
	#ifdef WINDOWS_OPERATING_SYSTEM
	PrologWindowsConsole * console = new PrologWindowsConsole (10);
	#endif

	root -> setMidi (midi_service . getReceptionLine (), midi_service . getTransmissionLine ());
	root -> setMidiPortServiceClass (& midi_service);

	if (threads_type == 1) beginthread (transport_runner, 0, root);
	midi_reader = new prolog_midi_reader (root);
	root -> setMidiReader (midi_reader);
	if (root -> pool == NULL) midi_service . set_reader (midi_reader);

	console -> open ();
	root -> insertCommander (console);
	if (strlen (name) == 0) root -> resolution ();
	else {
		if (strstr (name, ".prc") == NULL && strstr (name, ".prb") == NULL) strcat (name, ".prc");
		root -> resolution (name);
	}
	midi_service . setOutputPort (-1);
	midi_service . setInputPort (-1);
	//*
	console -> stop ();

	if (running) {
		running = false;
		while (! running) {
		#ifdef WINDOWS_OPERATING_SYSTEM
			Sleep (20);
		#endif
		#ifdef LINUX_OPERATING_SYSTEM
			usleep (20000);
		#endif
		}
		running = false;
	}

	root -> removeThreads ();
	if (root -> getCommander () != NULL) delete root -> getCommander ();
	if (midi_reader != NULL) delete midi_reader;
	delete root;
//	if (object_left ())
		drop_object_counter ();
	#ifdef WINDOWS_OPERATING_SYSTEM
	FreeConsole ();
	#endif
	return 0;
}
