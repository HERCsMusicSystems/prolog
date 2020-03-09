
program jack #machine := 'jack' [
		activate
		keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex
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

end := [[activate 'HERCs' 'in' 'out'] [command]] .
