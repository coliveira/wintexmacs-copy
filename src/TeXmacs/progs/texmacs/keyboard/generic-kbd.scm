
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : generic-kbd.scm
;; DESCRIPTION : general keyboard shortcuts for all modes
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (texmacs keyboard generic-kbd)
  (:use
    (utils edit circulate)
    (utils edit auto-close)
    (texmacs texmacs tm-files)
    (texmacs texmacs tm-print)
    (generic generic-edit)
    (source source-edit)
    (dynamic fold-edit)
    (doc help-funcs)))

(kbd-map
  ("space" " ")
  ("space tab" (make 'nbsp))
  ("return" (kbd-return))
  ("backspace" (kbd-remove #f))
  ("delete" (kbd-remove #t))
  ("tab" (kbd-tab))
  ("S-space" (insert " "))
  ("S-return" (kbd-shift-return))
  ("S-backspace" (kbd-remove #f))
  ("S-delete" (kbd-remove #t))
  ("C-backspace" (remove-structure-upwards))
  ("C-delete" (remove-structure-upwards))
  ("C-tab" (variant-circulate #t))
  ("C-S-tab" (variant-circulate #f))
  ("A-space" (make-space "0.2spc"))
  ("A-S-space" (make-space "-0.2spc"))
  ("A-backspace" (structured-remove #f))
  ("A-delete" (structured-remove #t))
  ("A-tab" (make-htab "5mm"))
  ("M-space" (make-space "0.2spc"))
  ("M-S-space" (make-space "-0.2spc"))
  ("M-backspace" (position-default))
  ("M-delete" (position-default))
  ("M-tab" (make-htab "5mm"))
  ("escape tab" (noop) (make-htab "5mm"))

  ("left" (kbd-left))
  ("right" (kbd-right))
  ("up" (kbd-up))
  ("down" (kbd-down))
  ("home" (kbd-start-line))
  ("end" (kbd-end-line))
  ("pageup" (kbd-page-up))
  ("pagedown" (kbd-page-down))
  ("S-left" (kbd-select kbd-left))
  ("S-right" (kbd-select kbd-right))
  ("S-up" (kbd-select kbd-up))
  ("S-down" (kbd-select kbd-down))
  ("S-home" (kbd-select kbd-start-line))
  ("S-end" (kbd-select kbd-end-line))
  ("S-pageup" (kbd-select kbd-page-up))
  ("S-pagedown" (kbd-select kbd-page-down))
  ("C-left" (structured-left))
  ("C-right" (structured-right))
  ("C-up" (structured-up))
  ("C-down" (structured-down))
  ("C-home" (structured-start))
  ("C-end" (structured-end))
  ("C-pageup" (structured-top))
  ("C-pagedown" (structured-bottom))
  ("A-left" (structured-insert #f))
  ("A-right" (structured-insert #t))
  ("A-up" (structured-insert-up))
  ("A-down" (structured-insert-down))
  ("A-home" (structured-insert-start))
  ("A-end" (structured-insert-end))
  ("A-pageup" (structured-insert-top))
  ("A-pagedown" (structured-insert-bottom))
  ("M-left" (position-left))
  ("M-right" (position-right))
  ("M-up" (position-up))
  ("M-down" (position-down))
  ("M-home" (position-start))
  ("M-end" (position-end))
  ("M-pageup" (position-top))
  ("M-pagedown" (position-bottom))
  ("M-A-left" (switch-to "previous"))
  ("M-A-right" (switch-to "next"))
  ("M-A-up" (switch-to "first"))
  ("M-A-down" (switch-to "last"))
  ("M-A-home" (selection-set-start) (select-from-keyboard #t))
  ("M-A-end" (selection-set-end) (select-from-keyboard #f))
  ("M-A-pageup" (fold))
  ("M-A-pagedown" (unfold))

  ("F1" (interactive '("Search words in the documentation:") 'docgrep-in-doc))
  ("S-F1" (load-help-buffer "about/welcome/welcome"))
  ("F2" (choose-file "Load file" "" 'load-buffer))
  ("S-F2" (choose-file "Load file" "" 'load-in-new-window))
  ("C-F2" (revert-buffer))
  ("M-F2" (new-buffer))
  ("M-S-F2" (open-window))
  ("M-C-F2" (clone-window))
  ("F3" (save-buffer))
  ("S-F3" (choose-file "Save TeXmacs file" "texmacs" 'save-buffer))
  ("F4" (print))
  ("S-F4" (interactive '("File name:") 'print-to-file))
  ("C-F4" (preview-with-ghostview))
  ("M-F4" (interactive '("First page:" "Last page:") 'print-pages))
  ("M-S-F4" (interactive
	     '("File name:" "First page:" "Last page:")
	     'print-pages-to-file))

  ("undo" (noop) (undo))
  ("redo" (noop) (redo))
  ("cancel" (noop) (clipboard-clear "primary"))
  ("cut" (noop) (clipboard-cut "primary"))
  ("paste" (noop) (clipboard-paste "primary"))
  ("copy" (noop) (clipboard-copy "primary"))
  ("find" (noop) (search-start #t))

  ("<" "<less>")
  (">" "<gtr>")
  ("(" (make-bracket-open "(" ")"))
  (")" (make-bracket-close ")" "("))
  ("[" (make-bracket-open "[" "]"))
  ("]" (make-bracket-close "]" "["))
  ("{" (make-bracket-open "{" "}"))
  ("}" (make-bracket-close "}" "{"))
  ("\\" (if (or (inside? "hybrid") (in-prog?))
	    (insert "\\")
	    (make-hybrid)))
  ("M-\\" (make-hybrid))
  ("accent:hat" "^")
  ("accent:deadhat" "^")
  ("accent:tilde" "~")
  ("accent:acute" "'")
  ("accent:grave" "`")
  ("symbol \\" "\\")
  ("symbol \"" "\"")
  ("symbol $" "$")
  ("symbol &" "&")
  ("symbol #" "#")
  ("symbol �" "�")
  ("symbol %" "%")
  ("symbol _" "_")
  ("symbol ^" "^")
  ("symbol \"" "\""))

(kbd-map ;; inserting tables
  ("table N t" (make 'tabular))
  ("table N T" (make 'tabular*))
  ("table N b" (make 'block))
  ("table N B" (make 'block*)))
