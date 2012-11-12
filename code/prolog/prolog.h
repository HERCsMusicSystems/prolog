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

#ifndef _PROLOG_
#define _PROLOG_

#include "midi_stream.h"
#include "encoder.h"
#include <stdio.h>

class HERCs_PROLOG_SDK_EXPORT PrologString;
class HERCs_PROLOG_SDK_EXPORT PrologAtom;
class HERCs_PROLOG_SDK_EXPORT PrologElement;
class HERCs_PROLOG_SDK_EXPORT PrologNativeCode;
class HERCs_PROLOG_SDK_EXPORT PrologQuery;
class HERCs_PROLOG_SDK_EXPORT PrologVariable;
class HERCs_PROLOG_SDK_EXPORT PrologMatch;
class HERCs_PROLOG_SDK_EXPORT PrologServiceClass;
class HERCs_PROLOG_SDK_EXPORT PrologDirectory;
class HERCs_PROLOG_SDK_EXPORT PrologRoot;
class HERCs_PROLOG_SDK_EXPORT PrologTransport;
class HERCs_PROLOG_SDK_EXPORT PrologTransportPool;
class HERCs_PROLOG_SDK_EXPORT PrologResolution;
class HERCs_PROLOG_SDK_EXPORT PrologResolutionPool;
class HERCs_PROLOG_SDK_EXPORT PrologReader;
class HERCs_PROLOG_SDK_EXPORT PrologLoader;
class HERCs_PROLOG_SDK_EXPORT PrologResourceLoader;
class HERCs_PROLOG_SDK_EXPORT PrologServiceClassLoader;
class HERCs_PROLOG_SDK_EXPORT PrologMidiPortServiceClass;
class HERCs_PROLOG_SDK_EXPORT PrologCommand;
class HERCs_PROLOG_SDK_EXPORT object_tracker;

#define PROLOG_STRING_SIZE 96
#define PROLOG_STRING_SIZE_1 95
#define PROLOG_PRINTABLE_FLOAT "%.80g"
typedef char PROLOG_STRING [PROLOG_STRING_SIZE];
HERCs_PROLOG_SDK_EXPORT extern void prolog_string_copy (char * to, char * from);
HERCs_PROLOG_SDK_EXPORT extern void prolog_string_cat (char * to, char * from);

#define AREA_SIZE 2048
#define AREA_SIZE_1 2047
typedef char AREA [AREA_SIZE];
HERCs_PROLOG_SDK_EXPORT extern int area_cat (char * area, char * from);
HERCs_PROLOG_SDK_EXPORT extern int area_cat (char * area, int ind, char * from);
HERCs_PROLOG_SDK_EXPORT extern int area_cat (char * area, int ind, char c);
HERCs_PROLOG_SDK_EXPORT extern int area_cat_number (char * area, int ind, int sub);
HERCs_PROLOG_SDK_EXPORT extern int area_cat_number (char * area, int ind, double db);
HERCs_PROLOG_SDK_EXPORT extern int indexOf (char * text, char c);
HERCs_PROLOG_SDK_EXPORT extern void drop_stack (PrologQuery * from, PrologQuery * to);


HERCs_PROLOG_SDK_EXPORT extern char * create_text (char * text);
HERCs_PROLOG_SDK_EXPORT extern void delete_text (char * text);

class PrologString {
public:
	char * text;
	PrologString * next;
	PrologString (char * text, PrologString * next);
	~ PrologString (void);
};

class PrologResourceLoader {public: virtual char * load (char * name);};
class PrologServiceClassLoader {public: virtual PrologServiceClass * load (char * name);};
class PrologMidiPortServiceClass {
public:
	virtual int getNumberOfInputs (void);
	virtual int getNumberOfOutputs (void);
	virtual char * getInputInfo (int ind);
	virtual char * getOutputInfo (int ind);
	virtual int getInputPort (void);
	virtual int getOutputPort (void);
	virtual bool setInputPort (int ind);
	virtual bool setOutputPort (int ind);
	virtual midi_stream * getTransmissionLine (void);
	virtual midi_stream * getReceptionLine (void);
	virtual void changeManufacturersId (void);
	virtual void changeManufacturersId (int ind);
	virtual void changeManufacturersId (int ind, int sub);
	virtual void changeProductId (char id1, char id2, char id3, char id4);
	virtual void changeProductVersion (char id1, char id2, char id3, char id4);
};

