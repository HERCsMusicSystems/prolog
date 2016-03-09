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

PrologLoader :: PrologLoader (PrologRoot * root) {
	instructions = 0;
	drop_main = false;
	echo = false;
	reload = false;
	old_caption_id = 0;
	old_auto_atoms = false;
	setRoot (root);
}

PrologLoader :: ~ PrologLoader (void) {}

void PrologLoader :: message (char * text) {root -> print (text); root -> print (root -> new_line_caption);}
void PrologLoader :: message_v (char * text, char * variable) {
	root -> print (text);
	root -> print (" [");
	root -> print (variable);
	root -> print ("]");
	root -> print (root -> new_line_caption);
}
void PrologLoader :: close (void) {
	ri = NULL;
	if (fi != NULL) fclose (fi);
	fi = NULL;
}
int PrologLoader :: move_z (void) {
	int ch;
	if (ri != NULL) {
		ch = (int) ((unsigned char) (* ri++));
		if (ch == '\0') {
			ri = NULL;
			return -1;
		}
		if (echo && ch > 0) root -> print_character (ch);
		return ch;
	}
	if (fi == NULL) return -1;
	ch = fgetc (fi);
	if (echo && ch > 0) root -> print_character (ch); //putchar (ch);
	return ch;
}

bool PrologLoader :: load (char * file_name) {
	drop_main = false;
	old_caption_id = root -> captionId ();
	old_auto_atoms = root -> autoAtoms ();
	root -> setCaptions (0, false);
	bool ret = LOAD (file_name);
	clear_context ();
	root -> setCaptions (old_caption_id, old_auto_atoms);
	return ret;
}

bool PrologLoader :: load (char * file_name, int captions, bool atoms) {
	drop_main = false;
	old_caption_id = root -> captionId ();
	old_auto_atoms = root -> autoAtoms ();
	root -> setCaptions (captions, atoms);
	bool ret = LOAD (file_name);
	clear_context ();
	root -> setCaptions (old_caption_id, old_auto_atoms);
	return ret;
}

bool PrologLoader :: load_without_main (char * file_name) {
	drop_main = true;
	old_caption_id = root -> captionId ();
	old_auto_atoms = root -> autoAtoms ();
	root -> setCaptions (0, false);
	bool ret = LOAD (file_name);
	clear_context ();
	root -> setCaptions (old_caption_id, old_auto_atoms);
	return ret;
}

