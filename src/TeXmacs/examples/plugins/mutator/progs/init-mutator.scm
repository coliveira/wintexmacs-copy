
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : init-mutator.scm
;; DESCRIPTION : Initialize the 'mutator' plugin
;; COPYRIGHT   : (C) 2004  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(use-modules (utils library tree))

(tm-define (mutate-date)
  (:secure #t)
  (with-mutator t
    (tree-set t (var-eval-system "date +\"%H:%M:%S\""))))

(tm-define (mutate-blink)
  (:secure #t)
  (with-mutator t
    (let* ((mod (lambda (x y) (* y (- (/ x y) (floor (/ x y))))))
	   (s (string->number (var-eval-system "date +\"%S\"")))
	   (e (mod s 4)))
      (if (and (<= e 1) (not (match? t '(strong :%1))))
	  (tree-set t `(strong ,t)))
      (if (and (>= e 2) (match? t '(strong :%1)))
	  (tree-set t (tree-ref t 0))))))

(kbd-map
  ("C-F11" (insert '(mutator "" "(mutate-date)")))
  ("C-F12" (insert-go-to '(mutator "text" "(mutate-blink)") '(0 4))))

(plugin-configure mutator
  (:require #t))
