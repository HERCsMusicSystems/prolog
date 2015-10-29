///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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

package Prolog;

import java . io . InputStream;
import java . io . FileInputStream;
import java . io . ByteArrayInputStream;
import java . util . concurrent . Semaphore;
import java . util . ArrayList;
import java . util . LinkedList;
import java . util . ListIterator;

class studio_code extends PrologNativeCode {
	public String name;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		System . out . println ("==============================> CALLED [" + name + "]");
		return false;
	}
	public studio_code (String name) {this . name = name;}
}

class pp extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {root . print (root . getValue (parameters . getLeft ())); parameters = parameters . getRight ();}
		return true;
	}
	public pp (PrologRoot root) {this . root = root;}
}

class pt extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {root . print (root . getTrueValue (parameters . getLeft ())); parameters = parameters . getRight ();}
		return true;
	}
	public pt (PrologRoot root) {this . root = root;}
}

class write extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isText ()) root . print (el . getText ());
			if (el . isInteger ()) root . print_character (el . getInteger ());
			while (el . isPair ()) {
				root . print (root . getValue (el . getLeft ()));
				el = el . getRight ();
			}
			parameters = parameters . getRight ();
		}
		return true;
	}
	public write (PrologRoot root) {this . root = root;}
};

class nl extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . print (root . new_line_caption); return true;}
	public nl (PrologRoot root) {this . root = root;}
}

class read extends PrologNativeCode {
	public PrologReader stdr;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		parameters . setText (stdr . getString ());
		return true;
	}
	public read (PrologReader stdr) {this . stdr = stdr;}
}

class readln extends PrologNativeCode {
	public PrologReader stdr;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		parameters . setText (stdr . readln ());
		return true;
	}
	public readln (PrologReader stdr) {this . stdr = stdr;}
}

class pr extends PrologNativeCode {
	public PrologReader stdr;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement dup = stdr . readElement ();
		if (dup != null) {parameters . setLeft (dup); return true;}
		return false;
	}
	public pr (PrologReader stdr) {this . stdr = stdr;}
}

class list extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {root . print (root . list () + root . new_line_caption); return true;}
		if (parameters . isVar ()) {
			PrologDirectory dr = root . root;
			while (dr != null) {parameters . setPair (root . text (dr . directoryName), root . earth ()); parameters = parameters . getRight (); dr = dr . next;}
			return true;
		}
		if (! parameters . isPair ()) return false;
		PrologElement ret = parameters . getRight ();
		parameters = parameters . getLeft ();
		if (parameters . isVar ()) {parameters . setText (root . list ()); return true;}
		if (parameters . isAtom ()) {
			PrologAtom atom = parameters . getAtom ();
			if (atom . getMachine () != null) {
				String area = atom . name () + " " + root . if_atom_caption + " " + root . machine_caption;
				if (ret . isVar ()) {ret . setPair (root . text (area), root . earth ()); return true;}
				area += root . new_line_caption;
				if (ret . isPair ()) ret . getLeft () . setText (area); else root . print (area);
				return true;
			}
			PrologElement clausa = atom . firstClause;
			if (ret . isVar ()) {
				if (clausa == null) {ret . setEarth (); return true;}
				while (clausa != null) {
					PrologElement dup = clausa . duplicate ();
					dup . getLeft () . getLeft () . setAtom (atom);
					ret . setPair (dup, root . earth ());
					ret = ret . getRight ();
					clausa = (PrologElement) clausa . getLeft () . getLeft () . getHead ();
				}
				return true;
			}
			String area = "";
			while (clausa != null) {
				PrologElement dup = clausa . duplicate ();
				dup . getLeft () . getLeft () . setAtom (atom);
				area += root . getValue (dup) + root . new_line_caption;
				clausa = (PrologElement) clausa . getLeft () . getLeft () . getHead ();
			}
			if (ret . isPair ()) ret . getLeft () . setText (area); else root . print (area);
			return true;
		}
		if (parameters . isText ()) {
			if (ret . isVar ()) {
				PrologDirectory dr = root . searchDirectory (parameters . getText ());
				if (dr == null) return false;
				PrologAtom atom = dr . firstAtom;
				ret . setEarth ();
				while (atom != null) {ret . setPair (root . atom (atom), root . earth ()); ret = ret . getRight (); atom = atom . next;}
				return true;
			}
			String area = root . list (parameters . getText ());
			if (ret . isPair ()) ret . getLeft () . setText (area); else root . print (area + root . new_line_caption);
		}
		return true;
	}
	public list (PrologRoot root) {this . root = root;}
}

class set_uap32_captions extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_uap32_captions (); return true;}
	public set_uap32_captions (PrologRoot root) {this . root = root;}
}
class set_standard_captions extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_standard_captions (); return true;}
	public set_standard_captions (PrologRoot root) {this . root = root;}
}
class set_edinburg_captions extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_edinburg_captions (); return true;}
	public set_edinburg_captions (PrologRoot root) {this . root = root;}
}
class set_marseille_captions extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_marseille_captions (); return true;}
	public set_marseille_captions (PrologRoot root) {this . root = root;}
}
class set_functional_captions extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_functional_captions (); return true;}
	public set_functional_captions (PrologRoot root) {this . root = root;}
}
class auto_atoms extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_auto_atoms (); return true;}
	public auto_atoms (PrologRoot root) {this . root = root;}
}
class scripted_atoms extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {root . set_scripted_atoms (); return true;}
	public scripted_atoms (PrologRoot root) {this . root = root;}
}

class CL extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		if (parameters . getLeft () . isAtom ()) {
			PrologAtom atom = parameters . getLeft () . getAtom ();
			parameters = parameters . getRight ();
			PrologElement e_counter = parameters;
			if (parameters . isPair ()) e_counter = parameters . getLeft ();
			if (e_counter . isVar () || ! parameters . isPair ()) {
				PrologElement clause = atom . firstClause;
				int counter = 0;
				while (clause != null) {counter++; clause = (PrologElement) clause . getLeft () . getLeft () . getHead ();}
				e_counter . setInteger (counter);
				return true;
			}
			if (! e_counter . isInteger ()) return false;
			if (! parameters . isPair ()) return false;
			PrologElement e_clause = parameters;
			parameters = parameters . getRight ();
			if (parameters . isEarth ()) {
				PrologElement clause = atom . firstClause;
				int counter = 0;
				while (clause != null) {counter++; clause = (PrologElement) clause . getLeft () . getLeft () . getHead ();}
				return e_counter . getInteger () == counter;
			}
			PrologElement dup = atom . firstClause;
			if (dup == null) return false;
			int ind = e_counter . getInteger ();
			while (ind > 0) {dup = (PrologElement) dup . getLeft () . getLeft () . getHead (); if (dup == null) return false; ind--;}
			dup = dup . duplicate ();
			dup . getLeft () . getLeft () . setAtom (atom);
			if (parameters . isVar ()) {e_clause . setRight (dup); return true;}
			parameters . setPair ();
			parameters . setLeft (dup);
			return true;
		}
		if (! parameters . getLeft () . isInteger ()) return false;
		int ind = parameters . getLeft () . getInteger ();
		parameters = parameters . getRight ();
		if (! parameters . isPair ()) return false;
		if (! parameters . getLeft () . isAtom ()) return false;
		PrologAtom atom = parameters . getLeft () . getAtom ();
		PrologElement e_clause = parameters;
		parameters = parameters . getRight ();
		PrologElement dup = atom . firstClause;
		if (dup == null) return false;
		while (ind > 0) {dup = (PrologElement) dup . getLeft () . getLeft () . getHead (); if (dup == null) return false; ind--;}
		dup = dup . duplicate ();
		dup . getLeft () . getLeft () . setAtom (atom);
		if (parameters . isVar ()) {e_clause . setRight (dup); return true;}
		parameters . setPair ();
		parameters . setLeft (dup);
		return true;
	}
}

class delcl extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologAtom atom = null;
		int ind = -1;
		while (parameters . isPair ()) {
			PrologElement e = parameters . getLeft ();
			if (e . isAtom ()) atom = e . getAtom ();
			else if (e . isInteger ()) ind = e . getInteger ();
			else return false;
			parameters = parameters . getRight ();
		}
		if (atom == null || ind < 0) return false;
		if (atom . Protected) return false;
		PrologElement dup = atom . firstClause;
		if (dup == null) return false;
		if (ind == 0) {atom . firstClause = (PrologElement) dup . getLeft () . getLeft () . getHead (); return true;}
		ind--;
		while (ind > 0) {dup = (PrologElement) dup . getLeft () . getLeft () . getHead (); if (dup == null) return false; ind--;}
		dup = dup . getLeft () . getLeft ();
		PrologElement next = (PrologElement) dup . getHead ();
		if (next == null) return false;
		dup . setHead ((PrologElement) next . getLeft () . getLeft () . getHead ());
		return true;
	}
}

class overwrite extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		if (! parameters . getLeft () . isInteger ()) return false;
		int ind = parameters . getLeft () . getInteger ();
		parameters = parameters . getRight ();
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft ();
		if (! parameters . isPair ()) return false;
		if (! parameters . getLeft () . isPair ()) return false;
		if (! parameters . getLeft () . getLeft () . isAtom ()) return false;
		PrologAtom atom = parameters . getLeft () . getLeft () . getAtom ();
		if (atom . Protected) return false;
		PrologElement dup = atom . firstClause;
		if (dup == null) return false;
		while (ind > 0) {dup = (PrologElement) dup . getLeft () . getLeft () . getHead (); if (dup == null) return false; ind--;}
		dup . getLeft () . setRight (parameters . getLeft () . getRight () . duplicate ());
		dup . setRight (parameters . getRight () . duplicate ());
		return true;
	}
}

class addcl0 extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement dup = parameters . getLeft () . duplicate ();
		return root . attachClause (parameters . getLeft () . duplicate (), 0) == 0;
	}
	public addcl0 (PrologRoot root) {this . root = root;}
}

class addcl extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement dup = parameters . getLeft () . duplicate ();
		parameters = parameters . getRight ();
		if (parameters . isEarth ()) {if (root . attachClause (dup) == 0) return true;}
		if (parameters . isPair ()) {
			parameters = parameters . getLeft ();
			if (parameters . isInteger ()) {
				if (root . attachClause (dup, parameters . getInteger ()) == 0) return true;
			}
		}
		return false;
	}
	public addcl (PrologRoot root) {this . root = root;}
}

class create_atom extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement left = parameters . getLeft ();
		if (left . isAtom ()) return true;
		if (left . isVar ()) {left . setAtom (new PrologAtom ()); return true;}
		if (! left . isText ()) return false;
		parameters = parameters . getRight ();
		if (parameters . isPair ()) {parameters . getLeft () . setAtom (new PrologAtom (left . getText ())); return true;}
		if (parameters . isVar ()) {parameters . setAtom (new PrologAtom (left . getText ())); return true;}
		root . createAtom (left . getText ());
		return true;
	}
	public create_atom (PrologRoot root) {this . root = root;}
}

class create_atoms extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isVar ()) el . setAtom (new PrologAtom ());
			else if (! el . isAtom ()) return false;
			parameters = parameters . getRight ();
		}
		return true;
	}
	public create_atoms (PrologRoot root) {this . root = root;}
}

/*
class search_atom : public PrologNativeCode {
public:
	PrologRoot * root;
	bool create;
	bool sub_code (PrologElement * parameters, char * name) {
		PrologAtom * atom = root -> search (name);
		if (atom == 0 && create) atom = root -> createAtom (name);
		if (atom == 0) return false;
		parameters -> setAtom (atom);
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * name = parameters -> getLeft (); if (! name -> isText ()) return false; parameters = parameters -> getRight ();
		if (parameters -> isVar ()) return sub_code (parameters, name -> getText ());
		if (! parameters -> isPair ()) return false;
		PrologElement * e = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (e -> isVar ()) return sub_code (e, name -> getText ());
		if (e -> isText ()) {
			PrologDirectory * dir = root -> searchDirectory (name -> getText ());
			if (dir == 0) return false;
			PrologAtom * atom = dir -> searchAtom (e -> getText ());
			if (atom == 0 && create) atom = dir -> createAtom (e -> getText ());
			if (atom == 0) return false;
			if (parameters -> isPair ()) parameters = parameters -> getLeft ();
			parameters -> setAtom (atom);
			return true;
		}
		return false;
	}
	search_atom (PrologRoot * root, bool create) {this -> root = root; this -> create = create;}
};

class unique_atoms : public PrologNativeCode {
public:
	PrologRoot * root;
	bool duplicate_found (PrologAtom * atom, PrologElement * * var) {
		if (atom -> Privated) return false;
		int duplicates = 0;
		PrologDirectory * dir = root -> root;
		while (dir != 0) {
			PrologAtom * at = dir -> firstAtom;
			while (at != 0) {
				if (at != atom) {
					if (! at -> Privated && strcmp (at -> name (), atom -> name ()) == 0) {
						if (* var == 0) printf ("@ %s . %s\n", dir -> name (), atom -> name ());
						else {
							PrologElement * el = * var;
							el -> setPair (); el = el -> getLeft ();
							el -> setPair (); el -> getLeft () -> setAtom (at); el = el -> getRight ();
							el -> setPair (); el -> getLeft () -> setText (dir -> name ());
							* var = (* var) -> getRight ();
						}
						duplicates++;
					}
				}
				at = at -> next;
			}
			dir = dir -> next;
		}
		return duplicates > 0;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		bool no_duplicates_found = true;
		bool need_processing = true;
		PrologElement * var = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isVar ()) {var = el; var -> setEarth ();}
			if (el -> isAtom ()) {
				need_processing = false;
				if (duplicate_found (el -> getAtom (), & var)) no_duplicates_found = false;
			}
			if (el -> isText ()) {
				need_processing = false;
				PrologDirectory * dir = root -> searchDirectory (el -> getText ());
				if (dir == 0) {printf ("Directory %s not found!\n", el -> getText ()); return false;}
				PrologAtom * atom = dir -> firstAtom;
				while (atom != 0) {
					if (duplicate_found (atom, & var)) no_duplicates_found = false;
					atom = atom -> next;
				}
			}
			parameters = parameters -> getRight ();
		}
		if (need_processing) {
			PrologDirectory * dir = root -> root;
			while (dir != 0) {
				PrologAtom * atom = dir -> firstAtom;
				while (atom != 0) {
					if (duplicate_found (atom, & var)) no_duplicates_found = false;
					atom = atom -> next;
				}
				dir = dir -> next;
			}
		}
		return var != 0 || no_duplicates_found;
	}
	unique_atoms (PrologRoot * root) {this -> root = root;}
};*/

class preprocessor extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologAtom preprocessor = root . getPreprocessor ();
		if (parameters . isEarth ()) {root . setPreprocessor (null); return true;}
		if (! parameters . isPair ()) {if (preprocessor != null) parameters . setAtom (preprocessor); else parameters . setEarth (); return true;}
		parameters = parameters . getLeft ();
		if (parameters . isAtom ()) {root . setPreprocessor (parameters . getAtom ()); return true;}
		if (parameters . isEarth ()) {root . setPreprocessor (null); return true;}
		if (parameters . isVar ()) {if (preprocessor != null) parameters . setAtom (preprocessor); else parameters . setEarth (); return true;}
		return false;
	}
	public preprocessor (PrologRoot root) {this . root = root;}
}

