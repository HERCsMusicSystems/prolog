
import studio

program test [TestWorks TestFails TestEq TestNotEq TestSummary Failures Successes ResetCounters]

[[ResetCounters]
	[TRY [has_machine Successes] [Successes]]
	[TRY [has_machine Failures] [Failures]]
	[ACCUMULATOR Successes] [ACCUMULATOR Failures]
]

auto := [[ResetCounters]]

[[TestEq *name *x *y : *action]
	[ONE : *action]
	[SELECT
		[[eq *x *y] [Successes *name] [foreground 0xff00] [write "OK: "] [foreground 0xffff00] [write *name " = " ] [foreground 0xff00] [pp *y] [nl]]
		[[Failures *name] [foreground 0xff0000] [write "FAILED: "] [foreground 0xffff00] [write *name " expected "] [foreground 0xff00] [pp *x] [foreground 0xffff00] [write " but was "] [foreground 0xff0000] [pp *y] [nl]]
	]
]

[[TestEq *name : *] [Failures *name] [foreground 0xff0000] [write "EQ FAILED: "] [foreground 0xffff00] [write *name] [nl]]

[[TestWorks *name : *action]
	[SELECT
		[[res : *action] [Successes *name] [foreground 0xff00] [write "OK: "] [foreground 0xffff00] [write *name] [foreground 0xff00] [write " worked."] [nl]]
		[[Failures *name] [foreground 0xff0000] [write "FAILED: "] [foreground 0xffff00] [write *name] [foreground 0xff0000] [write " failed."] [nl]]
	]
]

[[TestFails *name : *action]
	[SELECT
		[[res : *action] [Failures *name] [foreground 0xff0000] [write "FAILED: "] [foreground 0xffff00] [write *name] [foreground 0xff0000] [write " worked."] [nl]]
		[[Successes *name] [foreground 0xff00] [write "OK: "] [foreground 0xffff00] [write *name] [foreground 0xff00] [write " failed."] [nl]]
	]
]

[[TestSummary]
	[foreground 0xffff00]
	[show "Test Summary:"]
	[Successes : *s] [Failures : *f]
	[foreground 0xff00] [show *s]
	[foreground 0xff0000] [show *f]
	[foreground 0xffff00]
	[ResetCounters]
]

end .
