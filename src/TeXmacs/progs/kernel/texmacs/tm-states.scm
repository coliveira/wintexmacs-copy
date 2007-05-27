
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : tm-states.scm
;; DESCRIPTION : Dynamic states around (tm-define)-d functions
;; COPYRIGHT   : (C) 2007  Joris van der Hoeven and Henri Lesourd
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (kernel texmacs tm-states))

;; Low-level API
(define-public (state-create slotlist)
;; slotlist === ((<SLOT-DESCR>*) (<PROP-DESCR>*) (<COMPILED-PROP>*))
  slotlist)

(define (state-slots sr)
  (car sr))

(define (state-props sr)
  (cadr sr))

(define (state-cprops sr)
  (caddr sr))

(define (slotlist-load l)
 ;(display* "load=" l "\n")
  (for (e l)
     (primitive-eval
        `(begin
	   (if (not (defined? ',(car e)))
	       (define-public ,(car e) #f))
	   (set! ,(car e)
		 ,(with val (cadr e)
		     (if (and (pair? val) (eq? (car val) 'quote))
			 val
			`(quote ,(primitive-eval val)))))))))

(define (proplist-load l funcs)
 ;(display* "load[props]=" l "\n")
  (for (e l)
     (if (not (defined? `,(car e)))
	 (primitive-eval `(define-public ,(car e) #f))))
  (for (f funcs)
     (f)))

(define (slotlist-save l)
  (for (e l)
     (set-car! (cdr e) `(quote ,(primitive-eval (car e)))))
 ;(display* "save=" l "\n")
 ;(for (e l)
 ;   (primitive-eval `(undefine ,(car e))))
)

;; User-level API
(define current-state #f)

(define-public (state-synchronize)
  (with old-current current-state
    (set! current-state #f)
    (state-save old-current)))
  
(define-public (state-load sr)
 ;(display* "sl=" sr "\n")
  (if sr
      (begin
        (if (not (eq? sr current-state))
            (begin
              (state-synchronize)
              (slotlist-load (state-slots sr))))
        (proplist-load (state-props sr) (state-cprops sr))
        (set! current-state sr))))

(define-public (state-save sr)
  (if (and sr (not (eq? sr current-state)))
      (slotlist-save (state-slots sr)))
 ;(display* "ss=" sr "\n")
  )

(define (state-slotnames sr)
  (map car (state-slots sr)))

(define (state-propnames sr)
  (map car (state-props sr)))

(define-public (state-names sr)
  (append (state-slotnames sr) (state-propnames sr)))

(define-public (state-type sr name)
  (cond ((in? name (state-slotnames sr))
	 'slot)
	((in? name (state-propnames sr))
	 'prop)
	(else #f)))

(define (seek-pred? pred? l)
  (define res #f)
  (for (e l)
     (if (pred? (car e))
         (set! res e)))
  res)

(define (state-entry sr name)
  (define ref #f)
  (set! ref (seek-pred? (lambda (x) (eq? x name)) (state-slots sr)))
  (if (not ref)
      (set! ref (seek-pred? (lambda (x) (eq? x name)) (state-props sr))))
  ref)

(define-public (state-read sr name)
  (define ref #f)
  (set! ref (state-entry sr name))
  (if ref
      (cadr ref)))

(define-public (state-write sr name val)
  (define ref #f)
  (set! ref (state-entry sr name))
  (if ref
      (set-car! (cdr ref) val)))

(define-public-macro (define-state name . slotlists)
;; slotlists === ((slots ((<NAME-SLOT1> <INIT1>) ... (<NAME-SLOTN> <INITN>)))
;;                (props ((<NAME-PROP1> <PROP1>) ... (<NAME-PROPN> <PROPN>))))
  (let* ((theslots (copy-tree slotlists))
	 (slots (cadr (car theslots)))
	 (props (cadr (cadr theslots))))
    `(begin
	(if (not (defined? ',name))
	    (define-public ,name #f))
	(with cprops #f
	  (set! cprops (map (lambda (x)
			       (primitive-eval
				  `(lambda () (set! ,(car x) ,(cadr x)))))
			   ',props))
	  (set! ,name (state-create (append '(,slots ,props) `(,cprops))))))))

(define-public-macro (with-state sr . body)
 `(begin
     (state-load ,sr)
     (with res (begin . ,body)
       (state-save ,sr)
       res)))

(define-public-macro (with-state-by-name name . body)
 `(with sr ,name
     (with-state sr . ,body)))