/*
class prompt : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (root -> getCommander () == NULL) return false;
		if (! parameters -> isPair ()) {
			char * current_prompt = root -> getCommander () -> getPrompt ();
			if (current_prompt == NULL) return false;
			parameters -> setText (current_prompt);
			return true;
		}
		parameters = parameters -> getLeft ();
		if (parameters -> isText ()) {root -> getCommander () -> setPrompt (parameters -> getText ()); return true;}
		if (parameters -> isVar ()) {
			char * current_prompt = root -> getCommander () -> getPrompt ();
			if (current_prompt == NULL) return false;
			parameters -> setText (current_prompt);
			return true;
		}
		return false;
	}
	prompt (PrologRoot * root) {this -> root = root;}
};

class query_stack : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologQuery * query = resolution -> getQuery ();
		while (query != NULL) {
			drop_element (root, query -> query);
			query = query -> stack;
		}
		return true;
	}
	query_stack (PrologRoot * root) {this -> root = root;}
};

class object_counter_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			drop_object_counter ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		AREA area;
		drop_object_counters (area);
		parameters -> setText (area);
		return true;
	}
};*/

class is_atom extends PrologNativeCode {public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isAtom ();}}
class is_integer extends PrologNativeCode {public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isInteger ();}}
class is_double extends PrologNativeCode {public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isDouble ();}}
class is_number extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft ();
		return parameters . isInteger () || parameters . isDouble ();
	}
}
class is_var extends PrologNativeCode {public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isVar ();}}
class is_head extends PrologNativeCode {public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isHead ();}}
class is_text extends PrologNativeCode {public boolean code (PrologElement parameters, PrologResolution resolution) {if (! parameters . isPair ()) return false; return parameters . getLeft () . isText ();}}
class has_machine extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft ();
		if (parameters . isAtom ()) return parameters . getAtom () . getMachine () != null;
		if (parameters . isText ()) {
			PrologDirectory dir = root . searchDirectory (parameters . getText ());
			if (dir == null) return false;
			return dir . service_class != null;
		}
		return false;
	}
	public has_machine (PrologRoot root) {this . root = root;}
}
	/*
class text_list : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		if (parameters -> getLeft () -> isText ()) {
			char * text = parameters -> getLeft () -> getText ();
			parameters = parameters -> getRight ();
			if (! parameters -> isPair ()) return false;
			parameters = parameters -> getLeft ();
			while (* text != '\0') {
				parameters -> setPair ();
				parameters -> getLeft () -> setInteger (* text++);
				parameters = parameters -> getRight ();
			}
			return true;
		}
		PrologElement * text_ptr = parameters -> getLeft ();
		AREA area;
		int area_ind = 0;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		while (parameters -> isPair ()) {
			if (! parameters -> getLeft () -> isInteger ()) return false;
			area_ind = area_cat (area, area_ind, (char) parameters -> getLeft () -> getInteger ());
			parameters = parameters -> getRight ();
		}
		text_ptr -> setText (area);
		return parameters -> isEarth ();
	}
};

*/

class term_reader extends PrologReader {
	public InputStream fi;
	public int move_z () {try {return fi . read ();} catch (Exception ex) {return -1;}}
	public term_reader (PrologRoot root, String text) {try {fi = new ByteArrayInputStream (text . getBytes ("UTF-8"));} catch (Exception ex) {fi = null;} setRoot (root);}
}

class text_term extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		if (parameters . getLeft () . isText ()) {
			term_reader tr = new term_reader (root, parameters . getLeft () . getText ());
			parameters = parameters . getRight ();
			parameters . setEarth ();
			PrologElement el = tr . readElement ();
			while (el != null) {
				parameters . setPair (el, root . earth ());
				parameters = parameters . getRight ();
				tr . reset_var_number ();
				el = tr . readElement ();
			}
			return true;
		}
		PrologElement el = parameters . getLeft ();
		parameters = parameters . getRight ();
		String area = null;
		while (parameters . isPair ()) {
			if (area != null) area += " ";
			else area = "";
			area += root . getValue (parameters . getLeft ());
			parameters = parameters . getRight ();
		}
		el . setText (area);
		return true;
	}
	public text_term (PrologRoot root) {this . root = root;}
}

/*
class e32 : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		int ind = 0;
		PrologElement * el = parameters -> getLeft ();
		if (el -> isInteger ()) {
			ind = el -> getInteger ();
			parameters = parameters -> getRight (); parameters -> setPair (); parameters -> getLeft () -> setInteger (ind & 0xff);
			parameters = parameters -> getRight (); parameters -> setPair (); parameters -> getLeft () -> setInteger ((ind >> 8) & 0xff);
			parameters = parameters -> getRight (); parameters -> setPair (); parameters -> getLeft () -> setInteger ((ind >> 16) & 0xff);
			parameters = parameters -> getRight (); parameters -> setPair (); parameters -> getLeft () -> setInteger ((ind >> 24) & 0xff);
			return true;
		}
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) {el -> setInteger (ind); return true;}
		PrologElement * ex = parameters -> getLeft ();
		if (! ex -> isInteger ()) return false;
		ind = ex -> getInteger ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) {el -> setInteger (ind); return true;}
		ex = parameters -> getLeft ();
		if (! ex -> isInteger ()) return false;
		ind += ex -> getInteger () << 8;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) {el -> setInteger (ind); return true;}
		ex = parameters -> getLeft ();
		if (! ex -> isInteger ()) return false;
		ind += ex -> getInteger () << 16;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) {el -> setInteger (ind); return true;}
		ex = parameters -> getLeft ();
		if (! ex -> isInteger ()) return false;
		ind += ex -> getInteger () << 24;
		el -> setInteger (ind);
		return true;
	}
};
*/

class add1 extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e = parameters . getLeft ();
		parameters = parameters . getRight ();
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (e . isInteger ()) {parameters . setInteger (e . getInteger () + 1); return true;}
		if (e . isDouble ()) {parameters . setDouble (e . getDouble () + 1.0); return true;}
		return false;
	}
}

class sub1 extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e = parameters . getLeft ();
		parameters = parameters . getRight ();
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (e . isInteger ()) {parameters . setInteger (e . getInteger () - 1); return true;}
		if (e . isDouble ()) {parameters . setDouble (e . getDouble () - 1.0); return true;}
		return false;
	}
}

class sum extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e1 = parameters . getLeft (); parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e2 = parameters . getLeft (); parameters = parameters . getRight ();
		PrologElement e3 = parameters . isPair () ? parameters . getLeft () : parameters;
		if (e1 . isInteger ()) {
			if (e2 . isInteger ()) {e3 . setInteger (e1 . getInteger () + e2 . getInteger ()); return true;}
			if (e2 . isDouble ()) {e3 . setDouble (e1 . getInteger () + e2 . getDouble ()); return true;}
			if (e3 . isInteger ()) {e2 . setInteger (e3 . getInteger () - e1 . getInteger ()); return true;}
			if (e3 . isDouble ()) {e2 . setDouble (e3 . getDouble () - e1 . getInteger ()); return true;}
			return false;
		}
		if (e1 . isDouble ()) {
			if (e2 . isInteger ()) {e3 . setDouble (e1 . getDouble () + e2 . getInteger ()); return true;}
			if (e2 . isDouble ()) {e3 . setDouble (e1 . getDouble () + e2 . getDouble ()); return true;}
			if (e3 . isInteger ()) {e2 . setDouble (e3 . getInteger () - e1 . getDouble ()); return true;}
			if (e3 . isDouble ()) {e2 . setDouble (e3 . getDouble () - e1 . getDouble ()); return true;}
			return false;
		}
		if (e2 . isInteger ()) {
			if (e3 . isInteger ()) {e1 . setInteger (e3 . getInteger () - e2 . getInteger ()); return true;}
			if (e3 . isDouble ()) {e1 . setDouble (e3 . getDouble () - e2 . getInteger ()); return true;}
			return false;
		}
		if (e2 . isDouble ()) {
			if (e3 . isInteger ()) {e1 . setDouble (e3 . getInteger () - e2 . getDouble ()); return true;}
			if (e3 . isDouble ()) {e1 . setDouble (e3 . getDouble () - e2 . getDouble ()); return true;}
			return false;
		}
		if (e1 . isAtom  () || e1 . isText ()) {
			String e1c = e1 . isAtom () ? e1 . getAtom () . name () : e1 . getText ();
			if (e2 . isAtom () || e2 . isText ()) {e3 . setText (e1c + (e2 . isAtom () ? e2 . getAtom () . name () : e2 . getText ())); return true;}
			if (e3 . isAtom () || e3 . isText ()) {
				String e3c = e3 . isAtom () ? e3 . getAtom () . name () : e3 . getText ();
				if (e3c . startsWith (e1c)) {e2 . setText (e3c . substring (e1c . length ())); return true;}
				return false;
			}
		}
		if (e2 . isAtom () || e2 . isText ()) {
			String e2c = e2 . isAtom () ? e2 . getAtom () . name () : e2 . getText ();
			if (e3 . isAtom () || e3 . isText ()) {
				String e3c = e3 . isAtom () ? e3 . getAtom () . name () : e3 . getText ();
				if (e3c . endsWith (e2c)) {e1 . setText (e3c . substring (0, e3c . length () - e2c . length ())); return true;}
				return false;
			}
		}
		return false;
	}
}

class times extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e1 = parameters . getLeft (); parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e2 = parameters . getLeft (); parameters = parameters . getRight ();
		PrologElement e3 = parameters . isPair () ? parameters . getLeft () : parameters;
		int ind;
		double db;
		if (e1 . isInteger ()) {
			if (e2 . isInteger ()) {e3 . setInteger (e1 . getInteger () * e2 . getInteger ()); return true;}
			if (e2 . isDouble ()) {e3 . setDouble (e1 . getInteger () * e2 . getDouble ()); return true;}
			ind = e1 . getInteger ();
			if (ind == 0) return false;
			if (e3 . isInteger ()) {e2 . setInteger (e3 . getInteger () / ind); return true;}
			if (e3 . isDouble ()) {e2 . setDouble (e3 . getDouble () / ind); return true;}
			return false;
		}
		if (e1 . isDouble ()) {
			if (e2 . isInteger ()) {e3 . setDouble (e1 . getDouble () * e2 . getInteger ()); return true;}
			if (e2 . isDouble ()) {e3 . setDouble (e1 . getDouble () * e2 . getDouble ()); return true;}
			db = e1 . getDouble ();
			if (db == 0.0) return false;
			if (e3 . isInteger ()) {e2 . setDouble (e3 . getInteger () / db); return true;}
			if (e3 . isDouble ()) {e2 . setDouble (e3 . getDouble () / db); return true;}
			return false;
		}
		if (e2 . isInteger ()) {
			ind = e2 . getInteger ();
			if (ind == 0) return false;
			if (e3 . isInteger ()) {e1 . setInteger (e3 . getInteger () / ind); return true;}
			if (e3 . isDouble ()) {e1 . setDouble (e3 . getDouble () / ind); return true;}
			return false;
		}
		if (e2 . isDouble ()) {
			db = e2 . getDouble ();
			if (db == 0.0) return false;
			if (e3 . isInteger ()) {e1 . setDouble (e3 . getInteger () / db); return true;}
			if (e3 . isDouble ()) {e1 . setDouble (e3 . getDouble () / db); return true;}
		}
		return false;
	}
}

class mac extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		boolean is_double = false;
		PrologElement e1 = parameters . getLeft (); if (e1 . isDouble ()) is_double = true; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e2 = parameters . getLeft (); if (e2 . isDouble ()) is_double = true; parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e3 = parameters . getLeft (); if (e3 . isDouble ()) is_double = true; parameters = parameters . getRight ();
		PrologElement e4 = parameters . isPair () ? parameters . getLeft () : parameters; if (e4 . isDouble ()) is_double = true;
		if (e1 . isNumber ()) {
			if (e2 . isNumber ()) {
				if (e3 . isNumber ()) {
					double ind = e1 . getNumber () * e2 . getNumber () + e3 . getNumber ();
					if (e4 . isNumber ()) return e4 . getNumber () == ind;
					if (is_double) e4 . setDouble (ind); else e4 . setInteger ((int) ind);
					return true;
				}
				if (e4 . isNumber ()) {
					double ind = e4 . getNumber () - e1 . getNumber () * e2 . getNumber ();
					if (is_double) e3 . setDouble (ind); else e3 . setInteger ((int) ind);
					return true;
				}
			}
			double ind = e1 . getNumber ();
			if (ind == 0.0) return false;
			if (e3 . isNumber () && e4 . isNumber ()) {
				ind = (e4 . getNumber () - e3 . getNumber ()) / ind;
				if (is_double) e2 . setDouble (ind); else e2 . setInteger ((int) ind);
				return true;
			}
			return false;
		}
		if (e2 . isNumber ()) {
			double ind = e2 . getNumber ();
			if (ind == 0.0) return false;
			if (e3 . isNumber () && e4 . isNumber ()) {
				ind = (e4 . getNumber () - e3 . getNumber ()) / ind;
				if (is_double) e1 . setDouble (ind); else e1 . setInteger ((int) ind);
				return true;
			}
		}
		return false;
	}
}

class add extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement result = parameters . getLeft ();
		if (result . isVar ()) parameters = parameters . getRight ();
		int result_type = 0;
		int int_result = 0;
		double double_result = 0.0;
		String area = null;
		boolean added = false;
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isInteger ()) {
				switch (result_type) {
				case 0: int_result += el . getInteger (); break;
				case 1: double_result += (double) el . getInteger (); break;
				case 2: area += el . getInteger (); break;
				}
			}
			if (el . isDouble ()) {
				switch (result_type) {
				case 0: double_result += el . getDouble () + (double) int_result; result_type = 1; break;
				case 1: double_result += el . getDouble (); break;
				case 2: area += el . getDouble (); break;
				}
			}
			if (el . isText () || el . isAtom ()) {
				if (area == null) area = "";
				switch (result_type) {
				case 0:
					if (added) area += int_result;
					area += el . isText () ? el . getText () : el . getAtom () . name ();
					result_type = 2;
					break;
				case 1: area += double_result; area += el . isText () ? el . getText () : el . getAtom () . name (); result_type = 2; break;
				case 2: area += el . isText () ? el . getText () : el . getAtom () . name (); break;
				}
			}
			if (el . isPair ()) {
				PrologElement sub = el;
				boolean is_double = false;
				double accu = 1.0;
				while (sub . isPair ()) {
					PrologElement subel = sub . getLeft ();
					if (subel . isNumber ()) {accu *= subel . getNumber (); if (subel . isDouble ()) is_double = true;}
					sub = sub . getRight ();
				}
				if (is_double) {
					switch (result_type) {
					case 0: double_result += accu; result_type = 1; break;
					case 1: double_result += accu; break;
					case 2: area += accu; break;
					}
				} else {
					switch (result_type) {
					case 0: int_result += (int) accu; break;
					case 1: double_result += accu; break;
					case 2: area += (int) accu; break;
					}
				}
			}
			if (el . isVar ()) {
				switch (result_type) {
				case 0: el . setInteger (int_result); break;
				case 1: el . setDouble (double_result); break;
				case 2: el . setText (area); break;
				}
				return true;
			}
			parameters = parameters . getRight ();
			added = true;
		}
		if (parameters . isVar ()) result = parameters;
		switch (result_type) {
		case 0: result . setInteger (int_result); break;
		case 1: result . setDouble (double_result); break;
		case 2: result . setText (area); break;
		}
		return true;
	}
}

