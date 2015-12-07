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

static int object_counter = 0;
static int prolog_variable_counter = 0;
static int prolog_transport_pool_counter = 0;
static int prolog_transport_counter = 0;
static int prolog_noise_counter = 0;
static int prolog_root_counter = 0;
static int prolog_resolution_pool_counter = 0;
static int prolog_resolution_counter = 0;
static int prolog_var_voc_counter = 0;
static int prolog_reader_counter = 0;
static int prolog_query_counter = 0;
static int prolog_match_counter = 0;
static int prolog_element_counter = 0;
static int prolog_directory_counter = 0;
static int prolog_atom_counter = 0;
static int prolog_service_class_counter = 0;
static int prolog_native_code_counter = 0;
static int text_counter = 0;
static int starter_counter = 0;
static int prolog_commander_counter = 0;

#ifndef NO_TRACKING

TRACKER_DEFINITIONS

object_tracker :: object_tracker (int selector) {
	type = selector;
	switch (selector) {
	case 1: TRACKER_ENTER; prolog_variable_counter++; break;
	case 2: TRACKER_ENTER; prolog_transport_pool_counter++; break;
	case 3: TRACKER_ENTER; prolog_transport_counter++; break;
	case 4: TRACKER_ENTER; prolog_noise_counter++; break;
	case 5: TRACKER_ENTER; prolog_root_counter++; break;
	case 6: TRACKER_ENTER; prolog_resolution_pool_counter++; break;
	case 7: TRACKER_ENTER; prolog_resolution_counter++; break;
	case 8: TRACKER_ENTER; prolog_var_voc_counter++; break;
	case 9: TRACKER_ENTER; prolog_reader_counter++; break;
	case 10: TRACKER_ENTER; prolog_query_counter++; break;
	case 11: TRACKER_ENTER; prolog_match_counter++; break;
	case 12: TRACKER_ENTER; prolog_element_counter++; break;
	case 13: TRACKER_ENTER; prolog_directory_counter++; break;
	case 14: TRACKER_ENTER; prolog_atom_counter++; break;
	case 15: TRACKER_ENTER; prolog_service_class_counter++; break;
	case 16: TRACKER_ENTER; prolog_native_code_counter++; break;
	case 18: TRACKER_ENTER; starter_counter++; break;
	case 19: TRACKER_ENTER; prolog_commander_counter++; break;
	}
	object_counter++;
	TRACKER_LEAVE;
}

object_tracker :: ~ object_tracker (void) {
	switch (type) {
	case 1: TRACKER_ENTER; prolog_variable_counter--; break;
	case 2: TRACKER_ENTER; prolog_transport_pool_counter--; break;
	case 3: TRACKER_ENTER; prolog_transport_counter--; break;
	case 4: TRACKER_ENTER; prolog_noise_counter--; break;
	case 5: TRACKER_ENTER; prolog_root_counter--; break;
	case 6: TRACKER_ENTER; prolog_resolution_pool_counter--; break;
	case 7: TRACKER_ENTER; prolog_resolution_counter--; break;
	case 8: TRACKER_ENTER; prolog_var_voc_counter--; break;
	case 9: TRACKER_ENTER; prolog_reader_counter--; break;
	case 10: TRACKER_ENTER; prolog_query_counter--; break;
	case 11: TRACKER_ENTER; prolog_match_counter--; break;
	case 12: TRACKER_ENTER; prolog_element_counter--; break;
	case 13: TRACKER_ENTER; prolog_directory_counter--; break;
	case 14: TRACKER_ENTER; prolog_atom_counter--; break;
	case 15: TRACKER_ENTER; prolog_service_class_counter--; break;
	case 16: TRACKER_ENTER; prolog_native_code_counter--; break;
	case 18: TRACKER_ENTER; starter_counter--; break;
	case 19: TRACKER_ENTER; prolog_commander_counter--; break;
	}
	object_counter--;
	TRACKER_LEAVE;
}

#endif

char * create_text (char * text) {
	char * ret = new char [strlen (text) + 4];
	strcpy (ret, text);
#ifndef NO_TRACKING
	TRACKER_ENTER;
	object_counter++;
	text_counter++;
	TRACKER_LEAVE;
#endif
	return ret;
}

