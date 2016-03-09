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

#include "prolog_studio.h"
#include <string.h>

PrologRoot :: PrologRoot (void) TRACKING (5) {
	resource_loader = NULL;
	service_loader = NULL;
	main_query = NULL;
	root = new PrologDirectory ("user!", NULL);
	set_standard_captions ();
	auto_atoms = false;
	preprocessor = NULL;
	command = NULL;
	strcpy (serial_number, "");
	strcpy (key, "");
	search_directories = NULL;
	args = NULL;
	volume_id = 0;
	serial_shift = 0;
	current_foreground = 0xffff00;
	current_background = 0;
}

void PrologRoot :: deleteSearchDirectories (void) {
	if (search_directories != NULL) delete search_directories;
	search_directories = NULL;
}

PrologRoot :: ~ PrologRoot (void) {
	deleteSearchDirectories ();
	if (preprocessor != NULL) preprocessor -> removeAtom ();
	delete root;
	if (args != NULL) delete args; args = NULL;
}

int PrologRoot :: captionId (void) {return caption_id;}
bool PrologRoot :: autoAtoms (void) {return auto_atoms;}

void PrologRoot :: setCaptions (int caption_id, bool auto_atoms) {
	this -> auto_atoms = auto_atoms;
	switch (caption_id) {
	case 1: set_standard_captions (); break;
	case 2: set_edinburg_captions (); break;
	case 3: set_marseille_captions (); break;
	case 4: set_functional_captions (); break;
	default: set_uap32_captions (); break;
	}
}

