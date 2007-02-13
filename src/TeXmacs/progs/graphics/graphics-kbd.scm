
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : graphics-kbd.scm
;; DESCRIPTION : keyboard handling for graphics mode
;; COPYRIGHT   : (C) 2007  Joris van der Hoeven and Henri Lesourd
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (graphics graphics-kbd)
  (:use (utils library cursor) (utils library tree)
        (graphics graphics-utils) (graphics graphics-main)
        (graphics graphics-env) (graphics graphics-edit)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Keyboard handling
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define ShiftMask   1)
(tm-define LockMask    2)
(tm-define ControlMask 4)
(tm-define Mod1Mask    8)
(tm-define Mod2Mask   16)
(tm-define Mod3Mask   32)
(tm-define Mod4Mask   64)
(tm-define Mod5Mask  128)

(tm-define (kbd-tab)
  (:mode in-graphics?)
  (graphics-choose-point))

(tm-define (kbd-left)
  (:inside text-at)
  (go-to-remain-inside go-left 'text-at))

(tm-define (kbd-right)
  (:inside text-at)
  (go-to-remain-inside go-right 'text-at))

(tm-define (kbd-up)
  (:inside text-at)
  (go-to-remain-inside go-up 'text-at))

(tm-define (kbd-down)
  (:inside text-at)
  (go-to-remain-inside go-down 'text-at))

(tm-define (kbd-start-line)
  (:inside text-at)
  (with move (lambda () (go-to-remain-inside go-left 'text-at))
    (go-to-repeat move)))

(tm-define (kbd-end-line)
  (:inside text-at)
  (with move (lambda () (go-to-remain-inside go-right 'text-at))
    (go-to-repeat move)))

(define (in-active-graphics?)
  (and (in-graphics?) (== (get-env "preamble") "false")))

(define (graphics-kbd-remove forward?)
  (if (and (with-active-selection?)
	   (with-cursor (rcons (selection-path) 0)
	     (not (in-graphics?))))
      (begin
	(go-to (rcons (selection-path) 0))
	(clipboard-cut "primary"))))

(kbd-map
  (:mode in-active-graphics?)
  ("+" (graphics-zoom (/ 1.0 0.75)))
  ("-" (graphics-zoom 0.75))
  ("left" (graphics-move-origin "+0.1gw" "0gh"))
  ("right" (graphics-move-origin "-0.1gw" "0gh"))
  ("down" (graphics-move-origin "0gw" "+0.1gh"))
  ("up" (graphics-move-origin "0gw" "-0.1gh"))
  ("home" (graphics-zmove 'foreground))
  ("end" (graphics-zmove 'background))
  ("pageup" (graphics-zmove 'closer))
  ("pagedown" (graphics-zmove 'farther))
  ("C-left" (graphics-change-extents "-0.5cm" "0cm"))
  ("C-right" (graphics-change-extents "+0.5cm" "0cm"))
  ("C-down" (graphics-change-extents "0cm" "+0.5cm"))
  ("C-up" (graphics-change-extents "0cm" "-0.5cm"))
  ("M-left"  (if (current-is-textat?)
		 (text-at-change-halign current-path-under-mouse #f)))
  ("M-right" (if (current-is-textat?)
		 (text-at-change-halign current-path-under-mouse #t)))
  ("M-down"  (if (current-is-textat?)
		 (text-at-change-valign current-path-under-mouse #f)
		 (graphics-change-geo-valign #f)))
  ("M-up"    (if (current-is-textat?)
		 (text-at-change-valign current-path-under-mouse #t)
		 (graphics-change-geo-valign #t)))
  ("backspace" (graphics-kbd-remove #f))
  ("delete" (graphics-kbd-remove #t))
  ("C-g" (graphics-toggle-grid #f))
  ("C-G" (graphics-toggle-grid #t)))

(tm-define (inside-draw-over/under?)
  (or (inside? 'draw-over) (inside? 'draw-under)))

(tm-define (graphics-toggle-draw-over/under)
  (with-innermost t (lambda (x) (tree-in? x '(draw-over draw-under)))
    (if (tree-is? t 'draw-over)
	(begin
	  (tree-assign-node! t 'draw-under)
	  (tree-go-to t 0 :end))
	(begin
	  (tree-assign-node! t 'draw-over)
	  (if (tree-is? (tree-ref t 1) 'with)
	      (tree-go-to t 1 (- (tree-arity (tree-ref t 1)) 1) :end)
	      (tree-go-to t 1 :end))))))

(kbd-map
  (:mode inside-draw-over/under?)
  ("C-*" (graphics-toggle-draw-over/under)))