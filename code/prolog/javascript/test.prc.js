
studio . setResource (['test.prc'], `
import studio

program test [TestWorks TestFails TestEq TestNotEq TestSummary FailedTestSummary Failures Successes Timing ResetCounters foreground nl show]

[[ResetCounters]
	[TRY [machine? Successes] [Successes]]
	[TRY [machine? Failures] [Failures]]
	[TRY [machine? Timing] [Timing]]
	[ACCUMULATOR Successes] [ACCUMULATOR Failures] [VARIABLE Timing] [timestamp *time : *] [Timing *time]
]

auto := [[ResetCounters]]

[[TestEq *name *x *y : *action]
	[ONE : *action]
	[SELECT
		[[eq *x *y] [Successes *name] [write "%cOK: %c" *name " = %c" [*y] / 'color: #00ff00;' / 'color: #ffff00;' / 'color: #00ff00;']]
		[[Failures *name] [exit 1] [write "%cFAILED: %c" *name " expected %c" [*x] " %cbut was %c" [*y] / 'color: #ff0000;' / 'color: #ffff00;' / 'color: #00ff00;' / 'color: #ffff00;' / 'color: #ff0000;']]
	]
]

[[TestEq *name : *] [Failures *name] [exit 1] [write "%cEQ FAILED: %c" *name / 'color: #ff0000;' / 'color: #ffff00;']]

[[TestWorks *name : *action]
	[SELECT
		[[res : *action] [Successes *name] [write "%cOK: %c" *name " %cworked." / 'color: #00ff00;' / 'color: #ffff00;' / 'color: #00ff00;']]
		[[Failures *name] [exit 1] [write "%cFAILED: %c" *name " %cfailed." / 'color: #ff0000;' / 'color: #ffff00;' / 'color: #ff0000;']]
	]
]

[[TestFails *name : *action]
	[SELECT
		[[res : *action] [Failures *name] [exit 1] [write "%cFAILED: %c" *name " %cworked." / 'color: #ff0000;' / 'color: #ffff00;' / 'color: #ff0000;']]
		[[Successes *name] [write "%cOK: %c" *name " %cfailed." / 'color: #00ff00;' / 'color: #ffff00;' / 'color: #00ff00;']]
	]
]

[[TestSummary]
	[foreground 0xffff00]
	[show "Test Summary:"]
	[Successes : *s] [LENGTH *s *length_s] [Failures : *f] [LENGTH *f *length_f]
	[foreground 0xff00] [show *length_s " passed"] [show *s]
	[foreground 0xff0000] [show *length_f " failed"] [show *f]
	[foreground 0xffff00]
	[Timing : *from] [timestamp *to : *] [sum *from *time *to] [div *time 1000.0 *timed] [show "TIME = " *timed " seconds."]
	[ResetCounters]
]

[[FailedTestSummary]
	[Failures : *f] [LENGTH *f *length_f]
	[foreground 0xff0000] [show *length_f " failed"] [show *f]
	[foreground 0xffff00]
	[Timing : *from] [timestamp *to : *] [sum *from *time *to] [div *time 1000.0 *timed] [show "TIME = " *timed " seconds."]
	[ResetCounters]
]

private [Successes Failures]
protect [TestWorks TestFails TestEq TestNotEq TestSummary FailedTestSummary Failures Successes ResetCounters]

end .
`);
