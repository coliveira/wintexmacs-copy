
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : text.scm
;; DESCRIPTION : editing routines for text mode
;; COPYRIGHT   : (C) 2001  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (texmacs edit edit-text)
  (:use (texmacs tools tm-circulate))
  (:export
    ;; titles
    go-end-of-header-element make-header
    ;; sections
    inside-section? make-section make-unnamed-section toggle-section-number
    ;; lists
    inside-list? inside-description? make-tmlist make-item
    ;; auxiliary
    make-aux make-aux* make-bib))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Making titles
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (go-end-of-header-element)
  (if (inside? "address") (go-end-of "address"))
  (if (inside? "destination") (go-end-of "destination"))
  (if (inside? "cc") (go-end-of "cc"))
  (if (inside? "encl") (go-end-of "encl"))
  (go-end-line))

(define (make-header l)
  (go-end-of-header-element)
  (if (not (== (tree->object (the-line)) "")) (insert-return))
  (make l))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Sectional commands
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (inside-section?)
  (or (inside? "chapter")
      (inside? "chapter*")
      (inside? "appendix")
      (inside? "section")
      (inside? "section*")
      (inside? "subsection")
      (inside? "subsection*")
      (inside? "subsubsection")
      (inside? "subsubsection*")
      (inside? "paragraph")
      (inside? "paragraph*")
      (inside? "subparagraph")
      (inside? "subparagraph*")))

(define (make-section l)
  (if (not (make-return-after))
      (make l)))

(define (make-unnamed-section l)
  (if (not (make-return-after))
      (make l)
      (make-return-before)))

(define (toggle-section-number)
  (cond ((inside? "chapter") (variant-replace "chapter" "chapter*"))
	((inside? "chapter*") (variant-replace "chapter*" "chapter"))
	((inside? "section") (variant-replace "section" "section*"))
	((inside? "section*") (variant-replace "section*" "section"))
	((inside? "subsection") (variant-replace "subsection" "subsection*"))
	((inside? "subsection*") (variant-replace "subsection*" "subsection"))
	((inside? "subsubsection")
	 (variant-replace "subsubsection" "subsubsection*"))
	((inside? "subsubsection*")
	 (variant-replace "subsubsection*" "subsubsection"))
	((inside? "paragraph") (variant-replace "paragraph" "paragraph*"))
	((inside? "paragraph*") (variant-replace "paragraph*" "paragraph"))
	((inside? "subparagraph")
	 (variant-replace "subparagraph" "subparagraph*"))
	((inside? "subparagraph*")
	 (variant-replace "subparagraph*" "subparagraph"))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Routines for lists, enumerations and description
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define list-itemize-enumerate
  '("itemize" "itemize-minus" "itemize-dot" "itemize-arrow"
    "enumerate" "enumerate-numeric" "enumerate-roman"
    "enumerate-Roman" "enumerate-alpha" "enumerate-Alpha"))

(define list-description
  '("description" "description-compact" "description-aligned"
    "description-dash" "description-long"))

(define (inside-list?)
  (not (== (inside-which list-itemize-enumerate) "")))

(define (inside-description?)
  (not (== (inside-which list-description) "")))

(define (make-tmlist l)
  (make l)
  (make-item))

(define (make-item)
  (if (not (make-return-after))
      (with l (inside-which (append list-itemize-enumerate list-description))
	(cond ((in? l list-itemize-enumerate) (make 'item))
	      ((in? l list-description) (make 'item*))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Routines for inserting miscellaneous content
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (make-aux env aux)
  (if (not (make-return-after))
      (insert-object (list (string->symbol env) aux '(document "")))))

(define (make-aux* env aux name)
  (if (not (make-return-after))
      (insert-object (list (string->symbol env) aux name '(document "")))))

(define (make-bib style file-name)
  (if (not (make-return-after))
      (insert-object
       (list 'bibliography "bib" style file-name '(document "")))))