class division extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e1 = parameters . getLeft (); parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e2 = parameters . getLeft (); parameters = parameters . getRight (); if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (e2 . isInteger ()) {
			int ind = e2 . getInteger ();
			if (ind == 0) return false;
			if (e1 . isInteger ()) {parameters . setInteger (e1 . getInteger () / ind); return true;}
			if (e1 . isDouble ()) {parameters . setDouble (e1 . getDouble () / ind); return true;}
			return false;
		}
		if (e2 . isDouble ()) {
			double db = e2 . getDouble ();
			if (db == 0.0) return false;
			if (e1 . isInteger ()) {parameters . setDouble (e1 . getInteger () / db); return true;}
			if (e1 . isDouble ()) {parameters . setDouble (e1 . getDouble () / db); return true;}
		}
		return false;
	}
}

/*
class mod : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * e2 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * e_div = NULL;
		if (parameters -> isPair ()) {
			PrologElement * e = parameters -> getRight ();
			if (e -> isPair ()) e_div = e -> getLeft ();
			parameters = parameters -> getLeft ();
		}
		if (e2 -> isInteger ()) {
			int ind = e2 -> getInteger ();
			if (ind == 0) return false;
			if (e1 -> isInteger ()) {
				parameters -> setInteger (e1 -> getInteger () % ind);
				if (e_div != NULL) e_div -> setInteger (e1 -> getInteger () / ind);
				return true;
			}
			if (e1 -> isDouble ()) {
				parameters -> setDouble ((int) e1 -> getDouble () % ind);
				if (e_div != NULL) e_div -> setDouble ((int) e1 -> getDouble () / ind);
				return true;
			}
			return false;
		}
		if (e2 -> isDouble ()) {
			int db = (int) e2 -> getDouble ();
			if (db == 0) return false;
			if (e1 -> isInteger ()) {
				parameters -> setDouble (e1 -> getInteger () % db);
				if (e_div != NULL) e_div -> setDouble (e1 -> getInteger () / db);
				return true;
			}
			if (e1 -> isDouble ()) {
				parameters -> setDouble ((int) e1 -> getDouble () % db);
				if (e_div != NULL) e_div -> setDouble ((int) e1 -> getDouble () / db);
				return true;
			}
		}
		return false;
	}
};*/

class sub extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e1 = parameters . getLeft ();
		parameters = parameters . getRight ();
		if (! parameters . isPair ()) {
			if (e1 . isInteger ()) {parameters . setInteger (- e1 . getInteger ()); return true;}
			if (e1 . isDouble ()) {parameters . setDouble (- e1 . getDouble ()); return true;}
			return false;
		}
		PrologElement e2 = parameters . getLeft ();
		parameters = parameters . getRight ();
		if (parameters . isEarth ()) {
			if (e1 . isInteger ()) {e2 . setInteger (- e1 . getInteger ()); return true;}
			if (e1 . isDouble ()) {e2 . setDouble (- e1 . getDouble ()); return true;}
			return false;
		}
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (e1 . isInteger ()) {
			if (e2 . isInteger ()) {parameters . setInteger (e1 . getInteger () - e2 . getInteger ()); return true;}
			if (e2 . isDouble ()) {parameters . setDouble (e1 . getInteger () - e2 . getDouble ()); return true;}
			return false;
		}
		if (e1 . isDouble ()) {
			if (e2 . isInteger ()) {parameters . setDouble (e1 . getDouble () - e2 . getInteger ()); return true;}
			if (e2 . isDouble ()) {parameters . setDouble (e1 . getDouble () - e2 . getDouble ()); return true;}
		}
		return false;
	}
}