void PrologRoot :: set_uap32_captions (void) {
	caption_id = 0;
	prolog_string_copy (left_caption, "[");
	prolog_string_copy (right_caption, "]");
	prolog_string_copy (secondary_left_caption, "(");
	prolog_string_copy (secondary_right_caption, ")");
	prolog_string_copy (mid_caption, ":");
	prolog_string_copy (comment_caption, ";");
	prolog_string_copy (fail_caption, "fail");
	prolog_string_copy (slash_caption, "/");
	prolog_string_copy (quotation_caption, "\"");
	prolog_string_copy (atom_quotation_caption, "'");
	prolog_string_copy (escape_caption, "\\");
	prolog_string_copy (head_caption, ":head:");
	prolog_string_copy (separator_caption, "");
	prolog_string_copy (directive_head_caption, "\\");
	prolog_string_copy (machine_caption, "#machine");
	new_line_caption [0] = 13;
	new_line_caption [1] = 10;
	new_line_caption [2] = '\0';
	prolog_string_copy (var_head_captions, "*");
	prolog_string_copy (var_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM");
	prolog_string_copy (var_caption, "*");
	prolog_string_copy (atom_head_captions, "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|+-=%~#!$^{}");
	prolog_string_copy (atom_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|+-=%~#!");
	prolog_string_copy (at_caption, "@");
	prolog_string_copy (dot_caption, ".");
	prolog_string_copy (if_atom_caption, ":=");
	prolog_string_copy (and_atom_caption, "^");
	* operator_captions = '\0';
	prolog_string_copy (program_caption, "program");
	prolog_string_copy (import_caption, "import");
	prolog_string_copy (auto_atoms_caption, "#auto");
	prolog_string_copy (auto_caption, "auto");
	prolog_string_copy (preprocessor_caption, "preprocessor");
	prolog_string_copy (protect_caption, "protect");
	prolog_string_copy (private_caption, "private");
	prolog_string_copy (end_caption, "end");
}

void PrologRoot :: set_standard_captions (void) {
	caption_id = 1;
	prolog_string_copy (left_caption, "(");
	prolog_string_copy (right_caption, ")");
	prolog_string_copy (secondary_left_caption, "[");
	prolog_string_copy (secondary_right_caption, "]");
	prolog_string_copy (mid_caption, "|");
	prolog_string_copy (comment_caption, ";");
	prolog_string_copy (fail_caption, "fail");
	prolog_string_copy (slash_caption, "/");
	prolog_string_copy (quotation_caption, "\"");
	prolog_string_copy (atom_quotation_caption, "'");
	prolog_string_copy (escape_caption, "\\");
	prolog_string_copy (head_caption, ":head:");
	prolog_string_copy (separator_caption, ",");
	prolog_string_copy (directive_head_caption, "\\");
	prolog_string_copy (machine_caption, "#machine");
	new_line_caption [0] = 13;
	new_line_caption [1] = 10;
	new_line_caption [2] = '\0';
	prolog_string_copy (var_head_captions, "_XYZxyz");
	prolog_string_copy (var_tail_captions, "0123456789");
	prolog_string_copy (var_caption, "_");
	prolog_string_copy (atom_head_captions, "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|:+-=%~#!$^{}");
	prolog_string_copy (atom_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#!");
	prolog_string_copy (at_caption, "@");
	prolog_string_copy (dot_caption, ".");
	prolog_string_copy (if_atom_caption, ":-");
	prolog_string_copy (and_atom_caption, "+");
	* operator_captions = '\0';
	prolog_string_copy (program_caption, "program");
	prolog_string_copy (import_caption, "import");
	prolog_string_copy (auto_atoms_caption, "#auto");
	prolog_string_copy (auto_caption, "auto");
	prolog_string_copy (preprocessor_caption, "preprocessor");
	prolog_string_copy (protect_caption, "protect");
	prolog_string_copy (private_caption, "private");
	prolog_string_copy (end_caption, "end");
}

void PrologRoot :: set_edinburg_captions (void) {
	caption_id = 2;
	prolog_string_copy (left_caption, "(");
	prolog_string_copy (right_caption, ")");
	prolog_string_copy (secondary_left_caption, "[");

	prolog_string_copy (secondary_right_caption, "]");
	prolog_string_copy (mid_caption, "|");
	prolog_string_copy (comment_caption, ";");
	prolog_string_copy (fail_caption, "fail");
	prolog_string_copy (slash_caption, "!");
	prolog_string_copy (quotation_caption, "\"");
	prolog_string_copy (atom_quotation_caption, "'");
	prolog_string_copy (escape_caption, "\\");
	prolog_string_copy (head_caption, ":head:");
	prolog_string_copy (separator_caption, ",");
	prolog_string_copy (directive_head_caption, "\\");
	prolog_string_copy (machine_caption, "#machine");
	new_line_caption [0] = 13;
	new_line_caption [1] = 10;
	new_line_caption [2] = '\0';
	prolog_string_copy (var_head_captions, "_QWERTYUIOPASDFGHJKLZXCVBNM*");
	prolog_string_copy (var_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM");
	prolog_string_copy (var_caption, "_");
	prolog_string_copy (atom_head_captions, "_qwertyuiopasdfghjklzxcvbnm?<>&|:+-=%~#/$^{}");
	prolog_string_copy (atom_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#");
	prolog_string_copy (at_caption, "@");
	prolog_string_copy (dot_caption, ".");
	prolog_string_copy (if_atom_caption, ":-");
	prolog_string_copy (and_atom_caption, "+");
	* operator_captions = '\0';
	prolog_string_copy (program_caption, "program");
	prolog_string_copy (import_caption, "import");
	prolog_string_copy (auto_atoms_caption, "#auto");
	prolog_string_copy (auto_caption, "auto");
	prolog_string_copy (preprocessor_caption, "preprocessor");
	prolog_string_copy (protect_caption, "protect");
	prolog_string_copy (private_caption, "private");
	prolog_string_copy (end_caption, "end");
}

void PrologRoot :: set_marseille_captions (void) {
	caption_id = 3;
	prolog_string_copy (left_caption, "(");
	prolog_string_copy (right_caption, ")");
	prolog_string_copy (secondary_left_caption, "[");
	prolog_string_copy (secondary_right_caption, "]");
	prolog_string_copy (mid_caption, ":");
	prolog_string_copy (comment_caption, ";");
	prolog_string_copy (fail_caption, "fail");
	prolog_string_copy (slash_caption, "/");
	prolog_string_copy (quotation_caption, "\"");
	prolog_string_copy (atom_quotation_caption, "'");
	prolog_string_copy (escape_caption, "\\");
	prolog_string_copy (head_caption, ":head:");
	prolog_string_copy (separator_caption, ",");
	prolog_string_copy (directive_head_caption, "\\");
	prolog_string_copy (machine_caption, "#machine");
	new_line_caption [0] = 13;
	new_line_caption [1] = 10;
	new_line_caption [2] = '\0';
	prolog_string_copy (var_head_captions, "*");
	prolog_string_copy (var_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM&:+-=");
	prolog_string_copy (var_caption, "*");
	prolog_string_copy (atom_head_captions, "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|:+-=%~#!$^{}");
	prolog_string_copy (atom_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#!");
	prolog_string_copy (at_caption, "@");
	prolog_string_copy (dot_caption, ".");
	prolog_string_copy (if_atom_caption, "+");
	prolog_string_copy (and_atom_caption, "-");
	* operator_captions = '\0';
	prolog_string_copy (program_caption, "program");
	prolog_string_copy (import_caption, "import");
	prolog_string_copy (auto_atoms_caption, "#auto");
	prolog_string_copy (auto_caption, "auto");
	prolog_string_copy (preprocessor_caption, "preprocessor");
	prolog_string_copy (protect_caption, "protect");
	prolog_string_copy (private_caption, "private");
	prolog_string_copy (end_caption, "end");
}

void PrologRoot :: set_functional_captions (void) {
	caption_id = 4;
	prolog_string_copy (left_caption, "[");
	prolog_string_copy (right_caption, "]");
	prolog_string_copy (secondary_left_caption, "(");
	prolog_string_copy (secondary_right_caption, ")");
	prolog_string_copy (mid_caption, ":");
	prolog_string_copy (comment_caption, ";");
	prolog_string_copy (fail_caption, "fail");
	prolog_string_copy (slash_caption, "!");
	prolog_string_copy (quotation_caption, "\"");
	prolog_string_copy (atom_quotation_caption, "`");
	prolog_string_copy (escape_caption, "\\");
	prolog_string_copy (head_caption, ":head:");
	prolog_string_copy (separator_caption, "");
	prolog_string_copy (directive_head_caption, "\\");
	prolog_string_copy (machine_caption, "#machine");
	new_line_caption [0] = 13;
	new_line_caption [1] = 10;
	new_line_caption [2] = '\0';
	prolog_string_copy (var_head_captions, "~");
	prolog_string_copy (var_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM");
	prolog_string_copy (var_caption, "~");
	prolog_string_copy (atom_head_captions, "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM'?<>&|+-*=/%#$^{}");
	prolog_string_copy (atom_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM'?/<>&|+-*=%#");
	prolog_string_copy (at_caption, "@");
	prolog_string_copy (dot_caption, ".");
	prolog_string_copy (if_atom_caption, ":=");
	prolog_string_copy (and_atom_caption, "'");
	* operator_captions = '\0';
	prolog_string_copy (program_caption, "program");
	prolog_string_copy (import_caption, "import");
	prolog_string_copy (auto_atoms_caption, "#auto");
	prolog_string_copy (auto_caption, "auto");
	prolog_string_copy (preprocessor_caption, "preprocessor");
	prolog_string_copy (protect_caption, "protect");
	prolog_string_copy (private_caption, "private");
	prolog_string_copy (end_caption, "end");
}

void PrologRoot :: set_mathematical_captions (void) {
	caption_id = 4;
	prolog_string_copy (left_caption, "[");
	prolog_string_copy (right_caption, "]");
	prolog_string_copy (secondary_left_caption, "(");
	prolog_string_copy (secondary_right_caption, ")");
	prolog_string_copy (mid_caption, ":");
	prolog_string_copy (comment_caption, ";");
	prolog_string_copy (fail_caption, "fail");
	prolog_string_copy (slash_caption, "!");
	prolog_string_copy (quotation_caption, "\"");
	prolog_string_copy (atom_quotation_caption, "`");
	prolog_string_copy (escape_caption, "\\");
	prolog_string_copy (head_caption, ":head:");
	prolog_string_copy (separator_caption, "");
	prolog_string_copy (directive_head_caption, "\\");
	prolog_string_copy (machine_caption, "#machine");
	new_line_caption [0] = 13;
	new_line_caption [1] = 10;
	new_line_caption [2] = '\0';
	prolog_string_copy (var_head_captions, "~");
	prolog_string_copy (var_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM");
	prolog_string_copy (var_caption, "~");
	prolog_string_copy (atom_head_captions, "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM'?<>&|+-*=/%#$^{}");
	prolog_string_copy (atom_tail_captions, "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM'?/<>&|=%#");
	prolog_string_copy (at_caption, "@");
	prolog_string_copy (dot_caption, ".");
	prolog_string_copy (if_atom_caption, ":=");
	prolog_string_copy (and_atom_caption, "'");
	prolog_delimited_string_copy (operator_captions, "+\0-\0*\0^\0=\0");
	prolog_string_copy (program_caption, "program");
	prolog_string_copy (import_caption, "import");
	prolog_string_copy (auto_atoms_caption, "#auto");
	prolog_string_copy (auto_caption, "auto");
	prolog_string_copy (preprocessor_caption, "preprocessor");
	prolog_string_copy (protect_caption, "protect");
	prolog_string_copy (private_caption, "private");
	prolog_string_copy (end_caption, "end");
}

void PrologRoot :: set_auto_atoms (void) {auto_atoms = true;}
void PrologRoot :: set_scripted_atoms (void) {auto_atoms = false;}

void PrologRoot :: set_serial_number (char * serial_number) {strcpy (this -> serial_number, serial_number);}
void PrologRoot :: set_key (char * key) {strcpy (this -> key, key);}
void PrologRoot :: set_volume_id (unsigned long int volume_id) {this -> volume_id = volume_id;}
void PrologRoot :: set_serial_shift (int serial_shift) {this -> serial_shift = serial_shift;}

int PrologRoot :: getRightCaption (PrologElement * el, char * area, int ind) {
	if (el == NULL) return area_cat (area, ind, "<null element>");
	if (el -> getType () == 0) return ind;
	if (el -> getType () == 1) {
		ind = area_cat (area, ind, separator_caption);
		ind = area_cat (area, ind, " ");
		int sub = getValue (el -> getLeft (), area, ind);
		if (sub == ind) return ind;
		ind = getRightCaption (el -> getRight (), area, sub);
		return ind;
	}
	ind = area_cat (area, ind, " ");
	ind = area_cat (area, ind, mid_caption);
	ind = area_cat (area, ind, " ");
	ind = getValue (el, area, ind);
	return ind;
}

int PrologRoot :: getValue (PrologElement * el, char * area, int ind) {
	if (el == NULL) return area_cat (area, ind, "<null element>");
	PROLOG_STRING command;
	switch (el -> getType ()) {
	case 0:
		ind = area_cat (area, ind, left_caption);
		ind = area_cat (area, ind, right_caption);
		return ind;
	case 1:
		if (el -> getLeft () == NULL) return area_cat (area, ind, "Wrong left element.");
		if (el -> getRight () == NULL) return area_cat (area, ind, "Wrong right element.");
		ind = area_cat (area, ind, left_caption);
		ind = getValue (el -> getLeft (), area, ind);
		ind = getRightCaption (el -> getRight (), area, ind);
		ind = area_cat (area, ind, right_caption);
		return ind;
	case 2: sprintf (command, "%s%i", var_caption, el -> getInteger ()); return area_cat (area, ind, command);
	case 3: return area_cat (area, ind, el -> getAtom () -> name ());
	case 4: return area_cat (area, ind, slash_caption);
	case 5: return area_cat (area, ind, fail_caption);
	case 6:
		ind = area_cat (area, ind, quotation_caption);
		ind = area_cat (area, ind, el -> getText ());
		ind = area_cat (area, ind, quotation_caption);
		return ind;
	case 7: sprintf (command, "<%p>", el -> head); return area_cat (area, area_cat (area, ind, head_caption), command);
	case 8: sprintf (command, "%i", el -> getInteger ()); return area_cat (area, ind, command);
	case 9: sprintf (command, PROLOG_PRINTABLE_FLOAT, el -> getDouble ()); if (strstr (command, ".") == NULL) strcat (command, ".0"); return area_cat (area, ind, command);
	default: return area_cat (area, ind, "Unknown element type.");
	}
}

int PrologRoot :: getTrueValue (PrologElement * el, char * area, int ind) {
	if (el == NULL) return area_cat (area, ind, "<null element>");
	PROLOG_STRING command;
	switch (el -> getType ()) {
	case 0:
		ind = area_cat (area, ind, left_caption);
		ind = area_cat (area, ind, right_caption);
		return ind;
	case 1:
		if (el -> getLeft () == NULL) return area_cat (area, ind, "Wrong left element.");
		if (el -> getRight () == NULL) return area_cat (area, ind, "Wrong right element.");
		ind = area_cat (area, ind, left_caption);
		ind = getTrueValue (el -> getLeft (), area, ind);
		ind = area_cat (area, ind, " ");
		ind = area_cat (area, ind, mid_caption);
		ind = area_cat (area, ind, " ");
		ind = getTrueValue (el -> getRight (), area, ind);
		ind = area_cat (area, ind, right_caption);
		return ind;
	case 2: sprintf (command, "%s%i", var_caption, el -> getInteger ()); return area_cat (area, ind, command);
	case 3: return area_cat (area, ind, el -> getAtom () -> name ());
	case 4: return area_cat (area, ind, slash_caption);
	case 5: return area_cat (area, ind, fail_caption);
	case 6:
		ind = area_cat (area, ind, quotation_caption);
		ind = area_cat (area, ind, el -> getText ());
		ind = area_cat (area, ind, quotation_caption);
		return ind;
	case 7: return area_cat (area, ind, head_caption);
	case 8: sprintf (command, "%i", el -> getInteger ()); return area_cat (area, ind, command);
	case 9: sprintf (command, "%f", el -> getDouble ()); return area_cat (area, ind, command);
	default: return area_cat (area, ind, "Unknown element type.");
	}
}

void PrologRoot :: setPreprocessor (PrologAtom * atom) {
	if (preprocessor != NULL) preprocessor -> removeAtom ();
	preprocessor = atom;
	if (preprocessor != NULL) COLLECTOR_REFERENCE_INC (preprocessor)
//	if (preprocessor != NULL) preprocessor -> reference_counter++;
}

PrologAtom * PrologRoot :: getPreprocessor (void) {return preprocessor;}

void PrologRoot :: addSearchDirectory (char * directory) {
	search_directories = new PrologString (directory, search_directories);
}

bool PrologRoot :: searchDirectoryNotFound (char * directory) {
	PrologString * search_directory = search_directories;
	while (search_directory != NULL) {
		if (strcmp (search_directory -> text, directory) == 0) return false;
		search_directory = search_directory -> next;
	}
	return true;
}

void PrologRoot :: addArg (char * arg) {
	PrologString * sp = new PrologString (arg, NULL);
	if (args == NULL) {args = sp; return;}
	PrologString * ap = args;
	while (ap -> next != NULL) ap = ap -> next;
	ap -> next = sp;
}

void PrologRoot :: insertCommander (PrologCommand * command) {this -> command = command;}

void PrologRoot :: insertCommand (char * text) {
	if (command == NULL) return;
	command -> insert (text);
}

void PrologRoot :: setBackground (int background) {
	current_background = background;
	if (command != 0) command -> setBackground (background);
}

void PrologRoot :: setForeground (int foreground) {
	current_foreground = foreground;
	if (command != 0) command -> setForeground (foreground);
}

void PrologRoot :: openEditor (void) {
	if (command == NULL) return;
	command -> openEditor1 ();
}

void PrologRoot :: openEditor (int selector) {
	if (command == NULL) return;
	command -> openEditor2 (selector);
}

void PrologRoot :: openEditor (int selector, int sub_selector) {
	if (command == NULL) return;
	command -> openEditor3 (selector, sub_selector);
}

void PrologRoot :: closeEditor (void) {
	if (command == NULL) return;
	command -> closeEditor1 ();
}

void PrologRoot :: closeEditor (int selector) {
	if (command == NULL) return;
	command -> closeEditor2 (selector);
}

void PrologRoot :: closeEditor (int selector, int sub_selector) {
	if (command == NULL) return;
	command -> closeEditor3 (selector, sub_selector);
}

void PrologRoot :: setScreenCoordinates (int x, int y) {
	if (command == NULL) return;
	command -> setScreenCoordinates (x, y);
}

PrologCommand * PrologRoot :: getCommander (void) {return command;}

PrologServiceClass * PrologRoot :: getServiceClass (void) {
	if (root == NULL) return NULL;
	return root -> getServiceClass ();
}

PrologServiceClass * PrologRoot :: getServiceClass (char * name) {
	if (root == NULL) return NULL;
	return root -> getServiceClass (name);
}

PrologDirectory * PrologRoot :: createDirectory (char * name) {
	root = new PrologDirectory (name, root);
	return root;
}

PrologDirectory * PrologRoot :: createDirectory (char * name, PrologServiceClass * service) {
	root = new PrologDirectory (name, root, service);
	return root;
}

PrologDirectory * PrologRoot :: searchDirectory (char * name) {
	PrologDirectory * ret = root;
	while (ret != NULL) {
		if (strcmp (ret -> directoryName, name) == 0) return ret;
		ret = ret -> next;
	}
	return NULL;
}

PrologDirectory * PrologRoot :: getRoot (void) {return root;}

PrologAtom * PrologRoot :: search (char * name) {
	PrologAtom * ret = NULL;
	PrologDirectory * sub = root;
	while (sub != NULL) {
		ret = sub -> searchAtom (name);
		if (ret != NULL) return ret;
		sub = sub -> next;
	}
	return NULL;
}

int PrologRoot :: list (char * area, int ind) {
	if (root == NULL) return ind;
	return root -> names (area, ind);
}

int PrologRoot :: list (char * name, char * area, int ind) {
	PrologDirectory * sub = searchDirectory (name);
	if (sub == NULL) return ind;
	return sub -> list (area, ind);
}

void PrologRoot :: close (void) {
	if (root == NULL) return;
	root = root -> close ();
}

bool PrologRoot :: drop (void) {
	if (root == NULL) return false;
	if (root -> next == NULL) return false;
	PrologDirectory * to_delete = root;
	root = root -> next;
	to_delete -> next = NULL;
	delete to_delete;
	return true;
}

bool PrologRoot :: drop (char * name) {
	if (root == NULL) return false;
	PrologDirectory * to_delete = root;
	if (strcmp (root -> directoryName, name) == 0) {
		root = root -> next;
		to_delete -> next = NULL;
		delete to_delete;
		return true;
	}
	PrologDirectory * sub = root;
	while (sub -> next != NULL) {
		if (strcmp (sub -> next -> directoryName, name) == 0) {
			to_delete = sub -> next;
			sub -> next = sub -> next -> next;
			to_delete -> next = NULL;
			delete to_delete;
			return true;
		}
		sub = sub -> next;
	}
	return false;
}

PrologAtom * PrologRoot :: createAtom (char * name) {
	if (root == NULL) return NULL;
	return root -> createAtom (name);
}

bool PrologRoot :: removeAtom (PrologAtom * atom) {
	if (root == NULL) return false;
	return root -> removeAtom (atom);
}

PrologElement * PrologRoot :: earth (void) {return new PrologElement ();}
PrologElement * PrologRoot :: pair (PrologElement * left, PrologElement * right) {
	PrologElement * ret = new PrologElement ();
	ret -> setPair (left, right);
	return ret;
}
PrologElement * PrologRoot :: var (int num) {
	PrologElement * ret = new PrologElement ();
	ret -> setVar (num);
	return ret;
}
PrologElement * PrologRoot :: atom (PrologAtom * atom) {
	PrologElement * ret = new PrologElement ();
	ret -> setAtom (atom);
	return ret;
}
PrologElement * PrologRoot :: atomC (char * name) {
	PrologElement * ret = new PrologElement ();
	PrologAtom * atom = search (name);
	if (atom == NULL) atom = root -> createAtom (name);
	ret -> setAtom (atom);
	return ret;
}
PrologElement * PrologRoot :: atom (char * name) {
	PrologAtom * atom = search (name);
	if (atom == NULL) {
		message ("Atom not found:", name);
		return NULL;
	}
	PrologElement * ret = new PrologElement ();
	ret -> setAtom (atom);
	return ret;
}
PrologElement * PrologRoot :: atom (char * directory, char * name) {
	PrologDirectory * dir = searchDirectory (directory);
	if (dir == NULL) {
		message ("Qualifying directory not found:", directory);
		return NULL;
	}
	PrologAtom * atom = dir -> searchAtom (name);
	if (atom == NULL) {
		message ("Qualified atom not found:", name);
		return NULL;
	}
	PrologElement * ret = new PrologElement ();
	ret -> setAtom (atom);
	return ret;
}
PrologElement * PrologRoot :: slash (void) {
	PrologElement * ret = new PrologElement ();
	ret -> setSlash ();
	return ret;
}
PrologElement * PrologRoot :: fail (void) {
	PrologElement * ret = new PrologElement ();
	ret -> setFail ();
	return ret;
}
PrologElement * PrologRoot :: text (char * str) {
	PrologElement * ret = new PrologElement ();
	ret -> setText (str);
	return ret;
}
PrologElement * PrologRoot :: head (void * obj) {
	PrologElement * ret = new PrologElement ();
	ret -> setHead (obj);
	return ret;
}
PrologElement * PrologRoot :: integer (int i) {
	PrologElement * ret = new PrologElement ();
	ret -> setInteger (i);
	return ret;
}
PrologElement * PrologRoot :: Double (double d) {
	PrologElement * ret = new PrologElement ();
	ret -> setDouble (d);
	return ret;
}

bool PrologRoot :: Private (char * name) {
	if (root == NULL) return false;
	PrologAtom * atom = root -> searchPrivateAtom (name); //search (name);
	if (atom == NULL) return false;
	atom -> Private ();
	return true;
}

bool PrologRoot :: Protect (char * name) {
	if (root == NULL) return false;
	PrologAtom * atom = root -> searchPrivateAtom (name); //search (name);
	if (atom == NULL) return false;
	atom -> Protect ();
	return true;
}

PrologElement * PrologRoot :: getClausePointer (PrologAtom * atom , int position) {
	if (atom == NULL) return NULL;
	if (atom -> firstClause == NULL) return NULL;
	PrologElement * ret = atom -> firstClause;
	while (position > 0) {
		ret = ret -> getLeft ();
		if (ret == NULL) return NULL;
		ret = ret -> getLeft ();
		if (ret == NULL) return NULL;
		ret = (PrologElement *) ret -> getHead ();
		if (ret == NULL) return NULL;
		position--;
	}
	return ret;
}

PrologElement * PrologRoot :: getNextClausePointer (PrologElement * clause) {
	PrologElement * ret = clause -> getLeft ();
	if (ret == NULL) return NULL;
	ret = ret -> getLeft ();
	if (ret == NULL) return NULL;
	ret = (PrologElement *) ret -> getHead ();
	if (ret == NULL) return NULL;
	return ret;
}

int PrologRoot :: listAtom (char * name, char * area, int ind) {
	PrologAtom * atom = search (name);
	if (atom == NULL) {
		ind = area_cat (area, ind, name);
		ind = area_cat (area, ind, " not found.");
		ind = area_cat (area, ind, new_line_caption);
		return ind;
	}
	PrologElement * el = atom -> firstClause;
	while (el != NULL) {
		ind = area_cat (area, ind, left_caption);
		ind = area_cat (area, ind, left_caption);
		ind = area_cat (area, ind, name);
		ind = getRightCaption (el -> getLeft () -> getRight (), area, ind);
		ind = area_cat (area, ind, right_caption);
		ind = getRightCaption (el -> getRight (), area, ind);
		ind = area_cat (area, ind, right_caption);
		ind = area_cat (area, ind, new_line_caption);
		el = getNextClausePointer (el);
	}
	return ind;
}

int PrologRoot :: attachClause (PrologElement * clause) {
	PrologAtom * atom = NULL;
	PrologElement * sub = NULL;
	if (! clause -> isPair ()) return 1;
	sub = clause -> getLeft ();
	if (! sub -> isPair ()) return 2;
	sub = sub -> getLeft ();
	if (! sub -> isAtom ()) return 3;
	atom = sub -> getAtom ();
	if (atom -> Protected) return 4;
	sub -> setHead (NULL);
	if (atom -> firstClause == NULL) {
		atom -> firstClause = clause;
		return 0;
	}
	PrologElement * head = atom -> firstClause;
	if (! head -> isPair ()) return 5;
	head = head -> getLeft ();
	if (! head -> isPair ()) return 6;
	head = head -> getLeft ();
	if (! head -> isHead ()) return 7;
	while (head -> getHead () != NULL) {
		head = (PrologElement *) head -> getHead ();
		if (! head -> isPair ()) return 8;
		head = head -> getLeft ();
		if (! head -> isPair ()) return 9;
		head = head -> getLeft ();
		if (! head -> isHead ()) return 10;
	}
	head -> setHead (clause);
	return 0;
}

int PrologRoot :: attachClause (PrologElement * clause, int position) {
	PrologAtom * atom = NULL;
	PrologElement * sub = NULL;
	if (! clause -> isPair ()) return 1;
	sub = clause -> getLeft ();
	if (! sub -> isPair ()) return 2;
	sub = sub -> getLeft ();
	if (! sub -> isAtom ()) return 3;
	atom = sub -> getAtom ();
	if (atom -> Protected) return 4;
	if ((atom -> firstClause == NULL) || (position <= 0)) {
		sub -> setHead (atom -> firstClause);
		atom -> firstClause = clause;
		return 0;
	}
	position--;
	PrologElement * head = atom -> firstClause;
	if (! head -> isPair ()) return 5;
	head = head -> getLeft ();
	if (! head -> isPair ()) return 6;
	head = head -> getLeft ();
	if (! head -> isHead ()) return 7;
	while ((head -> getHead () != NULL) && (position > 0)) {
		position--;
		head = (PrologElement *) head -> getHead ();
		if (! head -> isPair ()) return 8;
		head = head -> getLeft ();
		if (! head -> isPair ()) return 9;
		head = head -> getLeft ();
		if (! head -> isHead ()) return 10;
	}
	sub -> setHead (head -> getHead ());
	head -> setHead (clause);
	return 0;
}

extern PrologServiceClass * load_plugin_service_class (char * directory, char * name);
PrologServiceClass * PrologRoot :: loadServiceClass (char * name) {
	PrologServiceClass * service_class = NULL;
	if (strcmp (name, "prolog.studio") == 0) service_class = new PrologStudio ();
	if (service_loader != NULL && service_class == NULL) service_class = service_loader -> load (name);
	if (service_class == NULL) service_class = load_plugin_service_class ("", name);
	PrologString * search_directory = search_directories;
	while (search_directory != NULL && service_class == NULL) {
		service_class = load_plugin_service_class (search_directory -> text, name);
		search_directory = search_directory -> next;
	}
	return service_class;
}

void PrologRoot :: message (char * head) {
	if (command == NULL) {printf ("%s []\n", head); return;}
	command -> print (head);
	command -> print (" []\n");
}

void PrologRoot :: message (char * head, int ind) {
	if (command == NULL) {printf ("%s [%i]\n", head, ind); return;}
	PROLOG_STRING text;
	sprintf (text, "%s [%i]\n", head, ind);
	command -> print (text);
}

void PrologRoot :: message (char * head, int ind, int sub) {
	if (command == NULL) {printf ("%s [%i %i]\n", head, ind, sub); return;}
	PROLOG_STRING text;
	sprintf (text, "%s [%i %i]\n", head, ind, sub);
	command -> print (text);
}

void PrologRoot :: message (char * head, char * variable) {
	if (command == NULL) {printf ("%s [%s]\n", head, variable); return;}
	command -> print (head); command -> print (" ["); command -> print (variable); command -> print ("]\n");
}

void PrologRoot :: print (char * text) {
	if (command == NULL) printf ("%s", text);
	else command -> print (text);
}

void PrologRoot :: print_character (int i) {
	if (i >= 0) {
		if (command == 0) {
			if (i <= 0xff) putchar (i);
			else if (i <= 0xffff) {putchar (i >> 8); putchar (i & 0xff);}
			else if (i <= 0xffffff) {putchar (i >> 16); putchar ((i >> 8) & 0xff); putchar (i & 0xff);}
			else {putchar (i >> 24); putchar ((i >> 16) & 0xff); putchar ((i >> 8) & 0xff); putchar (i & 0xff);}
		} else {
			if (i <= 0xff) command -> print (i);
			else if (i <= 0xffff) {command -> print (i >> 8); command -> print (i & 0xff);}
			else if (i <= 0xffffff) {command -> print (i >> 16); command -> print ((i >> 8) & 0xff); command -> print (i & 0xff);}
			else {command -> print (i >> 24); command -> print ((i >> 16) & 0xff); command -> print ((i >> 8) & 0xff); command -> print (i & 0xff);}
		}
		return;
	}
	i = -i;
	if (i <= 0x7f) {if (command == 0) putchar (i); else command -> print (i); return;}
	if (i <= 0x7ff) {
		int msb = 0xc0 | (i >> 6);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (msb); putchar (lsb);}
		else {command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0xffff) {
		int hsb = 0xe0 | (i >> 12);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0x1fffff) {
		int xlsb = 0xf0 | (i >> 18);
		int hsb = 0x80 | ((i >> 12) & 0x3f);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (xlsb); putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (xlsb); command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0x3ffffff) {
		int xmsb = 0xf8 | (i >> 24);
		int xlsb = 0x80 | ((i >> 18) & 0x3f);
		int hsb = 0x80 | ((i >> 12) & 0x3f);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (xmsb); putchar (xlsb); putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (xmsb); command -> print (xlsb); command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
	if (i <= 0x7fffffff) {
		int xhsb = 0xfc | (i >> 30);
		int xmsb = 0x80 | ((i >> 24) & 0x3f);
		int xlsb = 0x80 | ((i >> 18) & 0x3f);
		int hsb = 0x80 | ((i >> 12) & 0x3f);
		int msb = 0x80 | ((i >> 6) & 0x3f);
		int lsb = 0x80 | (i & 0x3f);
		if (command == 0) {putchar (xhsb); putchar (xmsb); putchar (xlsb); putchar (hsb); putchar (msb); putchar (lsb);}
		else {command -> print (xhsb); command -> print (xmsb); command -> print (xlsb); command -> print (hsb); command -> print (msb); command -> print (lsb);}
		return;
	}
}

int PrologRoot :: get_character (void) {
	if (command == NULL) return getchar ();
	return command -> get ();
}

void PrologRoot :: setResourceLoader (PrologResourceLoader * resource_loader) {this -> resource_loader = resource_loader;}
void PrologRoot :: setServiceClassLoader (PrologServiceClassLoader * service_loader) {this -> service_loader = service_loader;}

int PrologRoot :: resolution (PrologElement * query) {
	// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query
	// query is a clause, which is a list anyway
	// the head of the query is a return interface and can be any term
	// the rest of the query are calls
	// after successfull processing only the interface
	// is left from the original query matched with the result
	// after failure the query remains unchanged
	// query egzample: [[*x *y *z] [c1 *x] [c2 *y] [c3 *z]]
//	if (active_main) {
//		print ("Instructions dropped: ");
//		AREA area;
//		getValue (NULL, area, 0);
//		print (area);
//		print (new_line_caption);
//		return 4;
//	}
	PrologResolution resolution (this);
	return resolution . resolution (query);
}

bool PrologRoot :: resolutionHead (char * directory) {
	if (strstr (directory, ".prc") == 0) {
		PrologLoader loader (this);
		if (! loader . load ("studio.prc")) return false;
		if (main_query != 0) removeMainQuery ();
		auto_atoms = true;
		main_query = pair (pair (atom ("bootstrap"), pair (text (directory), earth ())), earth ());
		main_query = pair (head (0), main_query);
		return true;
	}
	if (strcmp (directory, "") == 0) return resolutionHead ();
	PrologLoader * loader = new PrologLoader (this);
	bool ret = loader -> load (directory);
	delete loader;
	return ret;
}

bool PrologRoot :: resolutionHead (void) {
	PrologLoader * loader = new PrologLoader (this);
	if (! loader -> load ("studio.prc")) {delete loader; return false;}
	delete loader;
	if (main_query != NULL) {
		AREA area;
		getValue (main_query, area, 0);
		message ("Instructions dropped <studio.prc>", area);
		removeMainQuery ();
	}
	main_query = pair (pair (atom ("command"), earth ()), earth ());
	main_query = pair (head (NULL), main_query);
	return true;
}

int PrologRoot :: resolution (char * directory) {
	if (! resolutionHead (directory)) return 4;
	int ctrl = resolution (main_query);
	removeMainQuery ();
	return ctrl;
}

int PrologRoot :: resolution (void) {
	if (! resolutionHead ()) return 4;
	int ctrl = resolution (main_query);
	removeMainQuery ();
	return ctrl;
}

void PrologRoot :: removeMainQuery (void) {
	if (main_query != NULL) delete main_query;
	main_query = NULL;
}

