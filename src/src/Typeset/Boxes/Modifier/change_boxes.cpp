
/******************************************************************************
* MODULE     : change.cpp
* DESCRIPTION: Boxes whose behaviour is changed
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "Boxes/composite.hpp"
#include "Boxes/construct.hpp"
#include "scheme.hpp"

/******************************************************************************
* changing the behaviour of a box
******************************************************************************/

struct change_box_rep: public composite_box_rep {
  bool child_flag, big_flag;
  change_box_rep (path ip, bool child_flag, bool big_flag= false);
  operator tree () { return tree (TUPLE, "change", (tree) bs[0]); }
  int find_child (SI x, SI y, SI delta, bool force);
  path find_left_box_path () {
    return child_flag?
      composite_box_rep::find_left_box_path ():
      path (0, bs[0]->find_left_box_path ()); }
  path find_right_box_path () {
    return child_flag?
      composite_box_rep::find_right_box_path ():
      path (0, bs[0]->find_right_box_path ()); }

  double left_slope () {
    return big_flag? bs[0]->left_slope(): box_rep::left_slope(); }
  double right_slope () { 
    return big_flag? bs[0]->right_slope(): box_rep::right_slope(); }
  SI left_correction () {
    return big_flag? bs[0]->left_correction():box_rep::left_correction(); }
  SI right_correction () {
    return big_flag? bs[0]->right_correction():box_rep::right_correction(); }
  SI lsub_correction () {
    return big_flag? bs[0]->lsub_correction(): box_rep::lsub_correction(); }
  SI lsup_correction () {
    return big_flag? bs[0]->lsup_correction(): box_rep::lsup_correction(); }
  SI rsub_correction () {
    return big_flag? bs[0]->rsub_correction(): box_rep::rsub_correction(); }
  SI rsup_correction () {
    return big_flag? bs[0]->rsup_correction(): box_rep::rsup_correction(); }
  SI sub_lo_base (int l) {
    return big_flag? bs[0]->sub_lo_base (l): box_rep::sub_lo_base (l); }
  SI sub_hi_lim  (int l) {
    return big_flag? bs[0]->sub_hi_lim (l): box_rep::sub_hi_lim (l); }
  SI sup_lo_lim  (int l) {
    return big_flag? bs[0]->sup_lo_lim (l): box_rep::sup_lo_lim (l); }
  SI sup_lo_base (int l) {
    return big_flag? bs[0]->sup_lo_base (l): box_rep::sup_lo_base (l); }
  SI sup_hi_lim  (int l) {
    return big_flag? bs[0]->sup_hi_lim (l): box_rep::sup_hi_lim (l); }

  SI get_leaf_offset (string search) {
    return sx1(0) + bs[0]->get_leaf_offset (search); }

  gr_selections graphical_select (SI x, SI y, SI dist);
  gr_selections graphical_select (SI x1, SI y1, SI x2, SI y2);
};

change_box_rep::change_box_rep (path ip, bool fl1, bool fl2):
  composite_box_rep (ip), child_flag (fl1), big_flag (fl2) {}

int
change_box_rep::find_child (SI x, SI y, SI delta, bool force) {
  if (child_flag) return composite_box_rep::find_child (x, y, delta, force);
  return 0;
}

gr_selections
change_box_rep::graphical_select (SI x, SI y, SI dist) { 
//TODO : Check if it is correct
  if (child_flag)
    return composite_box_rep::graphical_select (x, y, dist);
  else
    return bs[0]->graphical_select (x- sx(0), y- sy(0), dist);
}

gr_selections
change_box_rep::graphical_select (SI x1, SI y1, SI x2, SI y2) {
//TODO : Check if it is correct
  if (child_flag)
    return composite_box_rep::graphical_select (x1, y1, x2, y2);
  else
    return bs[0]->graphical_select (x1- sx(0), y1- sy(0),
				    x2- sx(0), y2- sy(0));
}

/******************************************************************************
* Moving boxes
******************************************************************************/

struct move_box_rep: public change_box_rep {
  move_box_rep (path ip, box b, SI x, SI y, bool fl1, bool fl2);
  int get_type () { return MOVE_BOX; }
  operator tree () { return tree (TUPLE, "move", (tree) bs[0]); }
};

move_box_rep::move_box_rep (path ip, box b, SI x, SI y, bool fl1, bool fl2):
  change_box_rep (ip, fl1, fl2)
{
  insert (b, x, y);
  position ();
  finalize ();
}

/******************************************************************************
* Resizing boxes
******************************************************************************/

