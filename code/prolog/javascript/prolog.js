
var prolog = new function () {

this . PrologAtom = function (name, root) {
	if (name === undefined) this . name = "<" + Math . random () . toString (16) . substring (2) + "#" + new Date () . getTime () . toString (16) + ">";
	else this . name = name + "";
	this . next = root === undefined ? null : root;
	this . Privated = false;
	this . Protected = false;
	this . firstClause = null;
	this . machine = null;
};
this . PrologAtom . prototype . names = function () {
	var area = "" + this . name;
	var sub = this . next;
	while (sub !== null) {area += " " + sub . name; sub = sub . next;}
	return area;
};
this . PrologAtom . prototype . search = function (name) {
	var ret = this;
	while (ret !== null && ret . Privated) {if (ret . name === name) return ret; ret = next;}
	return null;
};
this . PrologAtom . prototype . searchPrivate = function (name) {
	var ret = this;
	while (ret !== null) {if (ret . name === name) return ret; ret = next;}
	return null;
};
this . PrologAtom . prototype . setMachine = function (obj) {
	if (this . firstClause !== null || (this . Protected && obj !== null)) return false;
	this . Protected = (obj !== null);
	this . machine = obj;
	return true;
};
this . PrologAtom . prototype . isTypeOfAtom = function (atom) {
	if (this . machine === null) return false;
	var machine_type = atom . getMachine ();
	if (machine_type === null) return false;
	return this . machine . isTypeOf (machine_type . codeName ());
};
this . PrologAtom . prototype . isTypeOf = function (code) {
	if (this . machine === null) return false;
	if (code === null) return false;
	return this . machine . isTypeOf (code . codeName ());
};
this . PrologAtom . prototype . machineType = function () {return this . machine === null ? null : this . machine . codeName ();};
this . PrologAtom . prototype . getMachine = function () {return this . machine;};

this . PrologNativeCode = function () {};
var native_code_name = "PrologNativeCode";
this . PrologNativeCode . prototype . name = function () {return native_code_name;};

};
/*
package prolog;

public class PrologNativeCode {
	public static String name () {return "PrologNativeCode";}
	public String codeName () {return name ();}
	public boolean isTypeOf (String code_name) {return name () == code_name;}
	public boolean code (PrologElement parameters, PrologResolution resolution) {return true;}
};

static char * native_code_name = "PrologNativeCode";
char * PrologNativeCode :: name (void) {return native_code_name;}
char * PrologNativeCode :: codeName (void) {return name ();}
bool PrologNativeCode :: code (PrologElement * parameter, PrologResolution * resolution) {return true;}
bool PrologNativeCode :: isTypeOf (char * code_name) {return name () == code_name;}
PrologNativeCode :: PrologNativeCode (void) TRACKING (16) {}
PrologNativeCode :: ~ PrologNativeCode (void) {}
*/

