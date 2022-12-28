;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2002 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


program studio #machine := "prolog.studio"


	[
		set_uap32_captions set_standard_captions set_edinburg_captions
		set_marseille_captions set_functional_captions set_mathematical_captions
		auto_atoms scripted_atoms
		pr read readln
		write nl pp pt list
		file_writer import load consult batch run file_reader bootstrap shebang_reader reload
		remove_module create_module set_machine machine_type protect private protect? private?
		add_search_directory search_directories environment expand_search_directory
		cd relativise_path dir ls DIR ARGS args args_tail edit execute make_directory erase erase_directory move copy
		operating_system implementation version root
		CL cl addcl addcl0 DELCL OVERWRITE delcl delallcl lambda overwrite let
		create_atom create_atoms search_atom search_atom_c unique_atoms
		preprocessor prompt QueryPrompt ReplyPrompt FailPrompt
		+ - ++ -- ~ ~~ ~+ `*` % < = > <=> <= =< >= => <> << >> ! & | ^ := true false null nil
		::= toSymbolicForm addSymbolicForm multiplySymbolicForm sortSymbolicForm
		add add1 sub sub1 mult div mod and or xor shiftl shiftr sum times mac less less_eq greater greater_eq max min
		abs cos degrad e exp log2 log10 ln log pi pow sin tan trunc DFT FFT
		StringToLower StringToUpper StringReplaceOnce StringReplaceAll StringSplit StringSplitOnce
		timestamp
		is_atom is_integer is_double is_number is_var is_head is_text has_machine
		atom? int? integer? double? number? var? head? text? machine?
		text_list text_term e32 query_stack object_counter
		exit halt command save_history load_history inner inner_addcl inner_call minimise maximise
		res not eq not_eq rres ures lazy random_cl explode
		TRY ONE PROBE SELECT APPEND LENGTH REVERSE AT ONLIST INLIST NODUP MAP MEMBER REPLACE
		sort divide
		WHILE FOR FOREVER forever REPEAT IF
		CONSTANT VARIABLE ACCUMULATOR STACK QUEUE ARRAY INDEX var inc dec ALL all ISALL isall isallr isallq
		rnd rnd_control grnd series random grandom
		crack wait timeout enter signal semaphore msemaphore mutex accept select critical_section monitor task ThreadParameters
		background foreground open_editor close_editor screen_coordinates
		get_volume_serial_number security_check encoder
		SerialNumber VolumeSerialNumber SerialKey VolumeID SerialShift GenerateSerial ReadMachineVolumeID
		. computer-do-this define-this show
	]

[[version 2016 4]]
[[version] [version : *x] [show "VERSION: " *x]]

#machine ++ := "add1"
#machine add1 := "add1"
#machine -- := "sub1"
#machine sub1 := "sub1"
#machine sum := "sum"
#machine + := "add"
#machine add := "add"
#machine - := "sub"
#machine sub := "sub"
#machine times := "times"
#machine ~+ := 'mac'
#machine mac := "mac"
#machine ~ := "mult"
#machine `*` := "mult"
#machine mult := "mult"
#machine ~~ := "div"
#machine div := "div"
#machine mod := "mod"
#machine % := "mod"
#machine and := "and"
#machine & := "and"
#machine or := "or"
#machine | := "or"
#machine xor := "xor"
#machine ^ := "xor"
#machine shiftl := "shiftl"
#machine << := "shiftl"
#machine shiftr := "shiftr"
#machine >> := "shiftr"

#machine abs := "abs"
#machine cos := "cos"
#machine degrad := "degrad"
#machine e := "e"
#machine exp := "exp"
#machine log2 := "log2"
#machine log10 := "log10"
#machine ln := "ln"
#machine log := "log"
#machine pi := "pi"
#machine pow := "pow"
#machine sin := "sin"
#machine tan := "tan"
#machine trunc := "trunc"

