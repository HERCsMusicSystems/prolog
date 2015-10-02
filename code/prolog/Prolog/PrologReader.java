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

import java . util . ArrayList;

public class PrologReader {
	public PrologRoot root;
	public PrologDirectory search_context;
	public ArrayList <String> var_root = null;
	public int act_znak = 0;
	public void clear_context () {search_context = null;}
	public int move_z () {return 0;}
	public void setRoot (PrologRoot root) {this . root = root;}
	public int get_var_number (String name) {
		if (var_root == null) {var_root = new ArrayList <String> (); var_root . add (name . length () < 1 ? root . new_line_caption : name); return 0;}
		int ind = var_root . indexOf (name);
		if (ind >= 0) return ind;
		ind = var_root . size ();
		var_root . add (name . length () < 1 ? root . new_line_caption : name);
		return 0;
	}
	public void reset_var_number () {var_root = null;}
	public int lastRead () {return act_znak;}
	public String getString () {
		while (act_znak > 127 || act_znak <= 32) {if (act_znak < 0) return null; act_znak = move_z ();}
		String ret = "";
		while (act_znak > 32) {ret += (char) act_znak; act_znak = move_z ();}
		return ret;
	}
	public String getString (String char_set) {
		boolean not_readed = true;
		String ret = "";
		if (char_set . equals ("line")) {
			while (act_znak == 0 || act_znak == 13 || act_znak == 10) act_znak = move_z ();
			while (act_znak >= 0 && act_znak != 13 && act_znak != 10) {ret += (char) act_znak; act_znak = move_z (); not_readed = false;}
			if (act_znak < 0 && not_readed) return null;
			return ret;
		}
		if (char_set . equals ("word")) {
			while (act_znak >= 0 && act_znak <= 32) act_znak = move_z ();
			ret += (char) act_znak; act_znak = move_z ();
			while ("qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM-7894561230\"'`~." . indexOf ((char) act_znak) >= 0) {
				ret += (char) act_znak;
				act_znak = move_z ();
				not_readed = false;
			}
			if (act_znak < 0 && not_readed) return null;
			return ret;
		}
		while (act_znak >= 0 && char_set . indexOf ((char) act_znak) < 0) act_znak = move_z ();
		while (char_set . indexOf ((char) act_znak) >= 0) {ret += (char) act_znak; act_znak = move_z (); not_readed = false;}
		if (act_znak < 0 && not_readed) return null;
		return ret;
	}
	//
}
