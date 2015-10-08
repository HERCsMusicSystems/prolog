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

public class PrologLoader extends PrologReader {
	public boolean drop_main = false;
	public boolean echo = false;
	public boolean reload = false;
	public int old_caption_id = 0;
	public boolean old_auto_atoms = false;
	public InputStream fi = null;
	public PrologLoader (PrologRoot root) {setRoot (root);}
	public void close () {if (fi != null) try {fi . close ();} catch (Exception ex) {} fi = null;}
	public int move_z () {
		if (fi == null) return -1;
		int ch;
		try {ch = fi . read ();} catch (Exception ex) {return -1;}
		if (echo && ch > 0) root . print_character (ch);
		return ch;
	}
	public boolean load (String file_name) {
		drop_main = false;
		old_caption_id = root . captionId ();
		old_auto_atoms = root . autoAtoms ();
		root . setCaptions (0, false);
		boolean ret = LOAD (file_name);
		clear_context ();
		root . setCaptions (old_caption_id, old_auto_atoms);
		return ret;
	}
	public boolean load_without_main (String file_name) {
		drop_main = true;
		old_caption_id = root . captionId ();
		old_auto_atoms = root . autoAtoms ();
		root . setCaptions (0, false);
		boolean ret = LOAD (file_name);
		clear_context ();
		root . setCaptions (old_caption_id, old_auto_atoms);
		return ret;
	}
	private boolean rc (String text) {root . message (text); close (); return false;}
	private boolean rdc (String text) {root . message (text); root . drop (); close (); return false;}
	public boolean LOAD (String file_name) {
		fi = PrologLoader . class . getResourceAsStream ("/" + file_name);
		if (fi == null) {
			try {
				fi = new FileInputStream (file_name);
				int search_index = 0;
				while (fi == null && search_index < root . search_directories . size ()) {
					fi = new FileInputStream (root . search_directories . get (search_index++) + file_name);
				}
			} catch (Exception ex) {root . message ("File Input Stream crashed: " + file_name); return false;}
		}
		if (fi == null) {root . message ("File not found: " + file_name); return false;}
		get_symbol ();
		clear_context ();
		PrologDirectory user_directory = root != null ? root . root : null;
		PrologDirectory import_directory;
		boolean ret;
		while (symbol_control == 11 && root . import_caption . equals (symbol)) {
			get_symbol ();
			switch (symbol_control) {
			case 8: if (root . searchDirectoryNotFound (symbol)) root . addSearchDirectory (symbol); break;
			case 11:
				import_directory = root . searchDirectory (symbol);
				if (import_directory == null) {
					PrologLoader loader = new PrologLoader (root);
					if (drop_main) ret = loader . load_without_main (symbol + ".prc");
					else ret = loader . load (symbol + ".prc");
					if (! ret) return rc ("Module not loaded: " + symbol + ".prc");
					import_directory = root . searchDirectory (symbol);
				}
				if (import_directory != null) search_context = import_directory . duplicate (search_context);
				break;
			default: return rc ("Syntax error: import module or search path expected.");
			}
			get_symbol ();
		}
		while (symbol_control == 31) {
			if (symbol . equals ("uap32")) root . set_uap32_captions ();
			if (symbol . equals ("standard")) root . set_standard_captions ();
			if (symbol . equals ("edinburg")) root . set_edinburg_captions ();
			if (symbol . equals ("marseille")) root . set_marseille_captions ();
			if (symbol . equals ("functional")) root . set_functional_captions ();
			get_symbol ();
		}
		if (symbol_control != 11 || ! root . program_caption . equals (symbol)) return rc ("Syntax error: program header expected.");
		get_symbol ();
		if (symbol_control != 11) return rc ("Syntax error: program name expected.");
		String program_name = "" + symbol;
		PrologServiceClass service_class = null;
		if (reload) root . drop (program_name);
		else if (root . searchDirectory (program_name) != null) {close (); return true;}
		get_symbol ();
		PrologDirectory directory;
		if (symbol_control == 11 && root . machine_caption . equals (symbol)) {
			get_symbol ();
			if (symbol_control != 11 || ! root . if_atom_caption . equals (symbol)) return rc ("Syntax error: machine assignment expected in program header.");
			get_symbol ();
			if (symbol_control != 8) return rc ("Syntax error: service class name expected.");
			service_class = root . loadServiceClass (symbol);
			if (service_class == null) return rc ("Service class crashed or not found: " + symbol);
			get_symbol ();
			directory = root . createDirectory (program_name, service_class);
			service_class . init (root, directory);
		} else directory = root . createDirectory (program_name);
		root . auto_atoms = false;
		switch (symbol_control) {
		case 11:
			if (! root . auto_atoms_caption . equals (symbol)) return rdc ("Syntax error: auto expected.");
			root . auto_atoms = true;
			get_symbol ();
			break;
		case 6: get_symbol (); break;
		case 1:
			get_symbol ();
			while (symbol_control != 2) {
				switch (symbol_control) {
				case 11: root . createAtom (symbol); break;
				case 21: root . createAtom (symbol); break;
				default: return rdc ("Syntax error: atom expected.");
				}
				get_symbol ();
				if (root . separator_caption . length () > 0) {
					if (symbol_control != 23 && symbol_control != 2) return rdc ("Syntax error: separator missing.");
					if (symbol_control == 23) get_symbol ();
				}
			}
			get_symbol ();
			break;
		default: return rdc ("Syntax error: atome list expected.");
		}
		if (directory != null) search_context = directory . duplicate (search_context);
		PrologDirectory dt = search_context;
		if (dt != null) {
			while (dt . next != null) dt = dt . next;
			dt . next = user_directory;
		}
		PrologElement clause;
		PrologAtom preprocessor = null;
		PrologDirectory preprocessor_directory;
		PrologAtom atom;
		while (true) {
			switch (symbol_control) {
			case 11:
				if (root . machine_caption . equals (symbol)) {
					get_symbol ();
					if (symbol_control != 11) return rdc ("Syntax error: atom expected.");
					atom = searchAtomC (symbol);
					if (atom == null) return rdc ("Unknown atom: " + symbol);
					if (atom . getMachine () != null) return rdc ("Atom has already machine code attached: " + symbol);
					get_symbol (); if (symbol_control != 11 || ! root . if_atom_caption . equals (symbol)) return rdc ("Syntax error: machine assignment expected.");
					get_symbol (); if (symbol_control != 8) return rdc ("Syntax error: machine name expected.");
					if (service_class == null) return rdc ("Service class missing.");
					PrologNativeCode native_code = service_class . getNativeCode (symbol); if (native_code == null) return rdc ("Unknown native class: " + symbol);
					atom . setMachine (native_code);
					get_symbol ();
					break;
				}
				if (root . end_caption . equals (symbol)) {
					get_symbol ();
					clause = null;
					if (symbol_control == 11 && root . if_atom_caption . equals (symbol)) {
						clause = readElement (); if (clause == null) return rdc ("Suspicious drop 1");
						get_symbol ();
					}
					if (symbol_control == 21) {
						if (echo) root . message ("");
						root . close ();
						if (clause != null) {
							if (drop_main) root . message ("Illegal instructions dropped " + root . getValue (clause));
							else {
								if (root . main_query != null) {root . message ("Instructions dropped " + root . getValue (root . main_query)); root . main_query = null;}
								clause = root . pair (root .head (null), clause);
								root . main_query = clause;
							}
						}
						close ();
						return true;
					}
					return rdc ("Syntax error: dot expected.");
				}
				if (root . protect_caption . equals (symbol)) {
					get_symbol ();
					if (symbol_control == 6) {get_symbol (); break;}
					if (symbol_control != 1) return rdc ("Syntax error: atome list expected.");
					get_symbol ();
					while (symbol_control != 2) {
						if (symbol_control != 11) return rdc ("Syntax error: atom expected.");
						if (! root . Protect (symbol)) return rdc ("Can not protect unknown atom: " + symbol);
						get_symbol ();
						if (root . separator_caption . length () > 0) {
							if (symbol_control != 23 && symbol_control != 2) return rdc ("Syntax error: separator missing.");
							if (symbol_control == 23) get_symbol ();
						}
					}
					get_symbol ();
					break;
				}
				if (root . private_caption . equals (symbol)) {
					get_symbol ();
					if (symbol_control == 6) {get_symbol (); break;}
					if (symbol_control != 1) return rdc ("Syntax error: atome list expected.");
					get_symbol ();
					while (symbol_control != 2) {
						if (symbol_control != 11) return rdc ("Syntax error: atom expected.");
						if (! root . Private (symbol)) return rdc ("Can not lock unknown atom as private: " + symbol);
						get_symbol ();
						if (root . separator_caption . length () > 0) {
							if (symbol_control != 23 && symbol_control != 2) return rdc ("Syntax error: separator missing.");
							if (symbol_control == 23) get_symbol ();
						}
					}
					get_symbol ();
					break;
				}
				if (root . preprocessor_caption . equals (symbol)) {
					get_symbol (); if (symbol_control != 11 && ! root . if_atom_caption . equals (symbol)) return rdc ("Syntax error: preprocessor assignment expected.");
					get_symbol ();
					if (symbol_control == 6) {preprocessor = null; get_symbol (); break;}
					if (symbol_control != 11) {
						if (symbol_control == 22) {
							get_symbol (); if (symbol_control != 11) return rdc ("Syntax error: directory name expected in qualified preprocessor name.");
							preprocessor_directory = root . searchDirectory (symbol);
							if (preprocessor_directory == null) return rdc ("Directory not found in qualified preprocessor name: " + symbol);
							get_symbol (); if (symbol_control != 21) return rdc ("Syntax error: dot expected in qualified preprocessor name.");
							get_symbol (); if (symbol_control != 11) return rdc ("Syntax error: qualified preprocessor name expected.");
							atom = preprocessor_directory . searchAtom (symbol);
						} else return rdc ("Syntax error: atom expected.");
					} else atom = searchAtom (symbol);
					if (atom == null) return rdc ("Unknown atom: " + symbol);
					preprocessor = atom;
					get_symbol ();
					break;
				}
				if (root . auto_caption . equals (symbol)) {
					get_symbol (); if (symbol_control != 11 && ! root . if_atom_caption . equals (symbol)) return rdc ("Syntax error: auto assignment expected.");
					clause = readElement ();
					if (clause == null) return rdc ("Suspicious drop 2.");
					clause = root . pair (root . head (null), clause);
					new PrologResolution (root) . resolution (clause);
					get_symbol ();
					break;
				}
			case 1:
				if (preprocessor != null) {
					clause = readElement (); if (clause == null) return rdc ("Failed to read and preprocess clause (1).");
					clause = readRightSide (clause, false); if (clause == null) return rdc ("Failed to read and preprocess clause (2).");
					get_symbol ();
					clause = root . pair (root . var (0), root . pair (root . pair (root . atom (preprocessor), root . pair (clause, root . earth ())), root . earth ()));
					new PrologResolution (root) . resolution (clause);
					break;
				}
				clause = readClause ();
				get_symbol (); if (clause == null) return rdc ("Suspicious drop 3.");
				if (root . attachClause (clause) == 0) break;
				return rdc ("Suspicious drop 4.");
			default: return rdc ("Syntax error: at least clause expected.");
			}
		}
	}
}