HERCs_PROLOG_SDK_EXPORT void drop_object_counter (void);
HERCs_PROLOG_SDK_EXPORT void drop_object_counter (char * head);
HERCs_PROLOG_SDK_EXPORT void drop_object_counter (int selector);
HERCs_PROLOG_SDK_EXPORT void drop_object_counters (char * area);
HERCs_PROLOG_SDK_EXPORT bool object_left (void);
HERCs_PROLOG_SDK_EXPORT void drop_element (PrologRoot * root, PrologElement * el);

#include "object_tracker.h"

class PrologAtom TRACK {
public:
	char * atomName;
	PrologAtom * next;
	bool Privated;
	bool Protected;
	PrologElement * firstClause;
	PrologNativeCode * machine;
	COLLECTOR;
	PrologAtom (void);
	PrologAtom (char * name);
	PrologAtom (char * name, PrologAtom * root);
	~ PrologAtom (void);
	void removeAtoms (void);
	bool removeAtom (void);
	char * name (void);
	int names (char * area, int ind);
	PrologAtom * search (char * name);
	PrologAtom * searchPrivate (char * name);
	void Protect (void);
	void Private (void);
	void unProtect (void);
	void unPrivate (void);
	bool setMachine (PrologNativeCode * obj);
	PrologNativeCode * getMachine (void);
};

class PrologDirectory TRACK {
public:
	char * directoryName;
	PrologDirectory * next;
	PrologAtom * firstAtom;
	PrologServiceClass * service_class;
	PrologDirectory (char * name, PrologDirectory * root);
	PrologDirectory (char * name, PrologDirectory * root, PrologServiceClass * service_class);
	~ PrologDirectory (void);
	PrologServiceClass * getServiceClass (void);
	PrologServiceClass * getServiceClass (char * class_name);
	char * name (void);
//	void names (void);
	int names (char * area, int ind);
	PrologAtom * searchAtom (char * search);
	PrologAtom * searchPrivateAtom (char * search);
	PrologAtom * createAtom (char * name);
	bool removeAtom (PrologAtom * atom);
//	void list (void);
	int list (char * area, int ind);
	PrologDirectory * close (void);
	PrologDirectory * duplicate (PrologDirectory * root);
};

class PrologElement TRACK {
public:
	char * text;
	double floating_point;
	int integer;
	PrologElement * left;
	PrologElement * right;
	PrologAtom * atom;
	void * head;
	int type;
		// 0: earth
		// 1: pair
		// 2: var
		// 3: atom
		// 4: slash
		// 5: fail
		// 6: text
		// 7: head
		// 8: integer
		// 9: double
	PrologElement (void);
	~ PrologElement (void);
	bool isEarth (void);
	bool isPair (void);
	bool isVar (void);
	bool isAtom (void);
	bool isSlash (void);
	bool isFail (void);
	bool isText (void);
	bool isHead (void);
	bool isInteger (void);
	bool isDouble (void);
	void setEarth (void);
	void setPair (PrologElement * l, PrologElement * r);
	void setLeft (PrologElement * l);
	void setRight (PrologElement * r);
	void setPair (void);
	void setVar (int i);
	void setAtom (PrologAtom * a);
	void setSlash (void);
	void setFail (void);
	void setText (char * t);
	void setHead (void * o);
	void setInteger (int i);
	void setDouble (double d);
	PrologElement * getLeft (void);
	PrologElement * getRight (void);
	int getVar (void);
	PrologAtom * getAtom (void);
	char * getText (void);
	void * getHead (void);
	int getInteger (void);
	double getDouble (void);
	int getType (void);
	PrologElement * duplicate (void);
	void duplicate (PrologElement * e);
	void drop_one_right (void);
};

class PrologVariable TRACK {
public:
	bool location; // false = formal, true = actual
	PrologElement * term;
	int variable_id;
	PrologVariable * next;
	PrologVariable (PrologVariable * next);
	PrologVariable (PrologElement * term, bool location, PrologVariable * next);
	~ PrologVariable (void);
	int getId (int id);
};

