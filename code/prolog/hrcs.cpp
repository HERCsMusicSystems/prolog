///////////////////////////////////////////////////////
// Instructions                                      //
// create config.m4                                  //
// phpize                                            //
// ./configure --enable-hrcs                         //
// insert mysql_config --cflags                      //
// make                                              //
// sudo cp module/hrcs.so /usr/lib/php5/20...../.    //
// sudo apache2ctl -k restart                        //
///////////////////////////////////////////////////////



#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_hrcs.h"

#include "prolog.h"

extern char resource_0 [];
extern char resource_1 [];
extern char resource_2 [];
extern char resource_3 [];
extern char resource_4 [];
extern char resource_5 [];
extern char resource_6 [];
extern char resource_7 [];
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
		if (strcmp (name, "mysql") == 0) ret = resource_7;
		if (strcmp (name, "studio.prc") == 0) ret = resource_0;
		if (strcmp (name, "store.prc") == 0) ret = resource_1;
		if (strcmp (name, "f1.prc") == 0) ret = resource_2;
		if (strcmp (name, "help.prc") == 0) ret = resource_3;
		if (strcmp (name, "record.prc") == 0) ret = resource_4;
		if (strcmp (name, "neural.prc") == 0) ret = resource_5;
		if (strcmp (name, "keyboard.prc") == 0) ret = resource_6;
		if (strcmp (name, "mysql.prc") == 0) ret = resource_7;
		return ret;
	}
} resource_loader;

class phpecho : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		while (parameters -> isPair ()) {
			if (parameters -> getLeft () -> isText ()) php_printf ("%s", parameters -> getLeft () -> getText ());
			else {
				AREA area;
				root -> getValue (parameters -> getLeft (), area, 0);
				php_printf ("%s", area);
			}
			parameters = parameters -> getRight ();
		}
		return true;
	}
	phpecho (PrologRoot * root) {this -> root = root;}
};

#include "neural.h"
#include "prolog_mysql.h"

class service_class_loader_class : public PrologServiceClassLoader {
public:
	PrologServiceClass * load (char * name) {
		if (strcmp (name, "neural") == 0) return new neural_service ();
		if (strcmp (name, "prolog_mysql") == 0) return new MySQLServiceClass ();
		return NULL;
	}
} service_class_loader;

class php_term_reader : public PrologReader {
public:
	char * text;
	virtual void message (char * text) {php_printf ("READER [%s]", text);}
	virtual int move_z (void) {
		if (* text == '\0') return -1;
		return * text++;
	}
	void init (PrologRoot * root, char * text) {
		this -> text = text;
		setRoot (root);
	}
};

class hercs {
public:
	PrologRoot * root;
	void list (char * area) {root -> list (area, 0);}
	hercs (char * initial_directory = NULL) {
		root = new PrologRoot ();
		root -> setResourceLoader (& resource_loader);
		root -> setServiceClassLoader (& service_class_loader);
	
		root -> set_uap32_captions ();
		root -> greenThreads (50);
	
		root -> resolution ("studio.prc");

		root -> createDirectory ("php_bridge");
		PrologAtom * echo = root -> createAtom ("echo");
		echo -> setMachine (new phpecho (root));
		root -> close ();
		if (initial_directory != NULL) area_cat (root -> root_directory, initial_directory);
	}
	~ hercs (void) {
		root -> removeThreads ();
		delete root;
		drop_object_counter ();
	}
};

zend_object_handlers hrcs_object_handlers;

struct hrcs_object {
	zend_object std;
	hercs * h;
};

void hrcs_free_storage (void * object TSRMLS_DC) {
	hrcs_object * obj = (hrcs_object *) object;
	delete obj -> h;
	zend_hash_destroy (obj -> std . properties);
	FREE_HASHTABLE (obj -> std . properties);
	efree (obj);
}

zend_object_value hrcs_create_handler (zend_class_entry * type TSRMLS_DC) {
	zval * tmp;
	zend_object_value retval;
	hrcs_object * obj = (hrcs_object *) emalloc (sizeof (hrcs_object));
	memset (obj, 0, sizeof (hrcs_object));
	obj -> std . ce = type;
	ALLOC_HASHTABLE (obj -> std . properties);
	zend_hash_init (obj -> std . properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy (obj -> std . properties, & type -> default_properties, (copy_ctor_func_t) zval_add_ref, (void *) & tmp, sizeof (zval *));
	retval . handle = zend_objects_store_put (obj, NULL, hrcs_free_storage, NULL TSRMLS_CC);
	retval . handlers = & hrcs_object_handlers;
	return retval;
}

zend_class_entry * hrcs_ce;
PHP_METHOD(hercs, __construct) {
	char * initial_directory; int length = 0;
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "s", & initial_directory, & length) == FAILURE) initial_directory = NULL;
	hrcs_object * obj = (hrcs_object *) zend_object_store_get_object (getThis () TSRMLS_CC);
	obj -> h = new hercs (initial_directory);
}


PHP_METHOD(hercs, list) {
	hrcs_object * obj = (hrcs_object *) zend_object_store_get_object (getThis () TSRMLS_CC);
	if (obj -> h != NULL) {
		AREA area;
		obj -> h -> list (area);
		RETURN_STRING(area, 1);
	}
	RETURN_NULL();
}

PHP_METHOD(hercs, import) {
	char * input; int length;
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "s", & input, & length) == FAILURE) RETURN_BOOL(0);
	hrcs_object * obj = (hrcs_object *) zend_object_store_get_object (getThis () TSRMLS_CC);
	PrologLoader loader (obj -> h -> root);
	loader . echo = false;
	loader . reload = false;
	RETURN_BOOL(loader . load_without_main (input) ? 1 : 0);
}

