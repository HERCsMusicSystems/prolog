;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2002 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


program studio #machine := "prolog.studio"


	[
		set_uap32_captions set_standard_captions set_edinburg_captions set_marseille_captions set_functional_captions
		auto_atoms scripted_atoms
		pr read readln
		write nl pp pt list
		file_writer import load consult batch file_reader reload
		remove_module create_module set_machine
		add_search_directory search_directories root_directory crd
		cd dir DIR ARGS args args_tail edit execute make_file make_directory erase erase_directory move copy
		CL cl addcl addcl0 DELCL OVERWRITE delcl delallcl lambda overwrite let
		create_atom search_atom preprocessor prompt
		add_strings add sub mult div mod and or xor sum times less less_eq greater greater_eq
		abs cos degrad e exp log2 log10 ln log pi pow sin tan trunc
		timestamp
		is_atom is_integer is_double is_number is_var is_head is_text has_machine
		text_list text_term e32 query_stack object_counter
		exit command inner inner_addcl inner_call minimise maximise
		res not eq rres ures lazy random_cl explode TRY ONE PROBE SELECT APPEND LENGTH REVERSE ONLIST INLIST NODUP MAP
		sort divide
		WHILE FOR FOREVER forever IF
		CONSTANT VARIABLE ACCUMULATOR CLOSURE ARRAY var inc dec closure call BLOCK ALL ISALL isall isallr
		rnd rnd_control grnd series
		crack wait timeout enter signal semaphore mutex accept select critical_section monitor task
		set_colors OPEN_EDITOR open_editor close_editor
		screen_coordinates
		open_keyboard open_oscilloscope
		get_volume_serial_number security_check

		noteon noteoff computer-do-this define-this show

		income_midi drag_and_drop keyoff keyon polyaftertouch control programchange aftertouch pitch
		sysex sysexch SYSEX SYSEXCH timingclock START CONTINUE STOP activesensing
		attack release cutoff resonance mono poly bank banklsb portaon portaoff portatime
		volume reverb chorus foot breath pan modulation holdon holdoff
		CONTROL ATTACK RELEASE CUTOFF RESONANCE PORTATIME
		VOLUME REVERB CHORUS FOOT BREATH PAN MODULATION
		nrpn rpn NRPN RPN hit DCMOD
		midi_info midi_port midi midi_in_info midi_out_info midi_in_port midi_out_port midi_short_msg MIDI_SHORT_MSG midi_long_msg
		midi_manufacturers_id midi_product_id midi_product_version
		egcopy egcopy_freq egcopy_amp egcopy_index egcopy_pan
		
		C C# Cx Cb Cbb
		D D# Dx Db Dbb
		E E# Ex Eb Ebb
		F F# Fx Fb Fbb
		G G# Gx Gb Gbb
		A A# Ax Ab Abb
		B B# Bx Bb Bbb
		octave notestep notevalue notekeys note_less note_less_eq
		INTERVAL interval
	]

#machine sum := "sum"
#machine add_strings := "add_strings"
#machine add := "add"
#machine sub := "sub"
#machine times := "times"
#machine mult := "mult"
#machine div := "div"
#machine mod := "mod"
#machine and := "and"
#machine or := "or"
#machine xor := "xor"

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

#machine timestamp := "timestamp"