char * create_text (int size) {
	char * ret = new char [size];
	ret [0] = '\0';
#ifndef NO_TRACKING
	TRACKER_ENTER;
	object_counter++;
	text_counter++;
	TRACKER_LEAVE;
#endif
	return ret;
}

void delete_text (char * text) {
	delete [] text;
#ifndef NO_TRACKING
	TRACKER_ENTER;
	object_counter--;
	text_counter--;
	TRACKER_LEAVE;
#endif
}

PrologString :: PrologString (char * text, PrologString * next) {
	this -> text = create_text (text);
	this -> next = next;
}

PrologString :: ~ PrologString (void) {
	if (text != NULL) delete_text (text); text = NULL;
	if (next != NULL) delete next; next = NULL;
}

void prolog_string_copy (char * to, char * from) {
	int ind = PROLOG_STRING_SIZE - 1;
	while (* from != '\0') {
		if (ind-- == 0) {* to = '\0'; return;}
		* to++ = * from++;
	}
	* to = '\0';
}

void prolog_delimited_string_copy (char * to, char * from) {
	int ind = PROLOG_STRING_SIZE - 2;
	while (* from != '\0') {
		if (ind-- == 0) {* to++ = '\0'; * to = '\0'; return;}
		* to++ = * from++;
		if (* from == '\0') {
			if (ind-- == 0) {* to++ = '\0'; * to = '\0'; return;}
			* to++ = * from++;
		}
	}
	* to = '\0';
}

void prolog_string_cat (char * to, char * from) {
	int ind = 0;
	while (to [ind] != '\0' && ind < PROLOG_STRING_SIZE_1) ind++;
	while (* from != '\0' && ind < PROLOG_STRING_SIZE_1) to [ind++] = * from++;
	to [ind] = '\0';
}

int area_cat (char * area, char * from) {
	int ind = 0;
	while (area [ind] != '\0' && ind < AREA_SIZE_1) ind++;
	while (* from != '\0' && ind < AREA_SIZE_1) area [ind++] = * from++;
	area [ind] = '\0';
	return ind;
}

int area_cat (char * area, int ind, char * from) {
	while (* from != '\0' && ind < AREA_SIZE_1) area [ind++] = * from++;
	area [ind] = '\0';
	return ind;
}

int area_cat (char * area, int ind, char c) {
	if (ind < AREA_SIZE_1) {area [ind++] = c; area [ind] = '\0';}
	return ind;
}

int area_cat_number (char * area, int ind, int sub) {
	PROLOG_STRING text;
	sprintf (text, "%i", sub);
	return area_cat (area, ind, text);
}

int area_cat_number (char * area, int ind, double db) {
	PROLOG_STRING text;
	sprintf (text, "%f", db);
	return area_cat (area, ind, text);
}

void relativise (char * path, char * pwd, char * relative) {
	if (pwd == 0 || path == 0 || relative == 0) return;
	while (* pwd != '\0' && (* pwd == * path || (* pwd == '\\' && * path == '/'))) {pwd++; path++;}
	if (* path == '/' || * path == '\\') path++;
	int rpt = 0;
	bool should_escape = false;
	while (* pwd != '\0') {
		if (* pwd == '/' || * pwd == '\\') {should_escape = false; rpt = area_cat (relative, rpt, "../");}
		else should_escape = true;
		pwd++;
	}
	if (should_escape) rpt = area_cat (relative, rpt, "../");
	rpt = area_cat (relative, rpt, path);
}

int indexOf (char * text, char c) {
	int ind = 0;
	char cx = text [ind];
	while (cx != '\0') {
		if (cx == c) return ind;
		cx = text [++ind];
	}
	return -1;
}

