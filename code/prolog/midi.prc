;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2013 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program midi #machine := "prolog.midi"

	[
		noteon noteoff

		income_midi drag_and_drop keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex sysexch SYSEX SYSEXCH chex chexer timingclock START CONTINUE STOP activesensing
		attack release cutoff resonance mono poly bank banklsb portaon portaoff portatime
		volume reverb chorus foot breath pan modulation holdon holdoff
		PITCH CONTROL ATTACK RELEASE CUTOFF RESONANCE PORTATIME
		VOLUME REVERB CHORUS FOOT BREATH PAN MODULATION
		nrpn rpn NRPN hit DCMOD
		createLine createSource createDestination connectThru defaultDestination MidiLineType
		midi_info midi_port midi midi_in_info midi_out_info midi_in_port midi_out_port midi_message
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
#machine connectThru := "connectThru"
#machine defaultDestination := "defaultDestination"
#machine MidiLineType := "MidiLineType";
;#machine midi_in_info := "midi_in_info"
;#machine midi_out_info := "midi_out_info"
;#machine midi_in_port := "midi_in_port"
;#machine midi_out_port := "midi_out_port"

#machine DCMOD := "DCMOD"
#machine INTERVAL := "INTERVAL"

#machine midi_message := "midi_message"
#machine sysex := "sysex"
#machine sysexch := "sysexch"
#machine SYSEX := "SYSEX"
#machine SYSEXCH := "SYSEXCH"
#machine chex := "chex"
#machine chexer := "chexer"

; demo
[[noteon *key] [keyon 0 *key 100]]
[[noteoff *key] [keyoff 0 *key]]
[[noteoff] [keyoff 0]]

; main midi messages
#machine keyoff := "keyoff"
#machine keyon := "keyon"
#machine polyaftertouch := "polyaftertouch"
#machine control := "control"
#machine aftertouch := "aftertouch"
#machine programchange := "programchange"
#machine pitch := "pitch"
#machine timingclock := "timingclock"
#machine START := "START"
#machine STOP := "STOP"
#machine CONTINUE := "CONTINUE"
#machine activesensing := "activesensing"


; controllers
#machine attack := "attack"
#machine release := "release"
#machine cutoff := "cutoff"
#machine resonance := "resonance"
#machine bank := "bank"
#machine banklsb := "banklsb"
#machine portatime := "portatime"
#machine volume := "volume"
#machine reverb := "reverb"
#machine chorus := "chorus"
#machine foot := "foot"
#machine breath := "breath"
#machine pan := "pan"
#machine modulation := "modulation"
#machine mono := "mono"
#machine poly := "poly"
#machine portaon := "portaon"
#machine portaoff := "portaoff"
#machine holdon := "holdon"
#machine holdoff := "holdoff"
; capital controllers
#machine PITCH := "PITCH"
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

#machine nrpn := "nrpn"
#machine NRPN := "NRPN"
#machine rpn := "rpn"

; macros

[[hit *ch *vel]]
[[hit *ch *vel *key :*r]
	[keyon *ch *key *vel]/[hit *ch *vel :*r]]

#machine egcopy := "egcopy"
#machine egcopy_index := "egcopy_index"
#machine egcopy_freq := "egcopy_freq"
#machine egcopy_amp := "egcopy_amp"
#machine egcopy_pan := "egcopy_pan"

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