class PrologMatch TRACK {
public:
	int var_number;
	PrologVariable * var;
	PrologMatch * next;
	PrologMatch (int var_number, PrologVariable * var, PrologMatch * next);
	PrologMatch (int var_number, PrologMatch * next, PrologResolution * resolution);
	PrologMatch (int var_number, PrologElement * term, bool location, PrologMatch * next, PrologResolution * resolution);
	~ PrologMatch (void);
	PrologMatch * getVar (int i);
	void connect_var (PrologVariable * v1, PrologVariable * v2);
};

class PrologNativeCode TRACK {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution);
	PrologNativeCode (void);
	virtual ~ PrologNativeCode (void);
};

class PrologServiceClass TRACK {
public:
	virtual void init (PrologRoot * root);
	virtual PrologNativeCode * getNativeCode (char * name);
	PrologServiceClass (void);
	virtual ~ PrologServiceClass (void);
};

class PrologQuery TRACK {
public:
	PrologQuery * stack;
	PrologQuery * context;
	PrologQuery * fail_target;
	bool original;
	PrologElement * query;
	PrologQuery (PrologQuery * stack, PrologQuery * context, PrologElement * query);
	PrologQuery (PrologQuery * stack, PrologQuery * context, PrologQuery * fail_target, bool original, PrologElement * query);
	PrologQuery (PrologElement * query);
	~ PrologQuery (void);
	void drop_stack_to_fail_target (void);
};

