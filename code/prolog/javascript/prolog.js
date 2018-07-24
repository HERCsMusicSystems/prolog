
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
this . PrologAtom . prototype . setMachine = function (obj) {
	if (this . firstClause !== null || (this . Protected && obj !== null)) return false;
	this . Protected = (obj !== null);
	this . machine = obj;
	return true;
};
this . PrologAtom . prototype . getMachine = function () {return this . machine;};

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

//////// ELEMENET ////////
// 0: EARTH, 1: PAIR, 2: VAR, 3: ATOM, 4: SLASH, 5: FAIL, 6: NATIVE

this . PrologElement = function () {this . type = 0; return;};

this . PrologElement . prototype . setPair = function () {
	this . type = 1;
	this . left = new hrcs . PrologElement ();
	this . right = new hrcs . PrologElement ();
};
this . PrologElement . prototype . setVar = function (ind) {this . type = 2; this . left = ind;};
this . PrologElement . prototype . setAtom = function (atom) {this . type = 3; this . left = atom;};
this . PrologElement . prototype . setNative = function (el) {this . type = 6; this . left = el;};
this . PrologElement . prototype . duplicate = function (el) {
	if (el === undefined) el = new hrcs . PrologElement ();
	el . type = this . type;
	switch (this . type) {
		case 0: case 4: case 5: return el;
		case 1:
			el . left = this . left . duplicate ();
			el . right = this . right . duplicate ();
			return el;
		default: el . left = this . left; return el;
	}
	return el;
};
this . PrologElement . prototype . attach = function (position) {
	if (this . type !== 1) return false;
	var sub = this . left; if (sub . type !== 1) return false;
	sub = sub . left; if (sub . type !== 3) return false;
	var atom = sub . left; if (atom . Protected) return false;
	if (position === undefined) position = Number . MAX_VALUE;
	if (atom . firstClause === null || position < 1) {sub . setNative (atom . firstClause); atom . firstClause = this; return true;}
	var head = atom . firstClause . left . left;
	while (head . left !== null && position > 1) {head = head . left . left . left; position -= 1;}
	sub . setNative (head . left);
	head . setNative (this);
	return true;
};

//////// ROOT ////////

this . PrologRoot = function () {
	this . left_caption = '[';
	this . right_caption = ']';
	this . var_caption = '*';
	this . separator_caption = ',';
	this . mid_caption = ':';
	this . slash_caption = '/';
	this . fail_caption = 'fail';
	this . quotation_caption = '"';
	this . auto_atoms = false;
	this . root = new hrcs . PrologDirectory ("user!");
};
this . PrologRoot . prototype . getValue = function (el) {
	if (el == null) return "<null element>";
	switch (el . type) {
		case 0: return this . left_caption + this . right_caption;
		case 1:
			if (el . left == null) return "Wrong left element.";
			if (el . right == null) return "Wrong right element.";
			return this . left_caption + this . getValue (el . left) + this . getRightCaption (el . right) + this . right_caption;
		case 2: return this . var_caption + el . left;
		case 3: return el . left . name;
		case 4: return this . slash_caption;
		case 5: return this . fail_caption;
		case 6: return typeof (el . left) === 'number' ? el . left : this . quotation_caption + el . left + this . quotation_caption;
	}
	return "Unknown element type.";
};
this . PrologRoot . prototype . getRightCaption = function (el) {
	if (el == null) return "<null element>";
	switch (el . type) {
		case 0: return '';
		case 1:
			return this . separator_caption + ' '
				+ this . getValue (el . left) + this . getRightCaption (el . right);
	}
	return ' ' + this . mid_caption + ' ' + this . getValue (el);
};
this . PrologRoot . prototype . getServiceClass = function (name) {
	if (this . root === null) return null;
	return this . root . getServiceClass (name);
};
this . PrologRoot . prototype . createDirectory = function (name, service) {this . root = new hrcs . PrologDirectory (name, this . root, service);};
this . PrologRoot . prototype . searchDirectory = function (name) {
	var ret = this . root;
	while (ret !== null) {if (ret . name === name) return ret; ret = ret . next;}
	return null;
};
this . PrologRoot . prototype . search = function (name) {
	var sub = this . root;
	while (sub !== null) {
		var ret = sub . searchAtom (name);
		if (ret !== null) return ret;
		sub = sub . next;
	}
	return null;
};
this . PrologRoot . prototype . list = function (name) {
	if (name === undefined) {
		if (this . root === null) return [];
		return this . root . names ();
	}
	var sub = this . searchDirectory (name);
	if (sub === null) return [];
	return sub . list ();
};
this . PrologRoot . prototype . close = function () {if (this . root === null) return; this . root . close ();};
this . PrologRoot . prototype . drop = function (name) {
	if (this . root === null) return;
	if (name === undefined) {this . root = this . root . next; return;}
	if (this . root . name === name) this . root = this . root . next;
	var sub = this . root;
	while (sub . next !== null) {
		if (sub . next . name === name) {sub . next = sub . next . next; return;}
	}
};
this . PrologRoot . prototype . createAtom = function (name) {
	if (this . root === null) return null;
	return this . root . createAtom (name);
};
this . PrologRoot . prototype . removeAtom = function (name) {
	if (this . root === null) return false;
	return this . root . removeAtom (name);
};
this . PrologRoot . prototype . searchC = function (name) {
	var atom = this . search (name);
	if (atom === null) return this . createAtom (name);
	return atom;
};
this . PrologRoot . prototype . Private = function (name) {
	if (this . root === null) return false;
	var atom = this . root . searchPrivateAtom (name);
	if (atom === null) return false;
	atom . Privated = true;
};
this . PrologRoot . prototype . Protect = function (name) {
	if (this . root === null) return false;
	var atom = this . root . searchPrivateAtom (name);
	if (atom === null) return false;
	atom . Protected = true;
};
this . PrologRoot . prototype . listAtom = function (name) {
	var atom = this . search (name);
	if (atom === null) return [];
	var el = atom . firstClause;
	var ret = [];
	while (el !== null) {
		ret . push (this . left_caption + this . left_caption + name + this . getRightCaption (el . left . right) + this . right_caption + this . getRightCaption (el . right) + this . right_caption);
		el = el . left . left . left;
	}
	return ret;
};

};
/*
*/
root = new prolog.PrologRoot();
sonda = root.createAtom('sonda');
mariner = root.createAtom('mariner');
//////////
e = new prolog.PrologElement();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setAtom(mariner);
cl = new prolog.PrologElement();
cl.setPair();
cl.left = e;
console . log (cl.attach());
///////
e = new prolog.PrologElement();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setNative(123);
cl = new prolog.PrologElement();
cl.setPair();
cl.left = e;
console . log (cl.attach());
///////
e = new prolog.PrologElement();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setNative(124);
cl = new prolog.PrologElement();
cl.setPair();
cl.left = e;
console . log (cl.attach());
///////
e = new prolog.PrologElement();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setVar(124);
cl = new prolog.PrologElement();
cl.setPair();
cl.left = e;
console . log (cl.attach(0));
////////
console . log (root.listAtom('sonda').join('\n'));
