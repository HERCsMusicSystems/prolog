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

#ifdef LINUX_OPERATING_SYSTEM
#include <string.h>
#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
static int start_time (void) {timeval tv; gettimeofday (& tv, NULL); return tv . tv_sec;}
static int time_started = start_time ();
#define THREAD void *
#define DLL_OPEN(name) dlopen (name, RTLD_LAZY)
#define GET_SYMBOL(dll, name) dlsym (dll, name)
#define DLL_CLOSE(dll) dlclose (dll)
#define SYSTEM_DELAY(miliseconds) usleep (miliseconds * 1000)
#endif

#ifdef WINDOWS_OPERATING_SYSTEM
#include <windows.h>
#include <direct.h>
#include <process.h>
#define THREAD HINSTANCE
#define DLL_OPEN(name) LoadLibrary (name)
#define GET_SYMBOL(dll, name) GetProcAddress (dll, name)
#define DLL_CLOSE(dll) FreeLibrary (dll)
#define SYSTEM_DELAY(miliseconds) Sleep (miliseconds)
#endif

///////////////////////////////////
// PrologServiceClass (DLL load) //
///////////////////////////////////

typedef PrologServiceClass * (* service_class_creator) (void);

PrologServiceClass * load_plugin_service_class (char * directory, char * name) {
	char command [256];
#ifdef LINUX_OPERATING_SYSTEM
#ifdef MAC_OPERATING_SYSTEM
	sprintf (command, "%s%s.mdll", directory, name);
#else
	sprintf (command, "%s%s.so", directory, name);
#endif
#endif
#ifdef WINDOWS_OPERATING_SYSTEM
	sprintf (command, "%s%s.dll", directory, name);
#endif
	THREAD dll = DLL_OPEN (command);
	if (dll == 0) {
		#ifdef LINUX_OPERATING_SYSTEM
		printf ("Failed to open [%s] service library {%s}\n", command, dlerror ());
		#endif
		return 0;
	}
	service_class_creator create_service_class = (service_class_creator) GET_SYMBOL (dll, "create_service_class");
	if (! create_service_class) {
		printf ("create_service_class not found in [%s].\n", command);
		DLL_CLOSE (dll);
		return NULL;
	}
	return create_service_class ();
}

typedef char * (* module_code_finder) (void);
char * load_plugin_module (char * name) {
	char command [256];
	strcpy (command, name);
	char * prc = strstr (command, ".prc");
#ifdef LINUX_OPERATING_SYSTEM
	if (prc != 0) strcpy (prc, ".so");
#endif
#ifdef WINDOWS_OPERATING_SYSTEM
	if (prc != 0) strcpy (prc, ".dll");
#endif
	THREAD dll = DLL_OPEN (command);
	if (dll == 0) {
		#ifdef LINUX_OPERATING_SYSTEM
		printf ("Failed to open [%s] code library {%s}\n", command, dlerror ());
		#endif
		return 0;
	}
	module_code_finder finder = (module_code_finder) GET_SYMBOL (dll, "get_module_code");
	if (! finder) {
		printf ("get_module_code not found in [%s].\n", command);
		DLL_CLOSE (dll);
		return 0;
	}
	return finder ();
}

///////////////////////////////
// PrologRoot (file editing) //
///////////////////////////////

#ifdef LINUX_OPERATING_SYSTEM

#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

void PrologRoot :: get_search_directories_from_environment (char * text) {
	text = getenv (text);
	if (text == NULL) return;
	char * area = create_text (text);
	char * end = area;
	while (* end != '\0') {
		if (* end == ';') * end = '\0';
		end++;
	}
	char * cp = area;
	while (cp != end) {
		addSearchDirectory (cp);
		while (* cp != '\0' && cp != end) cp++;
		if (cp != end) cp++;
	}
	delete_text (area);
}

char * PrologRoot :: getCWD (void) {return getcwd (NULL, 0);}
bool PrologRoot :: change_directory (char * directory) {return chdir (directory) == 0;}

