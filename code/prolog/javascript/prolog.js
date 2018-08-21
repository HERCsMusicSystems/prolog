
var prolog = new function () {

//////// ATOM ////////

var Atom = function (name, root) {
	if (name === undefined) this . name = "<" + Math . random () . toString (16) . substring (2) + "#" + new Date () . getTime () . toString (16) + ">";
	else this . name = name + "";
	this . next = root === undefined ? null : root;
	this . Privated = false;
	this . Protected = false;
	this . firstClause = null;
	this . machine = null;
};
Atom . prototype . setMachine = function (obj) {
	if (this . firstClause !== null || (this . Protected && obj !== null)) return false;
	this . Protected = (obj !== null);
	this . machine = obj;
	return true;
};
Atom . prototype . getMachine = function () {return this . machine;};
Atom . prototype . clauseCount = function () {
	var count = 0, cl = this . firstClause;
	while (cl !== null) {count ++; cl = cl . left . left . left;}
	return count;
};
Atom . prototype . raw_clause_pointer = function (position) {
	if (position === undefined) position = 0;
	var cl = this . firstClause;
	while (cl !== null && position > 0) {cl = cl . left . left . left; position --;}
	if (cl === null) return null;
	return cl;
};
Atom . prototype . delcl = function (position) {
	if (this . Protected) return false;
	if (this . firstClause === null || this . Protected) return false;
	if (position < 0) return false;
	if (position === 0) {this . firstClause = this . firstClause . left . left . left; return true;}
	var clause = this . firstClause, next = clause . left . left . left;
	while (next !== null && position > 1) {clause = next; next = clause . left . left . left; position --;}
	if (next === null) return false;
	clause . left . left . left = next . left . left . left;
	return true;
};
this . Atom = Atom;

//////// DIRECTORY /////////

var Directory = function (name, root, service) {
	this . name = name + "";
	this . next = root == null ? null : root;
	this . firstAtom = null;
	this . service_class = service == null ? null : service;
};
Directory . prototype . getServiceClass = function (name) {
	if (name === undefined) return this . service_class;
	if (name === this . name) return this . service_class;
	if (this . next === null) return null;
	return this . next . getServiceClass (name);
};
Directory . prototype . names = function () {
	var area = [this . name];
	var next = this . next;
	while (next !== null) {area . push (next . name); next = next . next;}
	return area;
};
Directory . prototype . searchAtom = function (search) {
	if (this . firstAtom === null) return null;
	var atom = this . firstAtom;
	while (atom !== null) {if (atom . name === search && ! atom . Privated) return atom; atom = atom . next;}
	return null;
};
Directory . prototype . searchPrivateAtom = function (search) {
	if (this . firstAtom === null) return null;
	var atom = this . firstAtom;
	while (atom !== null) {if (atom . name === search) return atom; atom = atom . next;}
	return null;
};
Directory . prototype . createAtom = function (name) {
	this . firstAtom = new Atom (name, this . firstAtom);
	return this . firstAtom;
};
Directory . prototype . removeAtom = function (atom) {
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
Directory . prototype . list = function () {
	var area = [];
	var atom = this . firstAtom;
	while (atom !== null) {area . push (atom . name); atom = atom . next;}
	return area;
};
Directory . prototype . duplicate = function (root) {
	var dir = new Directory (this . name, root, this . service_class);
	dir . firstAtom = this . firstAtom;
	return dir;
};
this . Directory = Directory;

//////// ELEMENET ////////
// 0: EARTH, 1: PAIR, 2: VAR, 3: ATOM, 4: SLASH, 5: FAIL, 6: NATIVE

var Element = function () {this . type = 0; return;};

Element . prototype . setPair = function () {
	this . type = 1;
	this . left = new Element ();
	this . right = new Element ();
};
Element . prototype . setVar = function (ind) {this . type = 2; this . left = ind;};
Element . prototype . setAtom = function (atom) {this . type = 3; this . left = atom;};
Element . prototype . setNative = function (el) {this . type = 6; this . left = el;};
Element . prototype . setNativePair = function (native) {this . type = 1; this . left = new Element (); this . left . setNative (native); return this . right = new Element ();}
Element . prototype . duplicate = function (el) {
	if (el === undefined) el = new Element ();
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
Element . prototype . attach = function (position) {
	if (this . type !== 1) return false;
	var sub = this . left; if (sub . type !== 1) return false;
	sub = sub . left; if (sub . type !== 3) return false;
	var atom = sub . left; if (atom . Protected) return false;
	if (position === undefined) position = Number . MAX_VALUE;
	if (atom . firstClause === null || position < 1) {sub . setNative (atom . firstClause); atom . firstClause = this; return true;}
	var head = atom . firstClause . left . left;
	while (head . left !== null && position > 1) {head = head . left . left . left; position --;}
	sub . setNative (head . left);
	head . setNative (this);
	return true;
};

this . Element = Element;

//////// ROOT ////////

var Root = function () {
	this . left_caption = '[';
	this . right_caption = ']';
	this . secondary_left_caption = '(';
	this . secondary_right_caption = ')';
	this . separator_caption = '';
	this . mid_caption = ':';
	this . slash_caption = '/';
	this . fail_caption = 'fail';
	this . quotation_caption = '\'"';
	this . atom_quotation_caption = '\`';
	this . comment_caption = ';';
	this . at_caption = '@';
	this . dot_caption = '.';
	this . atom_head_captions = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM?<>&|+-=%~#!$^{}";
	this . atom_tail_captions = "_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM?/<>&|+-=%~#!";
	this . if_atom_caption = ':=';
	this . and_atom_caption = '^';
	this . operator_captions = '';
	this . directive_head_caption = '\\';
	this . escape_caption = '\\'
	this . var_head_captions = '*';
	this . var_tail_captions = '_qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM';
	this . var_caption = '*';
	this . program_caption = 'program';
	this . end_caption = 'end';
	this . assignment_caption = ':=';
	this . machine_caption = '#machine';
	this . auto_caption = 'auto';
	this . private_caption = 'private';
	this . protect_caption = 'protect';
	this . auto_atoms = false;
	this . root = new Directory ("user!");
};
Root . prototype . getValue = function (el) {
	if (el == null) return "<null element>";
	switch (el . type) {
		case 0: return this . left_caption + this . right_caption;
		case 1:
			if (el . left == null) return "Wrong left element.";
			if (el . right == null) return "Wrong right element.";
			return this . left_caption + this . getValue (el . left) + this . getRightCaption (el . right) + this . right_caption;
		case 2: return this . var_caption + el . left;
		case 3: return el . head !== undefined ? '!' + el . left . name : el . left . name;
		case 4: return this . slash_caption;
		case 5: return this . fail_caption;
		case 6: return typeof (el . left) === 'number' ? el . left : this . quotation_caption . charAt (0) + el . left + this . quotation_caption . charAt (0);
	}
	return "Unknown element type.";
};
Root . prototype . getRightCaption = function (el) {
	if (el == null) return "<null element>";
	switch (el . type) {
		case 0: return '';
		case 1:
			return this . separator_caption + ' '
				+ this . getValue (el . left) + this . getRightCaption (el . right);
	}
	return ' ' + this . mid_caption + ' ' + this . getValue (el);
};
Root . prototype . getServiceClass = function (name) {
	if (this . root === null) return null;
	return this . root . getServiceClass (name);
};
Root . prototype . createDirectory = function (name, service) {return this . root = new Directory (name, this . root, service);};
Root . prototype . searchDirectory = function (name) {
	var ret = this . root;
	while (ret !== null) {if (ret . name === name) return ret; ret = ret . next;}
	return null;
};
Root . prototype . search = function (name) {
	var sub = this . root;
	while (sub !== null) {
		var ret = sub . searchAtom (name);
		if (ret !== null) return ret;
		sub = sub . next;
	}
	return null;
};
Root . prototype . list = function (name) {
	if (name === undefined) {
		if (this . root === null) return [];
		return this . root . names ();
	}
	var sub = this . searchDirectory (name);
	return sub === null ? null : sub . list ();
};
Root . prototype . close = function () {
	if (this . root === null) return;
	var sub = this . root . next;
	if (sub === null) return;
	this . root . next = sub . next;
	sub . next = this . root;
	this . root = sub;
};
Root . prototype . drop = function (name) {
	if (this . root === null) return;
	if (name === undefined) {this . root = this . root . next; return;}
	if (this . root . name === name) this . root = this . root . next;
	var sub = this . root;
	while (sub . next !== null) {
		if (sub . next . name === name) {sub . next = sub . next . next; return;}
		sub = sub . next;
	}
};
Root . prototype . createAtom = function (name) {
	if (this . root === null) return null;
	return this . root . createAtom (name);
};
Root . prototype . removeAtom = function (name) {
	if (this . root === null) return false;
	return this . root . removeAtom (name);
};
Root . prototype . searchC = function (name) {
	var atom = this . search (name);
	if (atom === null) return this . createAtom (name);
	return atom;
};
Root . prototype . Private = function (name) {
	if (this . root === null) return false;
	var atom = this . root . searchPrivateAtom (name);
	if (atom === null) return false;
	atom . Privated = true;
};
Root . prototype . Protect = function (name) {
	if (this . root === null) return false;
	var atom = this . root . searchPrivateAtom (name);
	if (atom === null) return false;
	atom . Protected = true;
};
Root . prototype . listAtom = function (atom) {
	if (typeof (atom) === 'string') atom = this . search (atom);
	if (atom === null) return [];
	if (atom . machine !== null) return [atom . name + ' ' + this . assignment_caption + ' ' + this . machine_caption];
	var el = atom . firstClause;
	var ret = [];
	while (el !== null) {
		ret . push (this . left_caption + this . left_caption + atom . name + this . getRightCaption (el . left . right) + this . right_caption + this . getRightCaption (el . right) + this . right_caption);
		el = el . left . left . left;
	}
	return ret;
};
Root . prototype . load = function (name, no_overwrite) {
	if (no_overwrite === undefined) no_overwrite = false;
	var existing = this . searchDirectory (name);
	if (existing !== null) {
		if (no_overwrite) return new Element ();
		this . drop ();
	}
	var content = studio . readFile (name);
	if (content === null) content = studio . readFile (name + ".prc");
	if (content === null) return false;
	var reader = new Reader (this, content);
	return reader . readProgram ();
};
Root . prototype . log = function () {console . log . apply (this, arguments);};

this . Root = Root;

//////// READER ////////

var Reader = function (root, file) {
	this . root = root;
	this . file = file;
	this . act = '';
	this . ind = 0;
	this . control = 'eof';
	this . symbol = '';
	this . search_context = null;
	this . vars = [];
};
Reader . prototype . move = function () {
	this . act = this . file . charAt (this . ind ++);
	return this . act;
};
Reader . prototype . getSymbol = function () {
	if (this . act <= '') this . move ();
	this . symbol = '';
	while (this . act <= ' ' || this . root . comment_caption . indexOf (this . act) >= 0) {
		if (this . root . comment_caption . indexOf (this . act) >= 0) {
			while (this . act !== '\n' && this . act > '') this . move ();
		}
		if (this . act <= '') {this . control = 'eof'; return;}
		this . move ();
	}
	if (this . root . left_caption . indexOf (this . act) >= 0) {
		this . move ();
		while (this . act <= ' ' && this . act > '') this . move ();
		if (this . root . right_caption . indexOf (this . act) >= 0) {this . control = '[]', this . move (); return;}
		this . control = '[';
		return;
	}
	if (this . root . secondary_left_caption . indexOf (this . act) >= 0) {
		this . move ();
		while (this . act <= ' ' && this . act > '') this . move ();
		if (this . root . secondary_right_caption . indexOf (this . act) >= 0) {this . control = '()', this . move (); return;}
		this . control = '(';
		return;
	}
	if (this . root . right_caption . indexOf (this . act) >= 0) {this . control = ']'; this . move (); return;}
	if (this . root . secondary_right_caption . indexOf (this . act) >= 0) {this . control = ')'; this . move (); return;}
	if (this . root . slash_caption . indexOf (this . act) >= 0) {this . control = '/'; this . move (); return;}
	if (this . root . mid_caption . indexOf (this . act) >= 0) {
		this . symbol = ':'; this . move ();
		while (':=<>|/!+-' . indexOf (this . act) >= 0 && this . act > '') {this . symbol += this . act; this . move ();}
		this . control = this . symbol . length < 2 ? ':' : 'atom';
		return;
	}
	if (this . root . dot_caption . indexOf (this . act) >= 0) {this . control = '.'; this . move (); return;}
	if (this . root . at_caption . indexOf (this . act) >= 0) {this . control = '@'; this . move (); return;}
	if (this . root . separator_caption . indexOf (this . act) >= 0) {this . control = ',', this . move (); return;}
	var negative = false;
	if (this . act === '-') {negative = true; this . symbol = '-'; this . move();}
	if ('0123456789' . indexOf (this . act) >= 0) {
		while ('0123456789abcdefABCDEFxX.-' . indexOf (this . act) >= 0 && this . act > '') {this . symbol += this . act; this . move();}
		this . control = 'number';
		this . symbol = Number (this . symbol);
		return;
	}
	if (negative) {
		if (this . root . atom_head_captions . indexOf ('-') >= 0) {
			this . control = 'atom';
			while (this . root . atom_tail_captions . indexOf (this . act) >= 0 && this . act > '') {
				if (this . root . if_atom_caption === this . symbol) return;
				if (this . root . and_atom_caption === this . symbol) return;
				if (this . root . operator_captions . indexOf (this . symbol) >= 0) return;
				this . symbol += this . act;
				this . move ();
			}
			return;
		}
		this . control = 'unknown';
		this . root . log ("Lexical error (negative).");
		return;
	}
	if (this . root . directive_head_caption . indexOf (this . act) >= 0) {
		this . move ();
		while ('abcdefghijklmnopqrstuvwxyz' . indexOf (this . act) >= 0 && this . act > '') {
			this . symbol += this . act; this . move ();
		}
		this . control = 'directive';
		return;
	}
	if (this . root . quotation_caption . indexOf (this . act) >= 0) {
		var quotation = this . act;
		this . move ();
		while (this . act !== quotation && this . act > '') {
			if (this . root . escape_caption . indexOf (this . act) >= 0) {
				this . move ();
				switch (this . act) {
					case 'n': this . symbol += '\n'; break;
					default: this . symbol += this . act; break;
				}
			} else this . symbol += this . act;
			this . move ();
		}
		this . move ();
		this . control = 'text';
		return;
	}
	if (this . root . atom_quotation_caption . indexOf (this . act) >= 0) {
		var quotation = this . act;
		this . move ();
		while (this . act !== quotation && this . act > '') {
			if (this . root . escape_caption . indexOf (this . act) >= 0) {
				this . move ();
				switch (this . act) {
					case 'n': this . symbol += '\n'; break;
					default: this . symbol += this . act; break;
				}
			} else this . symbol += this . act;
			this . move ();
		}
		this . move ();
		this . control = 'atom';
		return;
	}
	if (this . root . var_head_captions . indexOf (this . act) >= 0) {
		this . symbol += this . act;
		this . move ();
		while (this . root . var_tail_captions . indexOf (this . act) >= 0 && this . act > '') {
			this . symbol += this . act; this . move ();
		}
		this . control = 'var';
		return;
	}
	if (this . root . atom_head_captions . indexOf (this . act) >= 0) {
		this . control = 'atom';
		do {
			this . symbol += this . act;
			this . move ();
			if (this . root . if_atom_caption === this . symbol) return;
			if (this . root . and_atom_caption === this . symbol) return;
			if (this . root . operator_captions . indexOf (this . symbol) >= 0) return;
		} while (this . root . atom_tail_captions . indexOf (this . act) >= 0 && this . act > '');
		if (this . symbol === this . root . fail_caption) this . control = 'fail';
		return;
	}
	this . control = 'unknown';
	this . move ();
	this . root . log ("Lexical error (unknown character sequence).");
};
Reader . prototype . atomC = function (name) {
	var atom;
	if (this . search_context === null) atom = this . root . search (name);
	else {
		atom = null;
		var dir = this . search_context;
		while (atom === null && dir !== null) {atom = dir . searchAtom (name); dir = dir . next;}
	}
	if (atom === null && this . root . auto_atoms) {
		atom = root . createAtom (name);
		if (this . search_context !== null) this . search_context . firstAtom = atom;
	}
	if (atom === null) return null;
	var el = new Element (); el . setAtom (atom); return el;
};
Reader . prototype . varNumber = function (name) {
	if (name === this . root . var_caption) return this . vars . push (name) - 1;
	var ind = this . vars . indexOf (name);
	if (ind < 0) return this . vars . push (name) - 1;
	return ind;
};
Reader . prototype . getElement = function () {
	this . getSymbol ();
	var el, dir;
	switch (this . control) {
		case 'atom':
			el = this . atomC (this . symbol);
			if (el === null) this . root . log ("Semantic error (unknown atom: " + this . symbol + ").");
			return el;
		case '@':
			this . getSymbol ();
			if (this . control !== 'atom') return this . error ("Syntax error (directory expected).");
			dir = this . root . searchDirectory (this . symbol);
			this . getSymbol ();
			if (dir === null) return this . error ("Semantic error (directory" + this . symbol + " does not exist)");
			if (this . control !== '.') return this . error ("Syntax error (dot expected).");
			this . getSymbol ();
			if (this . control !== 'atom') return this . error ("Syntax error (atom after dot expected).");
			el = dir . searchAtom (this . symbol);
			if (el === null) return this . error ("Semantic error (qualified atom " + this . symbol + " not found in " + dir . name + ").");
			dir = new Element (); dir . setAtom (el);
			return dir;
		case 'number': el = new Element (); el . setNative (this . symbol); return el;
		case '[]': case '()': return new Element ();
		case '/': el = new Element (); el . type = 4; return el;
		case 'fail': el = new Element (); el . type = 5; return el;
		case 'var': el = new Element (); el . setVar (this . varNumber (this . symbol)); return el;
		case 'text': el = new Element (); el . setNative (this . symbol); return el;
		case '[': return this . readRightSide (this . getElement (), ']');
		case '(': return this . readRightSide (this . getElement (), ')');
		case '.': return this . atomC ('.');
		case 'eof': return null;
		default: break;
	}
	return this . error ("Syntax error (unknown syntax).");
};
Reader . prototype . readRightSide = function (left, bracket) {
	if (left === null) return null;
	this . getSymbol ();
	var el, dir;
	switch (this . control) {
		case ']': case ')':
			if (bracket !== this . control) return this . error ("Syntax error (bracket mismatch.)");
			el = new Element (); el . setPair (); el . left = left; el . right = new Element ();
			return el;
		case ':':
			el = this . getElement (); if (el === null) return null;
			if (el . type === 0) return this . error ("Syntax error (earth not allowed here).");
			this . getSymbol ();
			if (this . control !== bracket) return this . error ("Syntax error (closing bracket expected).");
			dir = new Element (); dir . setPair (); dir . left = left; dir . right = el;
			return dir;
		default: break;
	}
	if (this . root . separator_caption !== '') {
		if (this . control === ',') this . getSymbol ();
		else return this . error ("Syntax error (separator expected).");
	}
	switch (this . control) {
		case 'atom':
			el = this . atomC (this . symbol);
			if (el === null) return this . error ("Semantic error (unknown atom: " + this . symbol + ").");
			break;
		case '@':
			this . getSymbol ();
			if (this . control !== 'atom') return this . error ("Syntax error (directory expected).");
			el = this . root . searchDirectory (this . symbol);
			this . getSymbol ();
			if (el === null) return this . error ("Semantic error (directory" + this . symbol + " does not exist)");
			if (this . control !== '.') return this . error ("Syntax error (dot expected).");
			this . getSymbol ();
			if (this . control !== 'atom') return this . error ("Syntax error (atom after dot expected).");
			dir = dir . searchAtom (this . symbol);
			if (dir === null) return this . error ("Semantic error (qualified atom " + this . symbol + " not found in " + el . name + ").");
			el = new Element (); dir . setAtom (dir);
			break;
		case 'number': el = new Element (); el . setNative (this . symbol); break;
		case '[]': case '()': el = new Element (); break;
		case '/': el = new Element (); el . type = 4; break;
		case 'fail': el = new Element (); el . type = 5; break;
		case 'var': el = new Element (); el . setVar (this . varNumber (this . symbol)); break;
		case 'text': el = new Element (); el . setNative (this . symbol); break;
		case '[': el = this . readRightSide (this . getElement (), ']'); break;
		case '(': el = this . readRightSide (this . getElement (), ')'); break;
		case '.': el = this . atomC ('.'); break;
		default: return null;
	}
	if (el === null) return null;
	el = this . readRightSide (el, bracket);
	if (el === null) return null;
	dir = new Element ();
	dir . setPair (); dir . left = left; dir . right = el;
	return dir;
};
Reader . prototype . readProgram = function () {
	this . getSymbol ();
	while (this . control === 'atom' && this . symbol === 'import') {
		this . getSymbol ();
		if (this . control !== 'atom') return this . error ("Syntax error (import name expected).");
		var dir = this . root . searchDirectory (this . symbol);
		if (dir === null) {
			this . root . load (this . symbol);
			dir = this . root . searchDirectory (this . symbol);
		}
		if (dir === null) return this . error ("Semantic error (program " + this . symbol + " could not be imported).");
		this . search_context = dir . duplicate (this . search_context);
		this . getSymbol ();
	}
	if (this . control !== 'atom' || this . symbol !== this . root . program_caption) return this . error ("Syntax error (program keyword expected).");
	this . getSymbol ();
	if (this . control !== 'atom') return this . error ("Syntax error (program name expected).");
	var service_class_name = null;
	var directory = this . root . createDirectory (this . symbol);
	this . getSymbol ();
	if (this . control === 'atom' && this . symbol === this . root . machine_caption) {
		this . getSymbol ();
		if (this . control !== 'atom' && this . symbol !== this . root . assignment_caption) return this . dropError ("Syntax error (service class machine assignment expected).");
		this . getSymbol ();
		if (this . control !== 'text') return this . dropError ("Syntax error (service class location expected).");
		service_class_name = this . symbol;
		this . getSymbol ();
	}
	switch (this . control) {
	case '[]': break;
	case '[':
		this . getSymbol ();
		while (this . control === 'atom') {this . root . createAtom (this . symbol); this . getSymbol ();}
		if (this . control !== ']') return this . dropError ("Syntax error (closing bracket after atome list expected).");
		break;
	default: return this . dropError ("Syntax error (atome list expected).");
	};
	this . search_context = this . root . root . duplicate (this . search_context);
	if (service_class_name !== null) {
		var service_class = studio . readResource (service_class_name);
		if (service_class === null) return this . dropError ("Semantic error (service class " + service_class_name + " not found).");
		if (typeof (service_class) === 'string') {
			eval . call (window, service_class);
			service_class = studio . readResource (service_class_name);
			if (service_class === null || typeof (service_class) === 'string') return this . dropError ("Semantic error (service class " + service_class_name + " could not be imported).");
		}
		this . root . root . service_class = new service_class (this . root, this . root . root);
	}
	this . getSymbol ();
	while (this . control === '[' || this . control === 'atom') {
		if (this . control === 'atom') {
			switch (this . symbol) {
				case this . root . machine_caption:
					this . getSymbol ();
					if (this . control !== 'atom') return this . dropError ("Syntax error (atom expected in machine assignment).");
					var atom = this . root . root . searchAtom (this . symbol);
					if (atom === null) return this . dropError ("Semantic error (atom " + this . symbol + " for machine assignment not found).");
					this . getSymbol ();
					if (this . control !== 'atom' || this . symbol !== this . root . assignment_caption) return this . dropError ("Syntax error (operator := expected in machine assignment).");
					this . getSymbol ();
					if (this . control !== 'text') return this . dropError ("Syntax error (location of native code in machine assignment expected).");
					if (this . root . root . service_class === null) return this . dropError ("Syntax error (no service class for machine assignment).");
					var machine = this . root . root . service_class . getNativeCode (this . symbol);
					if (machine === null) return this . dropError ("Semantic error (native code " + this . symbol + " not found).");
					if (! atom . setMachine (machine)) return this . dropError ("Semantic error (machine assignment of " + atom . name  + " failed).");
					this . getSymbol ();
					break;
				case this . root . end_caption:
					this . getSymbol ();
					if (this . control === '.') {this . root . close (); return new Element ();}
					if (this . control !== 'atom' || this . symbol !== this . root . assignment_caption) return this . dropError ("Syntax error (assignment in loading instructions expected).");
					var command = this . getElement ();
					if (command === null) return this . dropError ("Syntax error (loading instructions not readable).");
					this . getSymbol ();
					if (this . control !== '.') return this . dropError ("Syntax error (dot at the end expected).");
					this . root . close ();
					return command;
				case this . root . auto_caption:
					this . getSymbol ();
					if (this . control !== 'atom' || this . symbol !== this . root . assignment_caption) return this . dropError ("Syntax error (assignment expected in auto clause).");
					var auto_clause = new Element ();
					auto_clause . setPair ();
					auto_clause . right = this . getElement ();
					this . getSymbol ();
					this . root . resolution (auto_clause);
					break;
				case this . root . private_caption:
					this . getSymbol ();
					if (this . control === '[]') {this . getSymbol (); break;}
					if (this . control !== '[') return this . dropError ("Syntax error (list of private atoms expected).");
					this . getSymbol ();
					while (this . control === 'atom') {
						var atom = this . root . root . searchAtom (this . symbol);
						if (atom === null) return this . dropError ("Semantic error (atom " + this . symbol + " not found in private clause).");
						atom . Privated = true;
						this . getSymbol ();
					}
					if (this . control !== ']') return this . dropError ("Syntax error (closing bracket after private atome list expected).");
					this . getSymbol ();
					break;
				case this . root . protect_caption:
					this . getSymbol ();
					if (this . control === '[]') {this . getSymbol (); break;}
					if (this . control !== '[') return this . dropError ("Syntax error (list of protect atoms expected).");
					this . getSymbol ();
					while (this . control === 'atom') {
						var atom = directory . searchPrivateAtom (this . symbol);
						if (atom === null) return this . dropError ("Semantic error (atom " + this . symbol + " not found in protect clause).");
						atom . Protected = true;
						this . getSymbol ();
					}
					if (this . control !== ']') return this . dropError ("Syntax error (closing bracket after protect atome list expected).");
					this . getSymbol ();
					break;
				default: return this . dropError ("Syntax error (unknown keyword " + this . symbol + ").");
			}
		} else {
			this . vars = [];
			var el = this . readRightSide (this . getElement (), ']');
			if (el === null) {this . root . drop (); return null;}
			el . attach ();
			this . getSymbol ();
		}
	}
	return this . dropError ("Syntax error (at least end keyword expected but got this instead [" + this . control + ' ' + this . symbol + "]).");
};
Reader . prototype . error = function (error) {this . root . log (error); return null;};
Reader . prototype . dropError = function (error) {this . root . drop (); return this . error (error);};

this . Reader = Reader;

//////// RESOLUTION ////////

var Query = function (query, stack, context, fail_target, original) {
	this . query = query;
	this . stack = stack === undefined ? null : stack;
	this . context = context === undefined ? null : context;
	this . fail_target = fail_target === undefined ? null : fail_target;
	this . original = original === undefined ? true : original;
};
// drop_stack_to_fail_target = this . stack = this . fail_target

var Resolution = function (root) {this . root = root;};
Resolution . prototype . reset = function () {this . actuals = []; this . formals = []; this . vars = []; this . var_counter = 0;};
Resolution . prototype . match = function (actual, ac, formal, fc) {
	var v1, v2, vr, vr1, vr2;
	if (actual . type === 2) {
		if (formal . type === 2) {
			v1 = ac ? this . actuals [actual . left] : this . formals [actual . left];
			v2 = fc ? this . actuals [formal . left] : this . formals [formal . left];
			if (v1 === undefined) {
				if (v2 === undefined) {
					v2 = this . vars . length;
					if (fc) this . actuals [formal . left] = v2;
					else this . formals [formal . left] = v2;
					this . vars . push (vr = {term: null});
				}
				if (ac) this . actuals [actual . left] = v2;
				else this . formals [actual . left] = v2;
				return true;
			}
			if (v2 === undefined) {
				if (fc) this . actuals [formal . left] = v1;
				else this . formals [formal . left] = v1;
				return true;
			}
			vr1 = this . vars [v1];
			if (vr1 . term === null) {
				for (var ind in this . actuals) {if (this . actuals [ind] === v1) this . actuals [ind] = v2;}
				for (var ind in this . formals) {if (this . formals [ind] === v1) this . formals [ind] = v2;}
				return true;
			}
			vr2 = this . vars [v2];
			if (vr2 . term === null) {
				for (var ind in this . actuals) {if (this . actuals [ind] === v2) this . actuals [ind] = v1;}
				for (var ind in this . formals) {if (this . formals [ind] === v2) this . formals [ind] = v1;}
				return true;
			}
			return this . match (vr1 . term, vr1 . location, vr2 . term, vr2 . location);
		} else {
			if (ac) {
				if ((vr = this . actuals [actual . left]) === undefined) {
					this . actuals [actual . left] = this . vars . length;
					this . vars . push ({term: formal, location: fc});
					return true;
				}
			} else {
				if ((vr = this . formals [actual . left]) === undefined) {
					this . formals [actual . left] = this . vars . length;
					this . vars . push ({term: formal, location: fc});
					return true;
				}
			}
			vr = this . vars [vr];
			if (vr . term === null) {vr . term = formal, vr . location = fc; return true;}
			return this . match (vr . term, vr . location, formal, fc);
		}
	}
	if (formal . type === 2) {
		if (fc) {
			if ((vr = this . actuals [formal . left]) === undefined) {
				this . actuals [formal . left] = this . vars . length;
				this . vars . push ({term: actual, location: ac});
				return true;
			}
		} else {
			if ((vr = this . formals [formal . left]) === undefined) {
				this . formals [formal . left] = this . vars . length;
				this . vars . push ({term: actual, location: ac});
				return true;
			}
		}
		vr = this . vars [vr];
		if (vr . term === null) {vr . term = actual, vr . location = ac; return true;}
		return this . match (actual, ac, vr . term, vr . location);
	}
	if (actual . type !== formal . type) return false;
	switch (actual . type) {
		case 0: case 4: case 5: return true;
		case 3: case 6: return actual . left === formal . left;
		case 1:	return this . match (actual . left, ac, formal . left, fc) && this . match (actual . right, ac, formal . right, fc);
		default: return false;
	}
	return false;
};
Resolution . prototype . match_product = function (actual, ac) {
	var ret, vr;
	switch (actual . type) {
		case 0: return new Element ();
		case 3: case 6:
			ret = new Element ();
			ret . type = actual . type;
			ret . left = actual . left;
			return ret;
		case 1:
			ret = new Element ();
			ret . type = 1;
			ret . left = this . match_product (actual . left, ac);
			ret . right = this . match_product (actual . right, ac);
			return ret;
		case 2:
			if (ac) {
				if ((vr = this . actuals [actual . left]) === undefined) {
					ret = new Element (); ret . type = 2;
					this . actuals [actual . left] = this . vars . length;
					ret . left = this . var_counter;
					this . vars . push ({term: null, var_id: this . var_counter});
					this . var_counter ++;
					return ret;
				}
			} else {
				if ((vr = this . formals [actual . left]) === undefined) {
					ret = new Element (); ret . type = 2;
					this . formals [actual . left] = this . vars . length;
					ret . left = this . var_counter;
					this . vars . push ({term: null, var_id: this . var_counter});
					this . var_counter ++;
					return ret;
				}
			}
			vr = this . vars [vr];
			if (vr . term === null) {
				ret = new Element (); ret . type = 2;
				if (vr . var_id === undefined) {
					vr . var_id = this . var_counter; this . var_counter ++;
				}
				ret . left = vr . var_id;
				return ret;
			}
			return this . match_product (vr . term, vr . location);
		default:
			ret = new Element ();
			ret . type = actual . type;
			return ret;
	}
};
Resolution . prototype . res_forward = function () {
	var term = this . q_root . query;
	if (term . type !== 1) {this . root . log ("Wrong query (querries expected)."); return 0;}
	term = term . right;
	if (term . type !== 1) {this . root . log ("Wrong query (query expected)."); return 0;}
	term = term . left;
	if (term . type === 5) return 0; // genuine fail
	if (term . type === 4) {
		// slash maker
		this . q_root . query . right = this . q_root . query . right . right;
		this . q_root . stack = this . q_root . fail_target;
		if (this . q_root . original && this . q_root . stack === this . q_root . context && this . q_root . stack !== null) {
			if (this . q_root . stack . query . right . right . type == 0) {
				this . reset ();
				this . match (this . q_root . stack . query . right . left . right, true, this . q_root . query . left . right, false);
				term = new Element (); term . type = 1;
				term . left = this . match_product (this . q_root . stack . query . left, true);
				term . right = this . match_product (this . q_root . query . right, false);
				this . q_root . stack . query = term;
				this . q_root = this . q_root . stack;
				this . q_root . fail_target = this . q_root . stack;
				this . q_root . original = false;
			} else this . q_root . stack . query . right . left . left . head = null;
		}
		return this . q_root . query . right . type === 0 ? 2 : 1;
	}
	if (term . type !== 1) {this . root . log ("Wrong query (query is not a pair)."); return 0;}
	if (term . left === null) {this . root . log ("Wrong query (query head does not exist)."); return 0;}
	if (term . left . type !== 3) {this . root . log ("Wrong query (query head is not an atom)."); return 0;}
	var native = term . left . left . machine;
	if (native !== null) {
		// machine goes here
		this . reset ();
		var parameters = this . match_product (term . right, true);
		if (! native . code (parameters, this)) return 0;
		this . reset ();
		if (this . match (term . right, true, parameters, false)) {
			var new_head = this . match_product (this . q_root . query . left, true);
			var new_tail = this . match_product (this . q_root . query . right . right, true);
			var el = new Element (); el . type = 1; el . left = new_head; el . right = new_tail;
			this . q_root . query = el;
			return new_tail . type === 0 ? 2 : 1;
		}
		return 0;
	}
	var clausa = term . left . left . firstClause;
	if (clausa === null) {this . root . log ("Free atom [" + term . left . left . name + "]."); return 0;}
	var relation_atom = new Element ();
	relation_atom . type = 3; relation_atom . left = term . left . left;
	return this . sub_res_forward (relation_atom, term, clausa);
};
Resolution . prototype . sub_res_forward = function (relation_atom, term, clausa) {
	var next_clausa, new_tail, el;
	while (clausa !== null) {
		next_clausa = clausa . left . left . left;
		this . reset ();
		if (this . match (term . right, true, clausa . left . right, false)) {
			new_tail = this . match_product (clausa . right, false);
			if (new_tail . type === 0) {
				new_tail = this . match_product (this . q_root . query . right . right, true);
				el = new Element ();
					el . type = 1;
					el . left = this . match_product (this . q_root . query . left, true);
					el . right = new_tail;
				if (next_clausa === null) this . q_root . query = el;
				else {
					term . left . head = next_clausa;
					this . q_root = new Query (el, this . q_root, this . q_root . context, this . q_root . fail_target, this . q_root . original);
				}
				return new_tail . type === 0 ? 2 : 1;
			} else {
				// tail recursion optimisation
				if (next_clausa === null && this . q_root . query . right . right . type == 0) {
					el = new Element (); el . type = 1;
					el . left = this . match_product (this . q_root . query . left, true);
					el . right = new_tail;
					this . q_root . query = el;
					this . q_root . fail_target = this . q_root . stack;
					this . q_root . original = false;
					return 1;
				}
			}
			term . left . head = next_clausa;
			var ell = new Element ();
				ell . type = 1;
				ell . left = relation_atom;
				ell . right = this . match_product (clausa . left . right, false);
			el = new Element ();
				el . type = 1;
				el . left = ell;
				el . right = new_tail;
			this . q_root = new Query (el, this . q_root, this . q_root, this . q_root, true);
			return 1;
		}
		clausa = next_clausa;
	}
	return 0;
};
Resolution . prototype . res_back_back = function () {
	var context = this . q_root . context;
	if (context === null) return 2;
	this . reset ();
	this . match (context . query . right . left . right, true, this . q_root . query . left . right, false);
	var new_head = this . match_product (context . query . left, true);
	var new_tail = this . match_product (context . query . right . right, true);
	var el = new Element (); el . type = 1; el . left = new_head; el . right = new_tail;
	this . q_root . fail_target = context . fail_target;
	this . q_root . original = context . original;
	this . q_root . context = context . context;
	this . q_root . query = el;
	//this . root . log ('>>>>');
	//this . sa ();
	return new_tail . type === 0 ? 1 : 0;
};
Resolution . prototype . res_fail_back = function () {
	if ((this . q_root = this . q_root . stack) === null) return 5;
	var term = this . q_root . query . right . left;
	var ra = new Element (); ra . type = 3; ra . left = term . left . left;
	return this . sub_res_forward (ra, term, term . left . head);
};
Resolution . prototype . resolution = function (query) {
	// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query
	if (query === null) return null;
	if (query . type !== 1) return null;
	this . q_root = new Query (query . duplicate ());
	var ctrl;
	var limit = 512
	do {
		//this . sa ();
		ctrl = this . res_forward ();
		//this . root . log ('    ======= ', ctrl);
		//this . sa ();
		//this . root . log ('================')
		while (ctrl === 0) ctrl = this . res_fail_back ();
		if (ctrl === 2) {
			while (ctrl !== 0 && limit > 0) {
				ctrl = this . res_back_back ();
				if (ctrl === 2) {
					this . reset ();
					query = new Element (); query . type = 1;
					query . left = this . match_product (this . q_root . query . left, true);
					query . right = this . match_product (this . q_root . query . right, true);
					return query;
				}
				limit --;
			}
		}
		limit --;
	} while (ctrl !== 5 && limit > 0);
	return null;
};
Resolution . prototype . stackArray = function () {
	var tip = this . q_root;
	var ret = [];
	while (tip !== null) {
		ret . push (this . root . getValue (tip . query));
		tip = tip . stack;
	}
	return ret;
};
Resolution . prototype . sa = function () {this . root . log (this . stackArray () . join ('\n'));};

var toJS = function (el) {
	switch (el . type) {
		case 0: return [];
		case 1:
			var area = [];
			while (el . type === 1) {area . push (toJS (el . left)); el = el . right;}
			return area;
		case 2: return '*' + el . left;
		case 3: return el . left . name;
		case 4: return '/';
		case 5: return 'fail';
		case 6: return el . left;
		default: return null;
	}
};

Root . prototype . resolution = function (query) {
	var res = new Resolution (this) . resolution (query);
	return res === null ? null : toJS (res . left);
};

Root . prototype . textResolution = function (command) {
	return this . resolution (new Reader (this, command) . getElement ());
};

};

