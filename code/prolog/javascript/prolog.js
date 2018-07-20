
var prolog = new function () {

var hrcs = this;

//////// ATOM ////////

this . PrologAtom = function (name, root) {
	if (name === undefined) this . name = "<" + Math . random () . toString (16) . substring (2) + "#" + new Date () . getTime () . toString (16) + ">";
	else this . name = name + "";
	this . next = root === undefined ? null : root;
	this . Privated = false;
	this . Protected = false;
	this . firstClause = null;
	this . machine = null;
};
/*
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
*/
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

//////// NATIVE CODE ////////

this . PrologNativeCode = function () {};
this . PrologNativeCode . prototype . codeName = function () {return "PrologNativeCode";};
this . PrologNativeCode . prototype . isTypeOf = function (code_name) {return code_name === "PrologNativeCode";};
this . PrologNativeCode . prototype . code = function () {return true;};

//////// DIRECTORY /////////

this . PrologDirectory = function (name, root, service) {
	this . name = name + "";
	this . next = root == null ? null : root;
	this . firstAtom = null;
	this . service_class = service == null ? null : service;
};
this . PrologDirectory . prototype . getServiceClass = function (name) {
	if (name === undefined) return this . service_class;
	if (name === this . name) return this . service_class;
	if (this . next === null) return null;
	return this . next . getServiceClass (name);
};
this . PrologDirectory . prototype . names = function () {
	var area = [this . name];
	var next = this . next;
	while (next !== null) {area . push (next . name); next = next . next;}
	return area;
};
this . PrologDirectory . prototype . searchAtom = function (search) {
	if (this . firstAtom === null) return null;
	var atom = this . firstAtom;
	while (atom !== null) {if (atom . name === search && ! atom . Privated) return atom; atom = atom . next;}
	return null;
};
this . PrologDirectory . prototype . searchPrivateAtom = function (search) {
	if (this . firstAtom === null) return null;
	var atom = this . firstAtom;
	while (atom !== null) {if (atom . name === search) return atom; atom = atom . next;}
	return null;
};
this . PrologDirectory . prototype . createAtom = function (name) {
	this . firstAtom = new hrcs . PrologAtom (name, this . firstAtom);
	return this . firstAtom;
};
this . PrologDirectory . prototype . removeAtom = function (atom) {
	if (this . firstAtom === null) {if (this . next !== null) return this . next . removeAtom (atom); return false;}
	if (this . firstAtom === atom) {this . firstAtom = this . firstAtom . next; return true;}
	var ap = this . firstAtom;
	while (ap . next !== null) {
		if (ap . next === atom) {ap . next = ap . next . next; return true;}
		ap = ap . next;
	}
	if (this . next === null) return false;
	return this . next . removeAtom (atom);
};
this . PrologDirectory . prototype . list = function () {
	var area = [];
	var atom = this . firstAtom;
	while (atom !== null) {area . push (atom . name); atom = atom . next;}
	return area;
};
this . PrologDirectory . prototype . close = function () {
	if (this . next === null) return this;
	var sub = this . next; this . next = this . next . next; sub . next = this;
	return sub;
};
this . PrologDirectory . prototype . duplicate = function (root) {
	var dir = new hrcs . PrologDirectory (this . name, root, this . service_class);
	dir . fistAtom = this . firstAtom;
	return dir;
};

};
/*
*/
