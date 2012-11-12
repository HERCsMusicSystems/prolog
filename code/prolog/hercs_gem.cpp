///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2009 Robert P. Wolf                       //
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

#include "ruby.h"

#include "prolog_mysql.h"

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
		if (strcmp (name, "store") == 0) ret = resource_1;
		if (strcmp (name, "f1") == 0) ret = resource_2;
		if (strcmp (name, "help") == 0) ret = resource_3;
		if (strcmp (name, "record") == 0) ret = resource_4;
		if (strcmp (name, "neural") == 0) ret = resource_5;
		if (strcmp (name, "keyboard") == 0) ret = resource_6;
		if (strcmp (name, "notes") == 0) ret = resource_7;
		if (strcmp (name, "mysql") == 0) ret = resource_8;
		if (strcmp (name, "studio.prc") == 0) ret = resource_0;
		if (strcmp (name, "store.prc") == 0) ret = resource_1;
		if (strcmp (name, "f1.prc") == 0) ret = resource_2;
		if (strcmp (name, "help.prc") == 0) ret = resource_3;
		if (strcmp (name, "record.prc") == 0) ret = resource_4;
		if (strcmp (name, "neural.prc") == 0) ret = resource_5;
		if (strcmp (name, "keyboard.prc") == 0) ret = resource_6;
		if (strcmp (name, "notes.prc") == 0) ret = resource_7;
		if (strcmp (name, "mysql.prc") == 0) ret = resource_8;
		return ret;
	}
} resource_loader;
class service_class_loader_class : public PrologServiceClassLoader {
public:
	PrologServiceClass * load (char * name) {
		if (strcmp (name, "prolog_mysql") == 0) return new MySQLServiceClass ();
		//		if (strcmp (name, "neural.dll") == 0) return new neural_service ();
		return NULL;
	}
} service_class_loader;

class ruby_echo : public PrologNativeCode {
public:
	PrologRoot * root;
	char * area_p;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		while (parameters -> isPair ()) {
			if (parameters -> getLeft () -> isText ()) area_cat (area_p, parameters -> getLeft () -> getText ());
			else {
				AREA area;
				root -> getValue (parameters -> getLeft (), area, 0);
				area_cat (area_p, area);
			}
			parameters = parameters -> getRight ();
		}
		return true;
	}
	ruby_echo (PrologRoot * root, char * area) {this -> root = root; this -> area_p = area;}
};

class hercs_prolog_ruby_class {
public:
	PrologRoot * root;
	AREA local;
};

static void free_prolog (hercs_prolog_ruby_class * x) {
	x -> root -> removeThreads ();
	delete x -> root;
	delete x;
	drop_object_counter ();
}
static void mark_prolog (hercs_prolog_ruby_class * x) {}

static VALUE allocate_prolog (VALUE klass) {
	hercs_prolog_ruby_class * root = new hercs_prolog_ruby_class;
	root -> root = new PrologRoot ();
	strcpy (root -> local, "");
	root -> root -> setResourceLoader (& resource_loader);
	root -> root -> setServiceClassLoader (& service_class_loader);
	
	root -> root -> set_uap32_captions ();
	root -> root -> greenThreads (50);
	
	root -> root -> resolution ("studio.prc");

	root -> root -> createDirectory ("ruby_bridge");
	PrologAtom * echo = root -> root -> createAtom ("echo");
	echo -> setMachine (new ruby_echo (root -> root, root -> local));
	root -> root -> close ();

	return Data_Wrap_Struct (klass, mark_prolog, free_prolog, root);
}

VALUE prolog_to_ruby (PrologElement * e) {
	if (e -> isInteger ()) return INT2NUM(e -> getInteger ());
	if (e -> isDouble ()) return rb_float_new (e -> getDouble ());
	if (e -> isEarth ()) return rb_ary_new ();
	if (e -> isSlash ()) return ID2SYM(rb_intern ("/"));
	if (e -> isFail ()) return ID2SYM(rb_intern ("fail"));
	if (e -> isAtom ()) return ID2SYM(rb_intern (e -> getAtom () -> name ()));
	if (e -> isPair ()) {
		VALUE ary = rb_ary_new ();
		while (e -> isPair ()) {
			rb_ary_push (ary, prolog_to_ruby (e -> getLeft ()));
			e = e -> getRight ();
		}
		return ary;
		
	}
	if (e -> isText ()) return rb_str_new2 (e -> getText ());
	if (e -> isVar ()) {
		char command [32];
		sprintf (command, "x%i", e -> getVar ());
		return ID2SYM(rb_intern (command));
	}
	return Qnil;
}