PHP_METHOD(hercs, run) {
	char * input; int length;
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "s", & input, & length) == FAILURE) RETURN_BOOL(0);
	hrcs_object * obj = (hrcs_object *) zend_object_store_get_object (getThis () TSRMLS_CC);
	PrologRoot * root = obj -> h -> root;
	PrologDirectory * dir = root -> searchDirectory ("studio");
	if (dir == NULL) RETURN_BOOL(0);
	PrologAtom * batch_atom = dir -> searchAtom ("batch");
	if (batch_atom == NULL) RETURN_BOOL(0);
	PrologElement * query = root -> pair (root -> earth (), root -> earth ());
	PrologElement * head = query;
	head = head -> getRight ();
	head -> setPair ();
	head = head -> getLeft ();
	head -> setPair ();
	head -> getLeft () -> setAtom (batch_atom);
	head = head -> getRight ();
	head -> setPair ();
	head -> getLeft () -> setText (input);

	int ret = root -> resolution (query);
	delete query;
	RETURN_BOOL(ret == 1 ? 1 : 0);
}

static void to_php_array (zval * return_value, PrologElement * head) {
	while (head -> isPair ()) {
		PrologElement * left = head -> getLeft ();
		if (left -> isInteger ()) add_next_index_long (return_value, left -> getInteger ());
		if (left -> isDouble ()) add_next_index_double (return_value, left -> getDouble ());
		if (left -> isAtom ()) add_next_index_string (return_value, left -> getAtom () -> name (), 1);
		if (left -> isText ()) add_next_index_string (return_value, left -> getText (), 1);
		if (left -> isEarth ()) {zval * earth; ALLOC_INIT_ZVAL(earth); array_init (earth); add_next_index_zval (return_value, earth);}
		if (left -> isSlash ()) add_next_index_string (return_value, "/", 1);
		if (left -> isFail ()) add_next_index_string (return_value, "fail", 1);
		if (left -> isPair ()) {
			zval * sub;
			ALLOC_INIT_ZVAL(sub);
			array_init (sub);
			to_php_array (sub, left);
			add_next_index_zval (return_value, sub);
		}
		head = head -> getRight ();
	}
}

PHP_METHOD(hercs, res) {
	char * input; int length;
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "s", & input, & length) == FAILURE) RETURN_NULL();
	hrcs_object * obj = (hrcs_object *) zend_object_store_get_object (getThis () TSRMLS_CC);

	php_term_reader reader;
	PrologRoot * root = obj -> h -> root;
	reader . init (root, input);
	PrologElement * variables = reader . readElement ();
	PrologElement * parameters = reader . readElement ();

	if (parameters == NULL) RETURN_NULL();
	PrologElement * query = root -> pair (parameters, root -> earth ());
	query = root -> pair (variables, query);
	int ret = root -> resolution (query);
	PrologElement * head = query;
	if (head -> isPair ()) head = head -> getLeft ();
	if (ret != 1) {
		delete query;
		RETURN_BOOL(0);
	}
	AREA output;
	obj -> h -> root -> getValue (head, output, 0);
	array_init (return_value);
	add_assoc_string (return_value, "text", output, 1);
	to_php_array (return_value, head);
//	while (head -> isPair ()) {
//		PrologElement * left = head -> getLeft ();
//		if (left -> isInteger ()) add_next_index_long (return_value, left -> getInteger ());
//		if (left -> isDouble ()) add_next_index_double (return_value, left -> getDouble ());
//		head = head -> getRight ();
//	}
//	add_index_long (return_value, 42, 123);
//	add_next_index_string (return_value, "Joker was here!", 1);
//	add_next_index_string (return_value, output, 1);
//	zval * sub;
//	ALLOC_INIT_ZVAL(sub);
//	array_init (sub);
//	add_next_index_string (sub, "sonda", 1);
//	add_assoc_zval (return_value, "sub", sub);
	delete query;
//	RETURN_STRING(output, 1);
}

/*
PHP_FUNCTION(hello_array)
{
    char *mystr;
    zval *mysubarray;

    array_init(return_value);

    add_index_long(return_value, 42, 123);

    add_next_index_string(return_value, "I should now be found at index 43", 1);

    add_next_index_stringl(return_value, "I'm at 44!", 10, 1);

    mystr = estrdup("Forty Five");
    add_next_index_string(return_value, mystr, 0);

    add_assoc_double(return_value, "pi", 3.1415926535);

    ALLOC_INIT_ZVAL(mysubarray);
    array_init(mysubarray);
    add_next_index_string(mysubarray, "hello", 1);
    add_assoc_zval(return_value, "subarray", mysubarray);    
}
*/

static function_entry hrcs_methods [] = {
 	PHP_ME(hercs, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(hercs, list, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(hercs, res, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(hercs, import, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(hercs, run, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

static function_entry hrcs_functions[] = {
	PHP_FE(hrcs, NULL)
	{NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(hrcs) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "hrcs", hrcs_methods);
	hrcs_ce = zend_register_internal_class (& ce TSRMLS_CC);
	hrcs_ce -> create_object = hrcs_create_handler;
	memcpy (& hrcs_object_handlers, zend_get_std_object_handlers (), sizeof (zend_object_handlers));
	hrcs_object_handlers . clone_obj = NULL;
	return SUCCESS;
}

zend_module_entry hrcs_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_HRCS_EXTNAME,
    hrcs_functions,
    PHP_MINIT(hrcs),
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_HRCS_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HRCS
ZEND_GET_MODULE(hrcs)
#endif


PHP_FUNCTION(hrcs)
{
	RETURN_STRING("HERCs", 1);
}
