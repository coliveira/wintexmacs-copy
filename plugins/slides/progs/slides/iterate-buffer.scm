
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; arch-tag: df83048f-c3c1-464f-94bd-cabef20d87da
;;
;; MODULE      : slides/iterate-buffer.scm
;; DESCRIPTION : Iterate in buffer, robustly while modifying the buffer
;; COPYRIGHT   : (C) 2003  David Allouche
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (slides iterate-buffer)
  (:use (slides search-in-tree))
  (:export excursion clear-output))

(define (excursion thunk)
  (let ((marker #f))
    (dynamic-wind
	(lambda ()
	  (set! marker (tm-position-new))
	  (tm-position-set marker (the-path)))
	thunk
	(lambda ()
	  (tm-go-to (tm-position-get marker))
	  (tm-position-delete marker)))))

(define (go-root)
  (tm-go-to '(0 0)))

(define (go-child i)
  (let ((p (but-last (the-path))))
    (if (< i (tree-compound-arity (tm-subtree p)))
        (begin (tm-go-to (append p `(,i 0))) #t)
        #f)))

(define (go-parent)
  (let ((p (but-last (the-path))))
    (and (pair? p)
         (begin (tm-go-to (rcons (but-last p) 0)) #t))))

(define (go-next-sibling)
  (and-let* ((p (but-last (the-path)))
             ((pair? p))
             (i (1+ (last p)))
             (pp (but-last p)))
    (if (< i (tree-compound-arity (tm-subtree pp)))
        (begin (tm-go-to (append pp `(,i 0))) #t)
        #f)))

(define (go-preorder)
  (or (go-child 0)
      (go-next-sibling)
      (go-preorder/backtrack)))

(define (go-preorder/backtrack)
  (and (go-parent)
       (or (go-next-sibling)
           (go-preorder/backtrack))))

(define (clear-output/sub)
  (trace-display "the-path:" (the-path))
  (if (go-preorder)
      (begin (and-let*
            ((p (but-last (the-path)))
             ((tree-func? (tm-subtree p) "output"))
             (pp (but-last p))
             ((tree-func? (tm-subtree pp) "document"))
             ((pair? pp))
             (ppp (but-last pp))
             ((pair? ppp))
             ((tree-func? (tm-subtree ppp) "session")))
          (tm-remove p 1))
     (clear-output/sub))))

(define (clear-output)
  (excursion (lambda () (go-root) (clear-output/sub))))