#machine StringToLower := "StringToLower"
#machine StringToUpper := "StringToUpper"
#machine StringReplaceOnce := "StringReplaceOnce"
#machine StringReplaceAll := "StringReplaceAll"
#machine StringSplit :="StringSplit"
#machine StringSplitOnce := "StringSplitOnce"

#machine DFT := "DFT"
#machine FFT := "FFT"

#machine timestamp := "timestamp"

#machine is_atom := "is_atom"
#machine is_integer := "is_integer"
#machine is_double := "is_double"
#machine is_number := "is_number"
#machine is_var := "is_var"
#machine is_head := "is_head"
#machine is_text := "is_text"
#machine has_machine := "has_machine"
#machine atom? := "is_atom"
#machine int? := "is_integer"
#machine integer? := "is_integer"
#machine double? := "is_double"
#machine number? := "is_number"
#machine var? := "is_var"
#machine head? := "is_head"
#machine text? := "is_text"
#machine machine? := "has_machine"
#machine text_list := "text_list"
#machine text_term := "text_term"
#machine e32 := "e32"
#machine less := "less"
#machine less_eq := "less_eq"
#machine greater := "greater"
#machine greater_eq := "greater_eq"
#machine < := "less"
#machine <= := "less_eq"
#machine =< := "less_eq"
#machine > := "greater"
#machine >= := "greater_eq"
#machine => := "greater_eq"
#machine max := "max"
#machine min := "min"
#machine set_standard_captions := "set_standard_captions"
#machine set_uap32_captions := "set_uap32_captions"
#machine set_edinburg_captions := "set_edinburg_captions"
#machine set_marseille_captions := "set_marseille_captions"
#machine set_functional_captions := "set_functional_captions"
#machine set_mathematical_captions := "set_mathematical_captions"
#machine auto_atoms := "auto_atoms"
#machine scripted_atoms := "scripted_atoms"
#machine pr := "pr"
#machine read := "read"
#machine readln := "readln"
#machine pp := "pp"
#machine pt := "pt"
#machine write := "write"
#machine nl := "nl"
#machine list := "list"
#machine CL := "CL"
#machine addcl := "addcl"
#machine addcl0 := "addcl0"
#machine DELCL := "delcl"
#machine OVERWRITE := "overwrite"
#machine create_atom := "create_atom"
#machine create_atoms := "create_atoms"
#machine search_atom := "search_atom"
#machine search_atom_c := "search_atom_c"
#machine unique_atoms := "unique_atoms"
#machine preprocessor := "preprocessor"
#machine prompt := "prompt"
#machine query_stack := "query_stack"
#machine object_counter := "object_counter"
#machine crack := "crack"
#machine ThreadParameters := "ThreadParameters"
#machine wait := "wait"
#machine timeout := "timeout"
#machine semaphore := "semaphore"
#machine msemaphore := "msemaphore"
#machine mutex := "mutex"
#machine rnd := "rnd"
#machine rnd_control := "rnd_control"
#machine random := "random"
#machine series := "series"

#machine file_writer := "file_writer"
#machine file_reader := "file_reader"
#machine shebang_reader := "shebang_reader"
#machine import := "import_loader"
#machine load := "load_loader"
#machine consult := "consult_loader"
#machine remove_module := "remove_module"
#machine create_module := "create_module"
#machine set_machine := "set_machine"
#machine machine_type := "machine_type"
#machine private := "private"
#machine protect := "protect"
#machine private? := "private?"
#machine protect? := "protect?"
#machine add_search_directory := "add_search_directory"
#machine search_directories := "search_directories"
#machine environment := "environment"
#machine cd := "cd"
#machine relativise_path := "relativise_path"
#machine DIR := "DIR"
#machine ARGS := "ARGS"
#machine edit := "edit"
#machine make_directory := "make_directory"
#machine erase := "erase"
#machine erase_directory := "erase_directory"
#machine move := "move"
#machine copy := "copy"
#machine execute := "execute"
#machine exit := "exit_code"
#machine halt := "halt_code"
#machine root := "root_code"
#machine operating_system := "operating_system"
#machine implementation := "implementation"
#machine save_history := "save_history"
#machine load_history := "load_history"

