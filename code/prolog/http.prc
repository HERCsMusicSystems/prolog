
import studio

program http #machine := "prolog.http"

	[
		webserver daemon fork wait_for FULL_HEADER_TEXT param HTTP_URI HTTP_PROTOCOL HTTP_HEADER
		GET POST PUT PATCH DELETE COPY HEAD OPTIONS LINK UNLINK PURGE
		GETter FORM MULTIFORM FORMatter
		http_request http_responder
		cook cook_session header headers session_header hrcs
	]

#machine webserver := "webserver"
#machine daemon := "daemon"
#machine fork := "fork"
#machine wait_for := "wait_for"
#machine http_request := "http_request"
#machine http_responder := "http_responder"

[[GET *res *content] / [*res "<a href=\"/\" />" *content "</a>"]]
[[GET *res : *command] / [*res "<a href=\""] / [GETter *res : *command]]
[[GETter *res *p1 *p2 : *tail] / [*res "/" *p1] / [GETter *res *p2 : *tail]]
[[GETter *res *content] [*res "\" />" *content "</a>"]]
[[GETter *res]]

[[FORM *res *method] / [*res "<form action=\"/\" method=\"" *method "\" />"]]
[[FORM *res *method *p : *ps] / [*res "<form method=\"" *method "\" action=\""] / [FORMatter *res *p : *ps]]
[[FORM *res] / [*res "</form>"]]
[[MULTIFORM *res *method] / [*res "<form enctype=\"multipart/form-data\" action=\"/\" method=\"" *method "\" />"]]
[[MULTIFORM *res *method *p : *ps] / [*res "<form enctype=\"multipart/form-data\" method=\"" *method "\" action=\""] / [FORMatter *res *p : *ps]]
[[FORMatter *res *p1 : *ps] [*res "/" *p1] / [FORMatter *res : *ps]]
[[FORMatter *res] [*res "\" />"]]

[[param *res *name] [*res "<input type=\"text\" name=\"" *name "\" />"]]
[[param *res *type *name] [*res "<input type=\"" *type "\" name=\"" *name "\" />"]]
[[param *res *type *name *value] [*res "<input type=\"" *type "\" name=\"" *name "\" value=\"" *value "\" />"]]

[[cook * []]]
[[cook *req [*c : *cs]] [StringSplitOnce *c "=" : *ck] [addcl [[*req cook : *ck]]] / [cook *req *cs]]
[[cook *req]
	[*req HTTP_HEADER "Cookie" *cookie]
	[StringSplit *cookie "; " : *cookies]
	[cook *req *cookies]
]
[[cook *]]

[[cook_session *req *atom] [*req cook "atom" *name] / [search_atom_c *name *atom] [SELECT [[has_machine *atom]] [[root *atom]]]]
[[cook_session *req *atom] [search_atom_c *atom] [root *atom]]

[[session_header *req *res *session : *headers]
	[cook *req]
	[cook_session *req *session]
	[*res "HTTP/1.0 200 OK\nSet-Cookie: atom=" *session "; path=/; HttpOnly\n"]
	/ [headers *res : *headers]
]
[[header *res : *headers] [*res "HTTP/1.0 200 OK\n"] / [headers *res : *headers]]
[[headers *res] [*res "\n"]]
[[headers *res *header : *headers] [*res *header "\n"] / [headers *res : *headers]]

[[hrcs *res] [*res "
<script>
var hrcs = function (command) {
	var xhr = new XMLHttpRequest ();
	xhr . onreadystatechange = function () {
		if (this . readyState == 4 && this . status == 200) console . log (this . responseText);
	};
	xhr . open ('GET', 'call?command=[' + command + ']', false);
	xhr . send ();
	console . log ('=>', xhr . responseText);
	return JSON . parse (xhr . responseText);
};
</script>
"]]

protect [GET GETter FORM MULTIFORM FORMatter param]
private [GETter FORMatter]

end .