#machine is_atom := "is_atom"
#machine is_integer := "is_integer"
#machine is_double := "is_double"
#machine is_number := "is_number"
#machine is_var := "is_var"
#machine is_head := "is_head"
#machine is_text := "is_text"
#machine has_machine := "has_machine"
#machine text_list := "text_list"
#machine text_term := "text_term"
#machine e32 := "e32"
#machine less := "less"
#machine less_eq := "less_eq"
#machine greater := "greater"
#machine greater_eq := "greater_eq"
#machine set_standard_captions := "set_standard_captions"
#machine set_uap32_captions := "set_uap32_captions"
#machine set_edinburg_captions := "set_edinburg_captions"
#machine set_marseille_captions := "set_marseille_captions"
#machine set_functional_captions := "set_functional_captions"
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
#machine search_atom := "search_atom"
#machine preprocessor := "preprocessor"
#machine prompt := "prompt"
#machine query_stack := "query_stack"
#machine object_counter := "object_counter"
#machine crack := "crack"
#machine wait := "wait"
#machine timeout := "timeout"
#machine semaphore := "semaphore"
#machine mutex := "mutex"
#machine rnd := "rnd"
#machine rnd_control := "rnd_control"
#machine series := "series"

#machine file_writer := "file_writer"
#machine file_reader := "file_reader"
#machine import := "import_loader"
#machine load := "load_loader"
#machine consult := "consult_loader"
#machine remove_module := "remove_module"
#machine create_module := "create_module"
#machine set_machine := "set_machine"
#machine root_directory := "root_directory"
#machine crd := "crd"
#machine add_search_directory := "add_search_directory"
#machine search_directories := "search_directories"
#machine cd := "cd"
#machine DIR := "DIR"
#machine ARGS := "ARGS"
#machine edit := "edit"
#machine make_directory := "make_directory"
#machine erase := "erase"
#machine erase_directory := "erase_directory"
#machine move := "move"
#machine copy := "copy"
#machine execute := "execute"

#machine CONSTANT := "CONSTANT"
#machine VARIABLE := "VARIABLE"
#machine ACCUMULATOR := "ACCUMULATOR"
#machine CLOSURE := "CLOSURE"
#machine ARRAY := "ARRAY"

#machine midi_in_info := "midi_in_info"
#machine midi_out_info := "midi_out_info"
#machine midi_in_port := "midi_in_port"
#machine midi_out_port := "midi_out_port"

#machine DCMOD := "DCMOD"
#machine INTERVAL := "INTERVAL"

#machine midi_short_msg := "midi_short_msg"
#machine MIDI_SHORT_MSG := "MIDI_SHORT_MSG"
#machine midi_long_msg := "midi_long_msg"
#machine sysex := "sysex"
#machine sysexch := "sysexch"
#machine SYSEX := "SYSEX"
#machine SYSEXCH := "SYSEXCH"
#machine midi_manufacturers_id := "midi_manufacturers_id"
#machine midi_product_id := "midi_product_id"
#machine midi_product_version := "midi_product_version"

#machine set_colors := "set_colors"
#machine OPEN_EDITOR := "open_editor"
#machine close_editor := "close_editor"
#machine screen_coordinates := "screen_coordinates"

#machine get_volume_serial_number := "get_volume_serial_number"
#machine security_check := "security_check"

[[open_editor] [OPEN_EDITOR]]
[[open_editor *id] [OPEN_EDITOR *id]]
[[open_editor *x *y] [screen_coordinates *x *y] [OPEN_EDITOR]]
[[open_editor *id *x *y] [screen_coordinates *x *y] [OPEN_EDITOR *id]]
[[open_keyboard] [OPEN_EDITOR 1]]
[[open_keyboard *x *y] [screen_coordinates *x *y] [OPEN_EDITOR 1]]
[[open_oscilloscope] [OPEN_EDITOR 3]]
[[open_oscilloscope *x *y] [screen_coordinates *x *y] [OPEN_EDITOR 3]]


