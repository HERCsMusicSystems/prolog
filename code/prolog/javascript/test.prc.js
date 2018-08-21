
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
		[[eq *x *y] [Successes *name] [write "%cOK: %c" *name " = %c" [*y] / 'color: lime;' / 'color: yellow;' / 'color: lime;']]
		[[Failures *name] [exit 1] [write "%cFAILED: %c" *name " expected %c" [*x] " %cbut was %c" [*y] / 'color: red;' / 'color: yellow;' / 'color: lime;' / 'color: yellow;' / 'color: red;']]
	]
]

[[TestEq *name : *] [Failures *name] [exit 1] [write "%cEQ FAILED: %c" *name / 'color: red;' / 'color: yellow;']]

[[TestWorks *name : *action]
	[SELECT
		[[res : *action] [Successes *name] [write "%cOK: %c" *name " %cworked." / 'color: lime;' / 'color: yellow;' / 'color: lime;']]
		[[Failures *name] [exit 1] [write "%cFAILED: %c" *name " %cfailed." / 'color: red;' / 'color: yellow;' / 'color: red;']]
	]
]

[[TestFails *name : *action]
	[SELECT
		[[res : *action] [Failures *name] [exit 1] [write "%cFAILED: %c" *name " %cworked." / 'color: red;' / 'color: yellow;' / 'color: red;']]
		[[Successes *name] [write "%cOK: %c" *name " %cfailed." / 'color: lime;' / 'color: yellow;' / 'color: lime;']]
	]
]

[[TestSummary]
	[write "%cTest Summary:" / 'color: yellow;']
	[Successes : *s] [LENGTH *s *length_s] [Failures : *f] [LENGTH *f *length_f]
	[write '%c' [*length_s] " passed." / 'color: lime;'] [write '%c' [*s] / 'color: lime;']
	[write '%c' [*length_f] " failed." / 'color: red;'] [write '%c' [*f] / 'color: red;']
	[Timing : *from] [timestamp *to : *] [sum *from *time *to] [div *time 1000.0 *timed] [write "TIME = " [*timed] " seconds."]
	[ResetCounters]
]

[[FailedTestSummary]
	[Failures : *f] [LENGTH *f *length_f]
	[write '%c' [*length_f] " failed." / 'color: red;'] [write '%c' [*f] / 'color: red;']
	[Timing : *from] [timestamp *to : *] [sum *from *time *to] [div *time 1000.0 *timed] [write "%cTIME = " [*timed] " seconds." / 'color: yellow;']
	[ResetCounters]
]

private [Successes Failures]
protect [TestWorks TestFails TestEq TestNotEq TestSummary FailedTestSummary Failures Successes ResetCounters]

end .
`);