struct resize_box_rep: public change_box_rep {
  resize_box_rep (path ip, box b, SI x1, SI y1, SI x2, SI y2,
		  bool child_flag, bool adjust);
  operator tree () { return tree (TUPLE, "resize", (tree) bs[0]); }
};

resize_box_rep::resize_box_rep (
  path ip, box b, SI X1, SI Y1, SI X2, SI Y2, bool child_flag, bool adjust):
    change_box_rep (ip, child_flag)
{
  insert (b, 0, 0);
  position ();
  x1= X1; y1= Y1;
  x2= X2; y2= Y2;
  if (adjust) left_justify ();
  finalize ();
}

struct vcorrect_box_rep: public change_box_rep {
  vcorrect_box_rep (path ip, box b, SI top_cor, SI bot_cor);
  operator tree () { return tree (TUPLE, "vcorrect", (tree) bs[0]); }
};

vcorrect_box_rep::vcorrect_box_rep (path ip, box b, SI top_cor, SI bot_cor):
  change_box_rep (ip, false, false)
{
  insert (b, 0, -top_cor);
  position ();
  y1 -= bot_cor;
  y2 += top_cor;
  finalize ();
}

/******************************************************************************
* Clipped boxes
******************************************************************************/

struct clip_box_rep: public change_box_rep {
  tree xt, yt;
  SI old_clip_x1, old_clip_x2, old_clip_y1, old_clip_y2;
public:
  clip_box_rep (path ip, box b, SI x1, SI y1, SI x2, SI y2,
		tree xt, tree yt, SI scx, SI scy);
  operator tree () { return tree (TUPLE, "clip", (tree) bs[0]); }
  int get_type () {
    return xt!=UNINIT || yt!=UNINIT? SCROLL_BOX: change_box_rep::get_type(); }
  tree get_info (tree in) {
    if (in == "scroll-x") return xt;
    else if (in == "scroll-y") return yt;
    else return box_rep::get_info (in); }
  void pre_display (renderer &ren);
  void post_display (renderer &ren);
  selection find_selection (path lbp, path rbp);
};

clip_box_rep::clip_box_rep (
  path ip, box b, SI X1, SI Y1, SI X2, SI Y2,
  tree xt2, tree yt2, SI scx, SI scy):
  change_box_rep (ip, true), xt (xt2), yt (yt2)
{
  insert (b, scx, scy);
  position ();
  x1= X1; y1= Y1;
  x2= X2; y2= Y2;
  // left_justify ();
  finalize ();
}

void
clip_box_rep::pre_display (renderer &ren) {
  ren->get_clipping (old_clip_x1, old_clip_y1, old_clip_x2, old_clip_y2);
  ren->extra_clipping (x1, y1, x2, y2);
}

void
clip_box_rep::post_display (renderer &ren) {
  ren->set_clipping (
    old_clip_x1, old_clip_y1, old_clip_x2, old_clip_y2, true);
}

selection
clip_box_rep::find_selection (path lbp, path rbp) {
  selection sel= change_box_rep::find_selection (lbp, rbp);
  return selection (sel->rs & rectangle (x1, y1, x2, y2),
		    sel->start, sel->end, sel->valid);
}

/******************************************************************************
* horizontally repetition of one box inside another one
******************************************************************************/

struct repeat_box_rep: public change_box_rep {
  repeat_box_rep (path ip, box b, box repeat, SI xoff);
  operator tree () { return tree (TUPLE, "repeat", (tree) bs[0]); }
};

repeat_box_rep::repeat_box_rep (path ip, box b, box repeat, SI xoff):
  change_box_rep (ip, false)
{
  insert (b, 0, 0);
  position ();

  SI width= repeat->w ();
  if (width >= PIXEL) {
    int i;
    int i1= ((xoff+b->x1)/width)-1;
    int i2= ((xoff+b->x2)/width)+1;
    while (i1*width < (xoff+b->x1)) i1++;
    while (i2*width > (xoff+b->x2)) i2--;
    for (i=i1; i<i2; i++) {
      box bb= move_box (decorate_right (ip), repeat, 0, 0);
      insert (bb, i*width-xoff, 0);
    }
  }

  finalize ();
  x1= b->x1; y1= b->y1;
  x2= b->x2; y2= b->y2;
}

/******************************************************************************
* cell boxes for tables
******************************************************************************/

