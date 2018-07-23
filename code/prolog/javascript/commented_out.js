
//////// ATOM ////////

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

//////////////////

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

//////// NATIVE CODE ////////

this . PrologNativeCode = function () {};
this . PrologNativeCode . prototype . code = function () {return true;};
this . PrologNativeCode . prototype . codeName = function () {return "PrologNativeCode";};
this . PrologNativeCode . prototype . isTypeOf = function (code_name) {return code_name === "PrologNativeCode";};

//////// ELEMENT ////////

this . PrologElement . prototype . setEarth = function () {this . type = 0;};
this . PrologElement . prototype . setSlash = function () {this . type = 4;};
this . PrologElement . prototype . setFail = function () {this . type = 5;};
this . PrologElement . prototype . drop_one_right = function () {this . right = this . right . right;};
