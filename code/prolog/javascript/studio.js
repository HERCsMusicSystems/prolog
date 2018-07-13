
var studio = new function () {

this . edit_file = function (file_name, area) {
	var content = localStorage . getItem (file_name);
	if (content == null) {alert (`File ${file_name} not found!`); return;}
	document . getElementById (area) . value = content;
};

this . store_edited_file = function (file_name, editor) {
	var file_name = document . getElementById (file_name) . value;
	var file_content = document . getElementById (editor) . value;
	var already_exists = localStorage . getItem (file_name);
	if (already_exists != null) {if (! confirm (`File ${file_name} alread exists.\nOverwrite?`)) return;}
	localStorage . setItem (file_name, file_content);
};

this . refresh_file_tree = function (files_list, editor) {
	var getFunction = function (id) {return function () {studio . edit_file (id, editor);}};
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

};