struct cell_box_rep: public change_box_rep {
  SI    bl, br, bb, bt;
  color fg;
  tree  bg, old_bg;
  cell_box_rep (path ip, box b, SI x0, SI y0, SI x1, SI y1, SI x2, SI y2,
		SI bl, SI br, SI bb, SI bt, color fg, tree bg);
  operator tree () { return tree (TUPLE, "cell", (tree) bs[0]); }
  void pre_display (renderer &ren);
  void post_display (renderer &ren);
  void display (renderer ren);
};

cell_box_rep::cell_box_rep (
  path ip, box b, SI X0, SI Y0, SI X1, SI Y1, SI X2, SI Y2,
  SI Bl, SI Br, SI Bb, SI Bt, color Fg, tree Bg):
  change_box_rep (ip, false),
  bl (Bl<<1), br (Br<<1), bb (Bb<<1), bt (Bt<<1),
  fg (Fg), bg (Bg)
{
  insert (b, X0, Y0);
  position ();
  x1= X1; y1= Y1;
  x2= X2; y2= Y2;
  if ((bg != "") || (bl>0) || (br>0) || (bb>0) || (bt>0)) {
    // the 4*PIXEL extra space is sufficient for (shrinking factor) <= 8
    x3= min (x3, x1 - (bl>>1) - (bl>0? PIXEL<<2: 0));
    y3= min (y3, y1 - (bb>>1) - (bb>0? PIXEL<<2: 0));
    x4= max (x4, x2 + (br>>1) + (br>0? PIXEL<<2: 0));
    y4= max (y4, y2 + (bt>>1) + (bt>0? PIXEL<<2: 0));
  }
  finalize ();
}

void
cell_box_rep::pre_display (renderer& ren) {
  if (bg == "") return;
  old_bg= ren->get_background_pattern ();
  ren->set_background_pattern (bg);
  ren->clear_pattern (x1, y1, x2, y2);
}

void
cell_box_rep::post_display (renderer &ren) {
  if (bg == "") return;
  ren->set_background_pattern (old_bg);
}

void
cell_box_rep::display (renderer ren) {
  if ((bl>0) || (br>0) || (bb>0) || (bt>0)) {
    SI l= bl, r= br, b= bb, t= bt;
    if (ren->sfactor > 1) { // correction for screen display only
      SI  pixel= ren->pixel;
      l= ((l + (pixel - 1)) / pixel) * pixel;
      r= ((r + (pixel - 1)) / pixel) * pixel;
      b= ((b + (pixel - 1)) / pixel) * pixel;
      t= ((t + (pixel - 1)) / pixel) * pixel;
    }
    SI X1= x1-(l>>1), X2= x2+(r>>1);
    SI Y1= y1-(b>>1), Y2= y2+(t>>1);
    ren->set_color (fg);
    ren->fill (X1  , Y1  , X1+l, Y2  );
    ren->fill (X2-r, Y1  , X2  , Y2  );
    ren->fill (X1  , Y1  , X2  , Y1+b);
    ren->fill (X1  , Y2-t, X2  , Y2  );
  }
}

/******************************************************************************
* Remember boxes
******************************************************************************/

class remember_box_rep: public change_box_rep {
public:
  rectangles* logs_ptr;
  SI          ox, oy;
public:
  inline remember_box_rep (path ip, box b):
    change_box_rep (ip, true), logs_ptr (NULL)
  {
    insert (b, 0, 0);
    position ();
    finalize ();
  }
  inline ~remember_box_rep () {
    if (logs_ptr != NULL) {
      rectangles& logs= *logs_ptr;
      logs= rectangles (rectangle (ox+x3, oy+y3, ox+x4, oy+y4), logs);
      logs= rectangles (rectangle (0, 0, 0, 0), logs);
      // cout << "  8=X " << rectangle (ox+x3, oy+y3, ox+x4, oy+y4) << "\n";
    }
  }
  inline void position_at (SI x, SI y, rectangles& logs) {
    x += x0; y += y0;
    if (logs_ptr == NULL) logs= rectangles (rectangle (0, 0, 0, 0), logs);
    else logs= rectangles (rectangle (ox+x3, oy+y3, ox+x4, oy+y4), logs);
    ox= x; oy= y;
    logs= rectangles (rectangle (ox+x3, oy+y3, ox+x4, oy+y4), logs);
    logs_ptr= &logs;
  }
  inline void display (renderer ren) {
    ren->apply_shadow (x1, y1, x2, y2);
  }
};

/******************************************************************************
* Highlight boxes
******************************************************************************/

