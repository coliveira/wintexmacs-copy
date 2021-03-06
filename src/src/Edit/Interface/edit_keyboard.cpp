
/******************************************************************************
* MODULE     : edit_keyboard.cpp
* DESCRIPTION: Keyboard handling
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "edit_interface.hpp"
#include "analyze.hpp"
#include "tm_buffer.hpp"

/******************************************************************************
* Basic subroutines for keyboard handling
******************************************************************************/

int
edit_interface_rep::get_input_mode () {
  return input_mode;
}

void
edit_interface_rep::set_input_mode (int mode) {
  sh_s  = string ("");    // avoids keyboard shorthands when
  sh_len= 0;              // using the menu between two keystrokes

  if ((mode == INPUT_NORMAL) && (input_mode != INPUT_NORMAL)) {
    selection_cancel ();
    completions= array<string> ();
  }
  input_mode= mode;
}

void
edit_interface_rep::set_input_normal () {
  set_input_mode (INPUT_NORMAL);
}

bool
edit_interface_rep::in_normal_mode () {
  return input_mode == INPUT_NORMAL;
}

bool
edit_interface_rep::in_search_mode () {
  return input_mode == INPUT_SEARCH;
}

bool
edit_interface_rep::in_replace_mode () {
  return input_mode == INPUT_REPLACE;
}

bool
edit_interface_rep::in_spell_mode () {
  return input_mode == INPUT_SPELL;
}

bool
edit_interface_rep::kbd_get_command (string which, string& help, command& c) {
  return sv->kbd_get_command (which, help, c);
}

/******************************************************************************
* Main keyboard routines
******************************************************************************/

bool
edit_interface_rep::try_shortcut (string comb) {
  int     status;
  command cmd;
  string  shorth;
  string  help;


  sv->get_keycomb (comb, status, cmd, shorth, help);
  if ((status & 1) == 1) {
    if (sh_len>0) {
      tp= path_add (tp, -sh_len);
      remove (tp, sh_len);
    }
    cmd ();
    sh_s  = string ("");
    sh_len= 0;
    return true;
  }
  if ((status & 2) == 2) {
    sh_s= comb;
    if (sh_len>0) {
      tp= path_add (tp, -sh_len);
      if (sh_len>0) remove (tp, sh_len);
    }
    if (N(shorth)>0) insert_tree (shorth);
    sh_len= N (shorth);
    string rew= sv->kbd_post_rewrite (sh_s);
    if (N(help)>0) set_message (help, rew);
    else set_message ("keyboard shorthand: " * rew, shorth);
    return true;
  }

  return false;
}

static string
simplify_key_press (string key) {
  while ((N(key) >= 5) && (key(0,3) == "Mod") && (key[4] == '-') &&
	 (key[3] >= '1') && (key[3] <= '5')) key= key (5, N(key));
  if (key == "space") key= " ";
  return key;
}

void
edit_interface_rep::key_press (string key) {
  if (contains_unicode_char (key)) {
    if (input_mode == INPUT_NORMAL) insert_tree (key);
    return;
  }

  set_message ("", "");
  if (input_mode != INPUT_NORMAL)
    key= simplify_key_press (key);
  switch (input_mode) {
  case INPUT_NORMAL:
    break;
  case INPUT_SEARCH:
    if (search_keypress (key)) return;
    break;
  case INPUT_REPLACE:
    if (replace_keypress (key)) return;
    break;
  case INPUT_SPELL:
    if (spell_keypress (key)) return;
    break;
  case INPUT_COMPLETE:
    if (complete_keypress (key)) return;
    set_input_normal ();
    break;
  }

  string new_sh= N(sh_s)==0? key: sh_s * " " * key;
  if (try_shortcut (new_sh)) return;
  if (new_sh != key) {
    sh_s  = string ("");
    sh_len= 0;
    if (try_shortcut (key)) return;
  }

  string rew= sv->kbd_post_rewrite (key);
  if (N(rew)==1) {
    int i ((unsigned char) rew[0]);
    if ((((i >= 32) && (i <= 127)) || ((i >= 128) && (i <= 255))) &&
	!inside_active_graphics ())
      insert_tree (rew);
    sh_s  = string ("");
    sh_len= 0;
  }
}

void
edit_interface_rep::emulate_keyboard (string keys, string action) {
  string s= keys;
  while (s != "") {
    int i;
    for (i=1; i<N(s); i++)
      if (s[i]==' ') break;
    key_press (s (0, i));
    if (i<N(s)) i++;
    s= s (i, N(s));
  }
  if (N(action) != 0)
    set_message ("You can also obtain#" * action *
		 "#by typing#" * keys, action);
}

/******************************************************************************
* Event handlers
******************************************************************************/

void
edit_interface_rep::handle_keypress (string key, time_t t) {
  mark_undo_blocks ();
  key_press (key); (void) t;
  notify_change (THE_DECORATIONS);
}

void
edit_interface_rep::handle_keyboard_focus (bool has_focus, time_t t) {
  got_focus= has_focus; (void) t;
  notify_change (THE_FOCUS);
  if (got_focus) {
    focus_on_this_editor ();
    notify_change (THE_DECORATIONS);
  }
}