void drop_object_counter (void) {
	printf ("object_counter [%i", object_counter);
	if (prolog_variable_counter != 0) printf (" var=%i", prolog_variable_counter);
	if (prolog_transport_pool_counter != 0) printf (" transport_pool=%i", prolog_transport_pool_counter);
	if (prolog_transport_counter != 0) printf (" transport=%i", prolog_transport_counter);
	if (prolog_noise_counter != 0) printf (" noise=%i", prolog_noise_counter);
	if (prolog_root_counter != 0) printf (" root=%i", prolog_root_counter);
	if (prolog_resolution_pool_counter != 0) printf (" resolution_pool=%i", prolog_resolution_pool_counter);
	if (prolog_resolution_counter != 0) printf (" resolution=%i", prolog_resolution_counter);
	if (prolog_var_voc_counter != 0) printf (" var_voc=%i", prolog_var_voc_counter);
	if (prolog_reader_counter != 0) printf (" reader=%i", prolog_reader_counter);
	if (prolog_query_counter != 0) printf (" query=%i", prolog_query_counter);
	if (prolog_match_counter != 0) printf (" match=%i", prolog_match_counter);
	if (prolog_element_counter != 0) printf (" element=%i", prolog_element_counter);
	if (prolog_directory_counter != 0) printf (" directory=%i", prolog_directory_counter);
	if (prolog_atom_counter != 0) printf (" atom=%i", prolog_atom_counter);
	if (prolog_service_class_counter != 0) printf (" service=%i", prolog_service_class_counter);
	if (prolog_native_code_counter != 0) printf (" native=%i", prolog_native_code_counter);
	if (text_counter != 0) printf (" text=%i", text_counter);
	if (starter_counter != 0) printf (" starter=%i", starter_counter);
	if (prolog_commander_counter != 0) printf (" command=%i", prolog_commander_counter);
	printf ("]\n");
}

void drop_object_counters (char * area) {
	char command [256];
	int ind = 0;
	sprintf (command, "object_counter [%i", object_counter);
	ind = area_cat (area, ind, command);
	if (prolog_variable_counter != 0) {sprintf (command, " var=%i", prolog_variable_counter); ind = area_cat (area, ind, command);}
	if (prolog_transport_pool_counter != 0) {sprintf (command, " transport_pool=%i", prolog_transport_pool_counter); ind = area_cat (area, ind, command);}
	if (prolog_transport_counter != 0) {sprintf (command, " transport=%i", prolog_transport_counter); ind = area_cat (area, ind, command);}
	if (prolog_noise_counter != 0) {sprintf (command, " noise=%i", prolog_noise_counter); ind = area_cat (area, ind, command);}
	if (prolog_root_counter != 0) {sprintf (command, " root=%i", prolog_root_counter); ind = area_cat (area, ind, command);}
	if (prolog_resolution_pool_counter != 0) {sprintf (command, " resolution_pool=%i", prolog_resolution_pool_counter); ind = area_cat (area, ind, command);}
	if (prolog_resolution_counter != 0) {sprintf (command, " resolution=%i", prolog_resolution_counter); ind = area_cat (area, ind, command);}
	if (prolog_var_voc_counter != 0) {sprintf (command, " var_voc=%i", prolog_var_voc_counter); ind = area_cat (area, ind, command);}
	if (prolog_reader_counter != 0) {sprintf (command, " reader=%i", prolog_reader_counter); ind = area_cat (area, ind, command);}
	if (prolog_query_counter != 0) {sprintf (command, " query=%i", prolog_query_counter); ind = area_cat (area, ind, command);}
	if (prolog_match_counter != 0) {sprintf (command, " match=%i", prolog_match_counter); ind = area_cat (area, ind, command);}
	if (prolog_element_counter != 0) {sprintf (command, " element=%i", prolog_element_counter); ind = area_cat (area, ind, command);}
	if (prolog_directory_counter != 0) {sprintf (command, " directory=%i", prolog_directory_counter); ind = area_cat (area, ind, command);}
	if (prolog_atom_counter != 0) {sprintf (command, " atom=%i", prolog_atom_counter); ind = area_cat (area, ind, command);}
	if (prolog_service_class_counter != 0) {sprintf (command, " service=%i", prolog_service_class_counter); ind = area_cat (area, ind, command);}
	if (prolog_native_code_counter != 0) {sprintf (command, " native=%i", prolog_native_code_counter); ind = area_cat (area, ind, command);}
	if (text_counter != 0) {sprintf (command, " text=%i", text_counter); ind = area_cat (area, ind, command);}
	if (starter_counter != 0) {sprintf (command, " starter=%i", starter_counter); ind = area_cat (area, ind, command);}
	if (prolog_commander_counter != 0) {sprintf (command, " command=%i", prolog_commander_counter); ind = area_cat (area, ind, command);}
	ind = area_cat (area, ind, "]");
}