class PrologRoot TRACK {
public:
	PROLOG_STRING root_directory;
	PrologString * search_directories;
	PrologDirectory * root;
	PrologTransportPool * transport_pool;
	PrologTransport * root_transport;
	PrologResolutionPool * pool;
	PrologCommand * command;
	PrologResourceLoader * resource_loader;
	PrologServiceClassLoader * service_loader;
	PrologMidiPortServiceClass * midi_service_class;
	midi_reader * line_reader;
	midi_stream * midi_out;
	midi_stream * midi_in;
	PrologElement * main_query;
	PrologAtom * preprocessor;
	bool auto_atoms;
	bool transport_removed;
	int caption_id; // 0 = uap32, 1 = standard, 2 = edinburg, 3 = marseille, 4 = functional
	PROLOG_STRING left_caption;
	PROLOG_STRING right_caption;
	PROLOG_STRING secondary_left_caption;
	PROLOG_STRING secondary_right_caption;
	PROLOG_STRING mid_caption;
	PROLOG_STRING comment_caption;
	PROLOG_STRING fail_caption;
	PROLOG_STRING slash_caption;
	PROLOG_STRING quotation_caption;
	PROLOG_STRING atom_quotation_caption;
	PROLOG_STRING escape_caption;
	PROLOG_STRING head_caption;
	PROLOG_STRING separator_caption;
	PROLOG_STRING directive_head_caption;
	PROLOG_STRING machine_caption;
	PROLOG_STRING new_line_caption;
	PROLOG_STRING var_head_captions;
	PROLOG_STRING var_tail_captions;
	PROLOG_STRING var_caption;
	PROLOG_STRING atom_head_captions;
	PROLOG_STRING atom_tail_captions;
	PROLOG_STRING at_caption;
	PROLOG_STRING dot_caption;
	PROLOG_STRING if_atom_caption;
	PROLOG_STRING and_atom_caption;
	PROLOG_STRING program_caption;
	PROLOG_STRING import_caption;
	PROLOG_STRING auto_atoms_caption;
	PROLOG_STRING auto_caption;
	PROLOG_STRING preprocessor_caption;
	PROLOG_STRING protect_caption;
	PROLOG_STRING private_caption;
	PROLOG_STRING end_caption;
	char serial_number [64];
	char key [64];
	unsigned long int volume_id;
	int serial_shift;
	int current_foreground;
	int current_background;
	int native_threads_delay;
	PrologRoot (void);
	PrologRoot (PrologTransportPool * transport);
	PrologRoot (PrologTransportPool * transport, PrologResolutionPool * pool);
	~ PrologRoot (void);
	int captionId (void);
	bool autoAtoms (void);
	void setCaptions (int caption_id, bool auto_atoms);
	void set_uap32_captions (void);
	void set_standard_captions (void);
	void set_edinburg_captions (void);
	void set_marseille_captions (void);
	void set_functional_captions (void);
	void set_auto_atoms (void);
	void set_scripted_atoms (void);
	void set_serial_number (char * serial_number);
	void set_key (char * key);
	void set_volume_id (unsigned long int volume_id);
	void set_serial_shift (int serial_shift);
//	void getRightCaption (PrologElement * el);
//	void getValue (PrologElement * el);
	int getRightCaption (PrologElement * el, char * area, int ind);
	int getValue (PrologElement * el, char * area, int ind);
	int getTrueValue (PrologElement * el, char * area, int ind);
	void setPreprocessor (PrologAtom * atom);
	PrologAtom * getPreprocessor (void);
	void setRootDirectory (char * directory);
	bool searchDirectoryNotFound (char * name);
	void get_search_directories_from_environment (char * text);
	void addSearchDirectory (char * directory);
	void deleteSearchDirectories (void);
	void opaqueThreads (void);
	void opaqueThreads (int horizontal);
	void opaqueThreads (int horizontal, int seconds);
	void greenThreads (void);
	void greenThreads (int horizontal);
	void greenThreads (int horizontal, int seconds);
	void nativeThreads (int horizontal);
	void nativeThreads (int horizontal, int seconds);
	void removeThreads (void);
	void insertCommander (PrologCommand * command);
	PrologCommand * getCommander (void);
	void insertCommand (char * text);
	void setColors (int foreground, int background);
	void openEditor (void);
	void openEditor (int selector);
	void openEditor (int selector, int sub_selector);
	void closeEditor (void);
	void closeEditor (int selector);
	void closeEditor (int selector, int sub_selector);
	void setScreenCoordinates (int x, int y);
	PrologServiceClass * getServiceClass (void);
	PrologServiceClass * getServiceClass (char * name);
	PrologDirectory * createDirectory (char * name);
	PrologDirectory * createDirectory (char * name, PrologServiceClass * service);
	PrologDirectory * searchDirectory (char * name);
	PrologDirectory * getRoot (void);
	PrologAtom * search (char * name);
//	void list ();
//	void list (char * name);
	int list (char * area, int ind);
	int list (char * name, char * area, int ind);
	void close (void);
	bool drop (void);
	bool drop (char * name);
	char * getCWD (void);
	bool change_directory (char * directory);
	PrologElement * dir (char * location);
	bool edit (char * file_name);
	bool execute (char * command);
	bool make_directory (char * directory);
	bool erase_file (char * file_name);
	bool erase_directory (char * directory);
	bool move_file (char * from, char * to);
	bool copy_file (char * from, char * to);
	PrologAtom * createAtom (char * name);
	bool removeAtom (PrologAtom * atom);
	PrologElement * earth (void);
	PrologElement * pair (PrologElement * left, PrologElement * right);
	PrologElement * var (int num);
	PrologElement * atom (PrologAtom * atom);
	PrologElement * atomC (char * name);
	PrologElement * atom (char * name);
	PrologElement * atom (char * directory, char * name);
	PrologElement * slash (void);
	PrologElement * fail (void);
	PrologElement * text (char * str);
	PrologElement * head (void * obj);
	PrologElement * integer (int i);
	PrologElement * Double (double d);
	bool Private (char * name);
	bool Protect (char * name);
	PrologElement * getClausePointer (PrologAtom * atom, int position);
	PrologElement * getNextClausePointer (PrologElement * clause);
//	void listAtom (char * name);
	int listAtom (char * name, char * area, int ind);
	int attachClause (PrologElement * clause);
	int attachClause (PrologElement * clause, int position);
	PrologServiceClass * loadServiceClass (char * name);
//	void getQueryStack (PrologQuery * query);
	void message (char * head);
	void message (char * head, int ind);
	void message (char * head, int ind, int sub);
	void message (char * head, char * variable);
	void print (char * text);
	void print_character (int i);
	void wait (int delay);
	void microwait (void);
	int get_system_time (void);
	void start (PrologElement * parameters);
	void * create_system_semaphore (int ind);
	void destroy_system_semaphore (void * semaphore);
	void wait_system_semaphore (void * semaphore);
	void signal_system_semaphore (void * semaphore);
	bool enter_system_semaphore (void * semaphore);
	int get_character (void);
	void setResourceLoader (PrologResourceLoader * resource_loader);

