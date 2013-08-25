;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2013 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program midi #machine := "prolog.midi"

	[
		noteon noteoff

		income_midi drag_and_drop keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex sysexch SYSEX SYSEXCH timingclock START CONTINUE STOP activesensing
		attack release cutoff resonance mono poly bank banklsb portaon portaoff portatime
		volume reverb chorus foot breath pan modulation holdon holdoff
		CONTROL ATTACK RELEASE CUTOFF RESONANCE PORTATIME
		VOLUME REVERB CHORUS FOOT BREATH PAN MODULATION
		nrpn rpn NRPN RPN hit DCMOD
		createLine createSource createDestination
		midi_info midi_port midi midi_in_info midi_out_info midi_in_port midi_out_port midi_short_msg MIDI_SHORT_MSG midi_long_msg
		midi_manufacturers_id midi_product_id midi_product_version
		egcopy egcopy_freq egcopy_amp egcopy_index egcopy_pan
		
		C C# Cx Cb Cbb
		D D# Dx Db Dbb
		E E# Ex Eb Ebb
		F F# Fx Fb Fbb
		G G# Gx Gb Gbb
		A A# Ax Ab Abb
		B B# Bx Bb Bbb
		octave notestep notevalue notekeys note_less note_less_eq
		INTERVAL interval
	]

#machine createLine := "createLine"
#machine createSource := "createSource"
#machine createDestination := "createDestination"
;#machine midi_in_info := "midi_in_info"
;#machine midi_out_info := "midi_out_info"
;#machine midi_in_port := "midi_in_port"
;#machine midi_out_port := "midi_out_port"

;#machine DCMOD := "DCMOD"
;#machine INTERVAL := "INTERVAL"

;#machine midi_short_msg := "midi_short_msg"
;#machine MIDI_SHORT_MSG := "MIDI_SHORT_MSG"
;#machine midi_long_msg := "midi_long_msg"
;#machine sysex := "sysex"
;#machine sysexch := "sysexch"
;#machine SYSEX := "SYSEX"
;#machine SYSEXCH := "SYSEXCH"
;#machine midi_manufacturers_id := "midi_manufacturers_id"
;#machine midi_product_id := "midi_product_id"
;#machine midi_product_version := "midi_product_version"

; demo
[[noteon *key] [keyon 0 *key 100]]
[[noteoff *key] [keyoff 0 *key]]
[[noteoff] [keyoff 0]]

; main midi messages
[[keyoff *channel *key] [midi_short_msg 128 *channel *key 0]]
[[keyoff *channel *key *velocity] [midi_short_msg 128 *channel *key *velocity]]
[[keyon *channel *key *velocity] [midi_short_msg 144 *channel *key *velocity]]
[[polyaftertouch *channel *key *value] [midi_short_msg 160 *channel *key *value]]
#machine control := "control"
;[[control *channel *controller *value] [midi_short_msg 176 *channel *controller *value]]
[[programchange *channel *program] [midi_short_msg 192 *channel *program]]
[[programchange *channel *msb *lsb *program] [add *channel 64 *selector] [sysex *selector 84 *lsb *program *msb]]
[[aftertouch *channel *value] [midi_short_msg 208 *channel *value]]
[[pitch *channel *msb] [midi_short_msg 224 *channel 0 *msb]]
[[pitch *channel *lsb *msb] [midi_short_msg 224 *channel *lsb *msb]]
[[timingclock] [midi_short_msg 248]]
[[START] [midi_short_msg 250]]
[[CONTINUE] [midi_short_msg 251]]
[[STOP] [midi_short_msg 252]]
[[activesensing] [midi_short_msg 254]]


