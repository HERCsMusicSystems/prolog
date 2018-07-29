
var prolog = new function () {

var hrcs = this;

//////// ATOM ////////

this . Atom = function (name, root) {
	if (name === undefined) this . name = "<" + Math . random () . toString (16) . substring (2) + "#" + new Date () . getTime () . toString (16) + ">";
	else this . name = name + "";
	this . next = root === undefined ? null : root;
	this . Privated = false;
	this . Protected = false;
	this . firstClause = null;
	this . machine = null;
};
this . Atom . prototype . setMachine = function (obj) {
	if (this . firstClause !== null || (this . Protected && obj !== null)) return false;
	this . Protected = (obj !== null);
	this . machine = obj;
	return true;
};
this . Atom . prototype . getMachine = function () {return this . machine;};

//////// DIRECTORY /////////

this . Directory = function (name, root, service) {
	this . name = name + "";
	this . next = root == null ? null : root;
	this . firstAtom = null;
	this . service_class = service == null ? null : service;
};
this . Directory . prototype . getServiceClass = function (name) {
	if (name === undefined) return this . service_class;
	if (name === this . name) return this . service_class;
	if (this . next === null) return null;
	return this . next . getServiceClass (name);
};
this . Directory . prototype . names = function () {
	var area = [this . name];
	var next = this . next;
	while (next !== null) {area . push (next . name); next = next . next;}
	return area;
};
this . Directory . prototype . searchAtom = function (search) {
	if (this . firstAtom === null) return null;
	var atom = this . firstAtom;
	while (atom !== null) {if (atom . name === search && ! atom . Privated) return atom; atom = atom . next;}
	return null;
};
this . Directory . prototype . searchPrivateAtom = function (search) {
	if (this . firstAtom === null) return null;
	var atom = this . firstAtom;
	while (atom !== null) {if (atom . name === search) return atom; atom = atom . next;}
	return null;
};
this . Directory . prototype . createAtom = function (name) {
	this . firstAtom = new hrcs . Atom (name, this . firstAtom);
	return this . firstAtom;
};
this . Directory . prototype . removeAtom = function (atom) {
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
this . Directory . prototype . list = function () {
	var area = [];
	var atom = this . firstAtom;
	while (atom !== null) {area . push (atom . name); atom = atom . next;}
	return area;
};
this . Directory . prototype . duplicate = function (root) {
	var dir = new hrcs . Directory (this . name, root, this . service_class);
	dir . fistAtom = this . firstAtom;
	return dir;
};

//////// ELEMENET ////////
// 0: EARTH, 1: PAIR, 2: VAR, 3: ATOM, 4: SLASH, 5: FAIL, 6: NATIVE

this . Element = function () {this . type = 0; return;};

this . Element . prototype . setPair = function () {
	this . type = 1;
	this . left = new hrcs . Element ();
	this . right = new hrcs . Element ();
};
this . Element . prototype . setVar = function (ind) {this . type = 2; this . left = ind;};
this . Element . prototype . setAtom = function (atom) {this . type = 3; this . left = atom;};
this . Element . prototype . setNative = function (el) {this . type = 6; this . left = el;};
this . Element . prototype . duplicate = function (el) {
	if (el === undefined) el = new hrcs . Element ();
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
this . Element . prototype . attach = function (position) {
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

this . Root = function () {
	this . left_caption = '[';
	this . right_caption = ']';
	this . secondary_left_caption = '(';
	this . secondary_right_caption = ')';
	this . var_caption = '*';
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
	this . auto_atoms = false;
	this . root = new hrcs . Directory ("user!");
};
this . Root . prototype . getValue = function (el) {
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
		case 6: return typeof (el . left) === 'number' ? el . left : this . quotation_caption . charAt (0) + el . left + this . quotation_caption . charAt (0);
	}
	return "Unknown element type.";
};
this . Root . prototype . getRightCaption = function (el) {
	if (el == null) return "<null element>";
	switch (el . type) {
		case 0: return '';
		case 1:
			return this . separator_caption + ' '
				+ this . getValue (el . left) + this . getRightCaption (el . right);
	}
	return ' ' + this . mid_caption + ' ' + this . getValue (el);
};
this . Root . prototype . getServiceClass = function (name) {
	if (this . root === null) return null;
	return this . root . getServiceClass (name);
};
this . Root . prototype . createDirectory = function (name, service) {this . root = new hrcs . Directory (name, this . root, service);};
this . Root . prototype . searchDirectory = function (name) {
	var ret = this . root;
	while (ret !== null) {if (ret . name === name) return ret; ret = ret . next;}
	return null;
};
this . Root . prototype . search = function (name) {
	var sub = this . root;
	while (sub !== null) {
		var ret = sub . searchAtom (name);
		if (ret !== null) return ret;
		sub = sub . next;
	}
	return null;
};
this . Root . prototype . list = function (name) {
	if (name === undefined) {
		if (this . root === null) return [];
		return this . root . names ();
	}
	var sub = this . searchDirectory (name);
	if (sub === null) return [];
	return sub . list ();
};
this . Root . prototype . close = function () {
	if (this . root === null) return;
	var sub = this . root . next;
	if (sub === null) return;
	this . root . next = sub . next;
	sub . next = this . root;
	this . root = sub;
};
this . Root . prototype . drop = function (name) {
	if (this . root === null) return;
	if (name === undefined) {this . root = this . root . next; return;}
	if (this . root . name === name) this . root = this . root . next;
	var sub = this . root;
	while (sub . next !== null) {
		if (sub . next . name === name) {sub . next = sub . next . next; return;}
	}
};
this . Root . prototype . createAtom = function (name) {
	if (this . root === null) return null;
	return this . root . createAtom (name);
};
this . Root . prototype . removeAtom = function (name) {
	if (this . root === null) return false;
	return this . root . removeAtom (name);
};
this . Root . prototype . searchC = function (name) {
	var atom = this . search (name);
	if (atom === null) return this . createAtom (name);
	return atom;
};
this . Root . prototype . Private = function (name) {
	if (this . root === null) return false;
	var atom = this . root . searchPrivateAtom (name);
	if (atom === null) return false;
	atom . Privated = true;
};
this . Root . prototype . Protect = function (name) {
	if (this . root === null) return false;
	var atom = this . root . searchPrivateAtom (name);
	if (atom === null) return false;
	atom . Protected = true;
};
this . Root . prototype . listAtom = function (name) {
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
this . Root . prototype . load = function (name) {
	var content = studio . readFile (name);
	if (content === null) content = studio . readFile (name + ".prc");
	if (content === null) return false;
	var reader = new hrcs . Reader (this, content);
	return reader . readProgram ();
};

//////// READER ////////

this . Reader = function (root, file) {
	this . root = root;
	this . file = file;
	this . act = '';
	this . ind = 0;
	this . control = 'eof';
	this . symbol = '';
	this . search_context = null;
	this . vars = [];
};
this . Reader . prototype . move = function () {
	this . act = this . file . charAt (this . ind ++);
	return this . act;
};
this . Reader . prototype . getSymbol = function () {
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
			while (this . root . atom_tail_captions . indexOf (this . act && this . act > '') >= 0) {
				if (this . root . if_atom_caption === this . symbol) return;
				if (this . root . and_atom_caption === this . symbol) return;
				if (this . root . operator_captions . indexOf (this . symbol) >= 0) return;
				this . symbol += this . act;
				this . move ();
			}
			return;
		}
		this . control = 'unknown';
		console . log ("Lexical error (negative).");
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
	console . log ("Lexical error (unknown character sequence).");
};
this . Reader . prototype . atomC = function (name) {
	var atom;
	if (this . search_context === null) atom = this . root . search (name);
	else {
		var dir = this . search_context;
		while (atom === null && dir !== null) {atom = dir . search (name); dir = dir . next;}
	}
	if (atom === null && this . root . auto_atoms) {
		atom = root . createAtom (name);
		if (this . search_context !== null) this . search_context . firstAtom = atom;
	}
	if (atom === null) return null;
	var el = new hrcs . Element (); el . setAtom (atom); return el;
};
this . Reader . prototype . varNumber = function (name) {
	if (name === this . root . var_caption) return this . vars . push (name) - 1;
	var ind = this . vars . indexOf (name);
	if (ind < 0) return this . vars . push (name) - 1;
	return ind;
};
this . Reader . prototype . getElement = function () {
	this . getSymbol ();
	var el, dir;
	switch (this . control) {
		case 'atom':
			el = this . atomC (this . symbol);
			if (el === null) console . log ("Semantic error (unknown atom: " + this . symbol + ").");
			return el;
		case '@':
			this . getSymbol ();
			if (this . control !== 'atom') {console . log ("Syntax error (directory expected)."); return null;}
			dir = this . root . searchDirectory (this . symbol);
			this . getSymbol ();
			if (dir === null) {console . log ("Semantic error (directory" + this . symbol + " does not exist)"); return null;}
			if (this . control !== '.') {console . log ("Syntax error (dot expected)."); return null;}
			this . getSymbol ();
			if (this . control !== 'atom') {console . log ("Syntax error (atom after dot expected)."); return null;}
			el = dir . searchAtom (this . symbol);
			if (el === null) {console . log ("Semantic error (qualified atom " + this . symbol + " not found in " + dir . name + ")."); return null;}
			dir = new hrcs . Element (); dir . setAtom (el);
			return dir;
		case 'number': el = new hrcs . Element (); el . setNative (this . symbol); return el;
		case '[]': case '()': return new hrcs . Element ();
		case '/': el = new hrcs . Element (); el . type = 4; return el;
		case 'fail': el = new hrcs . Element (); el . type = 5; return el;
		case 'var': el = new hrcs . Element (); el . setVar (this . varNumber (this . symbol)); return el;
		case 'text': el = new hrcs . Element (); el . setNative (this . symbol); return el;
		case '[': return this . readRightSide (this . getElement (), ']');
		case '(': return this . readRightSide (this . getElement (), ')');
		case '.': return this . atomC ('.');
		case 'eof': console . log ("Syntax error (end of file)."); return null;
		default: break;
	}
	console . log ("Syntax error (unknown syntax).");
	return null;
};
this . Reader . prototype . readRightSide = function (left, bracket) {
	this . getSymbol ();
	var el, dir;
	switch (this . control) {
		case ']': case ')':
			if (bracket !== this . control) {console . log ("Syntax error (bracket mismatch.)"); return null;}
			el = new hrcs . Element (); el . setPair (); el . left = left; el . right = new hrcs . Element ();
			return el;
		case ':':
			el = this . getElement (); if (el === null) return null;
			if (el . type === 0) {console . log ("Syntax error (earth not allowed here)."); return null;}
			this . getSymbol ();
			if (this . control !== bracket) {console . log ("Syntax error (closing bracket expected)."); return null;}
			dir = new hrcs . Element (); dir . setPair (); dir . left = left; dir . right = el;
			return dir;
		default: break;
	}
	if (this . root . separator_caption !== '') {
		if (this . control === ',') this . getSymbol ();
		else {console . log ("Syntax error (separator expected)."); return null;}
	}
	switch (this . control) {
		case 'atom':
			el = this . atomC (this . symbol);
			if (el === null) console . log ("Semantic error (unknown atom: " + this . symbol + ").");
			break;
		case '@':
			this . getSymbol ();
			if (this . control !== 'atom') {console . log ("Syntax error (directory expected)."); return null;}
			el = this . root . searchDirectory (this . symbol);
			this . getSymbol ();
			if (el === null) {console . log ("Semantic error (directory" + this . symbol + " does not exist)"); return null;}
			if (this . control !== '.') {console . log ("Syntax error (dot expected)."); return null;}
			this . getSymbol ();
			if (this . control !== 'atom') {console . log ("Syntax error (atom after dot expected)."); return null;}
			dir = dir . searchAtom (this . symbol);
			if (dir === null) {console . log ("Semantic error (qualified atom " + this . symbol + " not found in " + el . name + ")."); return null;}
			el = new hrcs . Element (); dir . setAtom (dir);
			break;
		case 'number': el = new hrcs . Element (); el . setNative (this . symbol); break;
		case '[]': case '()': el = new hrcs . Element (); break;
		case '/': el = new hrcs . Element (); el . type = 4; break;
		case 'fail': el = new hrcs . Element (); el . type = 5; break;
		case 'var': el = new hrcs . Element (); el . setVar (this . varNumber (this . symbol)); break;
		case 'text': el = new hrcs . Element (); el . setNative (this . symbol); break;
		case '[': el = this . readRightSide (this . getElement (), ']'); break;
		case '(': el = this . readRightSide (this . getElement (), ')'); break;
		case '.': el = this . atomC ('.'); break;
		default: return null;
	}
	if (el === null) return null;
	el = this . readRightSide (el, bracket);
	if (el === null) return null;
	dir = new hrcs . Element ();
	dir . setPair (); dir . left = left; dir . right = el;
	return dir;
};
this . Reader . prototype . readProgram = function () {
	this . getSymbol ();
	if (this . control !== 'atom' || this . symbol !== 'program') return null;
	this . getSymbol ();
	if (this . control !== 'atom') return null;
	this . root . createDirectory (this . symbol);
	this . getSymbol ();
	switch (this . control) {
	case '[]': break;
	case '[':
		this . getSymbol ();
		while (this . control === 'atom') {this . root . createAtom (this . symbol); this . getSymbol ();}
		if (this . control !== ']') return null;
		break;
	default: return null;
	};
	this . getSymbol ();
	while (this . control === '[') {
		var el = this . readRightSide (this . getElement (), ']');
		el . attach ();
		this . getSymbol ();
	}
	if (this . control !== 'atom' && this . control !== 'end') return null;
	this . getSymbol ();
	if (this . control === '.') {this . root . close (); return new hrcs . Element ();}
	if (this . control !== 'atom' || this . symbol !== ':=') return null;
	var command = this . getElement ();
	this . root . close ();
	return command;
};

};
studio . search ('test_scripts');
root = new prolog.Root();
console . log ('load', root . load ('sonda'));
console . log (root . list ());
console . log (root . list ('sonda'));
/*
sonda = root.createAtom('sonda');
mariner = root.createAtom('mariner');
//////////
e = new prolog.Element();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setAtom(mariner);
cl = new prolog.Element();
cl.setPair();
cl.left = e;
console . log (cl.attach());
///////
e = new prolog.Element();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setNative(123);
cl = new prolog.Element();
cl.setPair();
cl.left = e;
console . log (cl.attach());
///////
e = new prolog.Element();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setNative(124);
cl = new prolog.Element();
cl.setPair();
cl.left = e;
console . log (cl.attach());
///////
e = new prolog.Element();
e.setPair();
e.left.setAtom(sonda);
e.right.setPair();
e.right.left.setVar(124);
cl = new prolog.Element();
cl.setPair();
cl.left = e;
console . log (cl.attach(0));
////////
console . log (root.listAtom('sonda').join('\n'));
reader = new prolog.Reader(root, studio . readFile ('test_scripts/directive.prc'));
function m () {reader . getSymbol (); console . log (reader.symbol); return [reader.control, reader.symbol];};
function ge () {return reader . getElement ();};
*/