/*
class mult : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * result = parameters -> getLeft ();
		if (result -> isVar ()) parameters = parameters -> getRight ();
		int result_type = 0;
		int int_result = 1;
		double double_result = 1.0;
		while (parameters -> isPair ()) {
			PrologElement * e1 = parameters -> getLeft ();
			if (e1 -> isInteger ()) {
				switch (result_type) {
				case 0: int_result *= e1 -> getInteger (); break;
				case 1: double_result *= (double) e1 -> getInteger (); break;
				}
			}
			if (e1 -> isDouble ()) {
				switch (result_type) {
				case 0: double_result *= e1 -> getDouble () * (double) int_result; result_type = 1; break;
				case 1: double_result *= e1 -> getDouble (); break;
				}
			}
			if (e1 -> isVar ()) {
				switch (result_type) {
				case 0: e1 -> setInteger (int_result); break;
				case 1: e1 -> setDouble (double_result); break;
				}
				return true;
			}
			parameters = parameters -> getRight ();
		}
		if (parameters -> isVar ()) result = parameters;
		switch (result_type) {
		case 0: result -> setInteger (int_result); break;
		case 1: result -> setDouble (double_result); break;
		}
		return true;
	}
};

class logical : public PrologNativeCode {
public:
	virtual int operation (int a, int b) {return 0;}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		if (! a -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * b = parameters -> getLeft ();
		if (! b -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setInteger (operation (a -> getInteger (), b -> getInteger ()));
		return true;
	}
};

class logical_and : public logical {
public:
	int operation (int a, int b) {return a & b;}
};

class logical_or : public logical {
public:
	int operation (int a, int b) {return a | b;}
};

class logical_xor : public logical {
public:
	int operation (int a, int b) {return a ^ b;}
};

class abs_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		double a;
		int ind;
		if (el -> isDouble ()) {a = el -> getDouble (); parameters -> setDouble (a >= 0.0 ? a : - a); return true;}
		if (el -> isInteger ()) {ind = el -> getInteger (); parameters -> setInteger (ind >= 0 ? ind : - ind); return true;}
		return false;
	}
};
class cos_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (cos (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setDouble (cos ((double) el -> getInteger () * M_PI / 180.0)); return true;}
		if (el -> isVar ()) {
			double sub = 2.0;
			if (parameters -> isDouble ()) sub = parameters -> getDouble ();
			if (parameters -> isInteger ()) sub = (double) parameters -> getInteger ();
			if (sub > 1.0 || sub < -1.0) return false;
			el -> setDouble (acos (sub));
			return true;
		}
		return false;
	}
};
class degrad_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isInteger ()) {parameters -> setDouble ((double) el -> getInteger () * M_PI / 180.0); return true;}
		if (el -> isDouble ()) {parameters -> setDouble (el -> getDouble () * M_PI / 180.0); return true;}
		if (parameters -> isDouble ()) {el -> setDouble (parameters -> getDouble () * 180.0 / M_PI); return true;}
		return false;
	}
};
class e_operation : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setDouble (M_E);
		return true;
	}
};
class exp_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (exp (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setInteger ((int) (exp ((double) el -> getInteger ()))); return true;}
		if (parameters -> isDouble ()) {double d = parameters -> getDouble (); if (d <= 0.0) return false; el -> setDouble (log (d)); return true;}
		if (parameters -> isInteger ()) {int d = parameters -> getInteger (); if (d <= 0) return false; el -> setInteger ((int) log ((double) d)); return true;}
		return false;
	}
};
class log2_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (ex -> isDouble ()) {double x = ex -> getDouble (); if (x <= 0.0) return false; parameters -> setDouble (log (x) / 0.69314718055994530941723212145818); return true;}
		if (ex -> isInteger ()) {int x = ex -> getInteger (); if (x <= 0) return false; parameters -> setInteger ((int) (log ((double) x) / 0.69314718055994530941723212145818)); return true;}
		if (parameters -> isDouble ()) {ex -> setDouble (pow (2.0, parameters -> getDouble ())); return true;}
		if (parameters -> isInteger ()) {ex -> setInteger ((int) pow (2.0, (double) parameters -> getInteger ())); return true;}
		return false;
	}
};
class log10_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (ex -> isDouble ()) {double x = ex -> getDouble (); if (x <= 0.0) return false; parameters -> setDouble (log10 (x)); return true;}
		if (ex -> isInteger ()) {int x = ex -> getInteger (); if (x <= 0) return false; parameters -> setInteger ((int) log10 ((double) x)); return true;}
		if (parameters -> isDouble ()) {ex -> setDouble (pow (10.0, parameters -> getDouble ())); return true;}
		if (parameters -> isInteger ()) {ex -> setInteger ((int) pow (10.0, (double) parameters -> getInteger ())); return true;}
		return false;
	}
};
class ln_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (ex -> isDouble ()) {double x = ex -> getDouble (); if (x <= 0.0) return false; parameters -> setDouble (log (x)); return true;}
		if (ex -> isInteger ()) {int x = ex -> getInteger (); if (x <= 0) return false; parameters -> setInteger ((int) log ((double) x)); return true;}
		if (parameters -> isDouble ()) {ex -> setDouble (exp (parameters -> getDouble ())); return true;}
		if (parameters -> isInteger ()) {ex -> setInteger ((int) exp ((double) parameters -> getInteger ())); return true;}
		return false;
	}
};
class log_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * ex = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * en = parameters -> isPair () ? parameters -> getLeft () : parameters;
		bool integer = true;
		double a, x, n;
		if (ea -> isDouble ()) {a = ea -> getDouble (); integer = false;}
		else {if (! ea -> isInteger ()) return false; a = (double) ea -> getInteger ();}
		if (ex -> isDouble ()) {x = ex -> getDouble (); integer = false;}
		else {if (! ex -> isInteger ()) return false; x = (double) ex -> getInteger ();}
		if (a <= 0.0) return false;
		if (a == 1.0) return false;
		if (x <= 0.0) return false;
		n = log (x) / log (a);
		if (integer) en -> setInteger ((int) n);
		else en -> setDouble (n);
		return true;
	}
};
class pi_operation : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setDouble (M_PI);
		return true;
	}
};
class pow_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * ea = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * en = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		PrologElement * ex = parameters -> isPair () ? parameters -> getLeft () : parameters;
		if (ea -> isDouble ()) {
			double a = ea -> getDouble ();
			if (en -> isDouble ()) {ex -> setDouble (pow (a, en -> getDouble ())); return true;}
			if (en -> isInteger ()) {ex -> setDouble (pow (a, (double) en -> getInteger ())); return true;}
			if (a <= 0.0 || a == 1.0) return false;
			if (ex -> isDouble ()) {double x = ex -> getDouble (); if (x <= 0.0) return false; en -> setDouble (log (x) / log (a)); return true;}
			if (ex -> isInteger ()) {int x = ex -> getInteger (); if (x <= 0) return false; en -> setDouble (log ((double) x) / log (a)); return true;}
		}
		if (ea -> isInteger ()) {
			int a = ea -> getInteger ();
			if (en -> isDouble ()) {ex -> setDouble (pow ((double) a, en -> getDouble ())); return true;}
			if (en -> isInteger ()) {ex -> setInteger ((int) pow ((double) a, (double) en -> getInteger ())); return true;}
			if (a <= 0 || a == 1) return false;
			if (ex -> isDouble ()) {double x = ex -> getDouble (); if (x <= 0.0) return false; en -> setDouble (log (x) / log ((double) a)); return true;}
			if (ex -> isInteger ()) {int x = ex -> getInteger (); if (x <= 0) return false; en -> setInteger ((int) (log ((double) x) / log ((double) a))); return true;}
		}
		if (en -> isDouble ()) {
			double n = en -> getDouble ();
			if (n == 0.0) return false;
			n = 1.0 / n;
			if (ex -> isDouble ()) {ea -> setDouble (pow (ex -> getDouble (), n)); return true;}
			if (ex -> isInteger ()) {ea -> setDouble (pow ((double) ex -> getInteger (), n)); return true;}
		}
		if (en -> isInteger ()) {
			double n = (double) en -> getInteger ();
			if (n == 0.0) return false;
			n = 1.0 / n;
			if (ex -> isDouble ()) {ea -> setDouble (pow (ex -> getDouble (), n)); return true;}
			if (ex -> isInteger ()) {ea -> setInteger ((int) pow ((double) ex -> getInteger (), n)); return true;}
		}
		return false;
	}
};
class sin_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (sin (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setDouble (sin ((double) el -> getInteger () * M_PI / 180.0)); return true;}
		if (el -> isVar ()) {
			double sub = 2.0;
			if (parameters -> isDouble ()) sub = parameters -> getDouble ();
			if (parameters -> isInteger ()) sub = (double) parameters -> getInteger ();
			if (sub > 1.0 || sub < -1.0) return false;
			el -> setDouble (asin (sub));
			return true;
		}
		return false;
	}
};
class tan_operation : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * el = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (el -> isDouble ()) {parameters -> setDouble (tan (el -> getDouble ())); return true;}
		if (el -> isInteger ()) {parameters -> setDouble (tan ((double) el -> getInteger () * M_PI / 180.0)); return true;}
		if (parameters -> isDouble ()) {el -> setDouble (atan (parameters -> getDouble ())); return true;}
		if (parameters -> isInteger ()) {el -> setDouble (atan ((double) parameters -> getInteger ())); return true;}
		return false;
	}
};
class trunc_operation : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * left = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (left -> isDouble ()) {parameters -> setInteger ((int) left -> getDouble ()); return true;}
		if (left -> isInteger ()) {parameters -> setDouble ((double) left -> getInteger ()); return true;}
		if (left -> isVar ()) {
			if (parameters -> isInteger ()) {left -> setDouble ((double) parameters -> getInteger ()); return true;}
			if (parameters -> isDouble ()) {left -> setDouble ((double) ((int) parameters -> getDouble ())); return true;}
			return false;
		}
		return false;
	}
};

class StringToLower : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * source = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (source -> isText () && parameters -> isVar ()) {
			char * command = create_text (source -> getText ());
			char * cp = command;
			while (* cp != '\0') {* cp = tolower (* cp); cp++;}
			parameters -> setText (command);
			delete_text (command);
			return true;
		}
		if (parameters -> isText () && source -> isVar ()) {
			char * command = create_text (parameters -> getText ());
			char * cp = command;
			while (* cp != '\0') {* cp = toupper (* cp); cp++;}
			source -> setText (command);
			delete_text (command);
			return true;
		}
		return false;
	}
};
class StringToUpper : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * source = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (source -> isText () && parameters -> isVar ()) {
			char * command = create_text (source -> getText ());
			char * cp = command;
			while (* cp != '\0') {* cp = toupper (* cp); cp++;}
			parameters -> setText (command);
			delete_text (command);
			return true;
		}
		if (parameters -> isText () && source -> isVar ()) {
			char * command = create_text (parameters -> getText ());
			char * cp = command;
			while (* cp != '\0') {* cp = tolower (* cp); cp++;}
			source -> setText (command);
			delete_text (command);
			return true;
		}
		return false;
	}
};
class StringReplaceOnce : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * source = parameters -> getLeft (); if (! source -> isText ()) return false;
		parameters = parameters -> getRight (); if (! parameters -> isPair ()) return false;
		PrologElement * pattern = parameters -> getLeft (); if (! pattern -> isText ()) return false;
		parameters = parameters -> getRight (); if (! parameters -> isPair ()) return false;
		PrologElement * replacement = parameters -> getLeft (); if (! replacement -> isText ()) return false;
		parameters = parameters -> getRight (); if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		char * src = source -> getText ();
		char * pat = pattern -> getText ();
		char * rep = replacement -> getText ();
		char * occu = strstr (src, pat);
		if (occu == 0) {parameters -> setText (src); return true;}
		char * dest = new char [strlen (src) + strlen (rep) + 1];
		char * cpf = src;
		char * cpt = dest;
		while (cpf < occu) * cpt++ = * cpf++; cpf = rep;
		while (* cpf != '\0') * cpt++ = * cpf++; cpf = occu + strlen (pat);
		while (* cpf != '\0') * cpt++ = * cpf++; * cpt = '\0';
		parameters -> setText (dest);
		delete [] dest;
		return true;
	}
};
class StringReplaceAll : public PrologNativeCode {
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * source = parameters -> getLeft (); if (! source -> isText ()) return false;
		parameters = parameters -> getRight (); if (! parameters -> isPair ()) return false;
		PrologElement * pattern = parameters -> getLeft (); if (! pattern -> isText ()) return false;
		parameters = parameters -> getRight (); if (! parameters -> isPair ()) return false;
		PrologElement * replacement = parameters -> getLeft (); if (! replacement -> isText ()) return false;
		parameters = parameters -> getRight (); if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (! parameters -> isVar ()) return false;
		int occurences = 0;
		char * cpf = source -> getText ();
		char * pat = pattern -> getText ();
		int pat_length = strlen (pat);
		while ((cpf = strstr (cpf, pat)) != 0) {occurences++; cpf += pat_length;}
		cpf = source -> getText ();
		if (occurences < 1) {parameters -> setText (cpf); return true;}
		char * rep = replacement -> getText ();
		char * dest = new char [strlen (cpf) + strlen (rep) * occurences + 1];
		char * occu;
		char * cpt = dest;
		while ((occu = strstr (cpf, pat)) != 0) {
			while (cpf < occu) * cpt++ = * cpf++; cpf = rep;
			while (* cpf != '\0') * cpt++ = * cpf++; cpf = occu + pat_length;
		}
		while (* cpf != '\0') * cpt++ = * cpf++; * cpt = '\0';
		parameters -> setText (dest);
		delete [] dest;
		return true;
	}
};

/////////////////////////////////////
// TRIGONOMETRICAL TRANSFORMATIONS //
/////////////////////////////////////

void CalculateDFT (double * f, double * rec, double * ims, int count) {
	// count = number of samples
	double frac = count > 1 ? 1.0 / (double) count : 1.0;
	double step = (frac + frac) * M_PI;
	double omega = 0.0;
	int spectrum_length = count >> 1;
	for (int ind = 0; ind <= spectrum_length; ind++) {
		double re = 0.0, im = 0.0;
		double time = 0.0;
		for (int sub = 0; sub < count; sub++) {
			re += f [sub] * cos (time);
			im += f [sub] * sin (time);
			time += omega;
		}
		rec [ind] = re * frac; ims [ind] = im * frac;
		omega += step;
	}
}

void CalculateFFTSub (double * f, double * rec, double * ims, int count) {
	if (count == 2) {
		rec [0] = (f [0] + f [1]) * 0.5;
		rec [1] = (f [0] - f [1]) * 0.5;
		ims [0] = 0.0;
		ims [1] = 0.0;
		return;
	}
	if (count == 4) {
		rec [0] = (f [0] + f [1] + f [2] + f [3]) * 0.25;
		rec [1] = (f [0] - f [2]) * 0.25;
		rec [2] = (f [0] - f [1] + f [2] - f [3]) * 0.25;
		rec [3] = rec [1];
		ims [0] = 0;
		ims [1] = (f [1] - f [3]) * 0.25;
		ims [2] = 0;
		ims [3] = - ims [1];
		return;
	}
	int spectrum_length = count >> 1;
	double * evens = new double [count + count + count];
	double * odds = evens + spectrum_length;
	double * evensrec = odds + spectrum_length;
	double * evensims = evensrec + spectrum_length;
	double * oddsrec = evensims + spectrum_length;
	double * oddsims = oddsrec + spectrum_length;
	int index = 0;
	for (int ind = 0; ind < spectrum_length; ind++) {
		evens [ind] = f [index++];
		odds [ind] = f [index++];
	}
	CalculateFFTSub (evens, evensrec, evensims, spectrum_length);
	CalculateFFTSub (odds, oddsrec, oddsims, spectrum_length);
	double step = M_PI / (double) spectrum_length;
	double omega = 0.0;
	index = spectrum_length;
	for (int ind = 0; ind < spectrum_length; ind++) {
		double cosine = cos (omega);
		double sine = sin (omega);
		double re = oddsrec [ind];
		double im = oddsims [ind];
		double oddre = cosine * re - sine * im;
		double oddim = cosine * im + sine * re;
		rec [ind] = 0.5 * (evensrec [ind] + oddre);
		ims [ind] = 0.5 * (evensims [ind] + oddim);
		rec [index] = 0.5 * (evensrec [ind] - oddre);
		ims [index] = 0.5 * (evensims [ind] - oddim);
		omega += step;
		index++;
	}
	delete [] evens;
}

void CalculateFFT (double * f, double * rec, double * ims, int count) {
	// count = number of samples
	if (count == 2) {CalculateFFTSub (f, rec, ims, 2); return;}
	if (count == 4) {CalculateFFTSub (f, rec, ims, 4); return;}
	if (count == 8) {CalculateFFTSub (f, rec, ims, 8); return;}
	switch (count) {
	case 2:
	case 4:
	case 8:
	case 16:
	case 32:
	case 64:
	case 128:
	case 256:
	case 512:
	case 1024:
	case 2048:
	case 4096:
	case 8192:
	case 16384:
	case 65536:
		CalculateFFTSub (f, rec, ims, count);
		break;
	default: CalculateDFT (f, rec, ims, count); break;
	}
}

void CalculateIDFT (double * f, double * rec, double * ims, int count) {
	// count = number of samples
	int spectrum_count = count >> 1;
	double frac = count > 1 ? 2.0 / (double) count : 1.0;
	double step = frac * M_PI;
	double omega = 0.0;
	for (int ind = 0; ind < count; ind++) {
		double fx = rec [0];
		double time = omega;
		for (int sub = 1; sub < spectrum_count; sub++) {
			double delta = rec [sub] * cos (time) + ims [sub] * sin (time);
			fx += delta + delta;
			time += omega;
		}
		fx += rec [spectrum_count] * cos (time);
		f [ind] = fx;
		omega += step;
	}
}

class DFT : public PrologNativeCode {
public:
	bool fast;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * F = 0, * REC = 0, * IMS = 0;
		while (parameters -> isPair ()) {
			if (F == 0) F = parameters -> getLeft ();
			else if (REC == 0) REC = parameters -> getLeft ();
			else IMS = parameters -> getLeft ();
			parameters = parameters -> getRight ();
		}
		if (REC == 0) return false;
		if (REC -> isVar ()) {
			int counter = 0;
			PrologElement * el = F;
			while (el -> isPair ()) {
				if (el -> getLeft () -> isNumber ()) counter++;
				el = el -> getRight ();
			}
			double * f = new double [counter + counter + counter];
			double * rec = f + counter;
			double * ims = rec + counter;
			el = F;
			int index = 0;
			while (el -> isPair ()) {
				if (el -> getLeft () -> isNumber ()) f [index++] = el -> getLeft () -> getNumber ();
				el = el -> getRight ();
			}
			if (fast) CalculateFFT (f, rec, ims, counter);
			else CalculateDFT (f, rec, ims, counter);
			counter >>= 1;
			for (int ind = 0; ind < counter; ind++) {
				REC -> setPair ();
				if (IMS == 0) REC -> getLeft () -> setDouble (sqrt (rec [ind] * rec [ind] + ims [ind] * ims [ind]));
				else {
					REC -> getLeft () -> setDouble (rec [ind]);
					IMS -> setPair (); IMS -> getLeft () -> setDouble (ims [ind]); IMS = IMS -> getRight ();
				}
				REC = REC -> getRight ();
			}
			REC -> setPair ();
			if (IMS == 0) REC -> getLeft () -> setDouble (rec [counter]);
			else {
				REC -> getLeft () -> setDouble (rec [counter]);
				IMS -> setPair (); IMS -> getLeft () -> setDouble (0.0);
			}
			delete [] f;
			return true;
		}
		if (F -> isVar ()) {
			if (IMS == 0) {
				int counter = 0;
				PrologElement * recel = REC;
				while (recel -> isPair ()) {if (recel -> getLeft () -> isNumber ()) counter++; recel = recel -> getRight ();}
				counter <<= 1;
				double * f = new double [counter + counter + counter];
				double * rec = f + counter;
				double * ims = rec + counter;
				int index = 0;
				recel = REC;
				while (recel -> isPair ()) {
					if (recel -> getLeft () -> isNumber ()) {
						rec [index] = 0.0;
						ims [index] = recel -> getLeft () -> getNumber ();
						index++;
					}
					recel = recel -> getRight ();
				}
				counter -= 2;
				* rec = * ims;
				CalculateIDFT (f, rec, ims, counter);
				for (int ind = 0; ind < counter; ind++) {
					F -> setPair ();
					F -> getLeft () -> setDouble (f [ind]);
					F = F -> getRight ();
				}
				delete [] f;
			} else {
				int counter = 0;
				PrologElement * recel = REC;
				PrologElement * imsel = IMS;
				while (recel -> isPair ())  {
					if (recel -> getLeft () -> isNumber ()) counter++;
					recel = recel -> getRight ();
				}
				while (imsel -> isPair ()) {
					if (imsel -> getLeft () -> isNumber ()) counter++;
					imsel = imsel -> getRight ();
				}
				double * f = new double [counter + counter + counter];
				double * rec = f + counter;
				double * ims = rec + counter;
				int index = 0;
				recel = REC;
				while (recel -> isPair ()) {
					if (recel -> getLeft () -> isNumber ()) rec [index++] = recel -> getLeft () -> getNumber ();
					recel = recel -> getRight ();
				}
				index = 0;
				imsel = IMS;
				while (imsel -> isPair ()) {
					if (imsel -> getLeft () -> isNumber ()) ims [index++] = imsel -> getLeft () -> getNumber ();
					imsel = imsel -> getRight ();
				}
				counter -= 2;
				CalculateIDFT (f, rec, ims, counter);
				for (int ind = 0; ind < counter; ind++) {
					F -> setPair ();
					F -> getLeft () -> setDouble (f [ind]);
					F = F -> getRight ();
				}
				delete [] f;
			}
			return true;
		}
		return false;
	}
	DFT (bool fast) {this -> fast = fast;}
};

class timestamp : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * stamp = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * year = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * month = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * day = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * year_day = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * week_day = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * hour = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * minute = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) parameters -> setPair ();
		PrologElement * second = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (year -> isVar () || year -> isEarth ()) {
			time_t time_stamp = 0;
			if (stamp -> isInteger ()) time_stamp = (time_t) stamp -> getInteger ();
			else {time_stamp = time (0); stamp -> setInteger ((int) time_stamp);}
			struct tm ptm;
			ptm = * localtime (& time_stamp);
			year -> setInteger (1900 + ptm . tm_year);
			month -> setInteger (ptm . tm_mon + 1);
			day -> setInteger (ptm . tm_mday);
			year_day -> setInteger (ptm . tm_yday);
			week_day -> setInteger (ptm . tm_wday);
			hour -> setInteger (ptm . tm_hour);
			minute -> setInteger (ptm . tm_min);
			second -> setInteger (ptm . tm_sec);
			return true;
		}
		struct tm tmp;
		tmp . tm_isdst = -1;
		tmp . tm_yday = -1;
		tmp . tm_wday = -1;
		if (year -> isInteger ()) tmp . tm_year = year -> getInteger () - 1900;
		if (month -> isInteger ()) tmp . tm_mon = month -> getInteger () - 1;
		if (day -> isInteger ()) tmp . tm_mday = day -> getInteger ();
		if (year_day -> isInteger ()) tmp . tm_yday = year_day -> getInteger ();
		if (week_day -> isInteger ()) tmp . tm_wday = week_day -> getInteger ();
		if (hour -> isInteger ()) tmp . tm_hour = hour -> getInteger ();
		if (minute -> isInteger ()) tmp . tm_min = minute -> getInteger ();
		if (second -> isInteger ()) tmp . tm_sec = second -> getInteger ();
		time_t time_stamp = mktime (& tmp);
		if (! stamp -> isInteger ()) stamp -> setInteger ((int) time_stamp);
		if (! year -> isInteger ()) year -> setInteger (1900 + tmp . tm_year);
		if (! month -> isInteger ()) month -> setInteger (tmp . tm_mon + 1);
		if (! day -> isInteger ()) day -> setInteger (tmp . tm_mday);
		if (! year_day -> isInteger ()) year_day -> setInteger (tmp . tm_yday);
		if (! week_day -> isInteger ()) week_day -> setInteger (tmp . tm_wday);
		if (! hour -> isInteger ()) hour -> setInteger (tmp . tm_hour);
		if (! minute -> isInteger ()) minute -> setInteger (tmp . tm_min);
		if (! second -> isInteger ()) second -> setInteger (tmp . tm_sec);
		return true;
	}
};

*/

class less extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement a = parameters . getLeft ();
		parameters = parameters . getRight ();
		while (parameters . isPair ()) {
			PrologElement b = parameters . getLeft ();
			if (a . isInteger ()) {
				if (b . isInteger ()) {if (a . getInteger () >= b . getInteger ()) return false;}
				else if (b . isDouble ()) {if ((double) a . getInteger () >= b . getDouble ()) return false;}
				else return false;
			} else if (a . isDouble ()) {
				if (b . isInteger ()) {if (a . getDouble () >= (double) b . getInteger ()) return false;}
				else if (b . isDouble ()) {if (a . getDouble () >= b . getDouble ()) return false;}
				else return false;
			} else if (a . isText ()) {
				if (b . isText ()) {if (a . getText () . compareTo (b . getText ()) >= 0) return false;}
				else if (b . isAtom ()) {if (a . getText () . compareTo (b . getAtom () . name ()) >= 0) return false;}
				else return false;
			} else if (a . isAtom ()) {
				if (b . isAtom ()) {if (a . getAtom () . name () . compareTo (b . getAtom () . name ()) >= 0) return false;}
				else if (b . isText ()) {if (a . getAtom () . name () . compareTo (b . getText ()) >= 0) return false;}
				else return false;
			} else return false;
			a = b;
			parameters = parameters . getRight ();
		}
		return true;
	}
}

class less_eq extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement a = parameters . getLeft ();
		parameters = parameters . getRight ();
		while (parameters . isPair ()) {
			PrologElement b = parameters . getLeft ();
			if (a . isInteger ()) {
				if (b . isInteger ()) {if (a . getInteger () > b . getInteger ()) return false;}
				else if (b . isDouble ()) {if ((double) a . getInteger () > b . getDouble ()) return false;}
				else return false;
			} else if (a . isDouble ()) {
				if (b . isInteger ()) {if (a . getDouble () > (double) b . getInteger ()) return false;}
				else if (b . isDouble ()) {if (a . getDouble () > b . getDouble ()) return false;}
				else return false;
			} else if (a . isText ()) {
				if (b . isText ()) {if (a . getText () . compareTo (b . getText ()) > 0) return false;}
				else if (b . isAtom ()) {if (a . getText () . compareTo (b . getAtom () . name ()) > 0) return false;}
				else return false;
			} else if (a . isAtom ()) {
				if (b . isAtom ()) {if (a . getAtom () . name () . compareTo (b . getAtom () . name ()) > 0) return false;}
				else if (b . isText ()) {if (a . getAtom () . name () . compareTo (b . getText ()) > 0) return false;}
				else return false;
			} else return false;
			a = b;
			parameters = parameters . getRight ();
		}
		return true;
	}
}