PrologElement * PrologRoot :: dir (char * location) {
	DIR * directory = opendir (strcmp (location, "*.*") == 0 ? "." : location);
	if (directory == NULL) return NULL;
	struct dirent * file = readdir (directory);
	struct stat st;
	PrologElement * directories = earth ();
	PrologElement * files = earth ();
	PrologElement * dp = directories;
	PrologElement * fp = files;
	while (file != NULL) {
		if (strcmp (file -> d_name, ".") != 0 && strcmp (file -> d_name, "..") != 0) {
			lstat (file -> d_name, & st);
			if (S_ISDIR (st . st_mode)) {
				dp -> setPair ();
				dp -> getLeft () -> setText (file -> d_name);
				dp = dp -> getRight ();
			} else {
				fp -> setPair ();
				fp -> getLeft () -> setText (file -> d_name);
				fp = fp -> getRight ();
			}
		}
		file = readdir (directory);
	}
	closedir (directory);
	return pair (directories, pair (files, earth ()));
}
static bool system_commander (char * command, char * parameters) {
	if (strlen (parameters) > 240) return false;
	char com [256];
	sprintf (com, "%s %s", command, parameters);
	return system (com) == 0;
}
bool PrologRoot :: edit (char * file_name) {return system_commander ("edit", file_name);}
bool PrologRoot :: execute (char * command) {system (command); return true;}
bool PrologRoot :: make_directory (char * directory) {return system_commander ("mkdir", directory);}
bool PrologRoot :: erase_file (char * file_name) {return system_commander ("rm", file_name);}
bool PrologRoot :: erase_directory (char * directory) {return system_commander ("rmdir", directory);}
bool PrologRoot :: move_file (char * from, char * to) {return rename (from, to) == 0 ? true : false;}
bool PrologRoot :: copy_file (char * from, char * to) {
	if (strlen (from) + strlen (to) > 240) return false;
	char command [256];
	sprintf (command, "cp %s %s", from, to);
	system (command);
	return true;
}

#endif

#ifdef WINDOWS_OPERATING_SYSTEM

void PrologRoot :: get_search_directories_from_environment (char * text) {
}

char * PrologRoot :: getCWD (void) {return _getcwd (NULL, 0);}
bool PrologRoot :: change_directory (char * directory) {return _chdir (directory) == 0;}

PrologElement * PrologRoot :: dir (char * location) {
	WIN32_FIND_DATA find_data;
	HANDLE handle = FindFirstFile (location, & find_data);
	if (handle == INVALID_HANDLE_VALUE) return NULL;
	PrologElement * directories = earth ();
	PrologElement * files = earth ();
	PrologElement * dp = directories;
	PrologElement * fp = files;
	do {
		if (find_data . dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			dp -> setPair ();
			dp -> getLeft () -> setText (find_data . cFileName);
			dp = dp -> getRight ();
		} else {
			fp -> setPair ();
			fp -> getLeft () -> setText (find_data . cFileName);
			fp = fp -> getRight ();
		}
	} while (FindNextFile (handle, & find_data));
	FindClose (handle);
	return pair (directories, pair (files, earth ()));
}

bool PrologRoot :: edit (char * file_name) {
	HINSTANCE res = ShellExecute (NULL, "edit", (LPCTSTR) file_name, NULL, NULL, SW_SHOWNORMAL);
	return res > (HINSTANCE) 32;
}
bool PrologRoot :: execute (char * command) {return system (command) == 0;}
bool PrologRoot :: make_directory (char * directory) {return _mkdir (directory) == 0;}
bool PrologRoot :: erase_file (char * file_name) {return DeleteFile (file_name) == TRUE;}
bool PrologRoot :: erase_directory (char * directory) {return _rmdir (directory) == 0;}
bool PrologRoot :: move_file (char * from, char * to) {return MoveFile (from, to) == TRUE;}
bool PrologRoot :: copy_file (char * from, char * to) {return CopyFile (from, to, FALSE) == TRUE;}

#endif

//////////////////////////////////
// PrologRoot (multi-threading) //
//////////////////////////////////

void PrologRoot :: wait (int delay) {SYSTEM_DELAY (delay);}

#ifdef LINUX_OPERATING_SYSTEM
	unsigned long int PrologRoot :: get_system_time (void) {
		timeval tv;
		gettimeofday (& tv, NULL);
		double time = (double) tv . tv_sec - time_started;
		time *= 1000.0;
		time += 0.001 * (double) tv . tv_usec;
		return (int) time;
	}
#endif

#ifdef WINDOWS_OPERATING_SYSTEM
	unsigned long int PrologRoot :: get_system_time (void) {return (int) GetTickCount ();}
#endif

class starter TRACK {
public:
	PrologRoot * root;
	PrologElement * clausa;
	void resolution (void) {root -> resolution (clausa);}
	starter (PrologRoot * root, PrologElement * parameters) TRACKING (18) {
		this -> root = root;
		this -> clausa = root -> pair (root -> head (NULL), parameters -> duplicate ());
	}
	~ starter (void) {delete clausa;}
};


static void * runner (void * parameters) {
	starter * x = (starter *) parameters;
	x -> resolution ();
	delete x;
	return 0;
}

void PrologRoot :: start (PrologElement * parameters) {
	pthread_t thread;
	pthread_create (& thread, 0, runner, new starter (this, parameters));
	pthread_detach (thread);
}

