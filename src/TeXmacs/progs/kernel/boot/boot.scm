
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : boot.scm
;; DESCRIPTION : some global variables, public macros, on-entry, on-exit and
;;               initialization of the TeXmacs module system
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define texmacs-user (current-module))
(define temp-module (current-module))
(define temp-value #f)

(define (guile-a?) (equal? (scheme-dialect) "guile-a"))
(define (guile-b?) (equal? (scheme-dialect) "guile-b"))
(define (guile-c?) (equal? (scheme-dialect) "guile-c"))
(define (guile-b-c?) (or (guile-b?) (guile-c?)))
(if (guile-c?) (use-modules (ice-9 rdelim) (ice-9 pretty-print)))

;; Should be defined 
(define dialogue-break #f)
(define dialogue-return #f)
(define dialogue-error #f)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Provide functions if not defined and public macros
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define-macro (provide-public head . body)
  (if (or (and (symbol? head) (not (defined? head)))
	  (and (pair? head) (symbol? (car head)) (not (defined? (car head)))))
      `(define-public ,head ,@body)
      '(noop)))

(define-macro (define-public-macro head . body)
  `(define-public ,(car head)
     ;; FIXME: why can't we use procedure->macro for a non-memoizing variant?
     (procedure->memoizing-macro
      (lambda (cmd env)
	(apply (lambda ,(cdr head) ,@body) (cdr cmd))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; On-entry and on-exit macros
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (quit-TeXmacs-scheme) (noop))

(define-macro (on-entry . cmd)
  `(begin ,@cmd))

(define-macro (on-exit . cmd)
  `(set! quit-TeXmacs-scheme (lambda () ,@cmd (,quit-TeXmacs-scheme))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Module switching
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define-macro (with-module module . body)
  `(begin
     (set! temp-module (current-module))
     (set-current-module ,module)
     ,@body
     (set-current-module temp-module)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Module handling
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(if (guile-a?)
    (begin
      (define import-from use-modules)
      (define re-export export)))

(if (guile-b-c?)
    (begin
      (define-macro (import-from . modules)
	`(process-use-modules
	  (list ,@(map (lambda (m)
			 `(list ,@(compile-interface-spec m)))
		       modules))))
      ;; FIXME: why does this not work?
      ;; (define-macro (import-from . modules)
      ;;   (define (import-from-body module)
      ;;     `(module-use! (current-module) (resolve-module ',module)))
      ;;   `(begin
      ;;     ,@(map import-from-body modules)))
      ))

(define-macro (inherit-modules . which-list)
  (define (module-exports which)
    (let* ((m (resolve-module which))
	   (m-public (module-ref m '%module-public-interface)))
      (module-map (lambda (sym var) sym) m-public)))
  (let ((l (apply append (map module-exports which-list))))
    `(begin
       (use-modules ,@which-list)
       (re-export ,@l))))

(define-macro (texmacs-module name . options)
  (define (transform action)
    (cond ((not (pair? action)) (noop))
	  ((equal? (car action) :use) (cons 'use-modules (cdr action)))
	  ((equal? (car action) :inherit) (cons 'inherit-modules (cdr action)))
	  ((equal? (car action) :export)
	   (display "Warning] The option :export is no longer supported\n")
	   (display "       ] Please use tm-define instead\n"))
	  (else '(noop))))
  (let ((l (map-in-order transform options)))
    (if (guile-b-c?)
	(set! l (cons `(module-use! (current-module) ,texmacs-user) l)))
    ;;(display "loading ") (display name) (display "\n")
    `(begin
       (define-module ,name)
       ,@l)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Lazy modules for extern mactos in style packages
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define markup-modules-list '((utils misc markup-funcs)))

(define-macro (lazy-markup-modules . l)
  `(set! markup-modules-list (append markup-modules-list ',l)))

(define-macro (lazy-markup-modules-force)
  `(begin
     (use-modules ,@markup-modules-list)
     (set! markup-modules-list '())))
