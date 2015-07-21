
program prcgtk #machine := "prcgtk" [gtk_init gtk_main gtk_stop gtk_command]

#machine gtk_init := "gtk_init"
#machine gtk_main := "gtk_main"
#machine gtk_stop := "gtk_stop"

[[gtk_command]
	[gtk_init]
	[crack [command] [gtk_stop]]
	[gtk_main]
]

end .

