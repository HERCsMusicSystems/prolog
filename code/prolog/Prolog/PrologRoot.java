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

import java . io . PrintStream;
import java . util . ArrayList;

public class PrologRoot {
	public PrologServiceClassLoader service_loader = null;
	public PrintStream command = System . out;
	public ArrayList <String> search_directories = new ArrayList <String> ();
	public ArrayList <String> args = new ArrayList <String> ();
	public boolean auto_atoms = false;
	public int caption_id = 1;
	public String left_caption = "(";
	public String right_caption = ")";
	public String secondary_left_caption = "[";
	public String secondary_right_caption = "]";
	public String mid_caption = "|";
	public String comment_caption = ";";
	public String fail_caption = "fail";
	public String slash_caption = "/";
	public String quotation_caption = "\"";
	public String atom_quotation_caption = "'";
	public String escape_caption = "\\";
	public String head_caption = ":head:";
	public String separator_caption = ",";
	public String directive_head_caption = "\\";
	public String machine_caption = "#machine";
	public String new_line_caption = "\n";
	public String var_head_captions = "_XYZxyz";
	public String var_tail_captions = "0123456789";
	public String var_caption = "_";
	public String atom_head_captions = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|:+-=%~#!$^{}";
	public String atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#!";
	public String at_caption = "@";
	public String dot_caption = ".";
	public String if_atom_caption = ":-";
	public String and_atom_caption = "+";
	public String program_caption = "program";
	public String import_caption = "import";
	public String auto_atoms_caption = "#auto";
	public String auto_caption = "auto";
	public String preprocessor_caption = "preprocessor";
	public String protect_caption = "protect";
	public String private_caption = "private";
	public String end_caption = "end";
	public String serial_number = "";
	public String key = "";
	public int volume_id = 0;
	public int serial_shift = 0;
	public PrologAtom preprocessor = null;
	public int current_foreground = 0xffff00;
	public int current_background = 0;
	public PrologDirectory root = new PrologDirectory ("user!", null);
	public void set_uap32_captions () {
		caption_id = 0;
		left_caption = "[";
		right_caption = "]";
		secondary_left_caption = "(";
		secondary_right_caption = ")";
		mid_caption = ":";
		comment_caption = ";";
		fail_caption = "fail";
		slash_caption = "/";
		quotation_caption = "\"";
		atom_quotation_caption = "'";
		escape_caption = "\\";
		head_caption = ":head:";
		separator_caption = "";
		directive_head_caption = "\\";
		machine_caption = "#machine";
		new_line_caption = "\n";
		var_head_captions = "*";
		var_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM";
		var_caption = "*";
		atom_head_captions = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|+-=%~#!$^{}";
		atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|+-=%~#!";
		at_caption = "@";
		dot_caption = ".";
		if_atom_caption = ":=";
		and_atom_caption = "^";
		program_caption = "program";
		import_caption = "import";
		auto_atoms_caption = "#auto";
		auto_caption = "auto";
		preprocessor_caption = "preprocessor";
		protect_caption = "protect";
		private_caption = "private";
		end_caption = "end";
	}
	public void set_standard_captions () {
		int caption_id = 1;
		left_caption = "(";
		right_caption = ")";
		secondary_left_caption = "[";
		secondary_right_caption = "]";
		mid_caption = "|";
		comment_caption = ";";
		fail_caption = "fail";
		slash_caption = "/";
		quotation_caption = "\"";
		atom_quotation_caption = "'";
		escape_caption = "\\";
		head_caption = ":head:";
		separator_caption = ",";
		directive_head_caption = "\\";
		machine_caption = "#machine";
		new_line_caption = "\n";
		var_head_captions = "_XYZxyz";
		var_tail_captions = "0123456789";
		var_caption = "_";
		atom_head_captions = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|:+-=%~#!$^{}";
		atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#!";
		at_caption = "@";
		dot_caption = ".";
		if_atom_caption = ":-";
		and_atom_caption = "+";
		program_caption = "program";
		import_caption = "import";
		auto_atoms_caption = "#auto";
		auto_caption = "auto";
		preprocessor_caption = "preprocessor";
		protect_caption = "protect";
		private_caption = "private";
		end_caption = "end";
	}
	public void set_edinburg_captions () {
		caption_id = 2;
		left_caption = "(";
		right_caption = ")";
		secondary_left_caption = "[";
		secondary_right_caption = "]";
		mid_caption = "|";
		comment_caption = ";";
		fail_caption = "fail";
		slash_caption = "!";
		quotation_caption = "\"";
		atom_quotation_caption = "'";
		escape_caption = "\\";
		head_caption = ":head:";
		separator_caption = ",";
		directive_head_caption = "\\";
		machine_caption = "#machine";
		new_line_caption = "\n";
		var_head_captions = "_QWERTYUIOPASDFGHJKLZXCVBNM*";
		var_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM";
		var_caption = "_";
		atom_head_captions = "_qwertyuiopasdfghjklzxcvbnm?<>&|:+-=%~#/$^{}";
		atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#";
		at_caption = "@";
		dot_caption = ".";
		if_atom_caption = ":-";
		and_atom_caption = "+";
		program_caption = "program";
		import_caption = "import";
		auto_atoms_caption = "#auto";
		auto_caption = "auto";
		preprocessor_caption = "preprocessor";
		protect_caption = "protect";
		private_caption = "private";
		end_caption = "end";
	}
	public void set_marseille_captions () {
		caption_id = 3;
		left_caption = "(";
		right_caption = ")";
		secondary_left_caption = "[";
		secondary_right_caption = "]";
		mid_caption = ":";
		comment_caption = ";";
		fail_caption = "fail";
		slash_caption = "/";
		quotation_caption = "\"";
		atom_quotation_caption = "'";
		escape_caption = "\\";
		head_caption = ":head:";
		separator_caption = ",";
		directive_head_caption = "\\";
		machine_caption = "#machine";
		new_line_caption = "\n";
		var_head_captions = "*";
		var_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM&:+-=";
		var_caption = "*";
		atom_head_captions = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|:+-=%~#!$^{}";
		atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|:+-=%~#!";
		at_caption = "@";
		dot_caption = ".";
		if_atom_caption = "+";
		and_atom_caption = "-";
		program_caption = "program";
		import_caption = "import";
		auto_atoms_caption = "#auto";
		auto_caption = "auto";
		preprocessor_caption = "preprocessor";
		protect_caption = "protect";
		private_caption = "private";
		end_caption = "end";
	}
	public void set_functional_captions () {
		caption_id = 4;
		left_caption = "[";
		right_caption = "]";
		secondary_left_caption = "(";
		secondary_right_caption = ")";
		mid_caption = ":";
		comment_caption = ";";
		fail_caption = "fail";
		slash_caption = "!";
		quotation_caption = "\"";
		atom_quotation_caption = "`";
		escape_caption = "\\";
		head_caption = ":head:";
		separator_caption = "";
		directive_head_caption = "\\";
		machine_caption = "#machine";
		new_line_caption = "\n";
		var_head_captions = "~";
		var_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM";
		var_caption = "~";
		atom_head_captions = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM'?<>&|+-*=/%#$^{}";
		atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM'?/<>&|+-*=%#";
		at_caption = "@";
		dot_caption = ".";
		if_atom_caption = ":=";
		and_atom_caption = "'";
		program_caption = "program";
		import_caption = "import";
		auto_atoms_caption = "#auto";
		auto_caption = "auto";
		preprocessor_caption = "preprocessor";
		protect_caption = "protect";
		private_caption = "private";
		end_caption = "end";
	}
	public void deleteSearchDirectories () {search_directories = new ArrayList <String> ();}
	public void addSearchDirectory (String directory) {
		if (search_directories == null) search_directories = new ArrayList <String> ();
		search_directories . add (directory);
	}
	public boolean searchDirectoryNotFound (String directory) {
		if (search_directories == null) return true;
		return ! search_directories . contains (directory);
	}
	public void get_search_directories_from_environment (String text) {
		text = System . getenv (text);
		if (text == null) return;
		String [] locations = text . split (";");
		for (int ind = 0; ind < locations . length; ind++) {addSearchDirectory (locations [ind]);}
	}
	public void addArg (String arg) {if (args == null) args = new ArrayList <String> (); args . add (arg);}
	public int captionId () {return caption_id;}
	public boolean autoAtoms () {return auto_atoms;}
	public void setCaptions (int caption_id, boolean auto_atoms) {
		this . auto_atoms = auto_atoms;
		switch (caption_id) {
		case 1: set_standard_captions (); break;
		case 2: set_edinburg_captions (); break;
		case 3: set_marseille_captions (); break;
		case 4: set_functional_captions (); break;
		default: set_uap32_captions (); break;
		}
	}
	public void set_auto_atoms () {auto_atoms = true;}
	public void set_scripted_atoms () {auto_atoms = false;}
	public void set_serial_number (String serial_number) {this . serial_number = serial_number;}
	public void set_key (String key) {this . key = key;}
	public void set_volume_id (int volume_id) {this . volume_id = volume_id;}
	public void set_serial_shift (int serial_shift) {this . serial_shift = serial_shift;}

