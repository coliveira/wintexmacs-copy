
/******************************************************************************
* MODULE     : tag_info.hpp
* DESCRIPTION: DRD information about tags
* COPYRIGHT  : (C) 2003  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef TAG_INFO_H
#define TAG_INFO_H
#include "tree.hpp"

/******************************************************************************
* The parent_info class contains outer information about tags
*
* -  The arity fields together with the child_mode determine the possible
*    arities and how to convert logical indices of children to
*    physical indices in the array 'ci'.
*
*    o ARITY_NORMAL    : the arity is given by arity_base+arity_extra
*    o ARITY_OPTIONS   : arity_base <= arity < arity_base+arity_extra
*    o ARITY_REPEAT    : arity= arity_base + any_number * arity_extra
*    o ARITY_VAR_REPEAT: as ARITY_REPEAT, but repetition "comes first"
*
*    o CHILD_UNIFORM : all children have the same properties
*    o CHILD_BIFORM  : two types of properties (corresponds to base/extra)
*    o CHILD_DETAILED: as many as arity_base+arity_extra types of properties
*
* - The no_border field specifies whether the cursor may be put behind and
*   before the tag or not (the 0 and 1 paths). For instance, this field
*   is true for CONCAT and false for FRAC.
*
* - The block field specifies when the parent should be considered
*   as a block. In the case of BLOCK_OR, the parent is a block
*   if one of the children satisfying BLOCK_REQUIRE_NONE is a block.
*
* - The freeze_* fields specify that the contents of the corresponding
*   fields may not be overwritten during the heuristic determination of
*   missing drd information.
******************************************************************************/

#define ARITY_NORMAL          0
#define ARITY_OPTIONS         1
#define ARITY_REPEAT          2
#define ARITY_VAR_REPEAT      3

#define CHILD_UNIFORM         0
#define CHILD_BIFORM          1
#define CHILD_DETAILED        2

#define BLOCK_NO              0
#define BLOCK_YES             1
#define BLOCK_OR              2

struct parent_info {
  unsigned arity_mode       : 2; // arity layout
  unsigned arity_base       : 6; // base arity (minimal arity)
  unsigned arity_extra      : 4; // extra arity (optional, repeated, etc.)
  unsigned child_mode       : 2; // child layout
  unsigned no_border        : 1; // is the border inaccessible?
  unsigned block            : 2; // is a block structure?
  unsigned freeze_arity     : 1; // true => disable heuristic determination
  unsigned freeze_no_border : 1;
  unsigned freeze_block     : 1;

  parent_info (int arity, int extra, int amode, int cmode, bool frozen= false);
  parent_info (string s);
  inline ~parent_info () {}
  operator string ();
  bool operator == (const parent_info& pi);
  bool operator != (const parent_info& pi);
  friend ostream& operator << (ostream& out, parent_info pi);
};

/******************************************************************************
* The child_info class contains more detailed information about each of
* the children of the tag.
*
* - The accessible field specifies whether the field can be accessed.
*   ACCESSIBLE_ALWAYS children can always be accessed, ACCESSIBLE_NEVER
*   children can only be accessed in source mode and ACCESSIBLE_HIDDEN
*   children may require unfolding in order to be accessed.
*
* - The writability field specifies whether an accessible field can be edited.
*   When the writability of a child is disabled, its whole descendance
*   becomes read-only, except for those parts whose writability are re-enabled.
*
* - The block field specifies whether the field is required to be
*   a block structure, an inline structure, or any of the two.
*
* - The mode field specifies the mode for each child. In case of MODE_PARENT,
*   the mode of the child is the same as the mode of its parent.
*
* - The freeze_* fields specify that the contents of the corresponding
*   fields may not be overwritten during the heuristic determination of
*   missing drd information.
******************************************************************************/

#define ACCESSIBLE_NEVER      0
#define ACCESSIBLE_HIDDEN     1
#define ACCESSIBLE_ALWAYS     2

#define WRITABILITY_NORMAL    0
#define WRITABILITY_DISABLE   1
#define WRITABILITY_ENABLE    2

#define BLOCK_REQUIRE_BLOCK   0
#define BLOCK_REQUIRE_INLINE  1
#define BLOCK_REQUIRE_NONE    2

#define MODE_PARENT           0
#define MODE_TEXT             1
#define MODE_MATH             2
#define MODE_PROG             3
#define MODE_SRC              4

struct child_info {
  unsigned accessible        : 2; // child is accessible?
  unsigned writability       : 2; // writability of child
  unsigned block             : 2; // require children to be blocks?
  unsigned mode              : 3; // in which mode is the child?
  unsigned freeze_accessible : 1; // true => disable heuristic determination
  unsigned freeze_writability: 1;
  unsigned freeze_block      : 1;
  unsigned freeze_mode       : 1;

  child_info (bool frozen= false);
  child_info (string s);
  inline ~child_info () {}
  operator string ();
  bool operator == (const child_info& pi);
  bool operator != (const child_info& pi);
  friend ostream& operator << (ostream& out, child_info ci);
};

class tag_info;
class tag_info_rep: concrete_struct {
public:
  parent_info       pi;
  array<child_info> ci;
  tree              extra;

  tag_info_rep (parent_info pi, array<child_info> ci, tree extra);
  tag_info_rep (int arity, int extra, int amode, int cmode, bool frozen);
  inline ~tag_info_rep () {}

  tag_info no_border ();
  tag_info accessible (int i);
  tag_info hidden (int i);
  tag_info disable_writable (int i);
  tag_info enable_writable (int i);
  tag_info name (string s);
  int      get_index (int child, int n);
  void     set_attribute (string which, tree val);
  tree     get_attribute (string which);

  friend class tag_info;
};

/******************************************************************************
* The main tag_info class consists of parent_info and an array of child_info
******************************************************************************/

class tag_info {
  CONCRETE(tag_info);
  tag_info (parent_info pi, array<child_info> ci, tree extra);
  tag_info (int arity=0, int extra=0,
	    int am=ARITY_NORMAL, int cm= CHILD_UNIFORM,
	    bool frozen= false);
  tag_info (tree t);
  operator tree ();
  child_info& operator () (int child, int n);
};
CONCRETE_CODE(tag_info);

bool operator == (tag_info ti1, tag_info ti2);
bool operator != (tag_info ti1, tag_info ti2);
ostream& operator << (ostream& out, tag_info ti);
tag_info copy (tag_info ti);

#endif // defined TAG_INFO_H