class greater extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement a = parameters . getLeft ();
		parameters = parameters . getRight ();
		while (parameters . isPair ()) {
			PrologElement b = parameters . getLeft ();
			if (a . isInteger ()) {
				if (b . isInteger ()) {if (a . getInteger () <= b . getInteger ()) return false;}
				else if (b . isDouble ()) {if ((double) a . getInteger () <= b . getDouble ()) return false;}
				else return false;
			} else if (a . isDouble ()) {
				if (b . isInteger ()) {if (a . getDouble () <= (double) b . getInteger ()) return false;}
				else if (b . isDouble ()) {if (a . getDouble () <= b . getDouble ()) return false;}
				else return false;
			} else if (a . isText ()) {
				if (b . isText ()) {if (a . getText () . compareTo (b . getText ()) <= 0) return false;}
				else if (b . isAtom ()) {if (a . getText () . compareTo (b . getAtom () . name ()) <= 0) return false;}
				else return false;
			} else if (a . isAtom ()) {
				if (b . isAtom ()) {if (a . getAtom () . name () . compareTo (b . getAtom () . name ()) <= 0) return false;}
				else if (b . isText ()) {if (a . getAtom () . name () . compareTo (b . getText ()) <= 0) return false;}
				else return false;
			} else return false;
			a = b;
			parameters = parameters . getRight ();
		}
		return true;
	}
}

/*
class greater : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		PrologElement * b;
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			b = parameters -> getLeft ();
			if (a -> isInteger ()) {
				if (b -> isInteger ()) {if (a -> getInteger () <= b -> getInteger ()) return false;}
				else if (b -> isDouble ()) {if ((double) a -> getInteger () <= b -> getDouble ()) return false;}
				else return false;
			}
			else if (a -> isDouble ()) {
				if (b -> isInteger ()) {if (a -> getDouble () <= (double) b -> getInteger ()) return false;}
				else if (b -> isDouble ()) {if (a -> getDouble () <= b -> getDouble ()) return false;}
				else return false;
			}
			else if (a -> isText ()) {
				if (b -> isText ()) {if (strcmp (a -> getText (), b -> getText ()) <= 0) return false;}
				else if (b -> isAtom ()) {if (strcmp (a -> getText (), b -> getAtom () -> name ()) <= 0) return false;}
				else return false;
			}
			else if (a -> isAtom ()) {
				if (b -> isAtom ()) {if (strcmp (a -> getAtom () -> name (), b -> getAtom () -> name ()) <= 0) return false;}
				else if (b -> isText ()) {if (strcmp (a -> getAtom () -> name (), b -> getText ()) <= 0) return false;}
				else return false;
			}
			else return false;
			a = b;
			parameters = parameters -> getRight ();
		}
		return true;
	}
};

class greater_eq : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * a = parameters -> getLeft ();
		PrologElement * b;
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			b = parameters -> getLeft ();
			if (a -> isInteger ()) {
				if (b -> isInteger ()) {if (a -> getInteger () < b -> getInteger ()) return false;}
				else if (b -> isDouble ()) {if ((double) a -> getInteger () < b -> getDouble ()) return false;}
				else return false;
			}
			else if (a -> isDouble ()) {
				if (b -> isInteger ()) {if (a -> getDouble () < (double) b -> getInteger ()) return false;}
				else if (b -> isDouble ()) {if (a -> getDouble () < b -> getDouble ()) return false;}
				else return false;
			}
			else if (a -> isText ()) {
				if (b -> isText ()) {if (strcmp (a -> getText (), b -> getText ()) < 0) return false;}
				else if (b -> isAtom ()) {if (strcmp (a -> getText (), b -> getAtom () -> name ()) < 0) return false;}
				else return false;
			}
			else if (a -> isAtom ()) {
				if (b -> isAtom ()) {if (strcmp (a -> getAtom () -> name (), b -> getAtom () -> name ()) < 0) return false;}
				else if (b -> isText ()) {if (strcmp (a -> getAtom () -> name (), b -> getText ()) < 0) return false;}
				else return false;
			}
			else return false;
			a = b;
			parameters = parameters -> getRight ();
		}
		return true;
	}
};

class max_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * res = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * maximum = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (e -> isInteger ()) {
				if (maximum -> isInteger ()) {if (e -> getInteger () > maximum -> getInteger ()) maximum = e;}
				else if (maximum -> isDouble ()) {if ((double) e -> getInteger () > maximum -> getDouble ()) maximum = e;}
				else return false;
			} else if (e -> isDouble ()) {
				if (maximum -> isDouble ()) {if (e -> getDouble () > maximum -> getDouble ()) maximum = e;}
				else if (maximum -> isInteger ()) {if (e -> getDouble () > (double) maximum -> getInteger ()) maximum = e;}
				else return false;
			} else if (e -> isText ()) {
				if (maximum -> isText ()) {if (strcmp (e -> getText (), maximum -> getText ()) > 0) maximum = e;}
				else if (maximum -> isAtom ()) {if (strcmp (e -> getText (), maximum -> getAtom () -> name ()) > 0) maximum = e;}
				else return false;
			} else if (e -> isAtom ()) {
				if (maximum -> isAtom ()) {if (strcmp (e -> getAtom () -> name (), maximum -> getAtom () -> name ()) > 0) maximum = e;}
				else if (maximum -> isText ()) {if (strcmp (e -> getAtom () -> name (), maximum -> getText ()) > 0) maximum = e;}
				else return false;
			} else return false;
			parameters = parameters -> getRight ();
		}
		if (maximum -> isInteger ()) {res -> setInteger (maximum -> getInteger ()); return true;}
		if (maximum -> isDouble ()) {res -> setDouble (maximum -> getDouble ()); return true;}
		if (maximum -> isText ()) {res -> setText (maximum -> getText ()); return true;}
		if (maximum -> isAtom ()) {res -> setAtom (maximum -> getAtom ()); return true;}
		return false;
	}
};

class min_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * res = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * minimum = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		while (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (e -> isInteger ()) {
				if (minimum -> isInteger ()) {if (e -> getInteger () < minimum -> getInteger ()) minimum = e;}
				else if (minimum -> isDouble ()) {if ((double) e -> getInteger () < minimum -> getDouble ()) minimum = e;}
				else return false;
			} else if (e -> isDouble ()) {
				if (minimum -> isDouble ()) {if (e -> getDouble () < minimum -> getDouble ()) minimum = e;}
				else if (minimum -> isInteger ()) {if (e -> getDouble () < (double) minimum -> getInteger ()) minimum = e;}
				else return false;
			} else if (e -> isText ()) {
				if (minimum -> isText ()) {if (strcmp (e -> getText (), minimum -> getText ()) < 0) minimum = e;}
				else if (minimum -> isAtom ()) {if (strcmp (e -> getText (), minimum -> getAtom () -> name ()) < 0) minimum = e;}
				else return false;
			} else if (e -> isAtom ()) {
				if (minimum -> isAtom ()) {if (strcmp (e -> getAtom () -> name (), minimum -> getAtom () -> name ()) < 0) minimum = e;}
				else if (minimum -> isText ()) {if (strcmp (e -> getAtom () -> name (), minimum -> getText ()) < 0) minimum = e;}
				else return false;
			} else return false;
			parameters = parameters -> getRight ();
		}
		if (minimum -> isInteger ()) {res -> setInteger (minimum -> getInteger ()); return true;}
		if (minimum -> isDouble ()) {res -> setDouble (minimum -> getDouble ()); return true;}
		if (minimum -> isText ()) {res -> setText (minimum -> getText ()); return true;}
		if (minimum -> isAtom ()) {res -> setAtom (minimum -> getAtom ()); return true;}
		return false;
	}
};

class file_write : public PrologNativeCode {
public:
	FILE * fw;
	PrologRoot * root;
	PrologAtom * atom;
	void sub_right_store (PrologElement * el, char * area) {
		if (el -> isEarth ()) return;
		if (el -> isPair ()) {
			PrologElement * left = el -> getLeft (); if (left == 0) return;
			PrologElement * right = el -> getRight (); if (right == 0) return;
			fprintf (fw, "%s", root -> separator_caption);
			fprintf (fw, " ");
			sub_store (left, area);
			sub_right_store (right, area);
			return;
		}
		fprintf (fw, " ");
		fprintf (fw, "%s", root -> mid_caption);
		fprintf (fw, " ");
		root -> getValue (el, area, 0);
		fprintf (fw, "%s", area);
	}
	void sub_store (PrologElement * el, char * area) {
		if (el -> isPair ()) {
			PrologElement * left = el -> getLeft (); if (left == 0) return;
			PrologElement * right = el -> getRight (); if (right == 0) return;
			fprintf (fw, "%s", root -> left_caption);
			sub_store (el -> getLeft (), area);
			sub_right_store (el -> getRight (), area);
			fprintf (fw, "%s", root -> right_caption);
			return;
		}
		root -> getValue (el, area, 0);
		fprintf (fw, "%s", area);
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		if (fw == NULL) return false;
		PrologElement * el;
		if (parameters -> isEarth ()) {
			fclose (fw);
			fw = 0;
			atom -> setMachine (0);
			delete this;
			return true;
		}
		while (parameters -> isPair ()) {
			el = parameters -> getLeft ();
			if (el -> isText ()) fprintf (fw, "%s", el -> getText ());
			if (el -> isInteger ()) {
				int i = el -> getInteger ();
				if (i >= 0) {
					if (i <= 0xff) fputc (i, fw);
					else if (i <= 0xffff) {fputc (i >> 8, fw); fputc (i & 0xff, fw);}
					else if (i <= 0xffffff) {fputc (i >> 16, fw); fputc ((i >> 8) & 0xff, fw); fputc (i & 0xff, fw);}
					else if (i <= 0xffffffff) {fputc (i >> 24, fw); fputc ((i >> 16) & 0xff, fw); fputc ((i >> 8) & 0xff, fw); fputc (i & 0xff, fw);}
				} else {
					i = -i;
					if (i <= 0x7f) fputc (i, fw);
					else if (i <= 0x7ff) {
						int msb = 0xc0 | (i >> 6);
						int lsb = 0x80 | (i & 0x3f);
						fputc (msb, fw); fputc (lsb, fw);
					} else if (i <= 0xffff) {
						int hsb = 0xe0 | (i >> 12);
						int msb = 0x80 | ((i >> 6) & 0x3f);
						int lsb = 0x80 | (i & 0x3f);
						fputc (hsb, fw); fputc (msb, fw); fputc (lsb, fw);
					} else if (i <= 0x1fffff) {
						int xlsb = 0xf0 | (i >> 18);
						int hsb = 0x80 | ((i >> 12) & 0x3f);
						int msb = 0x80 | ((i >> 6) & 0x3f);
						int lsb = 0x80 | (i & 0x3f);
						fputc (xlsb, fw); fputc (hsb, fw); fputc (msb, fw); fputc (lsb, fw);
					} else if (i <= 0x3ffffff) {
						int xmsb = 0xf8 | (i >> 24);
						int xlsb = 0x80 | ((i >> 18) & 0x3f);
						int hsb = 0x80 | ((i >> 12) & 0x3f);
						int msb = 0x80 | ((i >> 6) & 0x3f);
						int lsb = 0x80 | (i & 0x3f);
						fputc (xmsb, fw); fputc (xlsb, fw); fputc (hsb, fw); fputc (msb, fw); fputc (lsb, fw);
					} else if (i <= 0x7fffffff) {
						int xhsb = 0xfc | (i >> 30);
						int xmsb = 0x80 | ((i >> 24) & 0x3f);
						int xlsb = 0x80 | ((i >> 18) & 0x3f);
						int hsb = 0x80 | ((i >> 12) & 0x3f);
						int msb = 0x80 | ((i >> 6) & 0x3f);
						int lsb = 0x80 | (i & 0x3f);
						fputc (xhsb, fw); fputc (xmsb, fw); fputc (xlsb, fw); fputc (hsb, fw); fputc (msb, fw); fputc (lsb, fw);
					}
				}
			}
			while (el -> isPair ()) {
				sub_store (el -> getLeft (), area);
				el = el -> getRight ();
			}
			parameters = parameters -> getRight ();
		}
		return true;
	}
	file_write (PrologAtom * atom, PrologRoot * root, char * file_name) {
		this -> atom = atom;
		this -> root = root;
		fw = fopen (file_name, "wb");
	}
	~ file_write (void) {if (fw != NULL) fclose (fw);}
};

class file_writer : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * symbol = 0;
		PrologElement * name = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) symbol = el;
			if (el -> isVar ()) symbol = el;
			if (el -> isText ()) name = el;
			parameters = parameters -> getRight ();
		}
		if (symbol == 0 || name == 0) return false;
		if (symbol -> isVar ()) symbol -> setAtom (new PrologAtom ());
		PrologAtom * atom = symbol -> getAtom ();
		if (atom -> getMachine () != 0) return false;
		file_write * fw = new file_write (atom, root, name -> getText ());
		if (fw -> fw == 0) {delete fw; return false;}
		if (atom -> setMachine (fw)) return true;
		delete fw;
		return false;
	}
	file_writer (PrologRoot * root) {this -> root = root;}
};

class symbol_reader : public PrologReader {
public:
	FILE * fi;
	void message (char * text) {root -> print (text); root -> print (root -> new_line_caption);}
	void message_v (char * text, char * variable) {root -> message (text, variable);}
	int move_z (void) {return fgetc (fi);}
	void init (PrologRoot * root, FILE * fi) {
		this -> fi = fi;
		setRoot (root);
	}
};
*/

class symbol_reader extends PrologReader {
	public FileInputStream fi;
	public int move_z () {try {return fi . read ();} catch (Exception ex) {return -1;}}
	public symbol_reader (PrologRoot root, FileInputStream fi) {this . fi = fi; setRoot (root);}
}

class file_read extends PrologNativeCode {
	public FileInputStream fi;
	public symbol_reader sr;
	public PrologAtom atom;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (fi == null) {if (! parameters . isEarth ()) return false; atom . setMachine (null); return true;}
		if (parameters . isEarth ()) {try {fi . close ();} catch (Exception ex) {} fi = null; atom . setMachine (null); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement el = parameters . getLeft ();
		if (el . isText ()) {
			parameters = parameters . getRight ();
			if (! parameters . isPair ()) return false;
			String area = sr . getString (el . getText ());
			if (area == null) return false;
			parameters . getLeft () . setText (area);
			return true;
		}
		el = sr . readElement ();
		if (el == null) return false;
		parameters . setLeft (el);
		return true;
	}
	public file_read (PrologAtom atom, PrologRoot root, String file_name) {
		this . atom = atom;
		try {fi = new FileInputStream (file_name);} catch (Exception ex) {fi = null;}
		sr = new symbol_reader (root, fi);
	}
}

class file_reader extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		PrologElement symbol = null;
		PrologElement name = null;
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isAtom ()) symbol = el;
			if (el . isVar ()) symbol = el;
			if (el . isText ()) name = el;
			parameters = parameters . getRight ();
		}
		if (symbol == null || name == null) return false;
		if (symbol . isVar ()) symbol . setAtom (new PrologAtom ());
		PrologAtom atom = symbol . getAtom ();
		if (atom . getMachine () != null) return false;
		file_read fr = new file_read (atom, root, name . getText ());
		if (fr . fi == null) return false;
		return atom . setMachine (fr);
	}
	public file_reader (PrologRoot root) {this . root = root;}
}

class module_loader extends PrologNativeCode {
	public PrologRoot root;
	public boolean echo;
	public boolean reload;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {
			PrologElement module_name = parameters . getLeft ();
			if (! module_name . isText ()) return false;
			PrologLoader loader = new PrologLoader (root);
			loader . echo = echo;
			loader . reload = reload;
			if (module_name . getText () . contains (".prc")) {if (! loader . load_without_main (module_name . getText ())) return false;}
			else {if (! loader . load_without_main (module_name . getText () + ".prc")) return false;}
			parameters = parameters . getRight ();
		}
		return true;
	}
}
class import_loader extends module_loader {public import_loader (PrologRoot root) {this . root = root; this . echo = false; this . reload = false;}}
class load_loader extends module_loader {public load_loader (PrologRoot root) {this . root = root; this .echo = false; this . reload = true;}}
class consult_loader extends module_loader {public consult_loader (PrologRoot root) {this . root = root; this . echo = true; this . reload = true;}}

