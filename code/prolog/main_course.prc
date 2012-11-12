import studio

program main_course
	[
		light_meal main_course horse_duvre meat fish desert
		radishes pate beef pork sole tuna fruit icecream
	]


[[light_meal *energy *horse_duvre *main_course *desert]
	[horse_duvre *horse_duvre *hd_energy]
	[main_course *main_course *mc_energy]
	[desert *desert *des_energy]
	[sum *hd_energy *mc_energy *sub_energy]
	[sum *sub_energy *des_energy *energy]
	]

[[main_course *m *i] [meat *m *i]]
[[main_course *m *i] [fish *m *i]]

[[horse_duvre radishes 1]]
[[horse_duvre pate 6]]

[[meat beef 5]]
[[meat pork 7]]

[[fish sole 2]]
[[fish tuna 4]]

[[desert fruit 2]]
[[desert icecream 6]]

end .