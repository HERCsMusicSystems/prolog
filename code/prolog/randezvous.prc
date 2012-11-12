import help
import studio
import f1

program randezvous #auto

auto := [
	[VARIABLE kanapki] [kanapki 0]
	[VARIABLE pieczywo] [pieczywo 0]
	[VARIABLE mieso] [mieso 0]
	]

[[gotujmieso *m]
	[rnd *time 100 5000]
	[mieso : *m]
	[write "gotuje => "] [wait *time] [show "mieso " [*time *m]]
	[mieso 0]
	]

[[robkanapki]
	[pieczywo : *p]
	[mieso : *mieso]
	[less 0 *mieso] [less 0 *p]
	[gotujmieso *m]
	[add *m *p *k]
	[pieczywo 0]
	[kanapki *k]
	]
[[robkanapki] [write "		Nie moge zrobic kanapek.\n"]]

[[status]
	[mieso : *m] [pieczywo : *p] [kanapki : *k]
	[show "mieso " [*m] " pieczywo " [*p] " kanapki " [*k]]
	]

[[akceptuj_pieczywo *task 0]
	[accept *task dostarczpieczywo *pieczywo]
	[pieczywo *pieczywo]
	[show "Dostalem pieczywo => " *pieczywo]
	]
[[akceptuj_mieso *task 0]
	[accept *task dostarczmieso *mieso]
	[mieso *mieso]
	[show "Dostalem mieso => " *mieso]
	]
[[akceptuj_klienta *task *p *m]
	[less 0 *p] [less 0 *m]
	[accept *task gospoda *pk
		[robkanapki]
		[kanapki : *k]
		[show "	Zrobilem kanapki => " *k]
		[eq *k *pk]
		]
	[wait 4000] [kanapki 0]
	]

[[gospoda *task]
	[task *task [dostarczpieczywo dostarczmieso gospoda]
		[FOREVER
			[pieczywo : *p] [mieso : *m]
			[select *task
				[[akceptuj_pieczywo *task *p]]
				[[akceptuj_mieso *task *m]]
				[[akceptuj_klienta *task *p *m]]
				]
			]
		]
	]

auto := [[gospoda ada]]

end := [[set_colors 16776960 0] [auto_atoms] [command]] .
