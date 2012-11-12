;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copyright (C) 2006 Robert P. Wolf ;;
;;        ALL RIGHTS RESERVED        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program record [rec]

[[rec *seconds *file_name]
	[mod *seconds 128 *lsb]
	[div *seconds 128 *msb]
	[sysex 39 *lsb *msb *file_name]]

end .