#machine CONSTANT := "CONSTANT"
#machine VARIABLE := "VARIABLE"
#machine ACCUMULATOR := "ACCUMULATOR"
#machine STACK := "STACK"
#machine QUEUE := "QUEUE"
#machine ARRAY := "ARRAY"
#machine INDEX := "INDEX"

#machine background := "background"
#machine foreground := "foreground"
#machine open_editor := "open_editor"
#machine close_editor := "close_editor"
#machine screen_coordinates := "screen_coordinates"

#machine get_volume_serial_number := "get_volume_serial_number"
#machine security_check := "security_check"
#machine encoder := "encoder"
#machine SerialNumber := "SerialNumber"
#machine VolumeSerialNumber := "get_volume_serial_number"
#machine SerialKey := "SerialKey"
#machine VolumeID := "VolumeID"
#machine SerialShift := "SerialShift"
#machine ReadMachineVolumeID := "ReadMachineVolumeID"

[[grnd : *parameters] [rnd : *parameters]]
[[grnd : *parameters] / [grnd : *parameters]]
[[grandom : *parameters] [random : *parameters]]
[[grandom : *parameters] / [grandom : *parameters]]
[[eq *x *x]]
[[= *x *x]]
[[not_eq *x *x] / fail]
[[not_eq * *]]
[[<> *x *x] / fail]
[[<> * *]]
[[! : *x] *x / fail]
[[! : *]]
[[not :*x] *x / fail]
[[not :*x]]
[[res :*x]:*x]
[[ONE :*o][res :*o]/]
[[ALL :*o][res : *o] fail]
[[ALL :*]]
[[all : *o][res : *o][ReplyPrompt : *prompt][write *prompt][show *o][show "More?"][readln *s][StringToUpper *s *S][ONLIST *S ["N" "NO"]]/[show "No (more) answers."]]
[[all :*][show "No (more) answers."]]
[[TRY :*o]:*o]
[[TRY :*o]]
[[PROBE :*o][ONE :*o] fail]
[[PROBE :*o]]
[[SELECT] / fail]
[[SELECT *branch : *] : *branch]
[[SELECT * : *branches] / [SELECT : *branches]]
[[APPEND [] *l *l]]
[[APPEND [*head : *tail] *l [*head : *new]]/
	[APPEND *tail *l *new]]
