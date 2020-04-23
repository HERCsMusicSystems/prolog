
program jack #machine := 'jack' [
		activate
		keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex quarterframe songpositionpointer songselect uF4 uF5 tunerequest
		timingclock uF9 START CONTINUE STOP uFD activesensing systemreset
		rpn nrpn RPN NRPN CONTROL data DATA delta
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

[[rpn *channel *msb *lsb] [control *channel 101 *msb] [control *channel 100 *lsb]]
[[rpn *channel *msb *lsb *data] [control *channel 101 *msb] [control *channel 100 *lsb] [control *channel 6 *data]]
[[rpn *channel *msb *lsb *hh *ll] [control *channel 101 *msb] [control *channel 100 *lsb] [control *channel 38 *ll] [control *channel 6 *hh]]

[[nrpn *channel *msb *lsb] [control *channel 99 *msb] [control *channel 98 *lsb]]
[[nrpn *channel *msb *lsb *data] [control *channel 99 *msb] [control *channel 98 *lsb] [control *channel 6 *data]]
[[nrpn *channel *msb *lsb *hh *ll] [control *channel 99 *msb] [control *channel 98 *lsb] [control *channel 38 *ll] [control *channel 6 *hh]]

[[RPN *channel *msb *lsb *data] [>> *data 7 *hh] [& *data 127 *ll] [control *channel 101 *msb] [control *channel 100 *lsb] [control *channel 38 *ll] [control *channel 6 *hh]]
[[NRPN *channel *msb *lsb *data] [>> *data 7 *hh] [& *data 127 *ll] [control *channel 99 *msb] [control *channel 98 *lsb] [control *channel 38 *ll] [control *channel 6 *hh]]

[[data *channel *data] [control *channel 6 *data]]
[[data *channel *hh *ll] [control *channel 38 *ll] [control *channel 6 *hh]]
[[DATA *channel *data] [>> *data 7 *hh] [& *data 127 *ll] [control *channel 38 *ll] [control *channel 6 *hh]]
[[delta *channel *data] [< *data 0] / [sub 0 *data *mdata] [control *channel 97 *mdata]]
[[delta *channel *data] [control *channel 96 *data]]

[[CONTROL *channel *ctrl *data] [add *ctrl 32 *lsb] [>> *data 7 *hh] [& *data 127 *ll] [control *channel *lsb *ll] [control *channel *ctrl *hh]]

end := [[activate 'HERCs' 'in' 'out'] [command]] .
