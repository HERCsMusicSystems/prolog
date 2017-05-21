
import studio

program cas [
				apply exhaust trace
				rm0 rm1 rdb rdn srt ord dstr clt pwr
				distribute nror fterm
			]

[[rm0 [+ 0 : *t] [+ : *t]]]

[[rm1 [~ 1 : *t] [~ : *t]]]
[[rm1 [~ 1.0 : *t] [~ : *t]]]

[[rdb [+ [~ *x] : *t] [+ *x : *t]]]
[[rdb [~ [~ : *t1] : *t2] [~ : *t12]] [APPEND *t1 *t2 *t12]]
[[rdb [+ [+ : *t1] : *t2] [+ : *t12]] [APPEND *t1 *t2 *t12]]


[[srt [~ *y *x : *t] [~ *x *y : *t]] [is_number *x] [not is_number *y]]
[[srt [~ *y [*H : *T] : *t] [~ [*H : *T] *y : *t]] [is_atom *y]]
[[srt [~ *y *x : *t] [~ *x *y : *t]] [is_atom *x] [is_atom *y] [< *x *y]]

[[dstr [~ : *original] [~ : *processed]] [REPLACE [+ : *ht] *original *o2] [REPLACE *i *o2 *dstr *processed] [distribute *i [+ : *ht] *dstr]]

[[distribute *i [+] [+]]]
[[distribute *i [+ *x : *y] [+ [~ *i *x] : *yy]] [distribute *i [+ : *y] [+ : *yy]]]

[[nror *x] [is_number *x]]

[[rdn [+ : *original] [+ : *processed]] [REPLACE *i1 *original *o2] [nror *i] [REPLACE *i2 *o2 *i *processed] [nror *i2] [add *i *i1 *i2]]
[[rdn [~ *x *y : *t] [~ *z : *t]] [nror *x] [nror *y] [times *x *y *z]]

[[fterm *i *xt [+ : *A] [+ : *B]] [REPLACE [~ *i : *xt] *A *B] [nror *i]]
[[fterm 1 *xt [+ : *A] [+ : *B]] [REPLACE [~ : *xt] *A *B]]
[[fterm 1 [*x] [+ : *A] [+ : *B]] [REPLACE *x *A *B]]
[[fterm *i *xt [+ : *A] *new [+ : *B]] [REPLACE [~ *i : *xt] *A *new *B] [nror *i]]
[[fterm 1 *xt [+ : *A] *new [+ : *B]] [REPLACE [~ : *xt] *A *new *B]]
[[fterm 1 [*x] [+ : *A] *new [+ : *B]] [REPLACE *x *A *new *B]]

[[clt *a *b] [fterm *i1 *x *a *ab] [fterm *i2 *x *ab [~ *i : *x] *b] [sum *i1 *i2 *i]]

[[pwr [~ : *a] [~ : *b]] [REPLACE *x *a *ab] [REPLACE *x *ab [^ *x 2] *b]]
[[pwr [~ : *a] [~ : *b]] [REPLACE [^ *x *n] *a *ab] [nror *n] [REPLACE *x *ab [^ *x *nn] *b] [++ *n *nn]]


[[apply *before *after [*rule : *]] [*rule *before *after]]
[[apply [*op *left : *right] [*op *left : *next] *rules] [apply [*op : *right] [*op : *next] *rules]]
[[apply [*op *left : *right] [*op *next : *right] *rules] [apply *left *next *rules]]
[[apply *before *after [*rule : *rules]] [apply *before *after *rules]]

[[exhaust *before *after *rules] [apply *before *next *rules] / [exhaust *next *after *rules]]
[[exhaust *after *after *]]

[[trace *before *after [*rules : *more] [*trace : *traces]] [exhaust *before *trace *rules] / [trace *trace *after *more *traces]]
[[trace *after *after [] []]]

end := [[auto_atoms] [command]] .

