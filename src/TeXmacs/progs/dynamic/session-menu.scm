
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : session-menu.scm
;; DESCRIPTION : menus for prog mode
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (dynamic session-menu)
  (:use (dynamic session-edit)))

;(lazy-plugin-force)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Inserting sessions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(menu-bind supported-sessions-menu
  ("Scheme" (make-session "scheme" "default"))
  ---
  ;; Each plugin appends its own entry
  )

(define (supported-sessions-menu-promise)
  (lazy-plugin-force)
  (menu-dynamic
    (link supported-sessions-menu)))

(menu-bind insert-session-menu
  (when (and (style-has? "std-dtd") (in-text?))
	(promise (supported-sessions-menu-promise))
	---
	("Other" ...
	 (interactive '("Session type:" "Session name:") 'make-session))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Submenus of the Sessions menu
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(menu-bind session-input-menu
  (when (in-plugin-with-converters?)
	("Mathematical input" (toggle-session-math-input)))
  ("Multiline input" (toggle-session-multiline-input)))

(menu-bind session-insert-menu
  ("Insert text field" (session-insert-text-field))
  ("Insert input field above" "A-up" (session-insert-input-above))
  ("Insert input field below" "A-down" (session-insert-input-below))
  ("Fold input field" "A-right" (session-fold-input)))

(menu-bind session-remove-menu
  ("Remove input field above" "A-backspace" (session-remove-input #f))
  ("Remove input field" "A-delete" (session-remove-input #t))
  ("Remove all output fields" (session-remove-all-outputs)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The Session menu
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(menu-bind session-menu
  (-> "Input mode" (link session-input-menu))
  (-> "Insert fields" (link session-insert-menu))
  (-> "Remove fields" (link session-remove-menu))
  ("Split session" (session-split))
  ---
  ("Interrupt execution" (connection-interrupt))
  ("Close session" (connection-stop)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Sessions icons
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(menu-bind session-icons
  (=> (balloon (icon "tm_plugin_input.xpm") "Input mode")
      (link session-input-menu))
  (=> (balloon (icon "tm_session_insert.xpm") "Insert fields")
      (link session-insert-menu))
  (=> (balloon (icon "tm_session_delete.xpm") "Remove fields")
      (link session-remove-menu))
  ((balloon (icon "tm_session_split.xpm") "Split session")
   (session-split))
  (if (!= (get-env "prog-language") "scheme")
      |
      ((balloon (icon "tm_stop.xpm") "Interrupt execution")
       (connection-interrupt))
      ((balloon (icon "tm_clsession.xpm") "Close session")
       (connection-stop)))
  (if (in-math?)
      |))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Help icons
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(menu-bind session-help-icons
  ;; Each plugin appends its own entry
  )

(menu-bind help-icons
  (link session-help-icons))