[[grnd : *parameters] [rnd : *parameters]]
[[grnd : *parameters] / [grnd : *parameters]]
[[eq *x *x]]
[[not :*x] *x / fail]
[[not :*x]]
[[res :*x]:*x]
[[ONE :*o][res :*o]/]
[[ALL :*o][res : *o] fail]
[[ALL :*]]
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
[[ONLIST *x [*x : *]]]
[[ONLIST *x [* : *l]] [ONLIST *x *l]]
[[INLIST *l *x [*x : *l]]]
[[INLIST [*h : *l] *x [*h : *ll]] [INLIST *l *x *ll]]
[[NODUP [] []]/]
[[NODUP [*x : *t] *result] [ONLIST *x *t] / [NODUP *t *result]]
[[NODUP [*x : *t] [*x : *result]] / [NODUP *t *result]]
[[MAP [] [] []]]
[[MAP [[*x *y] : *xyt] [*x : *xt] [*y : *yt]] / [MAP *xyt *xt *yt]]
[[WHILE *condition : *call] [not not : *condition] / [PROBE : *call] / [WHILE *condition : *call]]
[[WHILE : *]]
[[FOREVER : *instructions] [PROBE : *instructions] / [FOREVER : *instructions]]
[[forever : *instructions] [res : *instructions] / [forever : *instructions]]
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
[[closure *atom *locals *parameters *term]
	[MAP *locals *local *values]
	[CLOSURE *atom : *values]
	[*atom : *atoms]
	[eq *local *atoms]
	[*atom [*term : *parameters]]]
[[call *atom : *parameters] [*atom *body : *parameters] : *body]
[[BLOCK *declarations : *body]
	[MAP *declarations *local *values]
	[create_atom *closure]
	[CLOSURE *closure : *values]
	[*closure : *atoms]
	[eq *local *atoms]
	[TRY : *body]]
[[ISALL *atom *template : *call]
	[res : *call]
	[*atom *template] fail]
[[ISALL :*]]
[[isall *list *template : *call]
	[create_atom *atom]
	[ACCUMULATOR *atom]
	[ISALL *atom *template : *call]
	[*atom : *list] /]
[[isallr *list *template : *call]
	[create_atom *atom]
	[ACCUMULATOR *atom]
	[ISALL *atom *template : *call]
	[*atom : *reversed_list]
	[REVERSE *reversed_list *list] /]

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





[[cl *x]/[cl 0 *y *x]]
[[cl *x *y]/[cl 0 *x *y]]
[[cl *x *x [[*a:*b]:*c]][CL *x *a [[*a:*b]:*c]]]
[[cl *x *y [[*a:*b]:*c]][add *x 1 *x2]/[CL *x2 *a *X]
	[cl *x2 *y [[*a:*b]:*c]]]

[[lambda *atom *parameters : *body] [create_atom *atom] [addcl [[*atom : *parameters] : *body]]]
[[lambda *atom *parameters : *body] [addcl [[*atom : *parameters] : *body]]]

[[delcl [[*a:*b]:*c]][cl *x [[*a:*b]:*c]][DELCL *x *a]]

[[delallcl *a] [DELCL 0 *a] / [delallcl *a]]
[[delallcl *a]]

[[overwrite [[*a:*b]:*c] [[*a:*h]:*i]][cl *x [[*a:*b]:*c]]
	[OVERWRITE *x [[*a:*h]:*i]]]
[[let *name *selector : *value]
	[overwrite [[*name *selector : *] : *] [[*name *selector : *value]]]]


; multithreading and synchronization

[[monitor *atom]
	[is_atom *atom]
	[semaphore *s]
	[addcl [[*atom *mode : *commands] [*s *mode] [TRY : *commands] [*s signal] [wait 0]]]
	]

[[critical_section [*atom : *parameters] : *commands]
	[is_atom *atom]
	[semaphore *s]
	[addcl [[*atom : *parameters] [*s wait] [TRY : *commands] [*s signal] [wait 0]]]
	]


[[task *atom]
	[is_atom *atom]
	[semaphore *trigger 0]
	[semaphore *e1 0]
	[semaphore *e2 0]
	[VARIABLE *v1]
	[addcl [[*atom task *trigger *v1 *e1 *e2]]]
	]