[[REVERSE *l1 *l2] [REVERSE *l1 [] *l2]]
[[REVERSE [] *x *x]]
[[REVERSE [*head : *tail] *l0 *list] [REVERSE *tail [*head : *l0] *list]]
[[LENGTH [] 0]]
[[LENGTH [*head : *tail] *length] [LENGTH *tail *l] [sum *l 1 *length]]
[[AT *from *from *e [*e : *]]]
[[AT *from *to *e [* : *list]] [++ *from *next] / [AT *next *to *e *list]]
[[AT *index *element *list] / [AT 0 *index *element *list]]
[[ONLIST *x [*x : *]]]
[[ONLIST *x [* : *l]] [ONLIST *x *l]]
[[INLIST *l *x [*x : *l]]]
[[INLIST [*h : *l] *x [*h : *ll]] [INLIST *l *x *ll]]
[[NODUP [] []]/]
[[NODUP [*x : *t] *result] [ONLIST *x *t] / [NODUP *t *result]]
[[NODUP [*x : *t] [*x : *result]] / [NODUP *t *result]]
[[MAP [] [] []]]
[[MAP [[*x *y] : *xyt] [*x : *xt] [*y : *yt]] / [MAP *xyt *xt *yt]]
[[MEMBER *x [*x : *]]]
[[MEMBER *x [* : *l]] [MEMBER *x *l]]
[[REPLACE *x [*x : *l] *l]]
[[REPLACE *x [*h : *l] [*h : *ll]] [REPLACE *x *l *ll]]
[[REPLACE *x [*x : *l] *y [*y : *l]]]
[[REPLACE *x [*h : *l] *y [*h : *ll]] [REPLACE *x *l *y *ll]]
[[WHILE *condition : *call] [not not : *condition] / [PROBE : *call] / [WHILE *condition : *call]]
[[WHILE : *]]
[[FOREVER : *instructions] [PROBE : *instructions] / [FOREVER : *instructions]]
[[forever : *instructions] [res : *instructions] / [forever : *instructions]]
[[REPEAT *ind : *instructions] [less 0 *ind] [PROBE : *instructions] [sub1 *ind *next] / [REPEAT *next : *instructions]]
[[REPEAT : *]]
[[FOR * [] : *] /]
[[FOR *head [*head : *] : *call] [ONE : *call] fail]
[[FOR *head [* : *tail] : *call] / [FOR *head *tail : *call]]
[[FOR *index *index *index *step : *call] [TRY : *call]/]
[[FOR *index *index *to *step : *call] [ONE : *call] fail]
[[FOR *index *from *to *step : *call]
	[add *from *step *next] /
	[FOR *index *next *to *step : *call]]
[[IF *condition *then] *condition / [TRY *then] /]
[[IF *condition *then] /]
[[IF *condition *then *else] *condition / [TRY *then] /]
[[IF *condition *then *else] [TRY *else] /]
[[var]]
[[var *var : *vars] [is_var *var] / [VARIABLE *var] / [var : *vars]]
[[var [*var *value] : *vars] / [VARIABLE *var *value] / [var : *vars]]
[[var *var : *vars] [VARIABLE *var] / [var : *vars]]
[[inc *var] [*var : *value] [add *value 1 *new] [*var *new]]
[[inc *var *inc] [*var : *value] [add *value *inc *new] [*var *new]]
[[dec *var] [*var : *value] [sub *value 1 *new] [*var *new]]
[[dec *var *dec] [*var : *value] [sub *value *dec *new] [*var *new]]
[[ISALL *atom *template : *call]
	[res : *call]
	[*atom *template] fail]
[[ISALL :*]]
[[isall *list *template : *call]
	[ACCUMULATOR *atom]
	[ISALL *atom *template : *call]
	[*atom : *list] /]
[[isallr *list *template : *call]
	[ACCUMULATOR *atom]
	[ISALL *atom *template : *call]
	[*atom : *reversed_list]
	[REVERSE *reversed_list *list] /]
[[isallq *list *template : *call]
	[QUEUE *atom]
	[ISALL *atom *template : *call]
	[*atom : *list] /]

[[minimise *x *from *to] / [minimise *x *from *to 1]]
[[minimise *to *to *to *step]/]
[[minimise *from *from *to *step]]
[[minimise *x *from *to *step] [less *from *to] [add *from *step *next] / [minimise *x *next *to *step]]

[[maximise *x *from *to] / [maximise *x *from *to 1]]
[[maximise *from *from *from *step]/]
[[maximise *to *from *to *step]]
[[maximise *x *from *to *step] [less *from *to] [sub *to *step *next] / [maximise *x *from *next *step]]

