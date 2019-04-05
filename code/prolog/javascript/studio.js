
if (typeof module !== 'undefined') node_file_system = require ('fs');

var studio = new function () {

var pwd = [];
var search = [];
var local_imports = [];

var resources = {};

this . pwd = function (path) {
	if (path === undefined) return pwd . length < 1 ? '' : pwd . join ('/') + '/';
	if (Array . isArray (path)) pwd = path;
	else if (typeof (path) === 'string') pwd = path . split ('/');
	return this . pwd ();
};

this . cd = function (dir) {
	if (dir === undefined) pwd . pop ();
	else if (Array . isArray (dir)) pwd = pwd . concat (dir);
	else pwd . push (dir);
	return pwd . length < 1 ? '' : pwd . join ('/') + '/';
};

this . search = function (path) {
	if (path === undefined) return search . map (function (el) {return el . length < 1 ? '' : el . join ('/') + '/';});
	if (Array . isArray (path)) search . push (path);
	else if (typeof (path) === 'string') search . push (path . split ('/'));
	return this . search ();
};

var st = this;

var resourceRead = function (path) {
	var content = resources;
	for (var ind in path) {
		content = content [path [ind]];
		if (content === undefined) return null;
	}
	return content;
};

var extractFile = function (path) {
	if (typeof localStorage === 'undefined') try {return node_file_system . readFileSync (path, 'utf-8');} catch (e) {return null;};
	return localStorage . getItem (path);
};

var fileRead = function (path) {
	var content = extractFile (st . pwd () + path);
	if (content === null) {
		var searches = st . search ();
		for (var ind in searches) {
			content = extractFile (searches [ind] + path);
			if (content !== null) return content;
		}
	}
	return content;
};

this . readFile = function (path) {
	var content = fileRead (path);
	if (content === null) content = resourceRead (path . split ('/') . map (function (a) {return a . trim ();}));
	if (content === null) content = resourceRead (path . split ('.') . map (function (a) {return a . trim ();}));
	return content;
};

this . readResource = function (path) {
	if (! Array . isArray (path)) path = String (path) . split ('.') . map (function (a) {return a . trim ();});
	var content = resourceRead (path);
	if (content === null) content = fileRead (path . join ('/'));
	return content;
};

this . setResource = function (path, resource) {
	var location = resources;
	var target = null, key = null;
	for (var ind in path) {
		key = path [ind];
		if (location [key] === undefined) location [key] = {}
		target = location;
		location = location [key];
	}
	if (target === null) return;
	target [key] = resource;
};

this . getService = function (name) {
	var service = this . readResource (name);
	if (service === null) return null;
	if (typeof (service) === 'string') {
		eval . call (window, service);
		service = this . readResource (name);
		if (service === null || typeof (service) === 'string') return null;
	}
	return service;
};

this . callback = function () {};

this . writeFile = function (path, content) {
	if (typeof localStorage === 'undefined') {node_file_system . writeFileSync (this . pwd () + path, content, 'utf-8'); return;}
	localStorage . setItem (this . pwd () + path, content); if (this . callback !== undefined) this . callback ();
};

this . edit_file = function (file_name, area, location) {
	var content = localStorage . getItem (file_name);
	if (content === null) {alert (`File ${file_name} not found!`); return;}
	if (area !== undefined) document . getElementById (area) . value = content;
	if (location !== undefined) document . getElementById (location) . value = file_name;
};

this . erase_file = function (file_name) {
	if (localStorage . getItem (this . pwd () + file_name) !== null) localStorage . removeItem (this . pwd () + file_name);
	else localStorage . removeItem (file_name);
	if (this . callback !== undefined) this . callback ();
};

this . store_edited_file = function (file_name, editor) {
	var file_name = document . getElementById (file_name) . value;
	var file_content = document . getElementById (editor) . value;
	var already_exists = localStorage . getItem (file_name);
	if (already_exists !== null) {if (! confirm (`File ${file_name} alread exists.\nOverwrite?`)) return;}
	localStorage . setItem (file_name, file_content);
	if (this . callback !== undefined) this . callback ();
};

this . import = function (file_name) {
	if (local_imports . includes (file_name)) return;
	var content = this . readFile (file_name);
	if (content === null) {console . log (`FILE ${file_name} NOT FOUND!`); return;}
	eval . call (window, content);
	local_imports . push (file_name);
};

this . reimport = function (file_name) {
	var content = this . readFile (file_name);
	if (content === null) {console . log (`FILE ${file_name} NOT FOUND!`); return;}
	eval . call (window, content);
	if (! local_imports . includes (file_name)) local_imports . push (file_name);
};

this . random_permutation = function (a) {
	var ind = a . length;
	while (ind > 1) {
		var r = Math . floor (Math . random () * ind); ind --;
		var swap = a [ind]; a [ind] = a [r]; a [r] = swap;
	}
	return a;
};

this . random_pop = function (a) {
	var ind = Math . floor (Math . random () * a . length);
	var e = a [ind];
	a . splice (ind, 1);
	return e;
};

};

if (typeof module !== 'undefined') module . exports . studio = studio;