struct highlight_box_rep: public change_box_rep {
  SI w, xpad, ypad;
  tree bg, old_bg;
  color sun, shad;
  highlight_box_rep (path ip, box b, SI w, SI xpad, SI ypad,
		     tree bg, color sun, color shad);
  operator tree () { return tree (TUPLE, "highlight", (tree) bs[0]); }
  void pre_display (renderer &ren);
  void post_display (renderer &ren);
  void display (renderer ren);
};

highlight_box_rep::highlight_box_rep (
  path ip, box b, SI w2, SI xp2, SI yp2, tree bg2, color sun2, color shad2):
  change_box_rep (ip, true), w (w2), xpad (xp2), ypad (yp2),
  bg (bg2), sun (sun2), shad (shad2)
{
  insert (b, w + xpad, 0);
  position ();
  x1= b->x1;
  y1= b->y1 - w - ypad;
  x2= b->x2 + 2 * (w + xpad);
  y2= b->y2 + w + ypad;
  x3= min (x1, b->x3 + w + xpad);
  y3= min (y1, b->y3);
  x4= max (x2, b->x4 + w + xpad);
  y4= max (y2, b->y4);
  finalize ();
}

void
highlight_box_rep::pre_display (renderer& ren) {
  old_bg= ren->get_background_pattern ();
  ren->set_background_pattern (bg);
  SI W= w;
  if (!ren->is_printer ()) {
    SI pixel= ren->pixel;
    W= ((w + pixel - 1) / pixel) * pixel;
  }
  ren->clear_pattern (x1+W, y1+W, x2-W, y2-W);
}

void
highlight_box_rep::post_display (renderer &ren) {
  ren->set_background_pattern (old_bg);
}

void
highlight_box_rep::display (renderer ren) {
  SI W= w;
  if (!ren->is_printer ()) {
    SI pixel= ren->pixel;
    W= ((w + pixel - 1) / pixel) * pixel;
  }
  ren->set_color (sun);
  ren->fill (x1  , y2-W, x2  , y2  );
  ren->fill (x1  , y1  , x1+W, y2  );
  ren->set_color (shad);
  ren->fill (x1+W, y1  , x2  , y1+W);
  ren->fill (x2-W, y1  , x2  , y2-W);
  ren->triangle (x1, y1, x1+W, y1, x1+W, y1+W);
  ren->triangle (x2, y2, x2, y2-W, x2-W, y2-W);
}

/******************************************************************************
* action boxes
******************************************************************************/

struct action_box_rep: public change_box_rep {
  tree    filter; // which events are accepted ?
  command cmd;    // command to be executed
  path    vip;    // store this location before execution
  action_box_rep (path ip, box b, tree f, command c, bool ch, path vip);
  operator tree () { return tree (TUPLE, "action", bs[0]); }
  tree action (tree t, SI x, SI y, SI delta);
};

action_box_rep::action_box_rep (
  path ip, box b, tree filter2, command cmd2, bool child_flag, path vip2):
  change_box_rep (ip, child_flag), filter (filter2), cmd (cmd2), vip (vip2)
{
  insert (b, 0, 0);
  position ();
  left_justify ();
  finalize ();
}

tree
action_box_rep::action (tree t, SI x, SI y, SI delta) {
  if (t == filter) {
    call ("action-set-path", reverse (vip));
    // FIXME: we should also reset the action path
    cmd ();
    return "done";
  }
  return change_box_rep::action (t, x, y, delta);
}

/******************************************************************************
* locus boxes
******************************************************************************/

struct locus_box_rep: public change_box_rep {
  list<string> ids;
  SI pixel;
  locus_box_rep (path ip, box b, list<string> ids, SI pixel);
  operator tree () { return tree (TUPLE, "locus"); }
  void loci (SI x, SI y, SI delta, list<string>& ids2, rectangles& rs);
};

locus_box_rep::locus_box_rep (path ip, box b, list<string> ids2, SI pixel2):
  change_box_rep (ip, true), ids (ids2), pixel (pixel2)
{
  insert (b, 0, 0);
  position ();
  left_justify ();
  finalize ();
}

void
locus_box_rep::loci (SI x, SI y, SI delta, list<string>& l, rectangles& rs) {
  bs[0]->loci (x, y, delta, l, rs);
  l = l * ids;
  rs= rs * outline (rectangles (rectangle (x1, y1, x2, y2)), pixel);
}

/******************************************************************************
* tag boxes
******************************************************************************/

