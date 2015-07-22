
#include <string.h>
#include <gtk/gtk.h>
#include "prolog.h"

#ifdef WINDOWS_OPERATING_SYSTEM
extern "C" {
	__declspec (dllexport) char * get_module_code (void) {
		HMODULE hm = GetModuleHandle ("prcgtk.dll");
		HRSRC resource = FindResource (hm, MAKEINTRESOURCE (104), RT_RCDATA);
		if (resource == 0) return 0;
		HGLOBAL loader = LoadResource (hm, resource);
		if (loader == 0) return 0;
		return (char *) LockResource (loader);
	}
	__declspec (dllexport) PrologServiceClass * create_service_class (void);
}
#endif

#ifdef LINUX_OPERATING_SYSTEM
extern "C" {
	extern char * get_module_code (void);
	extern PrologServiceClass * create_service_class (void);
}
extern char resource_prcgtk;
char * get_module_code (void) {return & resource_prcgtk;}
#endif

class gtk_init_class : public PrologNativeCode {public: bool code (PrologElement * parameters, PrologResolution * resolution) {gtk_init (0, 0); return true;}};
class gtk_main_class : public PrologNativeCode {public: bool code (PrologElement * parameters, PrologResolution * resolution) {gtk_main (); return true;}};
class gtk_stop_class : public PrologNativeCode {public: bool code (PrologElement * parameters, PrologResolution * resolution) {gtk_main_quit (); return true;}};

class prc_gtk_service : public PrologServiceClass {
public:
	PrologNativeCode * getNativeCode (char * name) {
		if (strcmp (name, "gtk_init") == 0) return new gtk_init_class ();
		if (strcmp (name, "gtk_main") == 0) return new gtk_main_class ();
		if (strcmp (name, "gtk_stop") == 0) return new gtk_stop_class ();
		return 0;
	}
};

PrologServiceClass * create_service_class (void) {return new prc_gtk_service ();}
