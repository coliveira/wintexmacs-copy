
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; arch-tag: 50b8b47d-3ab4-47b5-b0b6-adfcf98eff6e
;;
;; MODULE      : menu-proclus.scm
;; DESCRIPTION : Menus definitions for the 'proclus' plugin
;; COPYRIGHT   : (C) 2003--2004  Alain Herreman, David Allouche
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

(texmacs-module (menu menu-proclus)
  (:use (proclus) (proclus-types) (proclus-absname)
        (proclus-distill) (proclus-source) (proclus-absname-editor))
  (:export menu-proclus
           menu-proclus-links
           proclus-popup-menu))

(menu-bind menu-proclus-links
  (=> "Links"
      (when (in-proclus-target?)
            ("Active source" (active-source))
            ("Active but" (active-but)))
      ---
      (when (has-last-target?)
            ("Dernier locus" (go-to-last-target)))
      (when (has-source-buffer?)
            ("Document source" (go-to-source-buffer)))
      ("Constellation" (absname-editor))
      ---
      (when (has-source-buffer?)
            (link menu-proclus-types)
            ---
            (link menu-proclus-edition))))

(menu-bind menu-proclus
  (=> "Proclus"
      (when (has-absolute-name?)
            ("Locus" (target))
            (when (or (selection-active-any?) (in-proclus-target?))
                  ("Active source" (active-source))
                  ("Active but" (active-but))))
      (when (has-last-target?)
            ("Dernier locus" (go-to-last-target)))
      ---
      ("Constellation" (absname-editor))
      (when (has-conflicting-absolute-name?)
            ("Document d�plac�" (absolute-name-reregister-buffer)))
      (when (and (not (no-name?)) (not (has-absolute-name?)))
            ("Nommer ce document..." (interactive-absolute-name)))
      ("Apprendre un nom..." (absname-choose-file))
      ---
      (link menu-proclus-types)
      ---
      (when (in-proclus-editable?)
            ("Loci" (edit-targets)))
      (link menu-proclus-edition)))

(menu-bind menu-proclus-types
  (promise (type-menu-promise)))

(menu-bind menu-proclus-edition
  (when (in-proclus-target?)
        ("Liens" (edit-links))))

(menu-set! 'std-texmacs-popup-menu (menu-get 'texmacs-popup-menu))

(menu-bind proclus-popup-menu
  (if (in-proclus-target?)
      ("Activer source" (active-source))
      ("Activer but" (active-but))
      (link menu-proclus-types)
      ("Liens" (edit-links)))
  (if (not (in-proclus-target?))
      (link std-texmacs-popup-menu)))