	void setServiceClassLoader (PrologServiceClassLoader * service_loader);
	void setMidiPortServiceClass (PrologMidiPortServiceClass * midi_serice_class);
	PrologMidiPortServiceClass * getMidiPortServiceClass (void);
	void setMidiReader (midi_reader * reader);
	midi_reader * getMidiReader (void);
	void setMidiInput (midi_stream * in);
	void setMidiOutput (midi_stream * out);
	void setMidi (midi_stream * in, midi_stream * out);
	midi_stream * getMidiInput (void);
	midi_stream * getMidiOutput (void);
	int resolution (PrologElement * query);
		// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query
		// query is a clause, which is a list anyway
		// the head of the query is a return interface and can be any term
		// the rest of the query are calls
		// after successfull processing only the interface
		// is left from the original query matched with the result
		// after failure the query remains unchanged
		// query egzample: [[*x *y *z] [c1 *x] [c2 *y] [c3 *z]]
	bool resolutionHead (void);
	bool resolutionHead (char * directory);
	int resolution (void);
	int resolution (char * directory);
		// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query, 4 = file not found
	void removeMainQuery (void);
	int moveTransport (void);
		// never call moveTransport ();
		// unless you run opaque threads
		// it is called automatically by move ();
	bool startTransport (void);
	bool pauseTransport (void);
	bool stopTransport (void);
	void transportTempo (int beats_per_minute);
	void transportTempo (int beats_per_seconds, int seconds);
	void transportDivision (int beats_per_bar);
	void transportDivision (int beats_per_bar, int ticks_per_beat);
	void transportTickDivision (int ticks_per_beat);
	void transportMetrum (int top, int bottom);
	int getTransportBeatsPerSeconds (void);
	int getTransportSeconds (void);
	int getTransportBeatsPerBar (void);
	int getTransportTicksPerBeat (void);
	PrologTransportPool * getTransportPool (void);
	PrologTransport * getRootTransport (void);
	PrologResolutionPool * getResolutionPool (void);
	PrologTransport * insertTransport (void);
	bool dropTransport (PrologTransport * transport);

	void setQuery (void);
	void setQuery (PrologElement * query);
	int move (void);

};

class var_voc TRACK {
public:
	char * name;
	var_voc * next;
	var_voc (char * text);
	~ var_voc (void);
};

class PrologReader TRACK {
public:
	PrologRoot * root;
	PrologDirectory * search_context;
	var_voc * var_root;
	int act_znak;
	AREA symbol;
	int symbol_control;
	int int_symbol;
	double double_symbol;
	PrologReader (void);
	~ PrologReader (void);
	void clear_context (void);
	virtual int move_z (void);
	virtual void message (char * text);
	virtual void message_v (char * text, char * variable);
	void setRoot (PrologRoot * root);
	int get_var_number (char * name);
	void reset_var_number (void);
	int lastRead (void);
	int getString (char * area, int ind);
	int getString (char * area, int ind, char * char_set);
	int readln (char * area, int ind);
	bool prefetch_whites (void);
	void get_symbol (void);
	PrologAtom * searchAtom (char * name);
	PrologAtom * searchAtomC (char * name);
	PrologElement * atomC (char * name);
	PrologElement * readElement (void);
	PrologElement * readRightSide (PrologElement * LEFT, bool secondary);
	PrologElement * readClause (void);
};

class PrologLoader : public PrologReader {
private:
	void close (void);
public:
	bool drop_main;
	bool echo;
	bool reload;
	FILE * fi;
	char * ri;
	int old_caption_id;
	bool old_auto_atoms;
	virtual void message (char * text);
	virtual void message_v (char * text, char * variable);
	virtual int move_z (void);
	bool load (char * file_name);
	bool load (char * file_name, int captions, bool atoms);
	bool load_without_main (char * file_name);
	bool LOAD (char * file_name);
	PrologLoader (PrologRoot * root);
	~ PrologLoader (void);
};

class PrologResolution TRACK {
public:
	PrologRoot * root;
	PrologQuery * q_root;
	PrologElement * external_query_pointer;
	PrologMatch * root_actual;
	PrologMatch * root_formal;
	PrologVariable * var_root;
	int var_counter;
	PrologResolution * next;
	PrologResolutionPool * pool;
	PrologElement * call_again;
	unsigned long int timeout;
	PrologElement * timeout_query;

	PrologResolution (PrologRoot * root);
	PrologResolution (PrologRoot * root, PrologResolutionPool * pool, PrologResolution * insert_point);
	~ PrologResolution (void);

	PrologVariable * newVariable (void);
	PrologVariable * newVariable (PrologElement * term, bool location);