struct tag_box_rep: public change_box_rep {
  tree keys;
  tag_box_rep (path ip, box b, tree keys);
  operator tree () { return tree (TUPLE, "tag", bs[0]); }
  tree tag (tree t, SI x, SI y, SI delta);
  void collect_page_numbers (hashmap<string,tree>& h, tree page);
  path find_tag (string name);
};

tag_box_rep::tag_box_rep (path ip, box b, tree keys2):
  change_box_rep (ip, false), keys (keys2)
{
  insert (b, 0, 0);
  position ();
  left_justify ();
  finalize ();
}

void
tag_box_rep::collect_page_numbers (hashmap<string,tree>& h, tree page) {
  for (int i=0; i<N(keys); i++)
    h (keys[i]->label)= page;
  bs[0]->collect_page_numbers (h, page);
}

path
tag_box_rep::find_tag (string search) {
  for (int i=0; i<N(keys); i++)
    if (keys[i]->label == search)
      return reverse (descend_decode (ip, 1));
  return path ();
}

/******************************************************************************
* textat boxes
******************************************************************************/

struct textat_box_rep: public move_box_rep {
  textat_box_rep (path ip, box b, SI x, SI y):
    move_box_rep (ip, b, x, y, false, false) {}
  gr_selections graphical_select (SI x, SI y, SI dist);
  operator tree () { return tree (TUPLE, "textat", (tree) bs[0]); }
};

gr_selections
textat_box_rep::graphical_select (SI x, SI y, SI dist) {
  gr_selections res;
  if (graphical_distance (x, y) <= dist) {
    gr_selection gs;
    gs->dist= graphical_distance (x, y);
    gs->p= point (x, y);
    gs->cp << box_rep::find_tree_path (x, y, dist);
    gs->pts << gs->p;
    gs->c= curve ();
    res << gs;
  }
  return res;
}

/******************************************************************************
* box construction routines
******************************************************************************/

box
move_box (path ip, box b, SI x, SI y, bool child_flag, bool big_flag) {
  return tm_new<move_box_rep> (ip, b, x, y, child_flag, big_flag);
}

box
resize_box (path ip, box b, SI x1, SI y1, SI x2, SI y2,
	    bool child_flag, bool adjust) {
  return tm_new<resize_box_rep> (ip, b, x1, y1, x2, y2, child_flag, adjust);
}

box
clip_box (path ip, box b, SI x1, SI y1, SI x2, SI y2) {
  return tm_new<clip_box_rep> (ip, b, x1, y1, x2, y2, UNINIT, UNINIT, 0, 0);
}

box
clip_box (path ip, box b, SI x1, SI y1, SI x2, SI y2,
	  tree xt, tree yt, SI scx, SI scy) {
  return tm_new<clip_box_rep> (ip, b, x1, y1, x2, y2, xt, yt, scx, scy);
}

box
vcorrect_box (path ip, box b, SI top_cor, SI bot_cor) {
  return tm_new<vcorrect_box_rep> (ip, b, top_cor, bot_cor);
}

box
repeat_box (path ip, box ref, box repeat, SI xoff) {
  return tm_new<repeat_box_rep> (ip, ref, repeat, xoff);
}

box
cell_box (path ip, box b, SI x0, SI y0, SI x1, SI y1, SI x2, SI y2,
	  SI bl, SI br, SI bb, SI bt, color fg, tree bg)
{
  box cb= tm_new<cell_box_rep> (ip, b, x0, y0, x1, y1, x2, y2,
			    bl, br, bb, bt, fg, bg);
  return cb;
}

box
remember_box (path ip, box b) {
  return tm_new<remember_box_rep> (ip, b);
}

box
highlight_box (path ip, box b, SI w, SI xpad, SI ypad,
	       tree bg, color sun, color shad) {
  return tm_new<highlight_box_rep> (ip, b, w, xpad, ypad, bg, sun, shad);
}

box
action_box (path ip, box b, tree filter, command cmd, bool ch, path vip) {
  return tm_new<action_box_rep> (ip, b, filter, cmd, ch, vip);
}

box
action_box (path ip, box b, tree filter, command cmd, bool ch) {
  return tm_new<action_box_rep> (ip, b, filter, cmd, ch, decorate ());
}

box
locus_box (path ip, box b, list<string> ids, SI pixel) {
  return tm_new<locus_box_rep> (ip, b, ids, pixel);
}

box
tag_box (path ip, box b, tree keys) {
  return tm_new<tag_box_rep> (ip, b, keys);
}

box
textat_box (path ip, box b, SI x, SI y) {
  return tm_new<textat_box_rep> (ip, b, x, y);
}