[[lazy]]
[[lazy : *commands]
	[INLIST *next *command *commands]
	*command
	[lazy : *next]
]
[[rres [*atom : *parameters]]
	[CL *atom *clauses]
	[series 0 *clauses 1 *serie]
	/ [random_cl *serie *atom : *parameters]
]
[[ures [*atom : *parameters]]
	[isall *probabilities *ind [cl [[*atom *ind :*] :*]]]
	[LENGTH *probabilities *length]
	[sub *length 1 *next]
	[explode *probabilities *next *full_list]
	[series *full_list *permutated]
	/ [random_cl *permutated *atom : *parameters]
]
[[random_cl [*h : *t] *atom : *parameters]
	[CL *h *atom [[*atom : *parameters] : *body]]
	: *body
]
[[random_cl [* : *t] : *call] / [random_cl *t : *call]]
[[explode [] * []]/]
[[explode [*head : *tail] *ind [*ind : *subtail]]
	[less 0 *head]
	/ [sub *head 1 *next] / [explode [*next : *tail] *ind *subtail]
]
[[explode [* : *tail] *ind *subtail]
	[sub *ind 1 *next]
	/ [explode *tail *next *subtail]
]

[[divide *less *x [] *lt *lt *gt *gt]/]
[[divide *less *x [*h : *t] *lt *lta [*h : *gt] *gta] [*less *x *h] / [divide *less *x *t *lt *lta *gt *gta]]
[[divide *less *x [*h : *t] [*h : *lt] *lta *gt *gta] / [divide *less *x *t *lt *lta *gt *gta]]

[[sort *less [] *gs *gs]/]
[[sort *less [*h : *t] *gts *gtsa]
	[divide *less *h *t *lt *lta *gt *gta]/
	[sort *less *lt *gts [*h : *gs]]/
	[sort *less *gt *gs *gtsa]/
]
[[sort *in *out] / [sort less *in *out []]]

[[<=> *in *out] / [<=> < *in *out []]]
[[<=> *less [] *gs *gs]/]
[[<=> *less [*h : *t] *gts *gtsa]
	[divide *less *h *t *lt *lta *gt *gta] /
	[<=> *less *lt *gts [*h : *gs]] /
	[<=> *less *gt *gs *gtsa] /
]

[[cl *x]/[cl 0 *y *x]]
[[cl *x *y]/[cl 0 *x *y]]
[[cl *x *x [[*a:*b]:*c]][CL *x *a [[*a:*b]:*c]]]
[[cl *x *y [[*a:*b]:*c]][add *x 1 *x2]/[CL *x2 *a *X]
	[cl *x2 *y [[*a:*b]:*c]]]

[[lambda *atom *parameters : *body] [create_atom *atom] [addcl [[*atom : *parameters] : *body]]]
[[define-this : *x] [addcl *x]]
[[show] [nl] /]
[[show *text : *show] [is_text *text] [write *text] / [show : *show]]
[[show *x : *show] [pp *x] / [show : *show]]

[[delcl [[*a:*b]:*c]][cl *x [[*a:*b]:*c]][DELCL *x *a]]

[[delallcl *a] [DELCL 0 *a] / [delallcl *a]]
[[delallcl *a]]

[[overwrite [[*a:*b]:*c] [[*a:*h]:*i]][cl *x [[*a:*b]:*c]]
	[OVERWRITE *x [[*a:*h]:*i]]]
[[let *name *selector : *value]
	[overwrite [[*name *selector : *] : *] [[*name *selector : *value]]]
]
[[let *name *selector : *value]
	[addcl [[*name *selector : *value]]]
]


; multithreading and synchronization

[[monitor *atom]
	[create_atom *atom]
	[mutex *m]
	[addcl [[*atom *mode : *commands] [*m *mode] [TRY : *commands] [*m signal] [wait 0]]]
	]

[[critical_section [*atom : *parameters] : *commands]
	[create_atom *atom]
	[mutex *m]
	[addcl [[*atom : *parameters] [*m wait] [TRY : *commands] [*m signal] [wait 0]]]
	]

;;;;;;;;;;;;;;
; RENDEZVOUS ;
;;;;;;;;;;;;;;
; THREAD: .... [*task enter atom : *parameters]
; TASK:   .... [*task accept atom [*parameters:...] : *body]
;     OR: .... [SELECT
;                      [.... conditions ..... [*task select atom [*parameters:...] : *body] ..... body .... ]
;                      [.... conditions ..... [*task select sonda [*parameters:...] : *body] ..... body .... ]
;                      [.... conditions ..... [*task select extra [*parameters:...] : *body] ..... body .... ]
;                      [[*task wait]]     ; this prevents active pooling by a waiting for a semaphore activated by either [*task enter....] or [*task signal]
;              ]
;;;;;;;;;;;;;;

