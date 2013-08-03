;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2013 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

program conductor #machine := "prolog.conductor"

	[
		conductor
		start pause stop wt beat bar signal_tick signal_beat signal_bar reset tempo division metrum accel rit atempo
	]

#machine start := "start"
#machine pause := "pause"
#machine stop := "stop"
#machine wt := "wt"
#machine beat := "beat"
#machine bar := "bar"
#machine signal_tick := "signal_tick"
#machine signal_beat := "signal_beat"
#machine signal_bar := "signal_bar"
#machine reset := "reset"
#machine tempo := "tempo"
#machine accel := "accel"
#machine rit := "rit"
#machine atempo := "atempo"
#machine division := "tempo_division"
#machine metrum := "metrum"
#machine conductor := "conductor"

end .

