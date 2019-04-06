
studio = require ('./studio') . studio;
prolog = require ('./prolog') . prolog;
require ('./studio.prc');

Root = new prolog . Root ();
Root . load ('studio');

res = function (command) {return Root . rep (command);};

module . exports = {
	studio: studio,
	prolog: prolog,
	Root: Root,
	res: res
};

