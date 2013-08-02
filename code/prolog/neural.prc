;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2007 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program neural #machine := "prolog.neural"


	[
		NEURAL_LAYER NORMALISED_LAYER KOHONEN_LAYER
	]

; usage
; [NEURAL_LAYER layer *number_of_neurons *number_of_synapses]
; [layer : *weights]
; [layer *weights]
; [layer no_lin_function] (integer index of internal function)
; [layer train] (train is a fractional number)
; [layer [.... exposition ....] (:) *return]
; [layer [.... exposition ....] [.... expectation ....] (:) *return]
; *weights => [[*w11 *w12 ...] [*w21 *w22 ...] ...]
; *exposition *expectation *return => [*e1 *e2 *e3 ...]

; multi layer
; [NEURAL_LAYER a *number_of_neurons *number_of_synapses]
; [a b *number_of_neurons]
; [b c *number_of_neurons]
; [a no_lin_function_1] [b no_lin_function_2] [c no_lin_function_3]
; [c train] => train c, b and a
; [b train] => train b and a (but no c)
; [b [.... exposition ....] (:) *return] => exposition on a, return from b
; [c [.... exposition ....] (:) *return] => exposition on a, return from c
; [b [.... exposition ....] [.... expectation ....] (:) *return] => exposition on a, return from b, backpropagate b and a
; [c [.... exposition ....] [.... expectation ....] (:) *return] => exposition on a, return from c, backpropagate c, b and a
; [b] => makes a and c separate layers

#machine NEURAL_LAYER := "NEURAL_LAYER"
#machine NORMALISED_LAYER := "NORMALISED_LAYER"
#machine KOHONEN_LAYER := "KOHONEN_LAYER"

end .

