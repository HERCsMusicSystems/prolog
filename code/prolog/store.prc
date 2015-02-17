import studio

program store
	[store store_import store_header store_atoms store_relations store_clauses store_end store_database]

[[store *module_name *file_name]
	[is_text *module_name] [is_text *file_name] /
	[file_writer *fw *file_name]
	[TRY
		[list : *modules]
		[APPEND * [*module_name : *reversed_import] *modules] [REVERSE *reversed_import *import]
		[store_import *fw *import]
		[store_header *fw *module_name]
		[list *module_name : *reversed_atoms] [REVERSE *reversed_atoms *atoms]
		[store_atoms *fw *atoms]
		[store_relations *fw *atoms]
		[store_end *fw]
	]
	[*fw]
]

[[store *module]
	[create_atom *atom]
	[add *module ".prc" *file_name]
	[file_writer *atom *file_name]
	[store *atom *module]]
[[store *atom *module]
	[list : *modules]
	[eq [*user : *imports] *modules]
	[store_import *atom *imports]
	[store_header *atom *module]
	[list *user : *atoms]
	[store_atoms *atom *atoms]
	[store_relations *atom *atoms]
	[store_end *atom]
	[*atom] /]
[[store *atom *module] [*atom] fail]


[[store_import *atom []] [*atom "\n"] /]
[[store_import *atom [*import : *imports]]
	[*atom "import " *import "\n"] / [store_import *atom *imports]]
[[store_header *atom *module]
	[*atom "program " *module]
	[TRY [has_machine *module] [*atom " " *module " := " [*module]]]
	[*atom "\n"]]
[[store_atoms *atom *atoms] [*atom "	" [*atoms] "\n\n"]]
[[store_relations *atom []] [*atom "\n"] /]
[[store_relations *atom [*clause : *clauses]]
	[SELECT
		[[has_machine *clause] [*atom "#machine " [*clause] " := \"" [*clause] "\"\n\n"]]
		[[list *clause : *output] [eq *output [* : *]] [store_clauses *atom *output]]
		[]
	] / [store_relations *atom *clauses]]
[[store_clauses *atom []] [*atom "\n"] /]
[[store_clauses *atom [*clause : *clauses]]
	[*atom [*clause] "\n"] / [store_clauses *atom *clauses]]
[[store_end *atom] [*atom "end .\n"]]

[[store_database *file_name : *relations] [file_writer *fw *file_name] [store_relations *fw *relations] [*fw "[exit]\n"] [*fw]]

end .