class remove_module extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {
			PrologElement drop = parameters . getLeft ();
			if (! drop . isText ()) return false;
			if (! root . drop (drop . getText ())) return false;
			parameters = parameters . getRight ();
		}
		return true;
	}
	public remove_module (PrologRoot root) {this . root = root;}
}

class create_module extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {root . close (); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement em = parameters . getLeft (); if (! em . isText ()) return false; parameters = parameters . getRight ();
		if (parameters . isEarth ()) {root . createDirectory (em . getText ()); return true;}
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft (); if (! parameters . isText ()) return false;
		PrologServiceClass service = root . loadServiceClass (parameters . getText ());
		if (service == null) return false;
		PrologDirectory directory = root . createDirectory (em . getText (), service);
		service . init (root, directory);
		return true;
	}
	public create_module (PrologRoot root) {this . root = root;}
}

class set_machine extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement ea = parameters . getLeft ();
		if (! ea . isAtom ()) return false;
		if (ea . getAtom () . getMachine () != null) return false;
		parameters = parameters . getRight ();
		if (! parameters . isPair ()) return false;
		PrologElement et = parameters . getLeft ();
		if (! et . isText ()) return false;
		parameters = parameters . getRight ();
		if (parameters . isEarth ()) {
			PrologServiceClass service = root . getServiceClass ();
			if (service == null) return false;
			PrologNativeCode native_code = service . getNativeCode (et . getText ());
			if (native_code == null) return false;
			return ea . getAtom () . setMachine (native_code);
		}
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft ();
		if (! parameters . isText ()) return false;
		PrologServiceClass service = root . getServiceClass (et . getText ());
		if (service == null) return false;
		PrologNativeCode native_code = service . getNativeCode (parameters . getText ());
		if (native_code == null) return false;
		return ea . getAtom () . setMachine (native_code);
	}
	public set_machine (PrologRoot root) {this . root = root;}
}

class machine_type extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement argument = parameters . getLeft ();
		if (! argument . isAtom ()) return false;
		PrologAtom atom = argument . getAtom ();
		parameters = parameters . getRight ();
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isVar ()) {parameters . setText (atom . machineType ()); return true;}
		if (parameters . isAtom ()) return atom . isTypeOf (parameters . getAtom ());
		if (parameters . isEarth ()) return atom . isTypeOf (this);
		return false;
	}
}

class add_search_directory extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		while (parameters . isPair ()) {
			PrologElement dir = parameters . getLeft ();
			if (! dir . isText ()) return false;
			root . addSearchDirectory (dir . getText ());
			parameters = parameters . getRight ();
		}
		return true;
	}
	public add_search_directory (PrologRoot root) {this . root = root;}
}

class search_directories extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {
			root . message ("SEARCH DIRECTORIES:");
			ListIterator <String> it = root . search_directories . listIterator ();
			while (it . hasNext ()) root . message ("	[" + it . next () + "]");
			return true;
		}
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isEarth ()) {root . deleteSearchDirectories (); return true;}
		if (parameters . isVar ()) {
			if (root . search_directories . size () < 1) {parameters . setEarth (); return true;}
			ListIterator <String> it = root . search_directories . listIterator ();
			while (it . hasNext ()) {parameters . setPair (); parameters . getLeft () . setText (it . next ()); parameters = parameters . getRight ();}
			return true;
		}
		if (parameters . isPair ()) {
			root . deleteSearchDirectories ();
			while (parameters . isPair ()) {
				if (parameters . getLeft () . isText ()) root . search_directories . add (parameters . getLeft () . getText ());
				parameters = parameters . getRight ();
			}
			return true;
		}
		return false;
	}
	public search_directories (PrologRoot root) {this . root = root;}
}

/*
class cd : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {return root -> change_directory ("..");}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {parameters -> setText (root -> getCWD ()); return true;}
		if (parameters -> isText ()) return root -> change_directory (parameters -> getText ());
		return false;
	}
	cd (PrologRoot * root) {this -> root = root;}
};

class relativise_path : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * path = parameters -> getLeft ();
		if (! path -> isText ()) return false; parameters = parameters -> getRight ();
		AREA area;
		if (parameters -> isVar ()) {
			relativise (path -> getText (), root -> getCWD (), area);
			parameters -> setText (area);
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * argument = parameters -> getLeft ();
		if (argument -> isVar ()) {
			relativise (path -> getText (), root -> getCWD (), area);
			argument -> setText (area);
			return true;
		}
		if (! argument -> isText ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) {
			relativise (path -> getText (), argument -> getText (), area);
			parameters -> setText (area);
			return true;
		}
		return false;
	}
	relativise_path (PrologRoot * root) {this -> root = root;}
};

class DIR : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * left = parameters -> getLeft ();
		if (! left -> isText ()) return false;
		PrologElement * el = root -> dir (left -> getText ());
		if (el == 0) return false;
		parameters -> setRight (el);
		return true;
	}
	DIR (PrologRoot * root) {this -> root = root;}
};
*/

class ARGS extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (root . args . size () < 1) {parameters . setEarth (); return true;}
		ListIterator <String> it = root . args . listIterator ();
		while (it . hasNext ()) {parameters . setPair (); parameters . getLeft () . setText (it . next ()); parameters = parameters . getRight ();}
		return true;
	}
	public ARGS (PrologRoot root) {this . root = root;}
}
/*
class edit : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		return root -> edit (parameters -> getText ());
	}
	edit (PrologRoot * root) {this -> root = root;}
};

class execute : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		return root -> execute (parameters -> getText ());
	}
	execute (PrologRoot * root) {this -> root = root;}
};

class make_directory : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		while (parameters -> isPair ()) {
			PrologElement * dir = parameters -> getLeft ();
			if (! dir -> isText ()) return false;
			if (! root -> make_directory (dir -> getText ())) return false;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	make_directory (PrologRoot * root) {this -> root = root;}
};

class erase_file : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		while (parameters -> isPair ()) {
			PrologElement * eraser = parameters -> getLeft ();
			if (! eraser -> isText ()) return false;
			if (! root -> erase_file (eraser -> getText ())) return false;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	erase_file (PrologRoot * root) {this -> root = root;}
};

class erase_directory : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		while (parameters -> isPair ()) {
			PrologElement * eraser = parameters -> getLeft ();
			if (! eraser -> isText ()) return false;
			if (! root -> erase_directory (eraser -> getText ())) return false;
			parameters = parameters -> getRight ();
		}
		return true;
	}
	erase_directory (PrologRoot * root) {this -> root = root;}
};

class move_file : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * from = parameters -> getLeft ();
		if (! from -> isText ()) return false;
		parameters = parameters -> getRight ();

		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		return root -> move_file (from -> getText (), parameters -> getText ());
	}
	move_file (PrologRoot * root) {this -> root = root;}
};

class copy_file : public PrologNativeCode {
private:
	PrologRoot * root;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * from = parameters -> getLeft ();
		if (! from -> isText ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isText ()) return false;
		return root -> copy_file (from -> getText (), parameters -> getText ());
	}
	copy_file (PrologRoot * root) {this -> root = root;}
};

class history : public PrologNativeCode {
private:
	PrologRoot * root;
	bool save;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (root == 0) return false;
		PrologCommand * commander = root -> getCommander ();
		if (commander == 0) return false;
		PrologElement * history_size = 0;
		PrologElement * history_file = 0;
		while (parameters -> isPair ()) {
			PrologElement * e = parameters -> getLeft ();
			if (e -> isInteger ()) history_size = e;
			if (e -> isText ()) history_file = e;
			parameters = parameters -> getRight ();
		}
		if (save) commander -> save_history (history_size != 0 ? history_size -> getInteger () : 0, history_file != 0 ? history_file -> getText () : 0);
		else commander -> load_history (history_size != 0 ? history_size -> getInteger () : 0, history_file != 0 ? history_file -> getText () : 0);
		return true;
	}
	history (PrologRoot * root, bool save) {this -> root = root; this -> save = save;}
};
*/

class operating_system extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isText ()) return parameters . getText () . equals (System . getProperty ("os.name"));
		parameters . setText (System . getProperty ("os.name"));
		return true;
	}
}

class implementation_code extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isText ()) return parameters . getText () . equalsIgnoreCase ("java");
		parameters . setText ("java");
		return true;
	}
}

//////////
// META //
//////////

class constant extends PrologNativeCode {
	public PrologElement container;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		parameters . duplicate (container);
		return true;
	}
	public constant (PrologElement term) {container = term . duplicate ();}
}

class CONSTANT_CODE extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e = parameters . getLeft ();
		PrologAtom atom = null;
		if (e . isVar ()) e . setAtom (atom = new PrologAtom ());
		else {if (! e . isAtom ()) return false; atom = e . getAtom ();}
		parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		if (atom . getMachine () != null) return false;
		return atom . setMachine (new constant (parameters . getLeft ()));
	}
}
/*
class CONSTANT : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * e = parameters -> getLeft ();
		PrologAtom * atom = NULL;
		if (e -> isVar ()) {
			atom = new PrologAtom ();
			e -> setAtom (atom);
		} else {
			if (! e -> isAtom ()) return false;
			atom = e -> getAtom ();
		}
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		if (atom -> getMachine () != 0) return false;
		constant * c = new constant (parameters -> getLeft ());
		if (atom -> setMachine (c)) return true;
		delete c;
		return false;
	}
};
*/

class variable extends PrologNativeCode {
	public PrologAtom atom;
	public PrologElement container;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) {container = parameters . getLeft () . duplicate (); return true;}
		if (parameters . isVar ()) {parameters . duplicate (container); return true;}
		if (parameters . isEarth ()) {atom . setMachine (null); return true;}
		return false;
	}
	public variable (PrologAtom atom) {this . atom = atom; container = new PrologElement ();}
	public variable (PrologAtom atom, PrologElement element) {this . atom = atom; container = element . duplicate ();}
}
class variable_code extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement initial_value = parameters . getRight ();
		parameters = parameters . getLeft ();
		PrologAtom atom = null;
		if (parameters . isVar ()) {atom = new PrologAtom (); parameters . setAtom (atom);}
		else {if (! parameters . isAtom ()) return false; atom = parameters . getAtom ();}
		if (atom . getMachine () != null) return false;
		variable v;
		if (initial_value . isPair ()) v = new variable (atom, initial_value . getLeft ());
		else v = new variable (atom);
		return atom . setMachine (v);
	}
}

class accumulator extends PrologNativeCode {
	public PrologAtom atom;
	public PrologElement container;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) {
			resolution . reset ();
			PrologElement e = new PrologElement ();
			e . setPair (resolution . match_product (parameters . getLeft (), true), resolution . match_product (container, false));
			container = e;
			return true;
		}
		if (parameters . isVar ()) {parameters . duplicate (container); return true;}
		if (parameters . isEarth ()) {atom . setMachine (null); return true;}
		return false;
	}
	public accumulator (PrologAtom atom) {this . atom = atom; container = new PrologElement ();}
}

class accumulator_code extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft ();
		PrologAtom atom = null;
		if (parameters . isVar ()) {atom = new PrologAtom (); parameters . setAtom (atom);}
		else {if (! parameters . isAtom ()) return false; atom = parameters . getAtom ();}
		if (atom . getMachine () != null) return false;
		accumulator accu = new accumulator (atom);
		return atom . setMachine (accu);
	}
}

class stack extends PrologNativeCode {
	public PrologAtom atom;
	public LinkedList <PrologElement> q = new LinkedList <PrologElement> ();
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isVar ()) {
			parameters . setEarth ();
			ListIterator <PrologElement> it = q . listIterator ();
			while (it . hasNext ()) {parameters . setPair (); parameters . getLeft () . duplicate (it . next ()); parameters = parameters . getRight ();}
			return true;
		}
		if (parameters . isEarth ()) {atom . setMachine (null); return true;}
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isVar ()) {if (q . size () < 1) return false; PrologElement ell = q . removeFirst (); el . duplicate (ell);}
			else q . addFirst (el);
			parameters = parameters . getRight ();
		}
		return true;
	}
	public stack (PrologAtom atom) {this . atom = atom;}
}

class STACK_CODE extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isVar ()) parameters . setAtom (new PrologAtom ());
		if (! parameters . isAtom ()) return false;
		PrologAtom atom = parameters . getAtom ();
		if (atom . getMachine () != null) return false;
		return atom . setMachine (new stack (atom));
	}
}

class queue extends PrologNativeCode {
	public PrologAtom atom;
	public LinkedList <PrologElement> q = new LinkedList <PrologElement> ();
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isVar ()) {
			parameters . setEarth ();
			ListIterator <PrologElement> it = q . listIterator (q . size ());
			while (it . hasPrevious ()) {parameters . setPair (); parameters . getLeft () . duplicate (it . previous ()); parameters = parameters . getRight ();}
			return true;
		}
		if (parameters . isEarth ()) {atom . setMachine (null); return true;}
		while (parameters . isPair ()) {
			PrologElement el = parameters . getLeft ();
			if (el . isVar ()) {if (q . size () < 1) return false; PrologElement ell = q . removeLast (); el . duplicate (ell);}
			else q . addFirst (el);
			parameters = parameters . getRight ();
		}
		return true;
	}
	public queue (PrologAtom atom) {this . atom = atom;}
}

class QUEUE_CODE extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isVar ()) parameters . setAtom (new PrologAtom ());
		if (! parameters . isAtom ()) return false;
		PrologAtom atom = parameters . getAtom ();
		if (atom . getMachine () != null) return false;
		return atom . setMachine (new queue (atom));
	}
}

class array_dimension {
	public PrologElement [] elements;
	public array_dimension [] dimensions;
	public int size;
	public void insert_dimension (int size) {
		if (dimensions != null) {for (int ind = 0; ind < this . size; ind++) dimensions [ind] . insert_dimension (size);}
		else {dimensions = new array_dimension [this . size]; for (int ind = 0; ind < this . size; ind++) dimensions [ind] = new array_dimension (size);}
	}
	public void insert_elements () {
		if (dimensions != null) {for (int ind = 0; ind < size; ind++) dimensions [ind] . insert_elements ();}
		else {elements = new PrologElement [size]; for (int ind = 0; ind < size; ind++) elements [ind] = new PrologElement ();}
	}
	public array_dimension get_dimension (int location) {if (location < 0 || location >= size || dimensions == null) return null; return dimensions [location];}
	public PrologElement get_element (int location) {if (location < 0 || location >= size || elements == null) return null; return elements [location];}
	public boolean set_element (int location, PrologElement el) {if (location < 0 || location >= size || elements == null) return false; elements [location] = el; return true;}
	public array_dimension (int size) {this . size = size;}
}

class array_code extends PrologNativeCode {
	public PrologAtom atom;
	public array_dimension dimension;
	public int dimensions;
	public void insert_dimension (int size) {if (dimension == null) dimension = new array_dimension (size); else dimension . insert_dimension (size); dimensions++;}
	public void insert_elements () {if (dimension == null) return; dimension . insert_elements ();}
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {atom . setMachine (null); return true;}
		int ind = dimensions;
		array_dimension current_dimension = dimension;
		PrologElement left;
		while (ind > 1) {
			if (! parameters . isPair ()) return false;
			left = parameters . getLeft ();
			if (! left . isInteger ()) return false;
			parameters = parameters . getRight ();
			current_dimension = current_dimension . get_dimension (left . getInteger ());
			if (current_dimension == null) return false;
			ind--;
		}
		if (! parameters . isPair ()) return false;
		left = parameters . getLeft ();
		if (! left . isInteger ()) return false;
		parameters = parameters . getRight ();
		int location = left . getInteger ();
		if (parameters . isPair ()) return current_dimension . set_element (location, parameters . getLeft () . duplicate ());
		if (parameters . isVar ()) {parameters . duplicate (current_dimension . get_element (location)); return true;}
		return false;
	}
	public array_code (PrologAtom atom) {this . atom = atom;}
}

