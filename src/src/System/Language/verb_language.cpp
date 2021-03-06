
/******************************************************************************
* MODULE     : verb_language.cpp
* DESCRIPTION: the "verbatim" language
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "analyze.hpp"
#include "impl_language.hpp"
#include "Scheme/object.hpp"

verb_language_rep::verb_language_rep (string name):
  language_rep (name) {}

text_property
verb_language_rep::advance (string s, int& pos) {
  if (pos==N(s)) return &tp_normal_rep;
  if (s[pos]==' ') {
    pos++;
    return &tp_space_rep;
  }
  if (s[pos]=='-') {
    pos++;
    return &tp_hyph_rep;
  }
  while ((pos<N(s)) && (s[pos]!=' ') && (s[pos]!='-')) pos++;
  return &tp_normal_rep;
}

array<int>
verb_language_rep::get_hyphens (string s) {
  int i;
  array<int> penalty (N(s)+1);
  for (i=0; i<N(penalty); i++) penalty[i]= HYPH_INVALID;
  return penalty;
}

void
verb_language_rep::hyphenate (
  string s, int after, string& left, string& right)
{ 
  left = s(0, after);
  right= s(after, N(s));
}

/******************************************************************************
* Interface
******************************************************************************/

language
prog_language (string s) {
  if (language::instances -> contains (s)) return language (s);
  if (s == "scheme")
    return make (language, s, tm_new<scheme_language_rep> (s));
  if (s == "mathemagix")
    return make (language, s, tm_new<mathemagix_language_rep> (s));
  if (s == "cpp")
    return make (language, s, tm_new<cpp_language_rep> (s));
  return make (language, s, tm_new<verb_language_rep> (s));
}
