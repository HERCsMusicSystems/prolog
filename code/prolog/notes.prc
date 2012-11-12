import studio
import f1
import help

program notes #machine := "notes"
	[
		INTERVAL
		C C# Cx Cb Cbb
		D D# Dx Db Dbb
		E E# Ex Eb Ebb
		F F# Fx Fb Fbb
		G G# Gx Gb Gbb
		A A# Ax Ab Abb
		B B# Bx Bb Bbb
		major minor major7 major7m minor7 minor7m major6 major6m minor6 minor6m sus4
		octave notestep
		notevalue notekeys
		interval absolute_interval logical_interval chord scale get_scale
		consonance perfect_consonance
		note_less note_less_eq
		note_from_scale from_list on_list in_scale
	]

; Usage:
; [notevale [F# 1] 38 66]
; [notekeys [[A 1] [B 1] [C# 1] ...] [69 71 73 ....]]
; [interval [C 1] [F# 1] [3 6]]
; [chord [E major 1] [G# 0] [B 0] [E 1]]
; [scale major [E 1] [F# 1] [G# 1] .... [D 2]]
; [get_scale [E 1] major *scale]
; [note_from_scale *index *note/key *scale]

preprocessor := f1

#machine INTERVAL := "INTERVAL"

[[octave -2 14 24]]
[[octave -1 21 36]]
[[octave 0 28 48]]
[[octave 1 35 60]]
[[octave 2 42 72]]
[[octave 3 49 84]]

[[notestep C 0 0]]
[[notestep D 1 2]]
[[notestep E 2 4]]
[[notestep F 3 5]]
[[notestep G 4 7]]
[[notestep A 5 9]]
[[notestep B 6 11]]

[[notestep F# 3 6]]
[[notestep Bb 6 10]]
[[notestep C# 0 1]]
[[notestep Eb 2 3]]
[[notestep G# 4 8]]
[[notestep Ab 5 8]]
[[notestep D# 1 3]]
[[notestep Db 1 1]]
[[notestep A# 5 10]]
[[notestep Gb 4 6]]
[[notestep E# 2 5]]
[[notestep Cb 0 -1]]
[[notestep B# 6 12]]
[[notestep Fb 3 4]]

[[notestep Fx 3 7]]
[[notestep Bbb 6 9]]
[[notestep Cx 0 2]]
[[notestep Ebb 2 2]]
[[notestep Gx 4 9]]
[[notestep Abb 5 7]]
[[notestep Dx 1 4]]
[[notestep Dbb 1 0]]
[[notestep Ax 5 11]]
[[notestep Gbb 4 5]]
[[notestep Ex 2 6]]
[[notestep Cbb 0 -2]]
[[notestep Bx 6 13]]
[[notestep Fbb 3 3]]

[[perfect_consonance [0 0]]]
[[perfect_consonance [4 7]]]
[[perfect_consonance [*d *h]] [less 6 *d] [sum 7 *d1 *d] [sum 12 *h1 *h] / [perfect_consonance [*d1 *h1]]]
[[consonance [0 0]]]
[[consonance [4 7]]]
[[consonance [2 4]]]
[[consonance [2 3]]]
[[consonance [5 8]]]
[[consonance [5 9]]]
[[consonance [*d *h]] [less 6 *d] [sum 7 *d1 *d] [sum 12 *h1 *h] / [consonance [*d1 *h1]]]

[[notevalue [*note *octave : *] *diatonic *chromatic]
	[octave *octave *octave_diatonic *octave_chromatic]
	[notestep *note *note_diatonic *note_chromatic]
	[sum *octave_diatonic *note_diatonic *diatonic]
	[sum *octave_chromatic *note_chromatic *chromatic]
]

[[notekeys [] []]/]
[[notekeys [*note : *notes] [*key : *keys]] [notevalue *note * *key] / [notekeys *notes *keys]]

[[note_less : *notes] [notekeys *notes *keys] / [less : *keys]]
[[note_less_eq : *notes] [notekeys *notes *keys] / [less_eq : *keys]]

[[interval [*from_step *from_octave : *t] [*to_step *to_octave : *t] [*diatonic *chromatic]]
	[notestep *from_step *from_d *from_ch]
	[DCMOD *from_d *from_ch *from_octave *diatonic *chromatic *d *c *to_octave]
	[notestep *to_step *d *c]/
]
[[interval [*from_step *from_octave : *t] [*to_step *to_octave : *t] [*diatonic *chromatic]]
	[notevalue [*from_step *from_octave] *from_d *from_ch]
	[notevalue [*to_step *to_octave] *to_d *to_ch]
	[sum *from_d *diatonic *to_d]
	[sum *from_ch *chromatic *to_ch]
]
[[absolute_interval *from *to *interval] [note_less *from *to] / [interval *from *to *interval]]
[[absolute_interval *from *to *interval] [interval *to *from *interval]]
[[logical_interval *from *to *interval] [note_less *from *to] / [interval *from *to *interval]]
[[logical_interval *from *to *interval] [interval *to *to2 [7 12]] / [logical_interval *from *to2 *interval]]

[[chord [*note major 0] [*note *octave] *second *third]
	[interval [*note *octave] *second [2 4]]
	[interval *second *third [2 3]]
]
[[chord [*note major 1] *first *second [*note *octave]]
	[interval *first *second [2 3]]
	[interval *second [*note *octave] [3 5]]
]
[[chord [*note major 2] *first [*note *octave] *third]
	[interval *first [*note *octave] [3 5]]
	[interval *first *third [5 9]]
]
[[chord [*note minor 0] [*note *octave] *second *third]
	[interval [*note *octave] *second [2 3]]
	[interval *second *third [2 4]]
]
[[chord [*note minor 1] *first *second [*note *octave]]
	[interval *first *second [2 4]]
	[interval *second [*note *octave] [3 5]]
]
[[chord [*note minor 2] *first [*note *octave] *third]
	[interval *first [*note*octave] [3 5]]
	[interval *first *third [5 8]]
]

[[scale major *pryma *sekunda *tercja *kwarta *kwinta *seksta *septyma]
	[interval *pryma *sekunda [1 2]]
	[interval *pryma *tercja [2 4]]
	[interval *pryma *kwarta [3 5]]
	[interval *pryma *kwinta [4 7]]
	[interval *pryma *seksta [5 9]]
	[interval *pryma *septyma [6 11]]
]
[[scale minor *pryma *sekunda *tercja *kwarta *kwinta *seksta *septyma]
	[interval *pryma *sekunda [1 2]]
	[interval *pryma *tercja [2 3]]
	[interval *pryma *kwarta [3 5]]
	[interval *pryma *kwinta [4 7]]
	[interval *pryma *seksta [5 8]]
	[interval *pryma *septyma [6 10]]
]

[[get_scale *note *mode [*note : *scale]] / [scale *mode *note : *scale]]

[[note_from_scale *length *index *key *scale] [less *index 0] [add *index *length *new] / [note_from_scale *length *new *k2 *scale]
	[SELECT
		[[sub *k2 12 *key]]
		[[interval *key *k2 [7 12]]]
	]
]
[[note_from_scale *length *index *key *scale] [less_eq *length *index] [sub *index *length *new] / [note_from_scale *length *new *k2 *scale]
	[SELECT
		[[is_number *k2] [add *k2 12 *key]]
		[[interval *k2 *key [7 12]]]
	]
]
[[note_from_scale *length *index *key *scale] / [from_list *index *key *scale]]
[[note_from_scale *index *key *scale] [LENGTH *scale *length] / [note_from_scale *length *index *key *scale]]

[[from_list 0 *key [*key :*]]/]
[[from_list *index *key [* :*scale]] [sub *index 1 *next] / [from_list *next *key *scale]]
[[from_list :*]]

[[on_list *x [*x : *]]]
[[on_list *x [*h : *t]] / [on_list *x *t]]
[[in_scale [*note *] *scale] [on_list *note *scale]]

private [from_list]

end .



end . := [[preprocessor f1] [set_colors 0xffff00 0] [command]] .













[[speed]
	[wait *t1]
	[interval [C 3] *x1 [4 7]] [show *x1]
	[interval [C 3] *x2 [4 7]] [show *x2]
	[interval [C 3] *x3 [4 7]] [show *x3]
	[interval [C 3] *x4 [4 7]] [show *x4]
	[interval [C 3] *x5 [4 7]] [show *x5]
	[interval [C 3] *x6 [4 7]] [show *x6]
	[interval [C 3] *x7 [4 7]] [show *x7]
	[interval [C 3] *x8 [4 7]] [show *x8]
	[wait *t2]
	[sub *t2 *t1 *diff] [show *diff]
]
