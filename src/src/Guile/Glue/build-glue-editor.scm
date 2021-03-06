
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : build-glue-editor.scm
;; DESCRIPTION : Building basic glue for the editor
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(output-copyright "build-glue-editor.scm")

(build
  "get_server()->get_editor()->"
  "initialize_glue_editor"

  ;; important paths and trees
  (root-tree the_root (tree))
  (buffer-path the_buffer_path (path))
  (buffer-tree the_buffer (tree))
  (paragraph-tree the_line (tree))
  (cursor-path the_path (path))
  (selection-tree selection_get (tree))

  ;; low-level modification routines
  (path->tree the_subtree (tree path))
  (path-correct correct (void path))
  (path-insert-with insert_with (void path string content))
  (path-remove-with remove_with (void path string))

  (position-new-path position_new (observer path))
  (position-delete position_delete (void observer))
  (position-set position_set (void observer path))
  (position-get position_get (path observer))

  ;; general modification routines
  (inside? inside (bool tree_label))
  (insert insert_tree (void content))
  (insert-go-to var_insert_tree (void content path))
  (insert-raw-go-to insert_tree (void content path))
  (insert-raw-return insert_return (void))
  (remove-text remove_text (void bool))
  (remove-structure remove_structure (void bool))
  (remove-structure-upwards remove_structure_upwards (void))

  (make make_compound (void tree_label))
  (make-arity make_compound (void tree_label int))
  (activate activate (void))
  (insert-argument insert_argument (void bool))
  (remove-argument remove_argument (void bool))
  (make-with make_with (void string string))
  (make-mod-active make_mod_active (void tree_label))
  (make-style-with make_style_with (void string string))
  (make-hybrid make_hybrid (void))
  (activate-latex activate_latex (void))
  (activate-hybrid activate_hybrid (void bool))
  (activate-symbol activate_symbol (void))
  (activate-compound activate_compound (void))
  (make-return-before make_return_before (void))
  (make-return-after make_return_after (bool))
  (temp-proof-fix temp_proof_fix (void))

  ;; document-wide modifications
  (init-default-one init_default (void string))
  (init-env init_env (void string string))
  (init-env-tree init_env (void string content))
  (init-style init_style (void string))
  (init-add-package init_add_package (void string))
  (init-remove-package init_remove_package (void string))
  (get-style-tree get_style (tree))
  (set-style-tree set_style (void tree))
  (get-env get_env_string (string string))
  (get-env-tree get_env_value (tree string))
  (get-env-tree-at get_env_value (tree string path))
  (get-init-tree get_init_value (tree string))
  (context-has? defined_at_cursor (bool string))
  (style-has? defined_at_init (bool string))
  (init-has? defined_in_init (bool string))

  ;; modify text
  (make-htab make_htab (void string))
  (make-space make_space (void string))
  (make-var-space make_space (void string string string))
  (make-hspace make_hspace (void string))
  (make-var-hspace make_hspace (void string string string))
  (make-vspace-before make_vspace_before (void string))
  (make-var-vspace-before make_vspace_before
    (void string string string))
  (make-vspace-after make_vspace_after (void string))
  (make-var-vspace-after make_vspace_after (void string string string))
  (make-move make_move (void string string))
  (make-resize make_resize (void string string string string))
  (make-postscript make_postscript
    (void string bool string string string string string string))

  (length-decode as_length (int string))
  (length-add add_lengths (string string string))
  (length-mult multiply_length (string double string))
  (length? is_length (bool string))
  (length-divide divide_lengths (double string string))

  ;; modify mathematics
  (make-group make_group (void))
  (make-lprime make_lprime (void string))
  (make-rprime make_rprime (void string))
  (make-below make_below (void))
  (make-above make_above (void))
  (make-script make_script (void bool bool))
  (make-fraction make_fraction (void))
  (make-sqrt make_sqrt (void))
  (make-wide make_wide (void string))
  (make-wide-under make_wide_under (void string))
  (make-var-sqrt make_var_sqrt (void))
  (make-neg make_neg (void))
  (make-tree make_tree (void))

  ;; modify tables
  (make-subtable make_subtable (void))
  (table-disactivate table_disactivate (void))
  (table-extract-format table_extract_format (void))
  (table-insert-row table_insert_row (void bool))
  (table-insert-column table_insert_column (void bool))
  (table-remove-row table_remove_row (void bool))
  (table-remove-column table_remove_column (void bool))
  (table-nr-rows table_nr_rows (int))
  (table-nr-columns table_nr_columns (int))
  (table-which-row table_which_row (int))
  (table-which-column table_which_column (int))
  (table-cell-path table_search_cell (path int int))
  (table-go-to table_go_to (void int int))
  (table-set-format table_set_format (void string string))
  (table-get-format table_get_format (string string))
  (table-del-format table_del_format (void string))
  (table-row-decoration table_row_decoration (void bool))
  (table-column-decoration table_column_decoration (void bool))
  (table-format-center table_format_center (void))
  (table-correct-block-content table_correct_block_content (void))
  (set-cell-mode set_cell_mode (void string))
  (get-cell-mode get_cell_mode (string))
  (cell-set-format cell_set_format (void string string))
  (cell-get-format cell_get_format (string string))
  (cell-del-format cell_del_format (void string))
  (table-test table_test (void))

  ;; keyboard handling
  (key-press key_press (void string))
  (emulate-keyboard emulate_keyboard (void string))
  (emulate-keyboard-message emulate_keyboard (void string string))
  (complete-try? complete_try (bool))
  
  ;; moving the cursor
  (go-to-path go_to (void path))
  (go-left go_left (void))
  (go-right go_right (void))
  (go-up go_up (void))
  (go-down go_down (void))
  (go-start go_start (void))
  (go-end go_end (void))
  (go-start-of go_start_of (void tree_label))
  (go-end-of go_end_of (void tree_label))
  (go-start-with go_start_with (void string string))
  (go-end-with go_end_with (void string string))
  (go-start-line go_start_line (void))
  (go-end-line go_end_line (void))
  (go-page-up go_page_up (void))
  (go-page-down go_page_down (void))
  (go-start-paragraph go_start_paragraph (void))
  (go-end-paragraph go_end_paragraph (void))
  (go-to-label go_to_label (void string))
  (cursor-show-if-hidden show_cursor_if_hidden (void))

  ;; selections
  (select-all select_all (void))
  (select-line select_line (void))
  (select-from-cursor select_from_cursor (void))
  (select-from-keyboard select_from_keyboard (void bool))
  (select-from-shift-keyboard select_from_shift_keyboard (void))
  (select-enlarge select_enlarge (void))
  (select-enlarge-environmental select_enlarge_environmental (void))
  (selection-active-any? selection_active_any (bool))
  (selection-active-normal? selection_active_normal (bool))
  (selection-active-table? selection_active_table (bool))
  (selection-active-small? selection_active_small (bool))
  (selection-active-enlarging? selection_active_enlarging (bool))
  (selection-set-start selection_set_start (void))
  (selection-set-end selection_set_end (void))
  (selection-get-start selection_get_start (path))
  (selection-get-end selection_get_end (path))
  (selection-path selection_get_path (path))
  (selection-set-start-path selection_set_start (void path))
  (selection-set-end-path selection_set_end (void path))
  (clipboard-copy selection_copy (void string))
  (clipboard-cut selection_cut (void string))
  (clipboard-cut-at cut (void path))
  (clipboard-paste selection_paste (void string))
  (selection-move selection_move (void))
  (clipboard-clear selection_clear (void string))
  (selection-cancel selection_cancel (void))
  (clipboard-set-import selection_set_import (void string))
  (clipboard-set-export selection_set_export (void string))
  (clipboard-get-import selection_get_import (string))
  (clipboard-get-export selection_get_export (string))

  ;; undo and redo
  (remove-undo-mark remove_undo_mark (void))
  (add-undo-mark add_undo_mark (void))
  (unredoable-undo unredoable_undo (void))
  (forget-undo forget_undo (void))
  (undo undo (void))
  (redo redo (void))

  ;; graphics
  (in-graphics? inside_graphics (bool))
  (get-graphical-x get_x (double))
  (get-graphical-y get_y (double))
  (get-graphical-object get_graphical_object (tree))
  (set-graphical-object set_graphical_object (void tree))
  (invalidate-graphical-object invalidate_graphical_object (void))
  (graphical-select graphical_select (tree double double))
  (graphical-select-area graphical_select (tree double double double double))

  ;; search, replace and spell checking
  (in-normal-mode? in_normal_mode (bool))
  (in-search-mode? in_search_mode (bool))
  (in-replace-mode? in_replace_mode (bool))
  (in-spell-mode? in_spell_mode (bool))
  (search-start search_start (void bool))
  (search-button-next search_button_next (void))
  (replace-start replace_start (void string string bool))
  (spell-start spell_start (void))
  (spell-replace spell_replace (void string))

  ;; sessions
  (session-complete-try? session_complete_try (bool tree))

  ;; miscellaneous routines
  (view-set-property set_property (void scheme_tree scheme_tree))
  (view-get-property get_property (scheme_tree scheme_tree))
  (clear-buffer clear_buffer (void))
  (tex-buffer tex_buffer (void))
  (clear-local-info clear_local_info (void))
  (update-path typeset_invalidate (void path))
  (update-buffer typeset_invalidate_all (void))
  (generate-all-aux generate_aux (void))
  (generate-aux generate_aux (void string))
  (notify-page-change notify_page_change (void))
  (print-to-file print_to_file (void url))
  (print-pages-to-file print_to_file (void url string string))
  (print print_buffer (void))
  (print-pages print_buffer (void string string))
  (print-snippet print_snippet (array_int url content))
  (export-postscript export_ps (void url))
  (export-pages-postscript export_ps (void url string string))
  (footer-eval footer_eval (void string))
  (texmacs-exec texmacs_exec (tree content))
  (idle-time idle_time (int))
  (change-time change_time (int))
  (menu-before-action before_menu_action (void))
  (menu-after-action after_menu_action (void))

  (show-tree show_tree (void))
  (show-env show_env (void))
  (show-path show_path (void))
  (show-cursor show_cursor (void))
  (show-selection show_selection (void))
  (show-meminfo show_meminfo (void))
  (edit-special edit_special (void))
  (edit-test edit_test (void))

  (set-mouse-pointer set_pointer (void string string))
  (set-predef-mouse-pointer set_pointer (void string)))
