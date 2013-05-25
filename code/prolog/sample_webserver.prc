
[auto_atoms]
[import "http"]

[[capitol "Australia" "Canberra"]]
[[capitol "New Zealand" "Wellington"]]
[[capitol "France" "Paris"]]
[[capitol "Poland" "Warsaw"]]
[[capitol "USA" "Cupertino"]]

[[controller_index *req *res]
	[*res "HTTP/1.1 200 OK\n\n<body>\n"]
	[view_list_all *res]
	[*res "</body>"]
]

[[controller_delete *req *res *country]
	[*res "HTTP/1.1 200 OK\n\n<body>\n"]
	[*res "DELETING " *country "<br/><br/>\n"]
	[TRY [delcl [[capitol *country *]]]]
	[view_list_all *res]
	[*res "</body>"]
]


[[view_list_all *res]
	[*res "<table>\n"]
	[*res "<tr><td><h1>COUNTRY</h1></td><td><h1>CAPITOL</h1></td></tr>\n"]
	[ALL
		[capitol *country *city]
		[*res "<tr><td>" *country "</td><td>" *city "</td>"]
		[*res "<td><a href=\"http://localhost:3008/capitol/delete/" *country "\">delete</a></td></tr>\n"]
	]
	[*res "</table>\n"]
]

[[router *req *res]
	[*req FULL_HEADER_TEXT : *x]
	[show *x]
	[*req HTTP_URI : *route]
	[show [route *req *res : *route]]
	[route *req *res : *route]
]

[[route *req *res GET capitol] [controller_index *req *res]]
[[route *req *res GET capitol delete *country] [controller_delete *req *res *country]]


[show "ready"]

[crack [webserver 3008 router] [show "Done...."]]

[exit]