[[task *atom enter []]]
[[task *atom enter [*entry : *entries]]
	[is_atom *atom]
	[is_atom *entry]
	[semaphore *s1 0]
	[semaphore *s2 0]
	[addcl [[*atom enter *entry *s1 *s2]]]
	/ [task *atom enter *entries]
	]

[[task *atom [*entry : *entries] : *body] [task *atom] [task *atom enter [*entry : *entries]] [crack : *body]]
	

[[accept *task *s *parameters : *commands]
	[*task enter *s *s1 *s2]
	[*s1 enter]
	[*s2 signal]
;	[*trigger wait]
	[*task task *trigger *v *e1 *e2]
	[*e1 signal]
	[*e2 wait]
	[*v : *x]
	[eq *x *parameters]
	[TRY : *commands]
	[*v *x]
	[*e1 signal]
	[*e2 wait]
	[*trigger signal]
	]
[[accept *task]
	[*task task *trigger : *]
	[*trigger signal]
	]

[[enter *task *s *data]
	[*task enter *s *s1 *s2]
	[*s1 signal]                             ; ready for specific entry ...
	[*task task *trigger *v *e1 *e2]
	[*trigger signal]                        ; ... of specific task
	[*s2 wait]
	[*e1 wait]                               ; waiting for sending the data
	[*v *data]                               ; actual data transfer
	[*e2 signal]                             ; data transferred to the task
	[*e1 wait]                               ; wait for task to do it's job
	[*v : *return]                           ; receiving the data
	[*e2 signal]
	[eq *data *return]                       ; ...
	]

[[select *task : *branches]
	[*task task *trigger : *]
	[*trigger wait]
	[SELECT : *branches]
	]
[[select : *tail] / [select : *tail]]


; command processor

[[exit]]

;[[inner [[*atom : *] : *] []]]
[[inner [* : *] []]]
[[inner *x *y] [pr *y]]
[[inner [*x *y]] [pr *x] [inner *x *y] /]
[[inner [*x *y]] [readln *x] fail]

[[inner_addcl [[*atom : *t1] : *t2]]
	[preprocessor *preprocessor]
	[is_atom *preprocessor]
	/ [*preprocessor *t2 *t3] [addcl [[*atom : *t1] : *t3]]]
[[inner_addcl *cl] [addcl *cl]]

[[inner_call *command] [preprocessor *preprocessor] [is_atom *preprocessor] / [*preprocessor [*command] *c] : *c]
[[inner_call *command] *command]

[[command [exit]]]
[[command [[exit]]]]
[[command *x] [inner [*y *z]] [command *y *z] / [command [*y : *z]]]
[[command *x] [write "Doesn't work"] [nl] / [command *x]]
[[command] [command [command]]]
[[command [[*atom : *t1] : *t2] []] [inner_addcl [[*atom : *t1] :*t2]]]
[[command [[*atom : *t1] : *t2] []] [write "Can not add clause: " [[[*atom : *t1] : *t2]] "\n"] / fail]
[[command [*x : *y] []] / [inner_call [*x : *y]]]
[[command *x *y] [inner_call [*x : *y]]]


[[batch *text] [create_atom *atom] [file_reader *atom *text] / [batch [batch] *atom]]
[[batch [exit] *reader][*reader]]
[[batch *x *reader][*reader *head]/[batch *x *reader *head]] ;[*reader *tail][*head :*tail] / [batch [*head :*tail] *reader]]
[[batch *x *reader][*reader] / fail]
[[batch *x *reader *head] [eq *head [[* : *] : *]] / [inner_addcl *head] / [batch *head *reader]]
[[batch *x *reader *head] [is_atom *head] / [*reader *tail] [inner_call [*head : *tail]] / [batch [*head : *tail] *reader]]
[[batch *x *reader *head] / [inner_call *head] [batch *head *reader]]

[[reload] [list :*x] [eq *x [*user *loader :*tail]] [add *loader ".prc" *name] [load *name]]

