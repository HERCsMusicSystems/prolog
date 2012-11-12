 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ;; Copyright (C) 2005 Robert P. Wolf ;;
 ;;        ALL RIGHTS RESERVED        ;;
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


import studio
import help

\functional

program lisp
	[
		eval EVAL LIST_EVAL EXECUTE_EVAL NATIVE_CODE NATIVE_CALL
		define quote set lambda let let* letrec rec if cond else case begin '
		extended_syntax
		car cdr caar cadr cdar cddr cons length append list
		#true #false + - * / % = < > <= >= print display
		and or not AND OR XOR NOT e pi
		abs acos asin atan cos exp ln log pow sin tan trunc
		and? or?
		equal?? not? <? >? <=? >=? case?
		kellner lisp_parameters
		l letrec_processing

		open_editor close_editor

		noteon noteoff
		keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex sysexch SYSEX SYSEXCH timingclock START CONTINUE STOP
		attack release cutoff resonance mono poly bank banklsb portaon portaoff portatime
		volume reverb chorus foot breath pan modulation holdon holdoff
		CONTROL ATTACK RELEASE CUTOFF RESONANCE PORTATIME
		VOLUME REVERB CHORUS FOOT BREATH PAN MODULATION
		nrpn rpn NRPN RPN hit
		midi_close midi_in_info midi_out_info midi_in_port midi_out_port midi_short_msg MIDI_SHORT_MSG midi_long_msg
	]

[[lisp_parameters [] []]]
[[lisp_parameters [~variable : ~variables] [~atom : ~atoms]]
	[create_atom "#<parameter>" ~atom]
	[VARIABLE ~atom] [~atom ~variable] ! [lisp_parameters ~variables ~atoms]]

[[case? ~x ~key [else : ~expressions] : ~] ! [EXECUTE_EVAL ~x ~expressions]]
[[case? ~x ~key [~test : ~expressions] : ~] [EVAL ~key ~test] ! [EXECUTE_EVAL ~x ~expressions]]
[[case? ~x ~key ~ : ~clauses] ! [case? ~x ~key : ~clauses]]
[[case? ~ : ~]]


