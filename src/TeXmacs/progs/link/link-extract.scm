
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : link-navigate.scm
;; DESCRIPTION : navigation routines for links
;; COPYRIGHT   : (C) 2006  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (link link-extract)
  (:use (link link-navigate)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutines
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (tree-search t pred?)
  (with me (if (pred? t) (list t) '())
    (if (tree-atomic? t) me
	(append me (append-map (cut tree-search <> pred?)
			       (tree-children t))))))

(define (locified? x)
  (cond ((list? x) (list-and (map locified? x)))
	((tree? x) (or (tm-func? x 'locus) (tm-func? (tree-up x) 'locus)))
	(else #t)))

(define (locify x)
  (cond ((list? x) (for-each locify x))
	((locified? x) (noop))
	(else (tree-insert-node! x 1 `(locus (id ,(create-unique-id)))))))

(define (environment->locus t)
  (or (and (tm-func? t 'locus) t)
      (and (tm-func? (tree-up t) 'locus) (tree-up t))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Locus and environment pages
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (build-locus-page-sub name style l enum?)
  (let* ((ids (map (cut list 'id <>) (filter-map locus-id l)))
	 (loci (if (null? ids) '("No loci") (map automatic-link ids)))
	 (body (if enum? (build-enumeration loci) loci))
	 (doc `(document (style ,style) (body (document ,@body)))))
    (set-aux-buffer name name doc)))

(tm-define (build-locus-page)
  (let* ((name (string-append (get-abbr-buffer) " - loci"))
	 (style (tree->stree (get-style-tree)))
	 (l (tree-search (buffer-tree) (cut tm-func? <> 'locus))))
    (build-locus-page-sub name style l #t)))

(tm-define (build-environment-page env)
  (:synopsis "Build page with environments of type @env in current buffer.")
  (:argument env "Environment")
  (let* ((env-l (map string->symbol (string-tokenize-comma env)))
	 (pred-l (map (lambda (tag) (cut tm-func? <> tag)) env-l))
	 (l (append-map (cut tree-search (buffer-tree) <>) pred-l)))
    (dialogue
      (if (and (not (locified? l))
	       (dialogue-confirm? "Locify environments?" #f))
	  (locify l))
      (let* ((name (string-append (get-abbr-buffer) " - " env))
	     (style (tree->stree (get-style-tree)))
	     (r (filter-map environment->locus l)))
	(delayed (:pause 25) (build-locus-page-sub name style r #f))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Constellation page
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (build-constellation-page)
  (let* ((name "Link constellation")
	 (style '(tuple "generic"))
	 (cl (sort (get-constellation) string<=?))
	 (l (map (lambda (x) `(hlink ,x ,x)) cl))
	 (body (if (null? l) '("No linked files") l))
	 (doc `(document (style ,style) (body (document ,@body)))))
    (set-aux-buffer name name doc)))