[[make_file *file_name] [create_atom *atom] [file_writer *atom *file_name] [*atom]]

[[dir] / [DIR "." : *files] [dir : *files]]
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

[[args : *args] [ARGS : *texts] / [args_tail *texts *args]]
[[args_tail [] []] /]
[[args_tail [*t1 : *t] *a] [text_term *t1 : *b] [APPEND *b *c *a] / [args_tail *t *c]]

; demo
[[noteon *key] [keyon 0 *key 100]]
[[noteoff *key] [keyoff 0 *key]]
[[noteoff] [keyoff 0]]
[[computer-do-this : *x] : *x]
[[define-this : *x] [addcl *x]]
[[show] [nl] /]
[[show *text : *show] [is_text *text] [write *text] / [show : *show]]
[[show *x : *show] [pp *x] / [show : *show]]

; main midi messages
[[keyoff *channel *key] [midi_short_msg 128 *channel *key 0]]
[[keyoff *channel *key *velocity] [midi_short_msg 128 *channel *key *velocity]]
[[keyon *channel *key *velocity] [midi_short_msg 144 *channel *key *velocity]]
[[polyaftertouch *channel *key *value] [midi_short_msg 160 *channel *key *value]]
[[control *channel *controller *value] [midi_short_msg 176 *channel *controller *value]]
[[programchange *channel *program] [midi_short_msg 192 *channel *program]]
[[programchange *channel *msb *lsb *program] [add *channel 64 *selector] [sysex *selector 84 *lsb *program *msb]]
[[aftertouch *channel *value] [midi_short_msg 208 *channel *value]]
[[pitch *channel *msb] [midi_short_msg 224 *channel 0 *msb]]
[[pitch *channel *lsb *msb] [midi_short_msg 224 *channel *lsb *msb]]
[[timingclock] [midi_short_msg 248]]
[[START] [midi_short_msg 250]]
[[CONTINUE] [midi_short_msg 251]]
[[STOP] [midi_short_msg 252]]
[[activesensing] [midi_short_msg 254]]


; controllers
[[keyoff *channel] [midi_short_msg 176 *channel 123 0]]
[[attack *channel *value] [midi_short_msg 176 *channel 73 *value]]
[[release *channel *value] [midi_short_msg 176 *channel 72 *value]]
[[cutoff *channel *value] [midi_short_msg 176 *channel 74 *value]]
[[resonance *channel *value] [midi_short_msg 176 *channel 71 *value]]
[[mono *channel] [midi_short_msg 176 *channel 126 0]]
[[poly *channel] [midi_short_msg 176 *channel 127 0]]
[[bank *channel *msb *lsb] [midi_short_msg 176 *channel 0 *msb] [midi_short_msg 176 *channel 32 *lsb]]
[[bank *channel *value] [midi_short_msg 176 *channel 0 *value]]
[[bank *channel *msb *lsb] [add *channel 64 *selector] [sysex *selector 83 *lsb *msb]]
[[bank *channel *msb] [add *channel 64 *selector] [sysex *selector 82 *msb]]
[[banklsb *channel *value] [midi_short_msg 176 *channel 32 *value]]
[[portaon *channel] [midi_short_msg 176 *channel 65 127]]
[[portaoff *channel] [midi_short_msg 176 *channel 65 0]]
[[portatime *channel *value] [midi_short_msg 176 *channel 5 *value]]
[[volume *channel *value] [midi_short_msg 176 *channel 7 *value]]
[[reverb *channel *value] [midi_short_msg 176 *channel 91 *value]]
[[chorus *channel *value] [midi_short_msg 176 *channel 93 *value]]
[[foot *channel *value] [midi_short_msg 176 *channel 4 *value]]
[[breath *channel *value] [midi_short_msg 176 *channel 2 *value]]
[[pan *channel *value] [midi_short_msg 176 *channel 10 *value]]
[[modulation *channel *value] [midi_short_msg 176 *channel 1 *value]]
[[holdon *channel] [midi_short_msg 176 *channel 64 127]]
[[holdoff *channel] [midi_short_msg 176 *channel 64 0]]
; capital controllers
[[CONTROL *channel *controller *value] [MIDI_SHORT_MSG 176 *channel *controller *value]]
[[ATTACK *channel *value] [MIDI_SHORT_MSG 176 *channel 73 *value]]
[[RELEASE *channel *value] [MIDI_SHORT_MSG 176 *channel 72 *value]]
[[CUTOFF *channel *value] [MIDI_SHORT_MSG 176 *channel 74 *value]]
[[RESONANCE *channel *value] [MIDI_SHORT_MSG 176 *channel 71 *value]]
[[PORTATIME *channel *value] [MIDI_SHORT_MSG 176 *channel 5 *value]]
[[VOLUME *channel *value] [MIDI_SHORT_MSG 176 *channel 7 *value]]
[[REVERB *channel *value] [MIDI_SHORT_MSG 176 *channel 91 *value]]
[[CHORUS *channel *value] [MIDI_SHORT_MSG 176 *channel 93 *value]]
[[FOOT *channel *value] [MIDI_SHORT_MSG 176 *channel 4 *value]]
[[BREATH *channel *value] [MIDI_SHORT_MSG 176 *channel 2 *value]]
[[PAN *channel *value] [MIDI_SHORT_MSG 176 *channel 10 *value]]
[[MODULATION *channel *value] [MIDI_SHORT_MSG 176 *channel 1 *value]]