class ARRAY extends PrologNativeCode {
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement left = parameters . getLeft ();
		PrologAtom atom = null;
		if (left . isVar ()) left . setAtom (atom = new PrologAtom ());
		else {if (! left . isAtom ()) return false; atom = left . getAtom ();}
		if (atom . getMachine () != null) return false;
		array_code a = new array_code (atom);
		parameters = parameters . getRight ();
		while (parameters . isPair ()) {
			left = parameters . getLeft (); if (! left . isInteger ()) return false;
			a . insert_dimension (left . getInteger ());
			parameters = parameters . getRight ();
		}
		a . insert_elements ();
		return atom . setMachine (a);
	}
}

//////////////////////////
// MULTICORE RESOLUTION //
//////////////////////////

/*
class index_resser {
public:
	PrologElement * res;
	pthread_mutex_t critical;
	void accumulate (PrologElement * el) {
		pthread_mutex_lock (& critical);
		res -> setPair (); res -> setLeft (el -> duplicate ());
		res = res -> getRight ();
		pthread_mutex_unlock (& critical);
	}
	index_resser (PrologElement * res) {this -> res = res; critical = PTHREAD_MUTEX_INITIALIZER;}
	~ index_resser (void) {pthread_mutex_destroy (& critical);}
};

class index_substack : public PrologNativeCode {
public:
	index_resser * res;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		while (parameters -> isPair ()) {
			res -> accumulate (parameters -> getLeft ());
			parameters = parameters -> getRight ();
		}
		return true;
	}
	index_substack (index_resser * res) {this -> res = res;}
};

class multicore_index : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * processor;
	int clause_pointer;
	PrologElement * * clauses;
	void process_clause (index_resser * resser, PrologElement * pattern, PrologElement * parameters, int index) {
		PrologResolution reso (root);
		if (! reso . match (parameters, true, clauses [index] -> getLeft () -> getRight (), false)) {reso . reset (); return;}
		PrologAtom * accu = new PrologAtom ();
		index_substack * stacker = new index_substack (resser);
		accu -> setMachine (stacker);
		PrologElement * query = root -> pair (root -> fail (), root -> earth ());
		query = root -> pair (root -> pair (root -> atom (accu),
							root -> pair (reso . match_product (pattern, true),
							root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (processor),
							reso . match_product (clauses [index] -> getRight (), false)), query);
		query = root -> pair (root -> atom (accu), query);
		reso . reset ();
		root -> resolution (query);
		delete query;
	}
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		if (processor == 0) return false;
		if (! parameters -> isPair ()) return false;
		PrologElement * res = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * pattern = parameters -> getLeft (); parameters = parameters -> getRight ();
		index_resser resser (res);
		#pragma omp parallel for
		for (int ind = 0; ind < clause_pointer; ind++) {
			//printf ("<%i %i %i> ", ind, omp_get_thread_num (), omp_get_num_threads ());
			process_clause (& resser, pattern, parameters, ind);
		}
		return true;
	}
	multicore_index (PrologRoot * root, PrologDirectory * studio, PrologAtom * atom, PrologAtom * clause) {
		this -> root = root;
		this -> atom = atom;
		processor = studio -> searchPrivateAtom ("res");
		if (processor != 0) {COLLECTOR_REFERENCE_INC (processor);}
		clause_pointer = 0;
		PrologElement * clp = clause -> firstClause;
		while (clp != 0) {
			clause_pointer++;
			clp = (PrologElement *) clp -> getLeft () -> getLeft () -> getHead ();
		}
		if (clause_pointer < 1) {clauses = 0; return;}
		clauses = new PrologElementPointer [clause_pointer];
		clp = clause -> firstClause;
		int counter = 0;
		while (clp != 0) {
			clauses [counter++] = clp -> duplicate ();
			clp = (PrologElement *) clp -> getLeft () -> getLeft () -> getHead ();
		}
	}
	~ multicore_index (void) {
		if (processor != 0) processor -> removeAtom ();
		if (clauses != 0) {
			for (int ind = 0; ind < clause_pointer; ind++) delete clauses [ind];
			delete [] clauses;
		}
	}
};

class INDEX : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologDirectory * studio;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * atom = 0;
		PrologElement * clause = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isVar ()) atom = el;
			if (el -> isAtom ()) {if (atom == 0) atom = el; else clause = el;}
			parameters = parameters -> getRight ();
		}
		if (atom == 0 || clause == 0) return false;
		if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
		if (atom -> getAtom () -> getMachine () != 0) return false;
		multicore_index * i = new multicore_index (root, studio, atom -> getAtom (), clause -> getAtom ());
		if (atom -> getAtom () -> setMachine (i)) return true;
		delete i;
		return false;
	}
	INDEX (PrologRoot * root, PrologDirectory * studio) {this -> root = root; this -> studio = studio;}
};*/

/////////////////////
// NOISE GENERATOR //
/////////////////////

class PrologNoise {
	public int a, c, v, range, m;
	public int get () {return v = (v * a + c) & m;}
	public int get (int min, int max) {
		double out = (double) get () * (double) (max - min);
		out /= (double) range;
		return min + (int) out;
	}
	public void resolution (int bits) {range = 1 << bits; m = range - 1;}
	public void control (int v) {this . v = v;}
	public void control (int a, int c) {this . a = a; this . c = c;}
	public PrologNoise () {resolution (24); control (0); control (0x5599d1, 1);}
}

class rnd extends PrologNativeCode {
	public PrologNoise n;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isVar ()) {parameters . setInteger (n . get ()); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement a = parameters . getLeft (); parameters = parameters . getRight ();
		if (parameters . isEarth ()) {a . setInteger (n . get ()); return true;}
		if (! parameters . isPair ()) return false;
		PrologElement b = parameters . getLeft ();
		PrologElement c = parameters . getRight ();
		if (c . isPair ()) c = c . getLeft ();
		if (a . isInteger ()) {
			if (b . isInteger ()) {c . setInteger (n . get (a . getInteger (), b . getInteger ())); return true;}
			else if (c . isInteger ()) {b . setInteger (n . get (a . getInteger (), c . getInteger ())); return true;}
			else return false;
		}
		if (b . isInteger ()) {if (c . isInteger ()) {a . setInteger (n . get (b . getInteger (), c . getInteger ())); return true;}}
		return false;
	}
	public rnd (PrologNoise n) {this . n = n;}
}

class rnd_control extends PrologNativeCode {
	public PrologNoise n;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		// rnd_control [out]
		// rnd_control [a c]
		// rnd_control [out a c]
		// rnd_control [out bits a c]
		int [] vector = new int [4];
		int vector_counter = 0;
		while (parameters . isPair ()) {
			if (vector_counter > 3) return false;
			PrologElement el = parameters . getLeft ();
			if (el . isInteger ()) vector [vector_counter++] = el . getInteger ();
			else if (el . isDouble ()) vector [vector_counter++] = (int) el . getDouble ();
			else return false;
			parameters = parameters . getRight ();
		}
		switch (vector_counter) {
		case 1: n . control (vector [0]); return true;
		case 2: n . control (vector [0], vector [1]); return true;
		case 3: n . control (vector [0]); n . control (vector [1], vector [2]); return true;
		case 4: n . resolution (vector [1]); n . control (vector [0]); n . control (vector [2], vector [3]); return true;
		default: break;
		}
		return false;
	}
	public rnd_control (PrologNoise n) {this . n = n;}
}

class series extends PrologNativeCode {
	public PrologNoise n;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (! parameters . isPair ()) return false;
		PrologElement e_root = parameters . getLeft ();
		if (e_root . isPair () || e_root . isEarth ()) {
			PrologElement ret = parameters . getRight ();
			if (ret . isPair ()) ret = ret . getLeft ();
			int length = 0; while (e_root . isPair ()) {e_root = e_root . getRight (); length++;}
			if (length < 1) {ret . setEarth (); return true;}
			PrologElement [] table = new PrologElement [length];
			length = 0;
			parameters = parameters . getLeft ();
			while (parameters . isPair ()) {table [length++] = parameters . getLeft (); parameters = parameters . getRight ();}
			while (length > 0) {
				int select = n . get (0, length--);
				ret . setPair ();
				ret . setLeft (table [select] . duplicate ());
				ret = ret . getRight ();
				table [select] = table [length];
			}
			return true;
		}
		parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e_length = parameters . getLeft (); if (! e_length . isInteger ()) return false;
		int length = e_length . getInteger (); if (length < 1) return false;
		parameters = parameters . getRight (); if (! parameters . isPair ()) return false;
		PrologElement e_step = parameters . getLeft (); parameters = parameters . getRight (); if (parameters . isPair ()) parameters = parameters . getLeft ();
		double root;
		if (e_root . isInteger ()) root = (double) e_root . getInteger ();
		else {if (! e_root . isDouble ()) return false; root = e_root . getDouble ();}
		double step;
		if (e_step . isInteger ()) step = (double) e_step . getInteger ();
		else {if (! e_step . isDouble ()) return false; step = e_step . getDouble ();}
		double [] table = new double [length];
		for (int ind = 0; ind < length; ind++) {table [ind] = root; root += step;}
		boolean double_type = e_root . isDouble () || e_step . isDouble ();
		while (length > 0) {
			int select = n . get (0, length--);
			parameters . setPair ();
			if (double_type) parameters . getLeft () . setDouble (table [select]);
			else parameters . getLeft () . setInteger ((int) table [select]);
			parameters = parameters . getRight ();
			table [select] = table [length];
		}
		return true;
	}
	public series (PrologNoise n) {this . n = n;}
}

/////////////
// CRACKER //
/////////////

class wait extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isVar ()) {parameters . setInteger ((int) root . get_system_time ()); return true;}
		if (! parameters . isPair ()) return false;
		parameters = parameters . getLeft ();
		if (parameters . isVar ()) {parameters . setInteger ((int) root . get_system_time ()); return true;}
		if (! parameters . isInteger ()) return false;
		root . wait_time (parameters . getInteger ());
		return true;
	}
	public wait (PrologRoot root) {this . root = root;}
}

class timeout_code extends PrologNativeCode {
	public PrologRoot root;
	// [timeout]               immediate (clears previous commands)
	// [timeout []]            clears everything
	// [timeout 127]           obvious (clears previous commands)
	// [timeout [sonda]]       immediate (clears previous commands)
	// [timeout 127 [sonda]]   obvious (clears previous commands)
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {resolution . timeout_query = null; resolution . timeout = root . get_system_time (); return true;}
		PrologElement timeout = parameters . getLeft ();
		if (timeout . isEarth ()) {resolution . timeout_query = null; resolution . timeout = 0; return true;}
		if (! timeout . isInteger ()) {resolution . timeout_query = parameters . duplicate (); resolution . timeout = root . get_system_time (); return true;}
		parameters = parameters . getRight ();
		resolution . timeout_query = parameters . isEarth () ? null : parameters . duplicate ();
		resolution . timeout = root . get_system_time () + timeout . getInteger ();
		return true;
	}
	public timeout_code (PrologRoot root) {this . root = root;}
}

class crack extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {if (parameters . isEarth ()) return true; root . start (parameters); return true;}
	public crack (PrologRoot root) {this . root = root;}
}

class semaphore_code extends PrologNativeCode {
	public PrologAtom atom;
	public PrologAtom waitAtom;
	public PrologAtom enterAtom;
	public PrologAtom signalAtom;
	public Semaphore semaphore;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isEarth ()) {atom . setMachine (null); return true;}
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (! parameters . isAtom ()) return false;
		PrologAtom ctrl = parameters . getAtom ();
		if (ctrl == signalAtom) {semaphore . release (); return true;}
		if (ctrl == waitAtom) {try {semaphore . acquire ();} catch (Exception ex) {} return true;}
		if (ctrl == enterAtom) {return semaphore . tryAcquire ();}
		return false;
	}
	public semaphore_code (PrologAtom atom, PrologAtom waitAtom, PrologAtom enterAtom, PrologAtom signalAtom, int ind) {
		this . atom = atom;
		this . waitAtom = waitAtom;
		this . enterAtom = enterAtom;
		this . signalAtom = signalAtom;
		semaphore = new Semaphore (ind);
	}
}

class semaphore_maker extends PrologNativeCode {
	public PrologAtom waitAtom;
	public PrologAtom enterAtom;
	public PrologAtom signalAtom;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (waitAtom == null || enterAtom == null || signalAtom == null) return false;
		if (! parameters . isPair ()) return false;
		PrologElement ea = parameters . getLeft ();
		PrologAtom atom = null;
		if (ea . isVar ()) {ea . setAtom (atom = new PrologAtom ());}
		else {if (! ea . isAtom ()) return false; atom = ea . getAtom ();}
		int ind = 1;
		parameters = parameters . getRight ();
		if (parameters . isPair ()) {parameters = parameters . getLeft (); if (! parameters . isInteger ()) return false; ind = parameters . getInteger (); if (ind < 0) return false;}
		if (atom . getMachine () != null) return false;
		// if (mutexed)....
		return atom . setMachine (new semaphore_code (atom, waitAtom, enterAtom, signalAtom, ind));
	}
	public semaphore_maker (PrologDirectory dir) {
		waitAtom = enterAtom = signalAtom = null;
		if (dir == null) return;
		waitAtom = dir . searchAtom ("wait");
		enterAtom = dir . searchAtom ("enter");
		signalAtom = dir . searchAtom ("signal");
	}
}

///////////
// MUTEX //
///////////

class mutex_maker extends PrologNativeCode {
	public PrologAtom waitAtom;
	public PrologAtom enterAtom;
	public PrologAtom signalAtom;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (waitAtom == null || enterAtom == null || signalAtom == null) return false;
		if (! parameters . isPair ()) return false;
		PrologElement ea = parameters . getLeft ();
		PrologAtom atom = null;
		if (ea . isVar ()) {ea . setAtom (atom = new PrologAtom ());}
		else {if (! ea . isAtom ()) return false; atom = ea . getAtom ();}
		if (atom . getMachine () != null) return false;
		return atom . setMachine (new semaphore_code (atom, waitAtom, enterAtom, signalAtom, 1));
	}
	public mutex_maker (PrologDirectory dir) {
		waitAtom = enterAtom = signalAtom = null;
		if (dir == null) return;
		waitAtom = dir . searchAtom ("wait");
		enterAtom = dir . searchAtom ("enter");
		signalAtom = dir . searchAtom ("signal");
	}
}