bool ruby_to_prolog (VALUE re, PrologRoot * root, PrologElement * ep, int & maximum_variable) {
	char * cp;
	PrologAtom * atom;
	int length;
	switch (TYPE(re)) {
		case T_FIXNUM: ep -> setInteger (NUM2INT(re)); break;
		case T_FLOAT: ep -> setDouble (NUM2DBL(re)); break;
		case T_STRING: ep -> setText (RSTRING(re) -> ptr); break;
		case T_SYMBOL:
			cp = (char *) rb_id2name (SYM2ID(re));
			if (* cp == 'X' || * cp == 'x') {
				int x = (int) atof (cp + 1);
				if (x > maximum_variable) maximum_variable = x;
				ep -> setVar (x);
			} else if (strcmp (cp, "fail") == 0) {
				ep -> setFail ();
			} else if (strcmp (cp, "/") == 0) {
				ep -> setSlash ();
			} else {
				atom = root -> search (cp);
				if (atom == NULL) atom = root -> createAtom (cp);
				if (atom == NULL) return false;
				ep -> setAtom (atom);
			}
			break;
			case T_ARRAY:
			length = RARRAY(re) -> len;
			if (length == 0) {
				ep -> setEarth ();
			} else {
				for (int ind = 0; ind < length; ind++) {
					ep -> setPair ();
					if (! ruby_to_prolog (RARRAY(re) -> ptr [ind], root, ep -> getLeft (), maximum_variable)) return false;
					ep = ep -> getRight ();
				}
			}
			break;
			default: return true;
	}
	return true;
}

static VALUE method_Res (VALUE self, VALUE parameter) {
	if (TYPE(parameter) != T_ARRAY) return Qnil;
	hercs_prolog_ruby_class * hprc;
	Data_Get_Struct (self, hercs_prolog_ruby_class, hprc);
	PrologRoot * root = hprc -> root;
	int arity = RARRAY(parameter) -> len;
	PrologElement * parameters = root -> earth ();
	PrologElement * ep = parameters;
	PrologAtom * atom;
	char * cp;
	int maximum_number_of_variables = -1;
	if (! ruby_to_prolog (parameter, root, ep, maximum_number_of_variables)) {delete parameters; return Qfalse;}
	PrologElement * head = root -> earth ();
	PrologElement * hp = head;
	for (int ind = 0; ind <= maximum_number_of_variables; ind++) {
		hp -> setPair ();
		hp -> getLeft () -> setVar (ind);
		hp = hp -> getRight ();
	}
	PrologElement * query = root -> pair (parameters, root -> earth ());
	query = root -> pair (head, query);
	if (root -> resolution (query) != 1) {
		delete query;
		return Qfalse;
	}
	if (maximum_number_of_variables < 0) {
		delete query;
		return Qtrue;
	}
	PrologElement * e = query;
	if (! e -> isPair ()) {delete query; return Qnil;}
	e = e -> getLeft ();
	if (! e -> isPair ()) {delete query; return Qnil;}
	if (maximum_number_of_variables == 0) {
		VALUE ret = prolog_to_ruby (e -> getLeft ());
		delete query;
		return ret;
	}
	VALUE ret = prolog_to_ruby (e);
	delete query;
	return ret;
}

class ruby_term_reader : public PrologReader {
public:
	char * text;
	virtual void message (char * text) {printf ("READER [%s]", text);}
	virtual int move_z (void) {
		if (* text == '\0') return -1;
		return * text++;
	}
	void init (PrologRoot * root, char * text) {
		this -> text = text;
		setRoot (root);
	}
};