; controllers
[[keyoff *channel] [midi_short_msg 176 *channel 123 0]]
#machine attack := "attack"
#machine release := "release"
#machine cutoff := "cutoff"
#machine resonance := "resonance"
;[[attack *channel *value] [midi_short_msg 176 *channel 73 *value]]
;[[release *channel *value] [midi_short_msg 176 *channel 72 *value]]
;[[cutoff *channel *value] [midi_short_msg 176 *channel 74 *value]]
;[[resonance *channel *value] [midi_short_msg 176 *channel 71 *value]]
[[mono *channel] [midi_short_msg 176 *channel 126 0]]
[[poly *channel] [midi_short_msg 176 *channel 127 0]]
[[bank *channel *msb *lsb] [midi_short_msg 176 *channel 0 *msb] [midi_short_msg 176 *channel 32 *lsb]]
[[bank *channel *value] [midi_short_msg 176 *channel 0 *value]]
[[bank *channel *msb *lsb] [add *channel 64 *selector] [sysex *selector 83 *lsb *msb]]
[[bank *channel *msb] [add *channel 64 *selector] [sysex *selector 82 *msb]]
[[banklsb *channel *value] [midi_short_msg 176 *channel 32 *value]]
[[portaon *channel] [midi_short_msg 176 *channel 65 127]]
[[portaoff *channel] [midi_short_msg 176 *channel 65 0]]
#machine portatime := "portatime"
#machine volume := "volume"
#machine reverb := "reverb"
#machine chorus := "chorus"
#machine foot := "foot"
#machine breath := "breath"
#machine pan := "pan"
#machine modulation := "modulation"
;[[portatime *channel *value] [midi_short_msg 176 *channel 5 *value]]
;[[volume *channel *value] [midi_short_msg 176 *channel 7 *value]]
;[[reverb *channel *value] [midi_short_msg 176 *channel 91 *value]]
;[[chorus *channel *value] [midi_short_msg 176 *channel 93 *value]]
;[[foot *channel *value] [midi_short_msg 176 *channel 4 *value]]
;[[breath *channel *value] [midi_short_msg 176 *channel 2 *value]]
;[[pan *channel *value] [midi_short_msg 176 *channel 10 *value]]
;[[modulation *channel *value] [midi_short_msg 176 *channel 1 *value]]
[[holdon *channel] [midi_short_msg 176 *channel 64 127]]
[[holdoff *channel] [midi_short_msg 176 *channel 64 0]]
; capital controllers
#machine CONTROL := "CONTROL"
#machine ATTACK := "ATTACK"
#machine RELEASE := "RELEASE"
#machine CUTOFF := "CUTOFF"
#machine RESONANCE := "RESONANCE"
#machine PORTATIME := "PORTATIME"
#machine VOLUME := "VOLUME"
#machine REVERB := "REVERB"
#machine CHORUS := "CHORUS"
#machine FOOT := "FOOT"
#machine BREATH := "BREATH"
#machine PAN := "PAN"
#machine MODULATION := "MODULATION"
;[[CONTROL *channel *controller *value] [MIDI_SHORT_MSG 176 *channel *controller *value]]
;[[ATTACK *channel *value] [MIDI_SHORT_MSG 176 *channel 73 *value]]
;[[RELEASE *channel *value] [MIDI_SHORT_MSG 176 *channel 72 *value]]
;[[CUTOFF *channel *value] [MIDI_SHORT_MSG 176 *channel 74 *value]]
;[[RESONANCE *channel *value] [MIDI_SHORT_MSG 176 *channel 71 *value]]
;[[PORTATIME *channel *value] [MIDI_SHORT_MSG 176 *channel 5 *value]]
;[[VOLUME *channel *value] [MIDI_SHORT_MSG 176 *channel 7 *value]]
;[[REVERB *channel *value] [MIDI_SHORT_MSG 176 *channel 91 *value]]
;[[CHORUS *channel *value] [MIDI_SHORT_MSG 176 *channel 93 *value]]
;[[FOOT *channel *value] [MIDI_SHORT_MSG 176 *channel 4 *value]]
;[[BREATH *channel *value] [MIDI_SHORT_MSG 176 *channel 2 *value]]
;[[PAN *channel *value] [MIDI_SHORT_MSG 176 *channel 10 *value]]
;[[MODULATION *channel *value] [MIDI_SHORT_MSG 176 *channel 1 *value]]

