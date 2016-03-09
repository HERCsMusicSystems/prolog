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

#include <stdio.h>
#include "object_tracker.h"

class API PrologString;
class API PrologAtom;
class API PrologElement;
class API PrologNativeCode;
class API PrologQuery;
class API PrologVariable;
class API PrologMatch;
class API PrologServiceClass;
class API PrologDirectory;
class API PrologRoot;
class API PrologResolution;
class API PrologReader;
class API PrologLoader;
class API PrologResourceLoader;
class API PrologServiceClassLoader;
class API PrologCommand;

#define PROLOG_STRING_SIZE 96
#define PROLOG_STRING_SIZE_1 95
#define PROLOG_PRINTABLE_FLOAT "%.80g"
typedef char PROLOG_STRING [PROLOG_STRING_SIZE];
extern API void prolog_string_copy (char * to, char * from);
extern API void prolog_delimited_string_copy (char * to, char * from);
extern API void prolog_string_cat (char * to, char * from);

#define AREA_SIZE 2048
#define AREA_SIZE_1 2047
typedef char AREA [AREA_SIZE];
extern API int area_cat (char * area, char * from);
extern API int area_cat (char * area, int ind, char * from);
extern API int area_cat (char * area, int ind, char c);
extern API int area_cat_number (char * area, int ind, int sub);
extern API int area_cat_number (char * area, int ind, double db);
extern API void relativise (char * path, char * pwd, char * relative);
extern API int indexOf (char * text, char c);
extern API void drop_stack (PrologQuery * from, PrologQuery * to);


extern API char * create_text (char * text);
extern API char * create_text (int size);
extern API void delete_text (char * text);

class PrologString {
public:
	char * text;
	PrologString * next;
	PrologString (char * text, PrologString * next);
	~ PrologString (void);
};

class PrologResourceLoader {public: virtual char * load (char * name);};
class PrologServiceClassLoader {public: virtual PrologServiceClass * load (char * name);};

void API drop_object_counter (void);
void API drop_object_counter (char * head);
void API drop_object_counter (int selector);
void API drop_object_counters (char * area);
bool API object_left (void);
void API drop_element (PrologRoot * root, PrologElement * el);

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
	bool isTypeOf (PrologAtom * atom);
	bool isTypeOf (PrologNativeCode * code);
	char * machineType (void);
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
	PrologElement (PrologAtom * atom);
	PrologElement (char * text);
	PrologElement (void * head);
	PrologElement (int integer);
	PrologElement (double floating_point);
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
	bool isNumber (void);
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
	double getNumber (void);
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
	static char * name (void);
	virtual char * codeName (void);
	virtual bool isTypeOf (char * code_name);
	virtual bool code (PrologElement * parameters, PrologResolution * resolution);
	PrologNativeCode (void);
	virtual ~ PrologNativeCode (void);
};

class PrologServiceClass TRACK {
public:
	virtual void init (PrologRoot * root, PrologDirectory * directory);
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
	PrologString * search_directories;
	PrologString * args;
	PrologDirectory * root;
	PrologCommand * command;
	PrologResourceLoader * resource_loader;
	PrologServiceClassLoader * service_loader;
	PrologElement * main_query;
	PrologAtom * preprocessor;
	bool auto_atoms;
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
	PROLOG_STRING operator_captions;
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
	PrologRoot (void);
	~ PrologRoot (void);
	int captionId (void);
	bool autoAtoms (void);
	void setCaptions (int caption_id, bool auto_atoms);
	void set_uap32_captions (void);
	void set_standard_captions (void);
	void set_edinburg_captions (void);
	void set_marseille_captions (void);
	void set_functional_captions (void);
	void set_mathematical_captions (void);
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
	bool searchDirectoryNotFound (char * name);
	void get_search_directories_from_environment (char * text);
	void addSearchDirectory (char * directory);
	void deleteSearchDirectories (void);
	void addArg (char * arg);
	void insertCommander (PrologCommand * command);
	PrologCommand * getCommander (void);
	void insertCommand (char * text);
	void setBackground (int background);
	void setForeground (int foreground);
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
	unsigned long int get_system_time (void);
	void start (PrologElement * parameters);
	int get_character (void);
	void setResourceLoader (PrologResourceLoader * resource_loader);
	void setServiceClassLoader (PrologServiceClassLoader * service_loader);
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
};

class API var_voc TRACK {
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
	void shebang (void);
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
	PrologElement * instructions;
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
	unsigned long int timeout;
	PrologElement * timeout_query;

	PrologResolution (PrologRoot * root);
	~ PrologResolution (void);

	PrologVariable * newVariable (void);
	PrologVariable * newVariable (PrologElement * term, bool location);

	PrologQuery * getQuery (void);
	void reset (void);
	bool match (PrologElement * actual, bool ac, PrologElement * formal, bool fc);
	PrologElement * match_product (PrologElement * actual, bool ac);
	int res_forward (void);
	int sub_res_forward (PrologElement * relation_atom, PrologElement * term, PrologElement * clausa);
	int res_back_back (void);
	int res_fail_back (void);
	int resolution (PrologElement * query);

	bool query (PrologElement * query);
	PrologElement * query (void);
	void removeQuery (void);
};

class PrologCommand TRACK {
private:
	AREA command;
	int from;
	int to;
public:
	virtual void insert (char * text);
	virtual int get (void);
	virtual void print (int ch);
	virtual void print (char * text);
	virtual void setForeground (int foreground);
	virtual void setBackground (int background);
	virtual void setColours (int foreground, int background);
	virtual void openEditor1 (void);
	virtual void openEditor2 (int selector);
	virtual void openEditor3 (int selector, int sub_selector);
	virtual void closeEditor1 (void);
	virtual void closeEditor2 (int selector);
	virtual void closeEditor3 (int selector, int sub_selector);
	virtual void setScreenCoordinates (int x, int y);
	virtual char * getPrompt ();
	virtual void setPrompt (char * prompt);
	virtual void save_history (int size, char * file_name);
	virtual void load_history (int size, char * file_name);
	PrologCommand (void);
	virtual ~ PrologCommand (void);
};



#endif