static VALUE method_Enter (VALUE self, VALUE command) {
	if (TYPE(command) != T_STRING) return Qfalse;
	hercs_prolog_ruby_class * hprc;
	Data_Get_Struct (self, hercs_prolog_ruby_class, hprc);
	PrologRoot * root = hprc -> root;
	ruby_term_reader reader;
	reader . init (root, RSTRING(command) -> ptr);
	PrologElement * variables = reader . readElement ();
	PrologElement * parameters = reader . readElement ();
	if (parameters == NULL) return Qfalse;
	PrologElement * query = root -> pair (parameters, root -> earth ());
	query = root -> pair (variables, query);
	int ctrl = root -> resolution (query);
	PrologElement * head = query;
	if (head -> isPair ()) head = head -> getLeft ();
	if (ctrl != 1) {
		delete query;
		return Qfalse;
	}
	VALUE ret = prolog_to_ruby (head);
	delete query;
	return ret;
}

static VALUE method_List (VALUE self) {
	hercs_prolog_ruby_class * hprc;
	Data_Get_Struct (self, hercs_prolog_ruby_class, hprc);
	PrologRoot * root = hprc -> root;
	AREA command;
	root -> list (command, 0);
	return rb_str_new2 (command);
}

static VALUE method_Echo (VALUE self) {
	hercs_prolog_ruby_class * hprc;
	Data_Get_Struct (self, hercs_prolog_ruby_class, hprc);
	VALUE ret = rb_str_new2 (hprc -> local);
	strcpy (hprc -> local, "");
	return ret;
}

static VALUE method_Command (VALUE self) {
	hercs_prolog_ruby_class * hprc;
	Data_Get_Struct (self, hercs_prolog_ruby_class, hprc);
	PrologRoot * root = hprc -> root;
	PrologElement * query = root -> atom ("command");
	query = root -> pair (query, root -> earth ());
	query = root -> pair (query, root -> earth ());
	query = root -> pair (root -> head (NULL), query);
	int ctrl = root -> resolution (query);
	delete query;
	return ctrl == 1 ? Qtrue : Qfalse;
}

static VALUE method_Load (VALUE self, VALUE parameter) {
	if (TYPE(parameter) != T_STRING) return Qfalse;
	hercs_prolog_ruby_class * hprc;
	Data_Get_Struct (self, hercs_prolog_ruby_class, hprc);
	PrologRoot * root = hprc -> root;
	PrologElement * query = root -> pair (root -> text (RSTRING(parameter) -> ptr), root -> earth ());
	query = root -> pair (root -> atom ("load"), query);
	query = root -> pair (query, root -> earth ());
	query = root -> pair (root -> head (NULL), query);
	int ctrl = root -> resolution (query);
	delete query;
	return ctrl == 1 ? Qtrue : Qfalse;
}

static VALUE method_ObjectCounter (VALUE self) {AREA command; drop_object_counters (command); return rb_str_new2 (command);}

extern "C" {
void Init_HERCs() {
	VALUE HERCs = rb_define_module("HERCs");
	VALUE hercs_class = rb_define_class_under (HERCs, "Prolog", rb_cObject);
	rb_define_alloc_func (hercs_class, allocate_prolog);
	rb_define_method (hercs_class, "res", reinterpret_cast<VALUE(*)(...)>(method_Res), 1);
	rb_define_method (hercs_class, "enter", reinterpret_cast<VALUE(*)(...)>(method_Enter), 1);
	rb_define_method (hercs_class, "list", reinterpret_cast<VALUE(*)(...)>(method_List), 0);
	rb_define_method (hercs_class, "echo", reinterpret_cast<VALUE(*)(...)>(method_Echo), 0);
	rb_define_method (hercs_class, "load", reinterpret_cast<VALUE(*)(...)>(method_Load), 1);
	rb_define_method (hercs_class, "cafe", reinterpret_cast<VALUE(*)(...)>(method_Command), 0);
	rb_define_method (HERCs, "objectCounter", reinterpret_cast<VALUE(*)(...)>(method_ObjectCounter), 0);
}
}