[[task accept *atom *trigger []]]
[[task accept *atom *trigger [*entry : *entries]]
	[semaphore *enter 0] [semaphore *accept 0] [semaphore *complete 0]
	[var *data]
	[addcl [[*atom accept *entry : *body]
				[*enter wait]                                  ; wait for the enter trigger
				[*data *body]                                  ; transfer parameters and body
				[*accept signal]                               ; indicate transfer complete
				[*complete wait]                               ; wait for completion on the other side
			]
	]
	[addcl [[*atom enter *entry : *parameters]
				[*enter signal]                                ; trigger entry
				[*trigger signal]                              ; just in case if the task is suspended on default wait
				[*accept wait]                                 ; wait for parameters and body
				[*data : *d]                                   ; get them
				[TRY [eq [*parameters : *body] *d] : *body]    ; and perform
				[*complete signal]                             ; indicate the completion
			]
	]
	[addcl [[*atom select *entry : *body]
				[*enter enter]                                 ; main trigger has already been passed, so just wait for the particular trigger
				[*data *body]                                  ; transfer parameters and body
				[*accept signal]                               ; indicate transfer complete
				[*complete wait]                               ; wait for completion on the other side
			]
	]
	/ [task accept *atom *trigger *entries]
]
[[task *atom *entries : *commands]
	[create_atom *atom] [semaphore *trigger 0]
	[task accept *atom *trigger *entries]
	[addcl [[*atom wait]   [*trigger wait]]   ]                ; stop actively pooling until trigger
	[addcl [[*atom signal] [*trigger signal]] ]                ; re-activate active pooling
	[addcl [[*atom]        [delallcl *atom]]  ]                ; remove all task definitions
	[crack : *commands]
]

; command processor

;[[inner [[*atom : *] : *] []]]
[[inner [* : *] []]]
[[inner *x *y] [pr *y]]
[[inner [*x *y]] [pr *x] [inner *x *y] [TRY [eq exit *x] [save_history] [show 'Command history saved.']] /]
[[inner [*x *y]] [readln *x] fail]

[[inner_addcl [[*atom : *t1] : *t2]]
	[preprocessor *preprocessor]
	[is_atom *preprocessor]
	/ [*preprocessor *t2 *t3] [addcl [[*atom : *t1] : *t3]]]
[[inner_addcl *cl] [addcl *cl]]

[[inner_call *command] [preprocessor *preprocessor] [is_atom *preprocessor] / [*preprocessor [*command] *c] : *c]
[[inner_call *command] *command]

[[command [exit : *]]]
[[command [[exit : *]]]]
[[command *x] [QueryPrompt : *prompt] [write *prompt] [inner [*y *z]] [command *y *z] / [command [*y : *z]]]
[[command *x] [FailPrompt : *prompt] [write *prompt] [nl] / [command *x]]
[[command] [TRY [load_history]] [command [command]]]
[[command [[*atom : *t1] : *t2] []] [inner_addcl [[*atom : *t1] :*t2]]]
[[command [[*atom : *t1] : *t2] []] [write "Can not add clause: " [[[*atom : *t1] : *t2]] "\n"] / fail]
[[command [*x : *y] []] / [inner_call [*x : *y]] [ReplyPrompt : *prompt] [write *prompt] [show [*x : *y]]]
[[command *x *y] [inner_call [*x : *y]]]

auto := [[var QueryPrompt ReplyPrompt [FailPrompt "Doesn't work"]]]

[[bootstrap *command] [shebang_reader *atom *command] / [batch [batch] *atom]]
[[bootstrap *command] [import *command : *main] / : *main]