	public String getRightCaption (PrologElement el) {
		if (el == null) return "<null element>";
		if (el . type == 0) return "";
		if (el . type == 1) return separator_caption + " " + getValue (el . getLeft ()) + getRightCaption (el . getRight ());
		return " " + mid_caption + " " + getValue (el);
	}
	public String getValue (PrologElement el) {
		if (el == null) return "<null element>";
		switch (el . type) {
		case 0: return left_caption + right_caption;
		case 1:
			if (el . getLeft () == null) return "Wrong left element.";
			if (el . getRight () == null) return "Wrong right element.";
			return left_caption + getValue (el . getLeft ()) + getRightCaption (el . getRight ()) + right_caption;
		case 2: return var_caption + el . getInteger ();
		case 3: return el . getAtom () . name ();
		case 4: return slash_caption;
		case 5: return fail_caption;
		case 6: return quotation_caption + el . getText () + quotation_caption;
		case 7: return head_caption + "<" + el . head + ">";
		case 8: return "" + el . getInteger ();
		case 9: return "" + el . getDouble ();
		default: break;
		}
		return "Unknown element type.";
	}

	public String getTrueValue (PrologElement el) {
		if (el == null) return "<null element>";
		switch (el . type) {
		case 0: return left_caption + right_caption;
		case 1:
			if (el . getLeft () == null) return "Wrong left element.";
			if (el . getRight () == null) return "Wrong right element.";
			return left_caption + getTrueValue (el . getLeft ()) + " " + mid_caption + " " + getTrueValue (el . getRight ()) + right_caption;
		case 2: return var_caption + el . getInteger ();
		case 3: return el . getAtom () . name ();
		case 4: return slash_caption;
		case 5: return fail_caption;
		case 6: return quotation_caption + el . getText () + quotation_caption;
		case 7: return head_caption + "<" + el . head + ">";
		case 8: return "" + el . getInteger ();
		case 9: return "" + el . getDouble ();
		default: break;
		}
		return "Unknown element type.";
	}

