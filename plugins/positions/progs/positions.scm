
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; arch-tag: be376a3f-243d-45aa-a4a5-e326e833f550
;;
;; MODULE      : positions.scm
;; DESCRIPTION : User interface for the position facility in texmacs.
;; COPYRIGHT   : (C) 2003, 2004  David Allouche
;;
;;   This program is free software; you can redistribute it and/or modify
;;   it under the terms of the GNU General Public License as published by
;;   the Free Software Foundation; either version 2 of the License, or
;;   (at your option) any later version.
;;
;;   You should have received a copy of the GNU General Public License
;;   along with this program; if not, write to the Free Software
;;   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (positions)
  (:use (interactive-proc))
  (:export position-remember-ia position-jump-ia position-forget-ia))

(define positions (make-ahash-table))

(define (recall-position name)
  (let ((pos (ahash-ref positions name)))
    (if (not pos) (set-message
		   (string-append "Error: no position by this name: " name)
		   "error"))
    pos))

(define (position-remember-ia)
  (interactive-proc '("Remember position as:")
		    position-remember/callback))

(define (position-remember/callback name)
  (let ((pos (tm-position-new)))
    (tm-position-set pos (tm-where))
    (ahash-set! positions name pos)))

(define (position-jump-ia)
  (interactive-proc '("Jump to position:")
		    position-jump/callback))

(define (position-jump/callback name)
  (cond ((recall-position name) =>
	 (lambda (pos) (tm-go-to (tm-position-get pos))))))

(define (position-forget-ia)
  (interactive-proc '("Forget position:")
		    position-forget/callback))

(define (position-forget/callback name)
  (cond ((recall-position name) =>
	 (lambda (pos)
	   (tm-position-delete pos)
	   (ahash-remove! positions name)))))