[[batch *text] [file_reader *atom *text] / [batch [batch] *atom]]
[[batch [exit] *reader][*reader]]
[[batch *x *reader][*reader *head]/[batch *x *reader *head]]
[[batch *x *reader][*reader] / fail]
[[batch *x *reader *head] [eq *head [[* : *] : *]] / [inner_addcl *head] / [batch *head *reader]]
[[batch *x *reader *head] [is_atom *head] / [*reader *tail] [inner_call [*head : *tail]] / [batch [*head : *tail] *reader]]
[[batch *x *reader *head] / [inner_call *head] / [batch *head *reader]]

[[run : *batch] [batch : *batch]]

[[reload] [list :*x] [eq *x [*user *loader :*tail]] [add *loader ".prc" *name] [load *name]]

[[dir] / [DIR "*.*" : *files] [dir : *files]]
[[dir *location] [is_text *location] / [DIR *location : *files] [dir : *files]]
[[dir *result] [is_var *result] / [DIR "*.*" : *result]]
[[dir *location *result] [is_text *location] [is_var *result] / [DIR *location : *result]]
[[dir [*file : *files]] [write "	" *file] [nl] / [dir *files]]
[[dir []] /]
[[dir *directories *files]
	[write "Directories:\n"]
	[dir *directories]
	[write "Files:\n"]
	[dir *files]
	]
[[ls : *command] / [dir : *command]]

[[args : *args] [ARGS : *texts] / [args_tail *texts *args]]
[[args_tail [] []] /]
[[args_tail [*t1 : *t] *a] [text_term *t1 : *b] [APPEND *b *c *a] / [args_tail *t *c]]

[[expand_search_directory *]]
[[expand_search_directory *var *extension : *extensions]
    [environment *var *varv]
    [add *varv *extension *path]
    [add_search_directory *path]
    [expand_search_directory *var : *extensions]
]

[[GenerateSerial *head *serial *key] [GenerateSerial *head 16 *serial *key]]
[[GenerateSerial *head 0 *serial *key] [+ *head '-??' *number] [encoder *number *serial *key]]
[[GenerateSerial *head *ind *serial *key]
	[series [0 1 2 3 4 5 6 7 8 9 'A' 'B' 'C' 'D' 'E' 'F'] [*A : *]]
	[+ *head *A *new]
	[-- *ind *next]
	/ [GenerateSerial *new *next *serial *key]
]

[[:= *x *x] [is_number *x] /]
[[:= *x *c] [is_atom *c] / [*c *x]]
[[:= *x [*f *arg]] [is_atom *f] [:= *ff *arg] [*f *ff *x]]
[[:= *x [*f *a *b]] [is_atom *f] [:= *aa *a] [:= *bb *b] [*f *aa *bb *x]]
[[:= *x [*a : *b]] / [:= *x *a : *b] /]
[[:= *x *a ^ *b : *c] / [:= *aa *a] [:= *bb *b] [pow *aa *bb *aaa] / [:= *x *aaa : *c]]
[[:= *x *a / *b : *c] / [:= *aa *a] [:= *bb *b] [div *aa *bb *aaa] / [:= *x *aaa : *c]]
[[:= *x *a - : *b] / [:= *x *a + -1 : *b] /]
[[:= *x *a + : *b] / [:= *aa *a] [:= *bb *b] [add *x *aa *bb]]
[[:= *x *a *b ^ *n : *c] / [:= *bb *b] [:= *nn *n] [pow *bb *nn *bbb] / [:= *x *a *bbb : *c]]
[[:= *x *a *b : *c] / [:= *aa *a] [:= *bb *b] [mult *aaa *aa *bb] / [:= *x *aaa : *c] /]

[[toSymbolicForm *number [+ [*number]]] [is_number *number] /]
[[toSymbolicForm *atom [+ [1 *atom]]] [is_atom *atom] /]
[[toSymbolicForm [+ : *T] [+ : *T]] /]
[[toSymbolicForm *a *ex] [::= *ex : *a]]

