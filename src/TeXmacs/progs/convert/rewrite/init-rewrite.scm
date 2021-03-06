
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : init-rewrite.scm
;; DESCRIPTION : setup texmacs converters
;; COPYRIGHT   : (C) 2003  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (convert rewrite init-rewrite))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The main TeXmacs format
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (texmacs-recognizes? s)
  (and (string? s)
       (or (string-starts? s "<TeXmacs")
	   (string-starts? s "\\(\\)(TeXmacs")
	   (string-starts? s "TeXmacs")
	   (string-starts? s "edit"))))

(define-format texmacs
  (:name "TeXmacs")
  (:suffix "tm" "ts" "tp")
  (:must-recognize texmacs-recognizes?))

(converter texmacs-tree texmacs-stree
  (:function tree->stree))

(converter texmacs-stree texmacs-tree
  (:function stree->tree))

(converter texmacs-document texmacs-tree
  (:function parse-texmacs))

(converter texmacs-tree texmacs-document
  (:function serialize-texmacs))

(converter texmacs-snippet texmacs-tree
  (:function parse-texmacs-snippet))

(converter texmacs-tree texmacs-snippet
  (:function serialize-texmacs-snippet))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Scheme format for TeXmacs (no information loss)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (stm-recognizes? s)
  (and (string? s) (string-starts? s "(document (TeXmacs")))

(define-format stm
  (:name "Scheme")
  (:suffix "stm" "scm")
  (:must-recognize stm-recognizes?))

(converter texmacs-tree stm-document
  (:function texmacs->stm))

(converter stm-document texmacs-tree
  (:function stm->texmacs))

(converter texmacs-tree stm-snippet
  (:function texmacs->stm))

(converter stm-snippet texmacs-tree
  (:function stm-snippet->texmacs))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Verbatim
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (texmacs->verbatim x . opts)
  (if (list-1? opts) (set! opts (car opts)))
  (let* ((wrap? (== (assoc-ref opts "texmacs->verbatim:wrap") "on"))
	 (enc (or (assoc-ref opts "texmacs->verbatim:encoding") "iso-8859-1")))
    (cpp-texmacs->verbatim x wrap? enc)))

(tm-define (verbatim->texmacs x . opts)
  (if (list-1? opts) (set! opts (car opts)))
  (let* ((wrap? (== (assoc-ref opts "verbatim->texmacs:wrap") "on"))
	 (enc (or (assoc-ref opts "verbatim->texmacs:encoding") "iso-8859-1")))
    (cpp-verbatim->texmacs x wrap? enc)))

(tm-define (verbatim-snippet->texmacs x . opts)
  (if (list-1? opts) (set! opts (car opts)))
  (let* ((wrap? (== (assoc-ref opts "verbatim->texmacs:wrap") "on"))
	 (enc (or (assoc-ref opts "verbatim->texmacs:encoding") "iso-8859-1")))
    (cpp-verbatim-snippet->texmacs x wrap? enc)))

(define-format verbatim
  (:name "Verbatim")
  (:suffix "txt"))

(converter verbatim-document texmacs-tree
  (:function-with-options verbatim->texmacs)
  (:option "verbatim->texmacs:wrap" "off")
  (:option "verbatim->texmacs:encoding" "utf-8"))

(converter verbatim-snippet texmacs-tree
  (:function-with-options verbatim-snippet->texmacs)
  (:option "verbatim->texmacs:wrap" "off")
  (:option "verbatim->texmacs:encoding" "utf-8"))

(converter texmacs-tree verbatim-document
  (:function-with-options texmacs->verbatim)
  (:option "texmacs->verbatim:wrap" "off")
  (:option "texmacs->verbatim:encoding" "utf-8"))

(converter texmacs-tree verbatim-snippet
  (:function-with-options texmacs->verbatim)
  (:option "texmacs->verbatim:wrap" "off")
  (:option "texmacs->verbatim:encoding" "utf-8"))
