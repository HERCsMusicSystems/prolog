
program jack #machine := 'jack' [
		activate
		keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex quarterframe songpositionpointer songselect uF4 uF5 tunerequest
		timingclock uF9 START CONTINUE STOP uFD activesensing systemreset
		income_midi
	]

#machine activate := 'activate'
#machine keyoff := 'keyoff'
#machine keyon := 'keyon'
#machine polyaftertouch := 'polyaftertouch'
#machine control := 'control'
#machine programchange := 'programchange'
#machine aftertouch := 'aftertouch'
#machine pitch := 'pitch'
#machine sysex := 'sysex'
#machine quarterframe := 'quarterframe'
#machine songpositionpointer := 'songpositionpointer'
#machine songselect := 'songselect'
#machine uF4 := 'uF4'
#machine uF5 := 'uF5'
#machine tunerequest := 'tunerequest'
#machine timingclock := 'timingclock'
#machine uF9 := 'uF9'
#machine START := 'START'
#machine CONTINUE := 'CONTINUE'
#machine STOP := 'STOP'
#machine uFD := 'uFD'
#machine activesensing := 'activesensing'
#machine systemreset := 'systemreset'

[[income_midi : *command] [show *command]]

end := [[activate 'HERCs' 'in' 'out'] [command]] .