[[sortSymbolicForm *l1 *l2] [LENGTH *l1 *ll1] [LENGTH *l2 *ll2] [>= *ll1 *ll2]]

[[addSymbolicForm [+] *x *x] /]
[[addSymbolicForm [+ *A : *At] *B [+ : *SSF]] / [addSymbolicForm *A *B *AB] [addSymbolicForm [+ : *At] *AB [+ : *SF]] [<=> sortSymbolicForm *SF *SSF []]]
[[addSymbolicForm [0 : *] [+] [+]] /]
[[addSymbolicForm *x [+] [+ *x]] /]
[[addSymbolicForm [*A : *T] [+ [*B : *T] : *Ts] [+ : *Tx]] [is_number *A] [is_number *B] / [add *A *B *AB] [addSymbolicForm [*AB : *T] [+ : *Ts] [+ : *Tx]]]
[[addSymbolicForm *A [+ *B : *Ts] [+ *B : *Tx]] / [addSymbolicForm *A [+ : *Ts] [+ : *Tx]]]
[[addSymbolicForm [*A : *T] [*B : *T] [+ [*AB : *T]]] [is_number *A] [is_number *B] / [add *A *B *AB]]
[[addSymbolicForm *a *b [+ *a *b]]]

[[multiplySymbolicForm [+] * [+]] /]
[[multiplySymbolicForm [+ *A : *At] *B *C] / [multiplySymbolicForm *A *B *AB] [multiplySymbolicForm [+ : *At] *B *AtB] [addSymbolicForm *AB *AtB *C]]
[[multiplySymbolicForm * [+] [+]] /]
[[multiplySymbolicForm *A [+ *B : *Bt] [+ *AB : *ABt]] / [multiplySymbolicForm *A *B *AB] / [multiplySymbolicForm *A [+ : *Bt] [+ : *ABt]]]
[[multiplySymbolicForm [*A : *At] [*B : *Bt] [*AB : *SABt]] [is_number *A] [is_number *B] / [mult *A *B *AB] [APPEND *At *Bt *ABt] [<=> *ABt *SABt]]

[[::= *e [*a : *b] : *t] [not eq *a +] / [::= *ab *a : *b] [::= *e *ab : *t]]
[[::= *e - : *t] / [::= *e -1 : *t]]
[[::= *e *x] / [toSymbolicForm *x *e]]
[[::= *e *x ^ *n : *t] [> *n 1] / [-- *n *next] [::= *e *x ^ *next *x : *t]]
[[::= *e *x ^ *n : *t] / [::= *e *x : *t]]
[[::= *e *x - : *t] / [::= *e *x + -1 : *t]]
[[::= *e *x + : *t] / [toSymbolicForm *x *xse] [::= *ex : *t] / [addSymbolicForm *xse *ex *e]]
[[::= *e *x *y ^ *n : *t] [> *n 1] / [-- *n *next] [::= *e *x *y ^ *next *y : *t]]
[[::= *e *x *y : *t] / [toSymbolicForm *x *xse] [toSymbolicForm *y *yse] [multiplySymbolicForm *xse *yse *ex] [::= *e *ex : *t]]

protect [
	+ - ++ -- ~ ~~ ~+ `*` % < = > <=> <= =< >= => <> ! & | ^
	eq grnd grandom
	not res ONE ALL TRY PROBE SELECT APPEND LENGTH REVERSE ONLIST INLIST NODUP MAP MEMBER REPLACE
	exit command inner inner_addcl inner_call random_cl explode
	sort divide
	expand_search_directory
	toSymbolicForm sortSymbolicForm addSymbolicForm multiplySymbolicForm := ::=
	]
private [inner inner_addcl inner_call random_cl explode divide args_tail]


end := [[auto_atoms] [command]] .