; nrpn / rpn
[[nrpn *ch *pmsb *plsb *vmsb *vlsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]
	[control *ch 38 *vlsb]
	[control *ch  6 *vmsb]]
[[nrpn *ch *pmsb *plsb *vmsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]
	[control *ch  6 *vmsb]]
[[nrpn *ch *pmsb *plsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]]
[[nrpn *ch *vmsb]
	[control *ch 6 *vmsb]]
[[nrpn *ch]
	[control *ch 99 127]
	[control *ch 98 127]]
[[NRPN *ch *pmsb *plsb *vmsb]
	[control *ch 99 *pmsb]
	[control *ch 98 *plsb]
	[MIDI_SHORT_MSG 176 *ch  6 *vmsb]]
[[NRPN *ch *vmsb]
	[MIDI_SHORT_MSG 176 *ch 6 *vmsb]]



[[rpn *ch *pmsb *plsb *vmsb *vlsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]
	[control *ch 38 *vlsb]
	[control *ch  6 *vmsb]]
[[rpn *ch *pmsb *plsb *vmsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]
	[control *ch  6 *vmsb]]
[[rpn *ch *pmsb *plsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]]
[[rpn *ch *vmsb]
	[control *ch 6 *vmsb]]
[[rpn *ch]
	[control *ch 101 127]
	[control *ch 100 127]]
[[RPN *ch *pmsb *plsb *vmsb]
	[control *ch 101 *pmsb]
	[control *ch 100 *plsb]
	[MIDI_SHORT_MSG 176 *ch  6 *vmsb]]
[[RPN *ch *vmsb]
	[MIDI_SHORT_MSG 176 *ch 6 *vmsb]]

; macros

[[hit *ch *vel]]
[[hit *ch *vel *key :*r]
	[keyon *ch *key *vel]/[hit *ch *vel :*r]]

[[egcopy_index *channel *from *to] [sum *channel 64 *ch] [sysex *ch 66 *from *to]]
[[egcopy_freq *channel *from *to] [sum *channel 64 *ch] [sum *from 16 *f] [sum *to 16 *t] [sysex *ch 66 *f *t]]
[[egcopy_amp *channel *from *to] [sum *channel 64 *ch] [sum *from 32 *f] [sum *to 32 *t] [sysex *ch 66 *f *t]]
[[egcopy_pan *channel *from *to] [sum *channel 64 *ch] [sum *from 48 *f] [sum *to 48 *t] [sysex *ch 66 *f *t]]
[[egcopy : *command] [egcopy_amp : *command]]

