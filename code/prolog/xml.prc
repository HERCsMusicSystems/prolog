
program xml #machine := "prolog.xml" [xml xml_text xml_node]

#machine xml := "xml"

[[xml_node *root *atom *node] [is_atom *root] [*root *atom *node]]
[[xml_node *root *atom : *command] [is_atom *root] [*root *atom *node] [xml_node *node : *command]]
[[xml_text *node *text] [is_atom *node] [*node *text]]
[[xml_text *node *atom : *command] [is_atom *node] [*node *atom *next] [xml_text *next : *command]]

end .
