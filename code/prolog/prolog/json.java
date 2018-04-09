///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2018 Robert P. Wolf                       //
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

package prolog;

import java . io . FileWriter;
import java . io . StringReader;

class json_native_class extends PrologNativeCode {
  json service;
  PrologRoot root = new PrologRoot ();
  public PrologAtom atomC (String name) {
    PrologAtom atom = service . root . search (name);
    if (atom == null) atom = service . root . createAtom (name);
    return atom;
  }
  public void read_json_pair (PrologElement el, PrologReader reader) {
    el . setPair (); el . getLeft () . setAtom (service . equal_atom);
    if (reader . symbol_control != 8) {reader . get_symbol (); return;}
    el = el . getRight (); el . setPair (); el . getLeft () . setAtom (atomC (reader . symbol));
    el = el . getRight (); el . setPair ();
    reader . get_symbol (); if (reader . symbol_control != 3) return;
    reader . get_symbol ();
    read_json (el . getLeft (), reader);
  }
  public void read_json (PrologElement el, PrologReader reader) {
    switch (reader . symbol_control) {
    case 10: el . setInteger (reader . int_symbol); reader . get_symbol (); break;
    case 18: el . setDouble (reader . double_symbol); reader . get_symbol (); break;
    case 11:
      if (reader . symbol . equals ("true")) el . setAtom (service . true_atom);
      else if (reader . symbol . equals ("false")) el . setAtom (service . false_atom);
      else if (reader . symbol . equals ("null")) el . setAtom (service . null_atom);
      reader . get_symbol ();
      break;
    case 8: el . setAtom (atomC (reader . symbol)); reader . get_symbol (); break;
    case 1:
      el . setPair ();
      reader . get_symbol ();
      read_json (el . getLeft (), reader);
      while (reader . symbol_control == 32) {el = el . getRight (); el . setPair (); reader . get_symbol (); read_json (el . getLeft (), reader);}
      if (reader . symbol_control == 2) reader . get_symbol ();
      break;
    case 51:
      el . setPair ();
      reader . get_symbol ();
      read_json_pair (el . getLeft (), reader);
      while (reader . symbol_control == 32) {el = el . getRight (); el . setPair (); reader . get_symbol (); read_json_pair (el . getLeft (), reader);}
      if (reader . symbol_control == 52) reader . get_symbol ();
      break;
    default: break;
    }
  }
  public String drop_text (String text) {
    try {
      StringReader rd = new StringReader (text);
      int ch = rd . read ();
      String ret = "";
      while (ch >= 0) {
        if (ch == '\\') ret += "\\\\";
        else if (ch == '\"') ret += "\\\"";
        else ret += (char) ch;
        ch = rd . read ();
      }
      return "\"" + ret + "\"";
    } catch (Exception ex) {}
    return text;
  }
  public String tabs (int tab) {String ret = ""; for (int ind = 0; ind < tab; ind++) ret += " "; return ret;}
  public String multiline (int tab, PrologElement el, String bracket) {
    int tabss = tab + 2;
    boolean comma = false;
    String ret = "";
    while (el . isPair ()) {
      if (comma) {ret += ","; if (tab < 0) ret += " ";} comma = true;
      if (tab >= 0) ret += "\n" + tabs (tabss);
      ret += drop (tabss, el . getLeft ());
      el = el . getRight ();
    }
    if (tab >= 0) ret += "\n" + tabs (tab);
    return ret + bracket;
  }
  public String drop (int tab, PrologElement el) {
    if (el . isAtom ()) {
      PrologAtom atom = el . getAtom ();
      if (atom == service . true_atom) return "true";
      else if (atom == service . false_atom) return "false";
      else if (atom == service . null_atom) return "null";
      else return drop_text (atom . name ());
    }
    if (el . isText ()) return drop_text (el . getText ());
    if (el . isInteger ()) return "" + el . getInteger ();
    if (el . isDouble ()) return "" + el . getDouble ();
    if (el . isPair ()) {
      PrologElement left = el . getLeft ();
      PrologElement right = el . getRight ();
      if (left . isAtom () && left . getAtom () == service . equal_atom) {
        if (! right . isPair ()) return ""; left = right . getLeft (); right = right . getRight ();
        if (! right . isPair ()) return ""; right = right . getLeft ();
        return drop (tab + 2, left) + ": " + drop (tab, right);
      }
      String left_bracket = "[", right_bracket = "]";
      if (left . isPair ()) left = left . getLeft ();
      if (left . isAtom () && left . getAtom () == service . equal_atom) {left_bracket = "{"; right_bracket = "}";}
      return left_bracket + multiline (tab, el, right_bracket);
    }
    if (el . isFail ()) return "false";
    if (el . isSlash ()) return "true";
    if (el . isVar ()) return "null";
    return "";
  }
  public boolean code (PrologElement parameters, PrologResolution resolution) {
    PrologElement path = null, variable = null, json = null;
    while (parameters . isPair ()) {
      PrologElement el = parameters . getLeft ();
      if (el . isText ()) {if (path == null) path = el; else json = el;}
      else if (el . isVar ()) variable = el;
      else json = el;
      parameters = parameters . getRight ();
    }
    if (json != null) {
      if (path == null) {
        if (variable == null) System . out . println (drop (0, json));
        else variable . setText (drop (Integer . MIN_VALUE, json));
      } else {try {FileWriter fw = new FileWriter (path . getText ()); fw . write (drop (0, json)); fw . close ();} catch (Exception ex) {return false;}}
      return true;
    } else {
      if (variable != null && path != null) {
        prolog . studio . TermReader reader = new prolog . studio . TermReader (root, path . getText ());
        reader . get_symbol (); read_json (variable, reader);
        return true;
      }
    }
    return false;
  }
  public json_native_class (json service) {
    this . service = service;
    root . separator_caption = ",";
    root . left_caption = "[";
    root . right_caption = "]";
    root . secondary_left_caption = "{";
    root . secondary_right_caption = "}";
    root . mid_caption = ":";
  }
}

public class json extends PrologServiceClass {
  public PrologRoot root;
  public PrologAtom true_atom, false_atom, null_atom, equal_atom;
  public void init (PrologRoot root, PrologDirectory directory) {
    if (root == null) return;
    this . root = root;
    PrologDirectory studio = root . searchDirectory ("studio");
    if (studio != null) equal_atom = studio . searchAtom ("=");
    if (directory == null) return;
    true_atom = directory . searchAtom ("true");
    false_atom = directory . searchAtom ("false");
    null_atom = directory . searchAtom ("null");
  }
  public PrologNativeCode getNativeCode (String name) {
    if (name . equals ("json")) return new json_native_class (this);
    return null;
  }
}

