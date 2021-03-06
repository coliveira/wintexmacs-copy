
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : config-kbd.scm
;; DESCRIPTION : keyboard configuration
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (texmacs keyboard config-kbd)
  (:use (texmacs texmacs tm-server)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Prefix modifiers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define prefix-modifier-table (make-ahash-table))

(define default-modifier-done? #f)
(define default-alt-modifier "Mod1")
(define default-meta-modifier "Mod4")
(define default-hyper-modifier "Mod3")

(define (get-default-modifier key)
  (if (qt-gui?) ""
      (with s (eval-system (string-append "xmodmap | grep " key))
	(cond ((string-starts? s "mod")
	       (string-append "Mod" (substring s 3 4)))
	      (else "")))))

(define (init-default-modifiers)
  (let* ((alt (get-default-modifier "Alt_L"))
	 (meta (get-default-modifier "Meta_L"))
	 (super (get-default-modifier "Super_L"))
	 (hyper (get-default-modifier "Hyper_L")))
    (if (!= alt "")
	(set! default-alt-modifier alt))
    (if (and (!= meta "") (!= meta default-alt-modifier))
	(set! default-meta-modifier meta))
    (if (and (== meta "") (!= super "") (!= super default-alt-modifier))
	(set! default-meta-modifier super))
    (if (and (!= hyper "") (!= hyper default-alt-modifier)
	     (!= hyper default-meta-modifier))
	(set! default-hyper-modifier hyper))
    (set! default-modifier-done? #t)))

(define (notify-prefix-modifier var val)
  (if (not default-modifier-done?) (init-default-modifiers))
  (cond ((!= val "default")
	 (ahash-set! prefix-modifier-table var val)
	 (set-message
	  "Restart in order to let the new look and feel take effect"
	  "configure look and feel"))
	((== var "A") (notify-prefix-modifier var default-alt-modifier))
	((== var "M") (notify-prefix-modifier var default-meta-modifier))
	((== var "H") (notify-prefix-modifier var default-hyper-modifier))))

(define-preferences
  ("A" "default" notify-prefix-modifier)
  ("M" "default" notify-prefix-modifier)
  ("H" "default" notify-prefix-modifier))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Prefix modifiers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (get-prefix-modifier s)
  (ahash-ref prefix-modifier-table s))

(define (get-prefix-modifiers l)
  (list-sort (map-in-order get-prefix-modifier l) string>?))

(define (modifier-list->string l)
  (apply string-append (map-in-order (lambda (s) (string-append s "-")) l)))

(define (compute-wildcard-line l)
  (let* ((r (get-prefix-modifiers l))
	 (L (modifier-list->string l))
	 (R (modifier-list->string r)))
    (list R L #t)))

(tm-define (compute-wildcard-lines)
  (map-in-order compute-wildcard-line
   '(("A")
     ("M")
     ("H")
     ("M" "A")
     ("H" "A")
     ("H" "M")
     ("H" "M" "A"))))

(define (xmodmap s)
  (system (string-append "xmodmap -e \"" s "\"")))

(define (notify-key-prefix var val)
  (when (x-gui?)
    (let* ((Modx (get-prefix-modifier val)))
      ;;(display* var " -> " val " -> " Modx "\n")
      (cond ((== Modx #f) (noop))
	    ((== var "alt")
	     (xmodmap (string-append "clear " Modx))
	     (xmodmap (string-append "add " Modx " = Alt_L"))
	     (xmodmap (string-append "add " Modx " = Alt_R")))
	    ((== var "meta")
	     (xmodmap (string-append "clear " Modx))
	     (xmodmap (string-append "add " Modx " = Meta_L"))
	     (xmodmap (string-append "add " Modx " = Meta_R")))
	    ((== var "windows")
	     (xmodmap "keycode 115 = Super_L")
	     (xmodmap "keycode 116 = Super_R")
	     (xmodmap (string-append "clear " Modx))
	     (xmodmap (string-append "add " Modx " = Super_L"))
	     (xmodmap (string-append "add " Modx " = Super_R")))
	    ((== var "caps-lock")
	     (xmodmap "clear Lock")
	     (xmodmap (string-append "clear " Modx))
	     (xmodmap (string-append "add " Modx " = Caps_Lock")))))))

(define-preferences
  ("alt" "default" notify-key-prefix)
  ("meta" "default" notify-key-prefix)
  ("windows" "default" notify-key-prefix)
  ("caps-lock" "default" notify-key-prefix))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Cyrillic input method
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (notify-cyrillic-input-method var val)
  (cond
   ((== val "translit")
    (lazy-keyboard (text cyrillic translit-kbd) in-cyrillic-translit?))
   ((== val "jcuken")
    (lazy-keyboard (text cyrillic jcuken-kbd) in-cyrillic-jcuken?))
   ((== val "yawerty")
    (lazy-keyboard (text cyrillic yawerty-kbd) in-cyrillic-yawerty?))
   ((== val "koi8-r")
    (lazy-keyboard (text cyrillic koi8-kbd) in-cyrillic-koi8?))
   ((== val "cp1251")
    (lazy-keyboard (text cyrillic cp1251-kbd) in-cyrillic-cp1251?))))

(define-preferences
  ("cyrillic input method" "" notify-cyrillic-input-method))
