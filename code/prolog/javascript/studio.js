
var studio = {

edit_file: function (file_name, area) {
	var content = localStorage . getItem (file_name);
	if (content == null) {alert (`File ${file_name} not found!`); return;}
	document . getElementById (area) . value = content;
},

store_edited_file: function (file_name, editor) {
	var file_name = document . getElementById (file_name) . value;
	var file_content = document . getElementById (editor) . value;
	var already_exists = localStorage . getItem (file_name);
	if (already_exists != null) {if (! confirm (`File ${file_name} alread exists.\nOverwrite?`)) return;}
	localStorage . setItem (file_name, file_content);
},

refresh_file_tree: function (files_list, editor) {
	var getFunction = function (id) {return function () {studio . edit_file (id, editor);}};
	var ul = document . getElementById (files_list);
	while (ul . firstChild) {ul . removeChild (ul . firstChild);}
	for (var ind = 0; ind < localStorage . length; ind++) {
		var li = document . createElement ('li');
		li . innerHTML = localStorage . key (ind);
		li . onclick = getFunction (localStorage . key (ind));
		ul . appendChild (li);
	}
}

};
