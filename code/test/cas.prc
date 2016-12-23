
import studio

program cas [
				apply exhaust trace
				rm0 rm1 rdb rdn srt ord
			]

[[rm0 [+ 0 : *t] [+ : *t]]]

[[rm1 [~ 1 : *t] [~ : *t]]]

[[rdb [~ *x] *x]]

;[[rdn [~ *x *y : *t] [~ *z : *t]] [number *x] [number *y] [times *x *y *z]]

[[srt [~ *y *x : *t] [~ *x *y : *t]] [is_number *x] [not is_number *y]]
[[srt [~ *y [*H : *T] : *t] [~ [*H : *T] *y : *t]] [is_atom *y]]


[[apply *before *after [*rule : *]] [*rule *before *after]]
[[apply [*op *left : *right] [*op *left : *next] *rules] [apply [*op : *right] [*op : *next] *rules]]
[[apply [*op *left : *right] [*op *next : *right] *rules] [apply *left *next *rules]]
[[apply *before *after [*rule : *rules]] [apply *before *after *rules]]

[[exhaust *before *after *rules] [apply *before *next *rules] / [exhaust *next *after *rules]]
[[exhaust *after *after *]]

[[trace *before *after [*rules : *more] [*trace : *traces]] [exhaust *before *trace *rules] / [trace *trace *after *more *traces]]
[[trace *after *after [] []]]

end := [[auto_atoms] [command]] .

