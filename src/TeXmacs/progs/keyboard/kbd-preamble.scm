
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : kbd-preamble.scm
;; DESCRIPTION : shortcuts for dynamic markup
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (keyboard kbd-preamble)
  (:use (texmacs edit edit-format) (texmacs edit edit-misc)))

(kbd-map
  ("executable" "" "Insert executable markup")
  ("inactive" "" "Insert inactive markup")

  ("inactive a" (make 'arg))
  ("inactive A" (make 'map_args))
  ("inactive c" (make 'compound))
  ("inactive d" (make 'delay))
  ("inactive D" (make 'drd_props))
  ("inactive e" (make 'compound))
  ("inactive E" (make 'eval_args))
  ("inactive h" (make 'hold))
  ("inactive i" (make 'include))
  ("inactive l" (make-latex))
  ("inactive m" (make 'macro))
  ("inactive M" (make 'meaning))
  ("inactive n" (make 'get_arity))
  ("inactive N" (make 'get_label))
  ("inactive p" (make 'provides))
  ("inactive q" (make 'quote))
  ("inactive r" (make 'release))
  ("inactive s" (make 'surround))
  ("inactive t" (make 'tag))
  ("inactive v" (make 'value))
  ("inactive w" (make 'with 3))
  ("inactive W" (make 'write))
  ("inactive x" (make 'xmacro))
  ("inactive *" (make 'action))
  ("inactive =" (make 'assign))
  ("inactive (" (make 'tuple))
  ("inactive <" (make 'tuple))
  ("inactive @" (make 'attr))
  ("inactive >" (make 'hlink))
  ("inactive #" (make 'argument))
  ("inactive $" (make 'symbol))
  ("inactive G" (make 'graphics))
  ("inactive S" (make 'superpose))
  ("inactive P" (make-arity 'point 2))
  ("inactive L" (make 'line))
  ("inactive C" (make 'cline))
  ("inactive F" (make 'fill))

  ("executable c" (make 'case))
  ("executable d" (make 'div))
  ("executable e" (make 'extern))
  ("executable f" (make 'find_file))
  ("executable F" (make 'flag))
  ("executable i" (make 'if))
  ("executable l" (make 'length))
  ("executable m" (make 'mod))
  ("executable q" (make 'is_tuple))
  ("executable t" (make 'translate))
  ("executable w" (make 'while))
  ("executable x" (make 'eval))
  ("executable |" (make 'or))
  ("executable ^" (make 'xor))
  ("executable &" (make 'and))
  ("executable !" (make 'not))
  ("executable +" (make 'plus))
  ("executable -" (make 'minus))
  ("executable *" (make 'times))
  ("executable /" (make 'over))
  ("executable ;" (make 'merge))
  ("executable ," (make 'range))
  ("executable #" (make 'number))
  ("executable @" (make-date))
  ("executable [" (make 'look_up))
  ("executable =" (make 'equal))
  ("executable <" (make 'less))
  ("executable >" (make 'greater))
  ("executable ?" (make 'if))
  ("executable C-i" (make 'var_if))
  ("executable C-?" (make 'var_if))
  ("executable C-@" (make 'date))
  ("executable C-=" (make 'unequal))
  ("executable C-<" (make 'lesseq))
  ("executable C->" (make 'greatereq)))