/*
class PrologMutex : public PrologNativeCode {
public:
	PrologAtom * atom;
	PrologAtom * waitAtom, * enterAtom, * signalAtom;
	pthread_mutex_t mutex;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (! parameters -> isAtom ()) return false;
		PrologAtom * ctrl = parameters -> getAtom ();
		if (ctrl == signalAtom) {pthread_mutex_unlock (& mutex); return true;}
		if (ctrl == waitAtom) {pthread_mutex_lock (& mutex); return true;}
		if (ctrl == enterAtom) {return pthread_mutex_trylock (& mutex) == 0;}
		return false;
	}
	PrologMutex (PrologAtom * atom, PrologAtom * waitAtom, PrologAtom * enterAtom, PrologAtom * signalAtom) {
		this -> atom = atom;
		this -> waitAtom = waitAtom;
		this -> enterAtom = enterAtom;
		this -> signalAtom = signalAtom;
		mutex = PTHREAD_MUTEX_INITIALIZER;
	}
	~ PrologMutex (void) {pthread_mutex_destroy (& mutex);}
};

class MutexMaker : public PrologNativeCode {
public:
	PrologAtom * waitAtom;
	PrologAtom * enterAtom;
	PrologAtom * signalAtom;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (waitAtom == 0 || enterAtom == 0 || signalAtom == 0) return false;
		if (! parameters -> isPair ()) return false;
		PrologElement * ae = parameters -> getLeft ();
		PrologAtom * atom = 0;
		if (ae -> isVar ()) {ae -> setAtom (atom = new PrologAtom ());}
		else {if (! ae -> isAtom ()) return false; atom = ae -> getAtom ();}
		if (atom -> getMachine () != 0) return false;
		PrologMutex * mutex = new PrologMutex (atom, waitAtom, enterAtom, signalAtom);
		if (atom -> setMachine (mutex)) return true;
		delete mutex;
		return false;
	}
	MutexMaker (PrologDirectory * dir) {
		waitAtom = enterAtom = signalAtom = 0;
		if (dir == 0) return;
		waitAtom = dir -> searchAtom ("wait");
		enterAtom = dir -> searchAtom ("enter");
		signalAtom = dir -> searchAtom ("signal");
	}
};*/

////////////////////////
// console procedures //
////////////////////////

class bgcolour extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isInteger ()) {root . setBackground (parameters . getInteger ()); return true;}
		if (parameters . isVar ()) {parameters . setInteger (root . current_background); return true;}
		return false;
	}
	public bgcolour (PrologRoot root) {this . root = root;}
}

class fgcolour extends PrologNativeCode {
	public PrologRoot root;
	public boolean code (PrologElement parameters, PrologResolution resolution) {
		if (parameters . isPair ()) parameters = parameters . getLeft ();
		if (parameters . isInteger ()) {root . setForeground (parameters . getInteger ()); return true;}
		if (parameters . isVar ()) {parameters . setInteger (root . current_foreground); return true;}
		return false;
	}
	public fgcolour (PrologRoot root) {this . root = root;}
}

/*

class open_editor : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			root -> openEditor ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * selector = parameters -> getLeft ();
		if (! selector -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			root -> openEditor (selector -> getInteger ());
			return true;
		}
		PrologElement * sub_selector = parameters -> getLeft ();
		if (! sub_selector -> isInteger ()) return false;
		root -> openEditor (selector -> getInteger (), sub_selector -> getInteger ());
		return true;
	}
	open_editor (PrologRoot * root) {this -> root = root;}
};

class close_editor : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {
			root -> closeEditor ();
			return true;
		}
		if (! parameters -> isPair ()) return false;
		PrologElement * selector = parameters -> getLeft ();
		if (! selector -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (parameters -> isEarth ()) {
			root -> closeEditor (selector -> getInteger ());
			return true;
		}
		PrologElement * sub_selector = parameters -> getLeft ();
		if (! sub_selector -> isInteger ()) return false;
		root -> closeEditor (selector -> getInteger (), sub_selector -> getInteger ());
		return true;
	}
	close_editor (PrologRoot * root) {this -> root = root;}
};

class screen_coordinates : public PrologNativeCode {
public:
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * x = parameters -> getLeft ();
		if (! x -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		parameters = parameters -> getLeft ();
		if (! parameters -> isInteger ()) return false;
		root -> setScreenCoordinates (x -> getInteger (), parameters -> getInteger ());
		return true;
	}
	screen_coordinates (PrologRoot * root) {this -> root = root;}
};

///////////////////////////
// SERIAL NUMBER SERVICE //
///////////////////////////

#include "encoder.h"

class get_volume_serial_number : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		char serial_number [64];
		encoder e;
		e . volumize_serial (serial_number, root -> serial_number, root -> volume_id);
		parameters -> setPair (root -> text (serial_number), root -> earth ());
		return true;
	}
	get_volume_serial_number (PrologRoot * root) {this -> root = root;}
};

class security_check : public PrologNativeCode {
public:
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		encoder e;
		return e . check_serial (root -> serial_number, root -> volume_id, root -> key, root -> serial_shift);
	}
	security_check (PrologRoot * root) {this -> root = root;}
};

class encoder_class : public PrologNativeCode {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * el [4] = {0, 0, 0, 0};
		int element_counter = 0;
		encoder e;
		char serial [32];
		char key [32];
		while (parameters -> isPair () && element_counter < 4) {
			el [element_counter++] = parameters -> getLeft ();
			parameters = parameters -> getRight ();
		}
		if (element_counter == 3 && el [0] -> isText () && el [1] -> isText () && el [2] -> isText ()) {
			if (strlen (el [2] -> getText ()) < 2) return false;
			FILE * fr = fopen (el [0] -> getText (), "rb");
			if (fr == 0) return false;
			FILE * tc = fopen (el [1] -> getText (), "wb");
			if (tc == 0) {fclose (fr); return false;}
			char * pp = el [2] -> getText ();
			int ch = fgetc (fr);
			while (ch >= 0) {
				ch ^= (int) (* pp++);
				fputc (ch, tc);
				if (* pp == '\0') pp = el [2] -> getText ();
				ch = fgetc (fr);
			}
			fclose (fr); fclose (tc);
			return true;
		}
		if (element_counter == 3 && el [0] -> isText ()) {
			e . normalize_serial (serial, el [0] -> getText ());
			e . calculate_key (key, serial);
			el [1] -> setText (serial);
			el [2] -> setText (key);
			return true;
		}
		if (element_counter == 4 && el [0] -> isText () && el [1] -> isInteger ()) {
			e . normalize_serial (serial, el [0] -> getText ());
			e . calculate_key (key, serial, el [1] -> getInteger ());
			el [2] -> setText (serial);
			el [3] -> setText (key);
			return true;
		}
		return false;
	}
};

///////////////////
// SERVICE CLASS //
///////////////////

void PrologStudio :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
	this -> directory = directory;
	stdr . setRoot (root);
}

	return NULL;
}
*/

class PrologStudio extends PrologServiceClass {
	public PrologRoot root;
	public PrologDirectory directory;
	public PrologReader stdr = new PrologReader ();
	public PrologNoise n = new PrologNoise ();
	public void init (PrologRoot root, PrologDirectory directory) {this . root = root; this . directory = directory; stdr . setRoot (root);}
	public PrologNativeCode getNativeCode (String name) {
		if (name . equals ("add1")) return new add1 ();
		if (name . equals ("sub1")) return new sub1 ();
		if (name . equals ("sum")) return new sum ();
		if (name . equals ("add")) return new add ();
		if (name . equals ("sub")) return new sub ();
		if (name . equals ("times")) return new times ();
		if (name . equals ("mac")) return new mac ();
	/*
	if (strcmp (name, "mult") == 0) return new mult ();
	*/
		if (name . equals ("div")) return new division ();
	/*
	if (strcmp (name, "mod") == 0) return new mod ();
	if (strcmp (name, "and") == 0) return new logical_and ();
	if (strcmp (name, "or") == 0) return new logical_or ();
	if (strcmp (name, "xor") == 0) return new logical_xor ();

	if (strcmp (name, "abs") == 0) return new abs_operation ();
	if (strcmp (name, "cos") == 0) return new cos_operation ();
	if (strcmp (name, "degrad") == 0) return new degrad_operation ();
	if (strcmp (name, "e") == 0) return new e_operation ();
	if (strcmp (name, "exp") == 0) return new exp_operation ();
	if (strcmp (name, "log2") == 0) return new log2_operation ();
	if (strcmp (name, "log10") == 0) return new log10_operation ();
	if (strcmp (name, "ln") == 0) return new ln_operation ();
	if (strcmp (name, "log") == 0) return new log_operation ();
	if (strcmp (name, "pi") == 0) return new pi_operation ();
	if (strcmp (name, "pow") == 0) return new pow_operation ();
	if (strcmp (name, "sin") == 0) return new sin_operation ();
	if (strcmp (name, "tan") == 0) return new tan_operation ();
	if (strcmp (name, "trunc") == 0) return new trunc_operation ();

	if (strcmp (name, "StringToLower") == 0) return new StringToLower ();
	if (strcmp (name, "StringToUpper") == 0) return new StringToUpper ();
	if (strcmp (name, "StringReplaceOnce") == 0) return new StringReplaceOnce ();
	if (strcmp (name, "StringReplaceAll") == 0) return new StringReplaceAll ();

	if (strcmp (name, "DFT") == 0) return new DFT (false);
	if (strcmp (name, "FFT") == 0) return new DFT (true);

	if (strcmp (name, "timestamp") == 0) return new timestamp ();*/

		if (name . equals ("is_atom")) return new is_atom ();
		if (name . equals ("is_integer")) return new is_integer ();
		if (name . equals ("is_double")) return new is_double ();
		if (name . equals ("is_number")) return new is_number ();
		if (name . equals ("is_var")) return new is_var ();
		if (name . equals ("is_head")) return new is_head ();
		if (name . equals ("is_text")) return new is_text ();
		if (name . equals ("has_machine")) return new has_machine (root);
		/*
	if (strcmp (name, "has_machine") == 0) return new has_machine (root);
	if (strcmp (name, "text_list") == 0) return new text_list ();*/
		if (name . equals ("text_term")) return new text_term (root);
	/*if (strcmp (name, "e32") == 0) return new e32 ();
	*/
		if (name . equals ("less")) return new less ();
		if (name . equals ("less_eq")) return new less_eq ();
		if (name . equals ("greater")) return new greater ();
		/*
	if (strcmp (name, "greater") == 0) return new greater ();
	if (strcmp (name, "greater_eq") == 0) return new greater_eq ();
	if (strcmp (name, "max") == 0) return new max_class ();
	if (strcmp (name, "min") == 0) return new min_class ();
	*/
		if (name . equals ("set_uap32_captions")) return new set_uap32_captions (root);
		if (name . equals ("set_standard_captions")) return new set_standard_captions (root);
		if (name . equals ("set_edinburg_captions")) return new set_edinburg_captions (root);
		if (name . equals ("set_marseille_captions")) return new set_marseille_captions (root);
		if (name . equals ("set_functional_captions")) return new set_functional_captions (root);
		if (name . equals ("auto_atoms")) return new auto_atoms (root);
		if (name . equals ("scripted_atoms")) return new scripted_atoms (root);
		if (name . equals ("pr")) return new pr (stdr);
		if (name . equals ("read")) return new read (stdr);
		if (name . equals ("readln")) return new readln (stdr);
		if (name . equals ("pp")) return new pp (root);
		if (name . equals ("pt")) return new pt (root);
		if (name . equals ("write")) return new write (root);
		if (name . equals ("nl")) return new nl (root);
		if (name . equals ("list")) return new list (root);
		if (name . equals ("CL")) return new CL ();
		if (name . equals ("addcl")) return new addcl (root);
		if (name . equals ("addcl0")) return new addcl0 (root);
		if (name . equals ("delcl")) return new delcl ();
		if (name . equals ("overwrite")) return new overwrite ();
		if (name . equals ("create_atom")) return new create_atom (root);
		if (name . equals ("create_atoms")) return new create_atoms (root);
	/*
	if (strcmp (name, "create_atoms") == 0) return new create_atoms (root);
	if (strcmp (name, "search_atom") == 0) return new search_atom (root, false);
	if (strcmp (name, "search_atom_c") == 0) return new search_atom (root, true);
	if (strcmp (name, "unique_atoms") == 0) return new unique_atoms (root);*/
		if (name . equals ("preprocessor")) return new preprocessor (root);/*
	if (strcmp (name, "prompt") == 0) return new prompt (root);
	if (strcmp (name, "query_stack") == 0) return new query_stack (root);
	if (strcmp (name, "object_counter") == 0) return new object_counter_class ();
	*/
		if (name . equals ("crack")) return new crack (root);
		if (name . equals ("wait")) return new wait (root);
		if (name . equals ("timeout")) return new timeout_code (root);
		if (name . equals ("semaphore")) return new semaphore_maker (directory);
		if (name . equals ("msemaphore")) return new semaphore_maker (directory);
		if (name . equals ("mutex")) return new mutex_maker (directory);
	/*
	if (strcmp (name, "file_writer") == 0) return new file_writer (root);
	*/
		if (name . equals ("file_reader")) return new file_reader (root);
		if (name . equals ("import_loader")) return new import_loader (root);
		if (name . equals ("load_loader")) return new load_loader (root);
		if (name . equals ("consult_loader")) return new consult_loader (root);
		if (name . equals ("remove_module")) return new remove_module (root);
		if (name . equals ("set_machine")) return new set_machine (root);
		if (name . equals ("machine_type")) return new machine_type ();
		if (name . equals ("create_module")) return new create_module (root);
		if (name . equals ("add_search_directory")) return new add_search_directory (root);
		if (name . equals ("search_directories")) return new search_directories (root);
	/*
	if (strcmp (name, "cd") == 0) return new cd (root);
	if (strcmp (name, "relativise_path") == 0) return new relativise_path (root);
	if (strcmp (name, "DIR") == 0) return new DIR (root);
	*/
		if (name . equals ("ARGS")) return new ARGS (root);
	/*
	if (strcmp (name, "edit") == 0) return new edit (root);
	if (strcmp (name, "execute") == 0) return new execute (root);
	if (strcmp (name, "make_directory") == 0) return new make_directory (root);
	if (strcmp (name, "erase") == 0) return new erase_file (root);
	if (strcmp (name, "erase_directory") == 0) return new erase_directory (root);
	if (strcmp (name, "move") == 0) return new move_file (root);
	if (strcmp (name, "copy") == 0) return new copy_file (root);
	if (strcmp (name, "save_history") == 0) return new history (root, true);
	if (strcmp (name, "load_history") == 0) return new history (root, false);
	*/
		if (name . equals ("operating_system")) return new operating_system ();
		if (name . equals ("implementation")) return new implementation_code ();

		if (name . equals ("rnd")) return new rnd (n);
		if (name . equals ("rnd_control")) return new rnd_control (n);
		if (name . equals ("series")) return new series (n);

		if (name . equals ("CONSTANT")) return new CONSTANT_CODE ();
		if (name . equals ("VARIABLE")) return new variable_code ();
		if (name . equals ("ACCUMULATOR")) return new accumulator_code ();
		if (name . equals ("STACK")) return new STACK_CODE ();
		if (name . equals ("QUEUE")) return new QUEUE_CODE ();
		if (name . equals ("ARRAY")) return new ARRAY ();
	/*
	if (strcmp (name, "INDEX") == 0) return new INDEX (root, directory);

	if (strcmp (name, "background") == 0) return new bgcolour (root);
	if (strcmp (name, "foreground") == 0) return new fgcolour (root);*/
		if (name . equals ("background")) return new bgcolour (root);
		if (name . equals ("foreground")) return new fgcolour (root);
	/*if (strcmp (name, "open_editor") == 0) return new open_editor (root);
	if (strcmp (name, "close_editor") == 0) return new close_editor (root);
	if (strcmp (name, "screen_coordinates") == 0) return new screen_coordinates (root);

	if (strcmp (name, "get_volume_serial_number") == 0) return new get_volume_serial_number (root);
	if (strcmp (name, "security_check") == 0) return new security_check (root);
	if (strcmp (name, "encoder") == 0) return new encoder_class ();
*/
		return new studio_code (name);
	}
}

