
var prolog = new function () {

this . PrologAtom = function (name, root) {
	if (name === undefined) this . name = "<" + Math . random () . toString (16) . substring (2) + "#" + new Date () . getTime () . toString (16) + ">";
	else this . name = name + "";
	this . next = root === undefined ? null : root;
	this . Privated = false;
	this . Protected = false;
	this . firstClause = null;
	this . machine = null;
	this . names = function () {
		var area = "" + this . name;
		var sub = this . next;
		while (sub !== null) {area += " " + sub . name; sub = sub . next;}
		return area;
	};
	this . search = function (name) {
		var ret = this;
		while (ret !== null && ret . Privated) {if (ret . name === name) return ret; ret = next;}
		return null;
	};
	this . searchPrivate = function (name) {
		var ret = this;
		while (ret !== null) {if (ret . name === name) return ret; ret = next;}
		return null;
	};
	this . setMachine = function (obj) {
		if (this . firstClause !== null || (this . Protected && obj !== null)) return false;
		this . Protected = (obj !== null);
		this . machine = obj;
		return true;
	};
	this . isTypeOfAtom = function (atom) {
		if (this . machine === null) return false;
		var machine_type = atom . getMachine ();
		if (machine_type === null) return false;
		return this . machine . isTypeOf (machine_type . codeName ());
	};
	this . isTypeOf = function (code) {
		if (this . machine === null) return false;
		if (code === null) return false;
		return this . machine . isTypeOf (code . codeName ());
	};
	this . machineType = function () {return this . machine === null ? null : this . machine . codeName ();};
	this . getMachine = function () {return this . machine;};
};

};
/*
*/

