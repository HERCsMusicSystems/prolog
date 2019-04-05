
if (typeof module !== 'undefined') {
	studio = require ('./studio') . studio;
	prolog = require ('./prolog') . prolog;
}

studio . setResource (['test.prb'], `

[auto_atoms]

[batch "test_studio.prb"]
;[batch "test_io.prb"]
;[batch "test_midi.prb"]
;[batch "test_xml.prb"]
;[batch "test_json.prb"]

;[nl]
[FailedTestSummary]
;[nl]
;[foreground 0xffff00]

[exit]
`);
