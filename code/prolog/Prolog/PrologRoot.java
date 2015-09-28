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
	public void deleteSearchDirectories () {search_directories . clear ();}
	public void addSearchDirectory (String directory) {
		if (search_directories == null) search_directories = new ArrayList <String> ();
		search_directories . add (directory);
	}
	public void get_search_directories_from_environment (String text) {
		text = System . getenv (text);
		if (text == null) return;
		String [] locations = text . split (";");
		for (int ind = 0; ind < locations . length; ind++) {addSearchDirectory (locations [ind]);}
	}
	public void addArg (String arg) {if (args == null) args = new ArrayList <String> (); args . add (arg);}
	public void setServiceClassLoader (PrologServiceClassLoader loader) {service_loader = loader;}
	public void insertCommander (PrintStream command) {this . command = command;}
///
	public void resolution () {System . out . println ("Default resolution: studio");}
	public void resolution (String name) {System . out . println ("Named resolution: " + name);}
}