[[letrec_processing [] [] ~body ~body]!]
[[letrec_processing [[~i ~v] : ~tail] [[~i #false] : ~processed_tail] ~body ~processed_body] !
	[letrec_processing ~tail ~processed_tail [[set ~i ~v] : ~body] ~processed_body]]



[[extended_syntax [let* [~variable] : ~body] [let [~variable] : ~body]]!]
[[extended_syntax [let* [~head : ~tail] : ~body] [let [~head] ~internals]] !
	[extended_syntax [let* ~tail : ~body] ~internals]]
[[extended_syntax [letrec ~variables : ~body] [let ~processed_variables : ~processed_body]] !
	[letrec_processing ~variables ~processed_variables ~body ~processed_body]!]
[[extended_syntax [rec ~variable ~body] [letrec [[~variable ~body]] ~variable]]!]

;[[EVAL ~x ~original] [extended_syntax ~original ~extended] ! [write "Exteended => "] [pp ~extended] [nl]]
[[EVAL ~x ~original] [extended_syntax ~original ~extended] ! [EVAL ~x ~extended]]

[[EVAL #true [and]] !]
[[EVAL ~x [and ~expression]] ! [EVAL ~x ~expression]]
[[EVAL #false [and ~expression : ~]] [ONE [EVAL ~x ~expression]] [eq #false ~x] !]
[[EVAL ~x [and ~ : ~expressions]] ! [EVAL ~x [and : ~expressions]]]
[[EVAL #false [or]] !]
[[EVAL ~x [or ~expression]] ! [EVAL ~x ~expression]]
[[EVAL #true [or ~expression : ~]] [ONE [EVAL ~x ~expression]] [eq #true ~x] !]
[[EVAL ~x [or ~ : ~expressions]] ! [EVAL ~x [or : ~expressions]]]
[[EVAL ~x [case ~key : ~clauses]] [EVAL ~KEY ~key] ! [case? ~x ~KEY : ~clauses]]
[[EVAL ~x [cond [else : ~expressions] : ~]] ! [EXECUTE_EVAL ~x ~expressions]]
[[EVAL ~x [cond [~test : ~expressions] : ~]] [EVAL #true ~test] ! [EXECUTE_EVAL ~x ~expressions]]
[[EVAL ~x [cond ~ : ~tail]] ! [EVAL ~x [cond : ~tail]]]
[[EVAL ~ [cond : ~]]]
[[EVAL ~x [begin : ~expressions]] ! [EXECUTE_EVAL ~x ~expressions]]
[[EVAL ~x [if ~test ~then : ~]] [EVAL #true ~test] ! [EVAL ~x ~then]]
[[EVAL ~x [if ~test ~then]]]
[[EVAL ~x [if ~test ~then ~else]] [EVAL ~x ~else]]
[[EVAL ~x [let ~paired_list : ~body]] !
	[MAP ~paired_list ~left ~right]
	[EVAL ~x [[lambda ~left : ~body] : ~right]]]
[[EVAL ~ [define ~atom ~value]]
	[VARIABLE ~atom]
	[EVAL ~result ~value]
	[~atom ~result]
	]
[[EVAL ~ [set ~atom ~value]] ! [EVAL ~result ~value] [~atom ~result]]
[[EVAL ~x ' ~x] !]
[[EVAL ~x [quote ~x]] !]
[[EVAL #true #true]]
[[EVAL #false #false]]
[[EVAL [] []]]
[[EVAL ~x [lambda ~parameters : ~body]] !
	[create_atom "#<machine>" ~x]
	[CONSTANT ~x [~parameters ~body]]
	]
[[EVAL ~x [~function : ~parameters]] !
	[LIST_EVAL [~closure : ~values] [~function : ~parameters]]
	[NATIVE_CALL ~x ~closure ~values]]
[[EVAL ~x ~x] [is_number ~x] !]
[[EVAL ~x ~x] [is_text ~x] !]
[[EVAL ~x ~atom] [is_atom ~atom] ! [~atom : ~value] [eq ~x ~value] !]

[[NATIVE_CALL ~x [~values [NATIVE_CODE ~x] : ~body] ~values] ! : ~body]
[[NATIVE_CALL ~x ~closure ~values]
	[lisp_parameters ~values ~lisp_parameters]
	[~closure ~lisp_parameters ~body]
	[EXECUTE_EVAL ~x ~body]
	]

[[LIST_EVAL [] []] !]
[[LIST_EVAL [~value : ~values] [' ~value : ~tail]] !
	[LIST_EVAL ~values ~tail]]
[[LIST_EVAL [~value : ~values] [~x : ~tail]]
	[EVAL ~value ~x] ! [LIST_EVAL ~values ~tail]
	]

[[EXECUTE_EVAL ~x [~call]] ! [EVAL ~x ~call]]
[[EXECUTE_EVAL ~x [~call : ~tail]]
	[EVAL ~ ~call] ! [EXECUTE_EVAL ~x ~tail]]

[[eval ~x] [EVAL ~value ~x] [pp ~value] [nl]]

[[kellner] ! [kellner kellner]]
[[kellner exit]]
[[kellner ~] [pr ~term] [eval ~term] ! [kellner ~term]]
[[kellner ~track] [write "Utter failure!\n"] ! [kellner ~track]]

auto := [
	[CONSTANT display [[~term] [NATIVE_CODE ~out] [write ~term]]]
	[CONSTANT print [[~term] [NATIVE_CODE ~out] [pp ~term]]]
	[CONSTANT + [~parameters [NATIVE_CODE ~out] [add ~out : ~parameters]]]
	[CONSTANT - [[~x ~y] [NATIVE_CODE ~out] [sub ~x ~y ~out]]]
	[CONSTANT * [~parameters [NATIVE_CODE ~out] [mult ~out : ~parameters]]]
	[CONSTANT / [[~x ~y] [NATIVE_CODE ~out] [div ~x ~y ~out]]]
	[CONSTANT % [[~x ~y] [NATIVE_CODE ~out] [mod ~x ~y ~out]]]
	[CONSTANT < [[~x ~y] [NATIVE_CODE ~out] [<? ~x ~y ~out]]]
	[CONSTANT > [[~x ~y] [NATIVE_CODE ~out] [>? ~x ~y ~out]]]
	[CONSTANT <= [[~x ~y] [NATIVE_CODE ~out] [<=? ~x ~y ~out]]]
	[CONSTANT >= [[~x ~y] [NATIVE_CODE ~out] [>=? ~x ~y ~out]]]
	[CONSTANT AND [[~x ~y] [NATIVE_CODE ~out] [@ studio . and ~x ~y ~out]]]
	[CONSTANT OR [[~x ~y] [NATIVE_CODE ~out] [@ studio . or ~x ~y ~out]]]
	[CONSTANT XOR [[~x ~y] [NATIVE_CODE ~out] [@ studio . xor ~x ~y ~out]]]
	[CONSTANT NOT [[~x] [NATIVE_CODE ~out] [@ studio . xor ~x -1 ~out]]]
	[CONSTANT not [[~x] [NATIVE_CODE ~out] [not? ~x ~out]]]
	[CONSTANT car [[[~head : ~tail]] [NATIVE_CODE ~head]]]
	[CONSTANT cdr [[[~head : ~tail]] [NATIVE_CODE ~tail]]]
	[CONSTANT caar [[[[~head : ~] : ~]] [NATIVE_CODE ~head]]]
	[CONSTANT cadr [[[~ ~head : ~]] [NATIVE_CODE ~head]]]
	[CONSTANT cdar [[[[~ : ~tail] : ~]] [NATIVE_CODE ~tail]]]
	[CONSTANT cddr [[[~ ~ : ~tail]] [NATIVE_CODE ~tail]]]
	[CONSTANT cons [[~head ~tail] [NATIVE_CODE [~head : ~tail]]]]
	[CONSTANT list [~list [NATIVE_CODE ~list]]]
	[CONSTANT length [[~list] [NATIVE_CODE ~out] [LENGTH ~list ~out]]]
	[CONSTANT append [[~list1 ~list2] [NATIVE_CODE ~out] [APPEND ~list1 ~list2 ~out]]]
	[CONSTANT = [[~e1 ~e2] [NATIVE_CODE ~out] [equal?? ~e1 ~e2 ~out]]]
	[CONSTANT e [[] [NATIVE_CODE ~e] [@ studio . e ~e]]]
	[CONSTANT pi [[] [NATIVE_CODE ~pi] [@ studio . pi ~pi]]]
	[CONSTANT abs [[~x] [NATIVE_CODE ~out] [@ studio . abs ~x ~out]]]
	[CONSTANT acos [[~x] [NATIVE_CODE ~out] [@ studio . acos ~x ~out]]]
	[CONSTANT asin [[~x] [NATIVE_CODE ~out] [@ studio . asin ~x ~out]]]
	[CONSTANT atan [[~x] [NATIVE_CODE ~out] [@ studio . atan ~x ~out]]]
	[CONSTANT cos [[~x] [NATIVE_CODE ~out] [@ studio . cos ~x ~out]]]
	[CONSTANT exp [[~x] [NATIVE_CODE ~out] [@ studio . exp ~x ~out]]]
	[CONSTANT ln [[~x] [NATIVE_CODE ~out] [@ studio . ln ~x ~out]]]
	[CONSTANT log [[~x] [NATIVE_CODE ~out] [@ studio . log ~x ~out]]]
	[CONSTANT pow [[~x ~y] [NATIVE_CODE ~out] [@ studio . pow ~x ~y ~out]]]
	[CONSTANT sin [[~x] [NATIVE_CODE ~out] [@ studio . sin ~x ~out]]]
	[CONSTANT tan [[~x] [NATIVE_CODE ~out] [@ studio . tan ~x ~out]]]
	[CONSTANT trunc [[~x] [NATIVE_CODE ~out] [@ studio . trunc ~x ~out]]]
	[CONSTANT #true #true]
	[CONSTANT #false #false]

	[CONSTANT open_editor [~x [NATIVE_CODE #true] [@ studio . open_editor : ~x]]]
	[CONSTANT close_editor [~x [NATIVE_CODE #true] [@ studio . close_editor : ~x]]]
	[CONSTANT noteon [~x [NATIVE_CODE #true] [@ studio . noteon : ~x]]]
	[CONSTANT noteoff [~x [NATIVE_CODE #true] [@ studio . noteoff : ~x]]]
	[CONSTANT keyon [~x [NATIVE_CODE #true] [@ studio . keyon : ~x]]]
	[CONSTANT keyoff [~x [NATIVE_CODE #true] [@ studio . keyoff : ~x]]]
	[CONSTANT polyaftertouch [~x [NATIVE_CODE #true] [@ studio . polyaftertouch : ~x]]]
	[CONSTANT control [~x [NATIVE_CODE #true] [@ studio . control : ~x]]]
	[CONSTANT programchange [~x [NATIVE_CODE #true] [@ studio . programchange : ~x]]]
	[CONSTANT aftertouch [~x [NATIVE_CODE #true] [@ studio . aftertouch : ~x]]]
	[CONSTANT pitch [~x [NATIVE_CODE #true] [@ studio . pitch : ~x]]]
	[CONSTANT sysex [~x [NATIVE_CODE #true] [@ studio . sysex : ~x]]]
	[CONSTANT sysexch [~x [NATIVE_CODE #true] [@ studio . sysexch : ~x]]]
	[CONSTANT SYSEX [~x [NATIVE_CODE #true] [@ studio . SYSEX : ~x]]]
	[CONSTANT SYSEXCH [~x [NATIVE_CODE #true] [@ studio . SYSEXCH : ~x]]]
	[CONSTANT timingclock [~x [NATIVE_CODE #true] [@ studio . timingclock : ~x]]]
	[CONSTANT START [~x [NATIVE_CODE #true] [@ studio . START : ~x]]]
	[CONSTANT CONTINUE [~x [NATIVE_CODE #true] [@ studio . CONTINUE : ~x]]]
	[CONSTANT STOP [~x [NATIVE_CODE #true] [@ studio . STOP : ~x]]]
	[CONSTANT attack [~x [NATIVE_CODE #true] [@ studio . attack : ~x]]]
	[CONSTANT release [~x [NATIVE_CODE #true] [@ studio . release : ~x]]]
	[CONSTANT cutoff [~x [NATIVE_CODE #true] [@ studio . cutoff : ~x]]]
	[CONSTANT resonance [~x [NATIVE_CODE #true] [@ studio . resonance : ~x]]]
	[CONSTANT mono [~x [NATIVE_CODE #true] [@ studio . mono : ~x]]]
	[CONSTANT poly [~x [NATIVE_CODE #true] [@ studio . poly : ~x]]]
	[CONSTANT bank [~x [NATIVE_CODE #true] [@ studio . bank : ~x]]]
	[CONSTANT banklsb [~x [NATIVE_CODE #true] [@ studio . banklsb : ~x]]]
	[CONSTANT portaon [~x [NATIVE_CODE #true] [@ studio . portaon : ~x]]]
	[CONSTANT portaoff [~x [NATIVE_CODE #true] [@ studio . portaoff : ~x]]]
	[CONSTANT portatime [~x [NATIVE_CODE #true] [@ studio . portatime : ~x]]]
	[CONSTANT volume [~x [NATIVE_CODE #true] [@ studio . volume : ~x]]]
	[CONSTANT reverb [~x [NATIVE_CODE #true] [@ studio . reverb : ~x]]]
	[CONSTANT chorus [~x [NATIVE_CODE #true] [@ studio . chorus : ~x]]]
	[CONSTANT foot [~x [NATIVE_CODE #true] [@ studio . foot : ~x]]]
	[CONSTANT breath [~x [NATIVE_CODE #true] [@ studio . breath : ~x]]]
	[CONSTANT pan [~x [NATIVE_CODE #true] [@ studio . pan : ~x]]]
	[CONSTANT modulation [~x [NATIVE_CODE #true] [@ studio . modulation : ~x]]]
	[CONSTANT holdon [~x [NATIVE_CODE #true] [@ studio . holdon : ~x]]]
	[CONSTANT holdoff [~x [NATIVE_CODE #true] [@ studio . holdoff : ~x]]]
	[CONSTANT CONTROL [~x [NATIVE_CODE #true] [@ studio . CONTROL : ~x]]]
	[CONSTANT ATTACK [~x [NATIVE_CODE #true] [@ studio . ATTACK : ~x]]]
	[CONSTANT RELEASE [~x [NATIVE_CODE #true] [@ studio . RELEASE : ~x]]]
	[CONSTANT CUTOFF [~x [NATIVE_CODE #true] [@ studio . CUTOFF : ~x]]]
	[CONSTANT RESONANCE [~x [NATIVE_CODE #true] [@ studio . RESONANCE : ~x]]]
	[CONSTANT PORTATIME [~x [NATIVE_CODE #true] [@ studio . PORTATIME : ~x]]]
	[CONSTANT VOLUME [~x [NATIVE_CODE #true] [@ studio . VOLUME : ~x]]]
	[CONSTANT REVERB [~x [NATIVE_CODE #true] [@ studio . REVERB : ~x]]]
	[CONSTANT CHORUS [~x [NATIVE_CODE #true] [@ studio . CHORUS : ~x]]]
	[CONSTANT FOOT [~x [NATIVE_CODE #true] [@ studio . FOOT : ~x]]]
	[CONSTANT BREATH [~x [NATIVE_CODE #true] [@ studio . BREATH : ~x]]]
	[CONSTANT PAN [~x [NATIVE_CODE #true] [@ studio . PAN : ~x]]]
	[CONSTANT MODULATION [~x [NATIVE_CODE #true] [@ studio . MODULATION : ~x]]]
	[CONSTANT nrpn [~x [NATIVE_CODE #true] [@ studio . nrpn : ~x]]]
	[CONSTANT rpn [~x [NATIVE_CODE #true] [@ studio . rpn : ~x]]]
	[CONSTANT NRPN [~x [NATIVE_CODE #true] [@ studio . NRPN : ~x]]]
	[CONSTANT RPN [~x [NATIVE_CODE #true] [@ studio . RPN : ~x]]]
	[CONSTANT hit [~x [NATIVE_CODE #true] [@ studio . hit : ~x]]]

;	[CONSTANT $$ [~x [NATIVE_CODE #true] [@ studio . $$ : ~x]]]
	]

[[l ~x] [@ studio . list ~x]]

[[equal?? ~e ~e #true] !]
[[equal?? ~ ~ #false]]
[[not? #false #true] !]
[[not? ~ #false]]
[[<? ~x ~y #true] [less ~x ~y] !]
[[<? ~x ~y #false]]
[[>? ~x ~y #true] [less ~y ~x] !]
[[>? ~x ~y #false]]
[[<=? ~x ~y #true] [less_eq ~x ~y] !]
[[<=? ~x ~y #false]]
[[>=? ~x ~y #true] [less_eq ~y ~x] !]
[[>=? ~x ~y #false]]

private [equal?? and? or? not?]

end := [[set_functional_captions] [auto_atoms] [kellner]] .
end := [[auto_atoms] [command]] .




		noteon noteoff
		income_midi keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex sysexch SYSEX SYSEXCH timingclock START CONTINUE STOP
		attack release cutoff resonance mono poly bank banklsb portaon portaoff portatime
		volume reverb chorus foot breath pan modulation holdon holdoff
		CONTROL ATTACK RELEASE CUTOFF RESONANCE PORTATIME
		VOLUME REVERB CHORUS FOOT BREATH PAN MODULATION
		nrpn rpn NRPN RPN hit
		midi_close midi_in_info midi_out_info midi_in_port midi_out_port midi_short_msg MIDI_SHORT_MSG midi_long_msg