	void callAgain (PrologElement * parameters);
	bool callAgain (void);
	PrologQuery * getQuery (void);
	PrologResolutionPool * getResolutionPool (void);
	void reset (void);
	bool match (PrologElement * actual, bool ac, PrologElement * formal, bool fc);
	PrologElement * match_product (PrologElement * actual, bool ac);
	int res_forward (void);
	int sub_res_forward (PrologElement * relation_atom, PrologElement * term, PrologElement * clausa);
	int res_back_back (void);
	int res_fail_back (void);
	int resolution (PrologElement * query);

	int move (void);
	bool query (PrologElement * query);
	PrologElement * query (void);
	void removeQuery (void);
};

class PrologResolutionPool TRACK {
public:
	PrologRoot * root;
	PrologResolution * head;
	PrologResolution * main_thread;
	midi_stream * signal_line;
	int signals;
	PrologResolutionPool (PrologRoot * root);
	~ PrologResolutionPool (void);
	void drop_resolutions (void);
	void insert (PrologElement * query);
	void insertMain (PrologElement * query);
	void shift (void);
	void main_shift (void);
	int move (void);
};

class PrologTransport TRACK {
public:
	int miliseconds;
	int d, d1, d2;
	int tick;
	int beat;
	int bar;
	bool active;
	bool synchro;
	bool green_synchro;
	int sub_tick;
	int sub_beat;
	int ticks_per_beat;
	int beats_per_bar;
	int delay;
	int beats_per_seconds;
	int seconds;
	int atempo_beats_per_seconds;
	int atempo_seconds;
	int accelerator_ticks;
	int accelerator_current_tick;
	int accelerator_step;
	int accelerator_sentinel;
	PrologTransport * next;
	PrologTransport (void);
	PrologTransport (PrologTransport * next);
	~ PrologTransport (void);
	void reset (void);
	void signal (void);
	void signal_beat (void);
	void signal_bar (void);
	int move (int delta);
	void run (void);
	bool start (void);
	bool green_start (void);
	bool stop (void);
	bool pause (void);
	bool wt (int ind);
	bool Beat (int ind);
	bool Bar (int ind);
	int getTick (void);
	int getBeat (void);
	int getBar (void);
	bool isActive (void);
	void tempo (void);
	void tempo (int beats_per_minute);
	void tempo (int beats_per_seconds, int seconds);
	void accelerando (void);
	void accelerando (int steps);
	void accelerando (int steps, int ticks);
	void accelerando (int steps, int ticks, int sentinel);
	void ritardando (void);
	void ritardando (int steps);
	void ritardando (int steps, int ticks);
	void ritardando (int steps, int ticks, int sentinel);
	void atempo (void);
	void division (int beats_per_bar);
	void division (int beats_per_bar, int ticks_per_beat);
	void tick_division (int ticks_per_beat);
	void metrum (int top, int bottom);
	int get_beats_per_seconds (void);
	int get_seconds (void);
	int get_beats_per_bar (void);
	int get_ticks_per_beat (void);
};

class PrologTransportPool TRACK {
public:
	int seconds;
	int horizontal;
	int miliseconds;
	int delta_miliseconds_1, delta_miliseconds_2;
	int d, d1, d2;
	bool active;
	PrologTransport * transport;
	PrologTransportPool (void);
	PrologTransportPool (int horizontal);
	PrologTransportPool (int horizontal, int seconds);
	~ PrologTransportPool (void);
	PrologTransport * insert (void);
	bool drop (PrologTransport * t);
	int move (void);
	int getCurrentDelay (void);
	int getMiliseconds (void);
	void tempo (void);
};

class PrologCommand TRACK {
private:
	AREA command;
	int from;
	int to;
public:
	virtual void insert (char * text);
	virtual int get (void);
	virtual bool empty (void);
	virtual void print (char * text);
	virtual void open (void);
	virtual void close (void);
	virtual void setColors (int foreground, int background);
	virtual void openEditor1 (void);
	virtual void openEditor2 (int selector);
	virtual void openEditor3 (int selector, int sub_selector);
	virtual void closeEditor1 (void);
	virtual void closeEditor2 (int selector);
	virtual void closeEditor3 (int selector, int sub_selector);
	virtual void setScreenCoordinates (int x, int y);
	virtual char * getPrompt ();
	virtual void setPrompt (char * prompt);
	PrologCommand (void);
	~ PrologCommand (void);
};



#endif
