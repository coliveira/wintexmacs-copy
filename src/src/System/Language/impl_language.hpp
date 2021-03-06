
/******************************************************************************
* MODULE     : language_internals.h
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef IMPL_LANGUAGE_H
#define IMPL_LANGUAGE_H
#include "language.hpp"

extern text_property_rep tp_normal_rep;
extern text_property_rep tp_hyph_rep;
extern text_property_rep tp_space_rep;
extern text_property_rep tp_dspace_rep;
extern text_property_rep tp_blank_rep;
extern text_property_rep tp_period_rep;
extern text_property_rep tp_operator_rep;
extern text_property_rep tp_shortop_rep;

struct verb_language_rep: language_rep {
  verb_language_rep (string name);
  text_property advance (string s, int& pos);
  array<int> get_hyphens (string s);
  void hyphenate (string s, int after, string& left, string& right);
};

struct scheme_language_rep: language_rep {
  hashmap<string,string> colored;
  scheme_language_rep (string name);
  text_property advance (string s, int& pos);
  array<int> get_hyphens (string s);
  void hyphenate (string s, int after, string& left, string& right);
  string get_color (tree t, int start, int end);
};

struct mathemagix_language_rep: language_rep {
  hashmap<string,string> colored;
  mathemagix_language_rep (string name);
  text_property advance (string s, int& pos);
  array<int> get_hyphens (string s);
  void hyphenate (string s, int after, string& left, string& right);
  string get_color (tree t, int start, int end);
};

struct cpp_language_rep: language_rep {
  hashmap<string,string> colored;
  cpp_language_rep (string name);
  text_property advance (string s, int& pos);
  array<int> get_hyphens (string s);
  void hyphenate (string s, int after, string& left, string& right);
  string get_color (tree t, int start, int end);
};

#endif // defined IMPL_LANGUAGE_H
