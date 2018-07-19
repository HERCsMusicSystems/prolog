
var studio = new function () {

var pwd = [];
var search = [];
var local_imports = [];

this . resource = {};

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

this . readFile = function (path) {
	var content = localStorage . getItem (this . pwd () + path);
	if (content === null) {
		var searches = this . search ();
		for (var ind in searches) {
			content = localStorage . getItem (searches [ind] + path);
			if (content !== null) return content;
		}
	}
	if (content === null) content = this . resource [path];
	if (content == null) return null;
	return content;
};

this . writeFile = function (path, content) {localStorage . setItem (this . pwd () + path, content);};

this . edit_file = function (file_name, area, location) {
	var content = localStorage . getItem (file_name);
	if (content === null) {alert (`File ${file_name} not found!`); return;}
	if (area !== undefined) document . getElementById (area) . value = content;
	if (location !== undefined) document . getElementById (location) . value = file_name;
};

this . erase_file = function (file_name) {localStorage . removeItem (file_name);};

this . store_edited_file = function (file_name, editor) {
	var file_name = document . getElementById (file_name) . value;
	var file_content = document . getElementById (editor) . value;
	var already_exists = localStorage . getItem (file_name);
	if (already_exists !== null) {if (! confirm (`File ${file_name} alread exists.\nOverwrite?`)) return;}
	localStorage . setItem (file_name, file_content);
};

this . refresh_file_tree = function (files_list, editor, location) {
	var getFunction = function (id) {return function () {studio . edit_file (id, editor, location);}};
	var ul = document . getElementById (files_list);
	while (ul . firstChild) {ul . removeChild (ul . firstChild);}
	var collection = [];
	for (var ind = 0; ind < localStorage . length; ind++) collection . push (localStorage . key (ind));
	collection . sort ();
	for (var ind in collection) {
		var li = document . createElement ('li');
		li . innerHTML = collection [ind];
		li . onclick = getFunction (collection [ind]);
		ul . appendChild (li);
	}
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

};
