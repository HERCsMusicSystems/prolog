
import studio

program sql [sql adbcl adbcl0 dbcl dblist dbdelcl dberase dbcg dbres dbstore dbload]


[[dblist *conn *module] [is_text *module] / [*conn dblist *module : *atoms] [show *atoms]]
[[dblist *conn] / [*conn dblist : *modules] [show *modules]]
[[dblist *conn *atom]
	[is_atom *atom] /
	[TRY
		[*conn dbcl *atom : *count]
		[minimise *ind 0 *count]
		[*conn dbcl *atom *ind *cl]
		[show *cl]
		fail
	]
]

[[dberase *conn [[*atom : *parameters] : *body]]
	[is_atom *atom] /
	[TRY
		[*conn dbcl *atom : *count]
		[minimise *ind 0 *count]
		[*conn dbcl *atom *ind [[*atom : *parameters] : *body]]
		[*conn dbdelcl *atom *ind]
		fail
	]
]

[[dberase *conn *module]
	[is_text *module] /
	[*conn dblist *module : *atoms]
	/ [dberase *conn : *atoms]
]

[[dberase *conn]/]
[[dberase *conn *atom : *atoms]
	[is_atom *atom]
	[TRY [*conn dbdelcl *atom]]
	/ [dberase *conn : *atoms]
]

[[dbload *conn *module]
	[is_text *module] /
	[*conn dblist *module : *atoms]
	/ [dbload *conn : *atoms]
]

[[dbload *conn]/]
[[dbload *conn *atom : *atoms]
	[is_atom *atom]
	[*conn dbcl *atom : *count]
	[dbload *conn 0 *count *atom]
	/ [dbload *conn : *atoms]
]

[[dbload *conn *ind *count *atom]
	[is_integer *ind]
	[less *ind *count] /
	[*conn dbcl *atom *ind *clause]
	[addcl *clause]
	[sum *ind 1 *next]
	/ [dbload *conn *next *count *atom]
]

[[dbload *conn *ind *count *atom]
	[is_integer *ind]/
]

[[dbres *conn *atom : *parameters]
	[*conn dbcl *atom *i]
	[minimise *ind 0 *i]
	[*conn dbcl *atom *ind [[* : *parameters] :  *body]]
	: *body
]

[[dbstore *conn *directory]
	[is_text *directory]
	[list *directory : *atoms]
	/ [dbstore *conn : *atoms]
]
[[dbstore *conn *atom : *atoms]
	[is_atom *atom]
	[TRY
		[CL *atom *i]
		[minimise *ind 0 *i]
		[CL *atom *ind *cl]
		[*conn adbcl *cl]
		fail
	]
	/ [dbstore *conn : *atoms]
]
[[dbstore *conn]]


end .