void o_message (char * head, int ind) {
	printf ("%s [%i]\n", head, ind);
}

void o_message (char * head, int ind, int sub) {
	printf ("%s [%i %i]\n", head, ind, sub);
}

void drop_object_counter (char * head) {
	o_message (head, object_counter);
}

void drop_object_counter (int selector) {
	switch (selector) {
	case 0: o_message ("object_counter", object_counter); break;
	case 1: o_message ("variable_counter", object_counter, prolog_variable_counter); break;
	case 2: o_message ("transport_pool_counter", object_counter, prolog_transport_pool_counter); break;
	case 3: o_message ("transport_counter", object_counter, prolog_transport_counter); break;
	case 4: o_message ("noise_counter", object_counter, prolog_noise_counter); break;
	case 5: o_message ("root_counter", object_counter, prolog_root_counter); break;
	case 6: o_message ("resolution_pool_counter", object_counter, prolog_resolution_pool_counter); break;
	case 7: o_message ("resoluion_counter", object_counter, prolog_resolution_counter); break;
	case 8: o_message ("var_voc_counter", object_counter, prolog_var_voc_counter); break;
	case 9: o_message ("reader_counter", object_counter, prolog_reader_counter); break;
	case 10: o_message ("query_counter", object_counter, prolog_query_counter); break;
	case 11: o_message ("match_counter", object_counter, prolog_match_counter); break;
	case 12: o_message ("element_counter", object_counter, prolog_element_counter); break;
	case 13: o_message ("directory_counter", object_counter, prolog_directory_counter); break;
	case 14: o_message ("atom_counter", object_counter, prolog_atom_counter); break;
	case 15: o_message ("service_class_counter", object_counter, prolog_service_class_counter); break;
	case 16: o_message ("native_code_counter", object_counter, prolog_native_code_counter); break;
	case 17: o_message ("text_counter", object_counter, text_counter); break;
	case 18: o_message ("starter_counter", object_counter, starter_counter); break;
	case 19: o_message ("prolog_commander_counter", object_counter, prolog_commander_counter); break;
	default: o_message ("unknown_type_counter", object_counter); break;
	}
}

bool object_left (void) {
	if (object_counter != 0) return true;
	if (prolog_variable_counter != 0) return true;
	if (prolog_transport_pool_counter != 0) return true;
	if (prolog_transport_counter != 0) return true;
	if (prolog_noise_counter != 0) return true;
	if (prolog_root_counter != 0) return true;
	if (prolog_resolution_pool_counter != 0) return true;
	if (prolog_resolution_counter != 0) return true;
	if (prolog_var_voc_counter != 0) return true;
	if (prolog_reader_counter != 0) return true;
	if (prolog_query_counter != 0) return true;
	if (prolog_match_counter != 0) return true;
	if (prolog_element_counter != 0) return true;
	if (prolog_directory_counter != 0) return true;
	if (prolog_atom_counter != 0) return true;
	if (prolog_service_class_counter != 0) return true;
	if (prolog_native_code_counter != 0) return true;
	if (text_counter != 0) return true;
	if (starter_counter != 0) return true;
	if (prolog_commander_counter != 0) return true;
	return false;
}

void drop_element (PrologRoot * root, PrologElement * el) {
	AREA area;
	root -> getValue (el, area, 0);
	if (root -> getCommander ()) {root -> getCommander () -> print (area); root -> getCommander () -> print ("\n"); return;}
	printf ("%s", area); printf ("\n");
}

void drop_stack (PrologQuery * from, PrologQuery * to) {
	PrologQuery * q;
	while (from != NULL && from != to) {
		q = from -> stack;
		delete from;
		from = q;
	}
}
