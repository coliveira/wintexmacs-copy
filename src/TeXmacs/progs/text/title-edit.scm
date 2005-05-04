
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : title-edit.scm
;; DESCRIPTION : editing document titles, authors, etc.
;; COPYRIGHT   : (C) 2004  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (text title-edit))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Inserting document and author data
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define doc-data-inactive-tags
  '(doc-running-title doc-running-author doc-keywords doc-AMS-class))

(tm-define (make-doc-data)
  (insert-go-to '(doc-data (doc-title "")) '(0 0 0)))

(tm-define (make-doc-data-element l)
  (let* ((p (search-parent-upwards 'doc-data))
	 (q (rcons (cDr p) (+ (cAr p) 1))))
    (cond ((== l 'doc-author-data)
	   (tm-insert q `(doc-data (,l (author-name ""))))
	   (tm-go-to (rcons* q 0 0 0)))
	  ((== l 'doc-note)
	   (tm-insert q `(doc-data (,l (document ""))))
	   (tm-go-to (rcons* q 0 0 0)))
	  ((in? l doc-data-inactive-tags)
	   (tm-insert q `(doc-data (doc-inactive (,l ""))))
	   (tm-go-to (rcons* q 0 0 0)))
	  (else
	   (tm-insert q `(doc-data (,l "")))
	   (tm-go-to (rcons* q 0 0))))))

(tm-define (make-author-data-element l)
  (let* ((p (search-parent-upwards 'doc-author-data))
	 (q (rcons (cDr p) (+ (cAr p) 1))))
    (cond ((in? l '(author-address author-note))
	   (tm-insert q `(doc-author-data (,l (document ""))))
	   (tm-go-to (rcons* q 0 0 0)))
	  (else
	   (tm-insert q `(doc-author-data (,l "")))
	   (tm-go-to (rcons* q 0 0))))))

(tm-define (kbd-return)
  (:inside title)
  (go-end-line)
  (insert-return))

(tm-define (kbd-return)
  (:inside doc-title)
  (make-doc-data-element 'doc-author-data))

(tm-define (kbd-return)
  (:inside author-name)
  (make-author-data-element 'author-address))

(tm-define (kbd-return)
  (:inside doc-inactive)
  (doc-data-activate-here))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Activation and disactivation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (doc-data-activate-here)
  (with p (search-upwards "doc-inactive")
    (tm-rem-unary p)))

(tm-define (doc-data-disactivated?)
  (and (inside? "doc-data")
       (with p (search-upwards "doc-data")
	 (let* ((p (search-upwards "doc-data"))
		(t (tm-subtree p))
		(l (cdr (tree->list t))))
	   (list-or (map (lambda (t) (== (tm-car t) 'doc-inactive)) l))))))

(define (doc-data-activate-one t)
  (if (== (tm-car t) 'doc-inactive)
      (with p (tree-path t)
	(tm-rem-unary p))))

(tm-define (doc-data-activate-all)
  (let* ((p (search-upwards "doc-data"))
	 (t (tm-subtree p))
	 (l (cdr (tree->list t))))
      (for-each doc-data-activate-one l)))

(define (doc-data-disactivate-one t)
  (if (in? (tm-car t) doc-data-inactive-tags)
      (with p (tree-path t)
	(tm-ins-unary p 'doc-inactive))))

(tm-define (doc-data-disactivate-all)
  (let* ((p (search-upwards "doc-data"))
	 (t (tm-subtree p))
	 (l (cdr (tree->list t))))
      (for-each doc-data-disactivate-one l)))
