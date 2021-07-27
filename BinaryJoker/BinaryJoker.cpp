
#include <string.h>
#include <prolog.h>

#define SEPARATE_PRC_FILE

extern "C" {
	extern char * get_module_code (void);
	extern PrologServiceClass * create_service_class (void);
}

#ifdef SEPARATE_PRC_FILE
extern char _binary_BinaryJokerPrc_prc_start;
char * get_module_code (void) {return &_binary_BinaryJokerPrc_prc_start;};
#else
char * prc = "import studio program BinaryJoker [joker] [[joker 'Joker was here!']] end := [[command]] .";
char * get_module_code (void) {return prc;};
#endif

class joker : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setText ("Joker was here!");
		return true;
	}
};

class BinaryJokerService : public PrologServiceClass {
public:
	PrologNativeCode * getNativeCode (char * name) {if (strcmp (name, "joker") == 0) return new joker (); return 0;}
};


PrologServiceClass * create_service_class (void) {return new BinaryJokerService ();}