extern char * load_plugin_module (char * name);
bool PrologLoader :: LOAD (char * file_name) {
	bool ret;
	AREA command;
	ri = NULL;
	fi = NULL;
	if (root -> resource_loader != NULL) ri = root -> resource_loader -> load (file_name);
	if (ri == NULL) {
		fi = fopen (file_name, "rb");
		PrologString * root_directory = root -> search_directories;
		while (root_directory != NULL && fi == NULL) {
			sprintf (command, "%s%s", root_directory -> text, file_name);
			fi = fopen (command, "rb");
			root_directory = root_directory -> next;
		}
		if (fi == NULL) {
			ri = load_plugin_module (file_name);
			if (ri == 0) {message_v ("File not found: ", file_name); return false;}
		}
	}
	get_symbol ();
	PrologDirectory * import;
	clear_context ();
//	if (root != NULL) search_context = root -> root;
	PrologDirectory * user_directory = NULL;
	if (root != NULL) user_directory = root -> root;
	while (symbol_control == 11 && strcmp (root -> import_caption, symbol) == 0) {
		get_symbol ();
		switch (symbol_control) {
		case 8: if (root -> searchDirectoryNotFound (symbol)) root -> addSearchDirectory (symbol); break;
		case 11:
			import = root -> searchDirectory (symbol);
			if (import == NULL) {
				PrologLoader * loader = new PrologLoader (root);
				area_cat (command, 0, symbol);
				area_cat (command, ".prc");
				if (drop_main) ret = loader -> load_without_main (command);
				else ret = loader -> load (command);
				delete loader;
				if (! ret) {message_v ("Module not loaded: ", command); close (); return false;}
				import = root -> searchDirectory (symbol);
			}
			if (import != NULL) search_context = import -> duplicate (search_context);
			break;
		default: message ("Syntax error: import module or search path expected."); close (); return false;
		}
		get_symbol ();
	}
	while (symbol_control == 31) {
		if (strcmp (symbol, "uap32") == 0) root -> set_uap32_captions ();
		if (strcmp (symbol, "standard") == 0) root -> set_standard_captions ();
		if (strcmp (symbol, "edinburg") == 0) root -> set_edinburg_captions ();
		if (strcmp (symbol, "marseille") == 0) root -> set_marseille_captions ();
		if (strcmp (symbol, "functional") == 0) root -> set_functional_captions ();
		get_symbol ();
	}
	if (symbol_control != 11 || strcmp (root -> program_caption, symbol) != 0) {message ("Syntax error: program header expected."); close (); return false;}
	get_symbol ();
	if (symbol_control != 11) {message ("Syntax error: program name expected."); close (); return false;}
	PROLOG_STRING program_name;
	prolog_string_copy (program_name, symbol);
	PrologServiceClass * service_class = NULL;
	if (reload) root -> drop (program_name);
	else if (root -> searchDirectory (program_name) != NULL) {close (); return true;}
	get_symbol ();
	PrologDirectory * directory;
	if (symbol_control == 11 && strcmp (root -> machine_caption, symbol) == 0) {
		get_symbol ();
		if (symbol_control != 11 || strcmp (root -> if_atom_caption, symbol) != 0) {
			message ("Syntax error: machine assignment expected in program header.");
			close ();
			return false;
		}
		get_symbol ();
		if (symbol_control != 8) {message ("Syntax error: service class name expected."); close (); return false;}
		service_class = root -> loadServiceClass (symbol);
		if (service_class == NULL) {message_v ("Service class crashed or not found: ", symbol); close (); return false;}
		get_symbol ();
		directory = root -> createDirectory (program_name, service_class);
		service_class -> init (root, directory);
	} else directory = root -> createDirectory (program_name);
	root -> auto_atoms = false;
	switch (symbol_control) {
	case 11:
		if (strcmp (root -> auto_atoms_caption, symbol) != 0) {
			message ("Syntax error: auto expected.");
			root -> drop ();
			close ();
			return false;
		}
		root -> auto_atoms = true;
		get_symbol ();
		break;
	case 6: get_symbol (); break;
	case 1:
		get_symbol ();
		while (symbol_control != 2) {
			switch (symbol_control) {
			case 11: root -> createAtom (symbol); break;
			case 21: root -> createAtom (symbol); break;
			default:
				message ("Syntax error: atom expected.");
				root -> drop ();
				close ();
				return false;
				break;
			}
			get_symbol ();
			if (strlen (root -> separator_caption) > 0) {
				if (symbol_control != 23 && symbol_control != 2) {
					message ("Syntax error: separator missing.");
					root -> drop ();
					close ();
					return false;
				}
				if (symbol_control == 23) get_symbol ();
			}
		}
		get_symbol ();
		break;
	default: message ("Syntax error: atome list expected."); root -> drop (); close (); return false;
	}
	//
	if (directory != NULL) search_context = directory -> duplicate (search_context);
	PrologDirectory * dt = search_context;
	if (dt != NULL) {
		while (dt -> next != NULL) dt = dt -> next;
		dt -> next = user_directory;
	}
//	message (file_name);
//	dt = search_context;
//	while (dt != NULL) {message (dt -> directoryName); dt = dt -> next;}
//	message ("===");
//	dt = root -> root;
//	while (dt != NULL) {message (dt -> directoryName); dt = dt -> next;}
//	message ("");
	//
	PrologElement * clause = NULL;
	PrologAtom * atom = NULL;
	PrologNativeCode * native_code = NULL;
	PrologResolution * resolution = NULL;
	PrologAtom * preprocessor = NULL;
	PrologDirectory * preprocessor_directory;
	while (true) {
		switch (symbol_control) {
		case 11:
			if (strcmp (root -> machine_caption, symbol) == 0) {
				get_symbol ();
				if (symbol_control != 11) {
					message ("Syntax error: atom expected.");
					root -> drop ();
					close ();
					return false;
				}
				atom = searchAtomC (symbol); //root -> search (symbol);
				if (atom == NULL) {
					message_v ("Unknown atom: ", symbol);
					root -> drop ();
					close ();
					return false;
				}
				if (atom -> getMachine () != 0) {
					message_v ("Atom has already machine code attached: ", symbol);
					root -> drop ();
					close ();
					return false;
				}
				get_symbol ();
				if (symbol_control != 11 || strcmp (root -> if_atom_caption, symbol) != 0) {
					message ("Syntax error: machine assignment expected.");
					root -> drop ();
					close ();
					return false;
				}
				get_symbol ();
				if (symbol_control != 8) {
					message ("Syntax error: machine name expected.");
					root -> drop ();
					close ();
					return false;
				}
				if (service_class == NULL) {
					message ("Service class missing.");
					root -> drop ();
					close ();
					return false;
				}
				native_code = service_class -> getNativeCode (symbol);
				if (native_code == NULL) {
					message_v ("Unknown native class: ", symbol);
					root -> drop ();
					close ();
					return false;
				}
				atom -> setMachine (native_code);
				get_symbol ();
				break;
			}
//		case 11:
			if (strcmp (root -> end_caption, symbol) == 0) {
				get_symbol ();
				clause = NULL;
				if (symbol_control == 11 && strcmp (root -> if_atom_caption, symbol) == 0) {
					clause = readElement ();
					if (clause == NULL) {
						// suspiciouse drop 1
						root -> drop ();
						close ();
						return false;
					}
					get_symbol ();
				}
				if (symbol_control == 21) {
					if (echo) message ("");
					root -> close ();
					if (clause != NULL) {
						if (instructions != 0 && instructions -> isPair ()) instructions -> setRight (clause);
						else if (drop_main) {
							root -> getValue (clause, command, 0);
							root -> message ("Illegal instructions dropped", command);
							delete clause;
						} else {
							if (root -> main_query != NULL) {
								root -> getValue (root -> main_query, command, 0);
								root -> message ("Instructions dropped", command);
								root -> removeMainQuery ();
							}
							clause = root -> pair (root -> head (NULL), clause);
							root -> main_query = clause;
						}
					}
					close ();
					return true;
				}
				message ("Syntax error: dot expected.");
				root -> drop ();
				close ();
				return false;
			}
			if (strcmp (root -> protect_caption, symbol) == 0) {
				get_symbol ();
				if (symbol_control == 6) {get_symbol (); break;}
				if (symbol_control != 1) {
					message ("Syntax error: atome list expected.");
					root -> drop ();
					close ();
					return false;
				}
				get_symbol ();
				while (symbol_control != 2) {
					if (symbol_control != 11) {
						message ("Syntax error: atom expected.");
						root -> drop ();
						close ();
						return false;
					}
					if (! root -> Protect (symbol)) {
						root -> message ("Can not protect unknown atom:", symbol);
						root -> drop ();
						close ();
						return false;
					}
					get_symbol ();
					if (strlen (root -> separator_caption) > 0) {
						if (symbol_control != 23 && symbol_control != 2) {
							message ("Syntax error: separator missing.");
							root -> drop ();
							close ();
							return false;
						}
						if (symbol_control == 23) get_symbol ();
					}
				}
				get_symbol ();
				break;
			}
			if (strcmp (root -> private_caption, symbol) == 0) {
				get_symbol ();
				if (symbol_control == 6) {get_symbol (); break;}
				if (symbol_control != 1) {
					message ("Syntax error: atome list expected.");
					root -> drop ();
					close ();
					return false;
				}
				get_symbol ();
				while (symbol_control != 2) {
					if (symbol_control != 11) {
						message ("Syntax error: atom expected.");
						root -> drop ();
						close ();
						return false;
					}
					if (! root -> Private (symbol)) {
						root -> message ("Can not lock unknown atom as private:", symbol);
						root -> drop ();
						close ();
						return false;
					}
					get_symbol ();
					if (strlen (root -> separator_caption) > 0) {
						if (symbol_control != 23 && symbol_control != 2) {
							message ("Syntax error: separator missing.");
							root -> drop ();
							close ();
							return false;
						}
						if (symbol_control == 23) get_symbol ();
					}
				}
				get_symbol ();
				break;
			}
			if (strcmp (root -> preprocessor_caption, symbol) == 0) {
				get_symbol ();
				if (symbol_control != 11 && strcmp (root -> if_atom_caption, symbol) != 0) {
					message ("Syntax error: preprocessor assignment expected.");
					root -> drop ();
					close ();
					return false;
				}
				get_symbol ();
				if (symbol_control == 6) {
//					directory -> setPreprocessor (NULL);
					preprocessor = NULL;
					get_symbol ();
					break;
				}
				if (symbol_control != 11) {
					if (symbol_control == 22) {
						get_symbol ();
						if (symbol_control != 11) {
							message ("Syntax error: directory name expected in qualified preprocessor name.");
							root -> drop ();
							close ();
							return false;
						}
						preprocessor_directory = root -> searchDirectory (symbol);
						if (preprocessor_directory == NULL) {
							message_v ("Directory not found in qualified preprocessor name:", symbol);
							root -> drop ();
							close ();
							return false;
						}
						get_symbol ();
						if (symbol_control != 21) {
							message ("Syntax error: dot expected in qualified preprocessor name.");
							root -> drop ();
							close ();
							return false;
						}
						get_symbol ();
						if (symbol_control != 11) {
							message ("Syntax error: qualified preprocessor name expected.");
							root -> drop ();
							close ();
							return false;
						}
						atom = preprocessor_directory -> searchAtom (symbol);
					} else {
						message ("Syntax error: atom expected.");
						root -> drop ();
						close ();
						return false;
					}
				} else {
					atom = searchAtom (symbol); //root -> search (symbol);
				}
				if (atom == NULL) {
					message_v ("Unknown atom: ", symbol);
					root -> drop ();
					close ();
					return false;
				}
//				directory -> setPreprocessor (atom);
				preprocessor = atom;
				get_symbol ();
				break;
			}
			if (strcmp (root -> auto_caption, symbol) == 0) {
				get_symbol ();
				if (symbol_control != 11 && strcmp (root -> if_atom_caption, symbol) != 0) {
					message ("Syntax error: auto assignment expected.");
					root -> drop ();
					close ();
					return false;
				}
				clause = readElement ();
				// suspicious drop 2
				if (clause == NULL) {root -> drop (); close (); return false;}
				clause = root -> pair (root -> head (NULL), clause);
				//root -> resolution (clause);
				resolution = new PrologResolution (root);
				resolution -> resolution (clause);
				delete resolution;
				delete clause;
				get_symbol ();
				break;
			}
		case 1:
//			atom = root -> getPreprocessor ();
			if (preprocessor != NULL) {
				clause = readElement ();
				if (clause == NULL) {root -> drop (); close (); return false;}
				clause = readRightSide (clause, false);
				if (clause == NULL) {root -> drop (); close (); return false;}
				get_symbol ();
				clause = root -> pair (root -> var (0), root -> pair (root -> pair (root -> atom (preprocessor), root -> pair (clause, root -> earth ())), root -> earth ()));
				resolution = new PrologResolution (root);
				resolution -> resolution (clause);
				delete resolution;
				delete clause;
				break;
			}
			clause = readClause ();
			get_symbol ();
			// suspicious drop 3
			if (clause == NULL) {root -> drop (); close (); return false;}
			if (root -> attachClause (clause) == 0) break;
			// suspicious drop 4
			root -> drop ();
			close ();
			return false;
		default: message ("Syntax error: at least clause expected."); root -> drop (); close (); return false;
		}
	}
}

char * PrologResourceLoader :: load (char * name) {return NULL;}
PrologServiceClass * PrologServiceClassLoader :: load (char * name) {return NULL;}
