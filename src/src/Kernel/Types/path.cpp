
/******************************************************************************
* MODULE     : path.cpp
* DESCRIPTION: paths are integer lists,
*              which are for instance useful to select subtrees in trees
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include "path.hpp"
#include "analyze.hpp"

/******************************************************************************
* General routines on paths
******************************************************************************/

bool
zero_path (path p) {
  if (nil (p)) return true;
  return (p->item == 0) && zero_path (p->next);
}

int
hash (path p) {
  if (nil (p)) return 0;
  else {
    int h= hash (p->next);
    return p->item ^ ((h<<7) + (h>>25));
  }
}

string
as_string (path p) {
  if (nil (p)) return "";
  if (atom (p)) return as_string (p->item);
  return as_string (p->item) * "." * as_string (p->next);
}

path
as_path (string s) {
  int i, j, n= N(s);
  for (i=0; i<n; i++)
    if (is_digit (s[i])) break;
  if (i==n) return path ();
  for (j=i; j<n; j++)
    if (!is_digit (s[j])) break;
  return path (as_int (s (i, j)), as_path (s (j, n)));
}

bool
version_inf_eq (string v1, string v2) {
  if (starts (v2, v1)) return true;
  return path_inf_eq (as_path (v1), as_path (v2));
}

bool
version_inf (string v1, string v2) {
  if (v1 == v2) return false;
  return version_inf_eq (v1, v2);
}

/******************************************************************************
* Operations on paths
******************************************************************************/

path
path_add (path p, int plus) {
  if (atom (p)) return path (p->item+plus);
  return path (p->item, path_add (p->next, plus));
}

path
path_add (path p, int plus, int pos) {
  p= copy (p);
  p[pos]+=plus;
  return p;
}

path
path_up (path p) {
  if (nil (p)) fatal_error ("path too short", "path_up", "path.cpp");
  if (nil (p->next)) return path ();
  return path (p->item, path_up (p->next));
}

path
path_up (path p, int times) {
  return head (p, N(p)-times);
}

bool
path_inf (path p1, path p2) {
  if (nil (p1) || nil (p2)) return false;
  if (p1->item<p2->item) return true;
  if (p1->item>p2->item) return false;
  return path_inf (p1->next, p2->next);
}

bool
path_inf_eq (path p1, path p2) {
  if (nil (p1) || nil (p2)) return (p1 == p2);
  if (p1->item<p2->item) return true;
  if (p1->item>p2->item) return false;
  return path_inf_eq (p1->next, p2->next);
}

bool
path_less (path p1, path p2) {
  return path_less_eq (p1, p2) && (p1 != p2);
}

bool
path_less_eq (path p1, path p2) {
  if (nil (p1) || nil (p2)) return p1 == p2;
  if (atom (p1) || atom (p2)) {
    if (atom (p1) && atom (p2)) return p1->item <= p2->item;
    if ((p1->item == 0) && nil (p1->next)) return true;
    if ((p2->item == 1) && nil (p2->next)) return true;
    return false;
  }
  if (p1->item<p2->item) return true;
  if (p1->item>p2->item) return false;
  return path_less_eq (p1->next, p2->next);
}

path
operator - (path p, path q) {
  if (nil (q)) return p;
  else if (nil (p) || (p->item != q->item))
    fatal_error ("Path did not start with required path", "path::operator -");
  else return p->next - q-> next;
  return path (); // NOT REACHED
}

path
common (path start, path end) {
  if (nil (start) || nil (end)) return path ();
  if (start->item != end->item) return path ();
  return path (start->item, common (start->next, end->next));
}

/******************************************************************************
* Main modification routines
******************************************************************************/

tree&
subtree (tree& t, path p) {
  if (nil (p)) return t;
  else return subtree (t[p->item], p->next);
}

tree&
parent_subtree (tree& t, path p) {
  if (nil (p)) fatal_error ("path too short", "parent_subtree");
  if (nil (p->next)) return t;
  else return parent_subtree (t[p->item], p->next);
}