	public void setPreprocessor (PrologAtom atom) {preprocessor = atom;}
	public PrologAtom getPreprocessor () {return preprocessor;}

	public void setServiceClassLoader (PrologServiceClassLoader loader) {service_loader = loader;}
	public void insertCommander (PrintStream command) {this . command = command;}
	public PrintStream getCommander () {return command;}
	public void setBackground (int background) {current_background = background;}
	public void setForeground (int foreground) {current_foreground = foreground;}
	public PrologServiceClass getServiceClass () {if (root == null) return null; return root . getServiceClass ();}
	public PrologServiceClass getServiceClass (String name) {if (root == null) return null; return root . getServiceClass (name);}
	public PrologDirectory createDirectory (String name) {root = new PrologDirectory (name, root); return root;}
	public PrologDirectory createDirectory (String name, PrologServiceClass service) {root = new PrologDirectory (name, root, service); return root;}
	public PrologDirectory searchDirectory (String name) {
		PrologDirectory ret = root;
		while (ret != null) {if (ret . directoryName . equals (name)) return ret; ret = ret . next;}
		return null;
	}
	public PrologAtom search (String name) {
		PrologDirectory sub = root;
		while (sub != null) {PrologAtom ret = sub . searchAtom (name); if (ret != null) return ret; sub = sub . next;}
		return null;
	}
	public String list () {if (root == null) return ""; return root . names ();}
	public String list (String name) {
		PrologDirectory sub = searchDirectory (name);
		if (sub == null) return "";
		return sub . list ();
	}
	public void close () {if (root == null) return; root = root . close ();}
	public boolean drop () {
		if (root == null) return false;
		if (root . next == null) return false;
		root = root . next;
		return true;
	}
	public boolean drop (String name) {
		if (root == null) return false;
		if (root . directoryName . equals (name)) {root = root . next; return true;}
		PrologDirectory sub = root;
		while (sub . next != null) {
			if (sub . next . directoryName . equals (name)) {sub . next = sub . next . next; return true;}
			sub = sub . next;
		}
		return false;
	}
	public PrologAtom createAtom (String name) {if (root == null) return null; return root . createAtom (name);}
	public boolean removeAtom (PrologAtom atom) {if (root == null) return false; return root . removeAtom (atom);}
	public PrologElement earth () {return new PrologElement ();}
	public PrologElement pair (PrologElement left, PrologElement right) {
		PrologElement ret = new PrologElement ();
		ret . setPair (left, right);
		return ret;
	}
	public PrologElement var (int num) {PrologElement ret = new PrologElement (); ret . setVar (num); return ret;}
	public PrologElement atom (PrologAtom atom) {PrologElement ret = new PrologElement (); ret . setAtom (atom); return ret;}
	public PrologElement atomC (String name) {
		PrologElement ret = new PrologElement ();
		PrologAtom atom = search (name);
		if (atom == null) atom = root . createAtom (name);
		ret . setAtom (atom);
		return ret;
	}
	public PrologElement atom (String name) {
		PrologAtom atom = search (name);
		if (atom == null) {message ("Atom not found: " + name); return null;}
		PrologElement ret = new PrologElement ();
		ret . setAtom (atom);
		return ret;
	}
	PrologElement atom (String directory, String name) {
		PrologDirectory dir = searchDirectory (directory);
		if (dir == null) {message ("Qualifying directory not found: " + directory); return null;}
		PrologAtom atom = dir . searchAtom (name);
		if (atom == null) {message ("Qualified atom not found: " + name); return null;}
		PrologElement ret = new PrologElement ();
		ret . setAtom (atom);
		return ret;
	}
	public PrologElement slash () {PrologElement ret = new PrologElement (); ret . setSlash (); return ret;}
	public PrologElement fail () {PrologElement ret = new PrologElement (); ret . setFail (); return ret;}
	public PrologElement text (String text) {PrologElement ret = new PrologElement (); ret . setText (text); return ret;}
	public PrologElement head (Object obj) {PrologElement ret = new PrologElement (); ret . setHead (obj); return ret;}
///
	public void message (String text) {if (command != null) command . println (text);}
	public void resolution () {System . out . println ("Default resolution: studio");}
	public void resolution (String name) {System . out . println ("Named resolution: " + name);}
}