; nrpn / rpn
[[nrpn *ch *pmsb *plsb *vmsb *vlsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]
	[control *ch 38 *vlsb]
	[control *ch  6 *vmsb]]
[[nrpn *ch *pmsb *plsb *vmsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]
	[control *ch  6 *vmsb]]
[[nrpn *ch *pmsb *plsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]]
[[nrpn *ch *vmsb]
	[control *ch 6 *vmsb]]
[[nrpn *ch]
	[control *ch 99 127]
	[control *ch 98 127]]
[[NRPN *ch *pmsb *plsb *vmsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]
	[MIDI_SHORT_MSG 176 *ch  6 *vmsb]]
[[NRPN *ch *vmsb]
	[MIDI_SHORT_MSG 176 *ch 6 *vmsb]]



[[rpn *ch *pmsb *plsb *vmsb *vlsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]
	[control *ch 38 *vlsb]
	[control *ch  6 *vmsb]]
[[rpn *ch *pmsb *plsb *vmsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]
	[control *ch  6 *vmsb]]
[[rpn *ch *pmsb *plsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]]
[[rpn *ch *vmsb]
	[control *ch 6 *vmsb]]
[[rpn *ch]
	[control *ch 101 127]
	[control *ch 100 127]]
[[RPN *ch *pmsb *plsb *vmsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]
	[MIDI_SHORT_MSG 176 *ch  6 *vmsb]]
[[RPN *ch *vmsb]
	[MIDI_SHORT_MSG 176 *ch 6 *vmsb]]

; macros

[[hit *ch *vel]]
[[hit *ch *vel *key :*r]
	[keyon *ch *key *vel]/[hit *ch *vel :*r]]

[[egcopy_index *channel *from *to] [sum *channel 64 *ch] [sysex *ch 66 *from *to]]
[[egcopy_freq *channel *from *to] [sum *channel 64 *ch] [sum *from 16 *f] [sum *to 16 *t] [sysex *ch 66 *f *t]]
[[egcopy_amp *channel *from *to] [sum *channel 64 *ch] [sum *from 32 *f] [sum *to 32 *t] [sysex *ch 66 *f *t]]
[[egcopy_pan *channel *from *to] [sum *channel 64 *ch] [sum *from 48 *f] [sum *to 48 *t] [sysex *ch 66 *f *t]]
[[egcopy : *command] [egcopy_amp : *command]]

; midi port manipulations

[[midi_info] [midi_in_info] [midi_out_info]]
[[midi_info *in *out] [midi_in_info : *in] [midi_out_info : *out]]
[[midi_port *in *out] [midi_in_port *in] [midi_out_port *out]]
[[midi] [midi_info]]
[[midi *in *out] [midi_port *in *out] [midi_info]]

; musical definitions

[[octave 0 28 48]]
[[octave -1 21 36]]
[[octave 1 35 60]]
[[octave -2 14 24]] ; grand piano lowest a = 21
[[octave 2 42 72]]
[[octave -3 7 12]]
[[octave 3 49 84]]
[[octave -4 0 0]]
[[octave 4 56 96]]
[[octave 5 63 108]] ; grand piano highest c = 108
[[octave 6 70 120]]

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

[[notevalue [*note *octave : *] *diatonic *chromatic]
	[notestep *note *note_diatonic *note_chromatic]
	[octave *octave *octave_diatonic *octave_chromatic]
	[sum *octave_diatonic *note_diatonic *diatonic]
	[sum *octave_chromatic *note_chromatic *chromatic]
]

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

[[notekeys [] []]/]
[[notekeys [*note : *notes] [*key : *keys]] [notevalue *note * *key] / [notekeys *notes *keys]]

[[note_less : *notes] [notekeys *notes *keys] / [less : *keys]]
[[note_less_eq : *notes] [notekeys *notes *keys] / [less_eq : *keys]]

end .