; midi port manipulations

[[midi_info] [midi_in_info] [midi_out_info]]
[[midi_info *in *out] [midi_in_info : *in] [midi_out_info : *out]]
[[midi_port *in *out] [midi_in_port *in] [midi_out_port *out]]
[[midi] [midi_info]]
[[midi *in *out] [midi_port *in *out] [midi_info]]

; musical definitions

[[octave 0 28 48]]
[[octave -1 21 36]]
[[octave 1 35 60]]
[[octave -2 14 24]] ; grand piano lowest a = 21
[[octave 2 42 72]]
[[octave -3 7 12]]
[[octave 3 49 84]]
[[octave -4 0 0]]
[[octave 4 56 96]]
[[octave 5 63 108]] ; grand piano highest c = 108
[[octave 6 70 120]]

[[notestep C 0 0]]
[[notestep D 1 2]]
[[notestep E 2 4]]
[[notestep F 3 5]]
[[notestep G 4 7]]
[[notestep A 5 9]]
[[notestep B 6 11]]

[[notestep F# 3 6]]
[[notestep Bb 6 10]]
[[notestep C# 0 1]]
[[notestep Eb 2 3]]
[[notestep G# 4 8]]
[[notestep Ab 5 8]]
[[notestep D# 1 3]]
[[notestep Db 1 1]]
[[notestep A# 5 10]]
[[notestep Gb 4 6]]
[[notestep E# 2 5]]
[[notestep Cb 0 -1]]
[[notestep B# 6 12]]
[[notestep Fb 3 4]]

[[notestep Fx 3 7]]
[[notestep Bbb 6 9]]
[[notestep Cx 0 2]]
[[notestep Ebb 2 2]]
[[notestep Gx 4 9]]
[[notestep Abb 5 7]]
[[notestep Dx 1 4]]
[[notestep Dbb 1 0]]
[[notestep Ax 5 11]]
[[notestep Gbb 4 5]]
[[notestep Ex 2 6]]
[[notestep Cbb 0 -2]]
[[notestep Bx 6 13]]
[[notestep Fbb 3 3]]

[[notevalue [*note *octave : *] *diatonic *chromatic]
	[notestep *note *note_diatonic *note_chromatic]
	[octave *octave *octave_diatonic *octave_chromatic]
	[sum *octave_diatonic *note_diatonic *diatonic]
	[sum *octave_chromatic *note_chromatic *chromatic]
]

[[interval [*from_step *from_octave : *t] [*to_step *to_octave : *t] [*diatonic *chromatic]]
	[notestep *from_step *from_d *from_ch]
	[DCMOD *from_d *from_ch *from_octave *diatonic *chromatic *d *c *to_octave]
	[notestep *to_step *d *c]/
]
[[interval [*from_step *from_octave : *t] [*to_step *to_octave : *t] [*diatonic *chromatic]]
	[notevalue [*from_step *from_octave] *from_d *from_ch]
	[notevalue [*to_step *to_octave] *to_d *to_ch]
	[sum *from_d *diatonic *to_d]
	[sum *from_ch *chromatic *to_ch]
]

[[notekeys [] []]/]
[[notekeys [*note : *notes] [*key : *keys]] [notevalue *note * *key] / [notekeys *notes *keys]]

[[note_less : *notes] [notekeys *notes *keys] / [less : *keys]]
[[note_less_eq : *notes] [notekeys *notes *keys] / [less_eq : *keys]]

protect [
	not
	exit command inner inner_addcl inner_call random_cl explode
	APPEND REVERSE LENGTH ONLIST NODUP
	sort divide
	]
private [inner inner_addcl inner_call random_cl explode divide args_tail]


end.
