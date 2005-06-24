
/******************************************************************************
* MODULE     : composite.cpp
* DESCRIPTION: Boxes with a list of child boxes
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include "Boxes/composite.hpp"
#include "Boxes/construct.hpp"

/******************************************************************************
* Setting up composite boxes
******************************************************************************/

composite_box_rep::composite_box_rep (path ip): box_rep (ip) { }

composite_box_rep::composite_box_rep (path ip, array<box> B): box_rep (ip) {
  bs= B;
  position ();
}

composite_box_rep::composite_box_rep (
  path ip, array<box> B, bool init_sx_sy):
    box_rep (ip)
{
  bs= B;
  if (init_sx_sy) {
    int i, n= N(bs);
    for (i=0; i<n; i++) {
      sx(i)= 0;
      sy(i)= 0;
    }
  }
  position ();
}

composite_box_rep::composite_box_rep (
  path ip, array<box> B, array<SI> x, array<SI> y):
    box_rep (ip)
{
  bs= B;
  int i, n= subnr();
  for (i=0; i<n; i++) {
    sx(i)= x[i];
    sy(i)= y[i];
  }
  position ();
}

composite_box_rep::~composite_box_rep () {}

void
composite_box_rep::insert (box b, SI x, SI y) {
  int n= N(bs);
  bs << b;
  sx(n)= x;
  sy(n)= y;
}

void
composite_box_rep::position () {
  int i, n= subnr();
  if (n == 0) {
    x1= y1= x3= y3= 0;
    x2= y2= x4= y4= 0;
    fatal_error ("empty composite box", "composite_box::position");
  }
  else {
    x1= y1= x3= y3= MAX_SI;
    x2= y2= x4= y4= -MAX_SI;
    for (i=0; i<n; i++) {
      x1= min (x1, sx1(i));
      y1= min (y1, sy1(i));
      x2= max (x2, sx2(i));
      y2= max (y2, sy2(i));
      x3= min (x3, sx3(i));
      y3= min (y3, sy3(i));
      x4= max (x4, sx4(i));
      y4= max (y4, sy4(i));
    }
  }
}

void
composite_box_rep::left_justify () {
  int i, n= subnr();
  SI d= x1;
  x1-=d; x2-=d; x3-=d; x4-=d;
  for (i=0; i<n; i++) sx(i) -= d;
}

/******************************************************************************
* Routines for composite boxes
******************************************************************************/

void
composite_box_rep::display (ps_device dev) {
  (void) dev;
}

int
composite_box_rep::subnr () {
  return N(bs);
}

box
composite_box_rep::subbox (int i) {
  return bs[i];
}

tree
composite_box_rep::action (tree t, SI x, SI y, SI delta) {
  int m= find_child (x, y, delta, true);
  if (m == -1) return "";
  else return bs[m]->action (t, x- sx(m), y- sy(m),
			     delta + get_delta (x, x1, x2));
}

void
composite_box_rep::collect_page_numbers (hashmap<string,tree>& h, tree page) {
  int i, n= N(bs);
  for (i=0; i<n; i++)
    bs[i]->collect_page_numbers (h, page);
}

path
composite_box_rep::find_tag (string name) {
  int i, n= N(bs);
  for (i=0; i<n; i++) {
    path p= bs[i]->find_tag (name);
    if (!nil (p)) return p;
  }
  return path ();
}

bool
composite_box_rep::access_allowed () {
  return true;
}

box
composite_box_rep::transform (frame fr) {
  int i;
  array<box> bs;
  for (i= 0; i<subnr(); i++) {
    if (!nil (subbox (i))) {
      box sb= subbox (i)->transform (fr);
      if (!nil (sb)) bs << sb;
    }
  }
  return N (bs)==0?box ():composite_box (ip, bs);
}

/******************************************************************************
* Cursor routines
******************************************************************************/

void
composite_box_rep::finalize () {
  int i, n= subnr ();
  lip= descend (ip, 0);
  rip= descend (ip, 1);
  for (i=0; i<n; i++) {
    path l= bs[i]->find_lip ();
    path r= bs[i]->find_rip ();
    /*
    cout << "  i  = " << i << "\n";
    cout << "  l  = " << l << "\n";
    cout << "  r  = " << r << "\n";
    */
    if (is_accessible (l) && is_accessible (r)) {
      if (is_decoration (lip) || path_less (reverse (l), reverse (lip)))
	lip= l;
      if (is_decoration (rip) || path_less (reverse (rip), reverse (r)))
	rip= r;
    }
  }
  /*
  cout << ((tree) (*((box_rep*) this))) << " " << ip << "\n";
  cout << "  lip= " << lip << "\n";
  cout << "  rip= " << rip << "\n";
  */
}

int
composite_box_rep::find_child (SI x, SI y, SI delta, bool force) {
  if (outside (x, delta, x1, x2) && (is_accessible (ip) || force)) return -1;
  int i, n= subnr(), d= MAX_SI, m= -1;
  for (i=0; i<n; i++)
    if (distance (i, x, y, delta)< d)
      if (bs[i]->accessible () || force) {
	d= distance (i, x, y, delta);
	m= i;
      }
  return m;
}

path
composite_box_rep::find_box_path (SI x, SI y, SI delta, bool force) {
  int m= find_child (x, y, delta, force);
  if (m==-1) return box_rep::find_box_path (x, y, delta, force);
  else {
    SI xx= x- sx(m), yy= y- sy(m);
    SI dd= delta + get_delta (xx, bs[m]->x1, bs[m]->x2);
    return path (m, bs[m]->find_box_path (xx, yy, dd, force));
  }
}

path
composite_box_rep::find_lip () {
  return lip;
}

path
composite_box_rep::find_rip () {
  return rip;
}

path
composite_box_rep::find_box_path (path p, bool& found) {
  int n= subnr();
  // cout << "Search cursor " << p << " among " << n
  //      << " at " << box (this) << " " << reverse (ip) << "\n";
  if (n == 0) return box_rep::find_box_path (p, found);

  int start= n>>1, acc= start, step= (start+1)>>1;
  while (step > 0) {
    path sr= bs[acc]->find_rip ();
    while (!is_accessible (sr)) {
      acc--;
      if (acc<0) break;
      sr= bs[acc]->find_rip ();
    }
    if (acc<0) {
      start= 0;
      break;
    }
    if (path_less (reverse (sr), p)) {
      int old_start= start, old_acc= acc;
      start= min (n-1, start+ step);
      acc  = start;
      while ((acc > old_start) &&
	     (!is_accessible (bs[acc]->find_rip ()))) acc--;
      if (acc == old_start) acc= old_acc;
    }
    else {
      start= max (0, start- step);
      acc  = min (acc, start);
    }
    if (step <= 1) break;
    step= (step+1)>>1;
  }

  path bp;
  bool flag= false;
  int i= start;
  found= false;
  while (true) {
    path sl= bs[i]->find_lip ();
    path sr= bs[i]->find_rip ();
    // cout << "  " << i << ":\t" << reverse(sl) <<", "<< reverse(sr) << "\n";
    if (is_accessible (sl) && is_accessible (sr) &&
	path_less_eq (reverse (sl), p) && path_less_eq (p, reverse (sr)))
      {
	flag= true;
	bp= path (i, bs[i]->find_box_path (p, found));
	if (found) return bp;
      }
    i++;
    if (i==n) i=0;
    if (i==start) break;
  }

  if (is_accessible (ip) && (path_up (p) == reverse (ip)) && access_allowed ())
    return box_rep::find_box_path (p, found);
  if (flag) return bp;
  if (start > 0) {
    path sl= bs[start-1]->find_rip ();
    path sr= bs[start  ]->find_lip ();
    if (is_accessible (sl) && is_accessible (sr) &&
	path_less_eq (reverse (sl), p) && path_less_eq (p, reverse (sr)))
      {
	int c1= N (common (reverse (sl), p));
	int c2= N (common (reverse (sr), p));
	int i = (c1 >= c2? start-1: start);
	return path (i, bs[i]->find_box_path (p, found));
      }
  }
  return box_rep::find_box_path (p, flag);
}

path
composite_box_rep::find_tree_path (path bp) {
  if (atom (bp)) return box_rep::find_tree_path (bp);
  return bs[bp->item]->find_tree_path (bp->next);
}

cursor
composite_box_rep::find_cursor (path bp) {
  if (atom (bp)) return box_rep::find_cursor (bp);
  else {
    int i= bp->item;
    cursor cu= bs[i]->find_cursor (bp->next);
    cu->delta -= get_delta (cu->ox, bs[i]->x1, bs[i]->x2);
    cu->ox    += sx(i);
    cu->oy    += sy(i);
    return cu;
  }
}

selection
composite_box_rep::find_selection (path lbp, path rbp) {
  if ((!atom (lbp)) && (!atom (rbp)) && (lbp->item == rbp->item)) {
    int i= lbp->item;
    selection sel= bs[i]->find_selection (lbp->next, rbp->next);
    return selection (translate (sel->rs, sx(i), sy(i)), sel->start, sel->end);
  }
  else return box_rep::find_selection (lbp, rbp);
}

gr_selections
composite_box_rep::graphical_select (SI x, SI y, SI dist) {
  gr_selections res;
  if (graphical_distance (x, y) <= dist) {
    int i, n= subnr();
    for (i=0; i<n; i++)
      res << bs[i]->graphical_select (x- sx(i), y- sy(i), dist);
  }
  return res;
}

/******************************************************************************
* Concrete composite box
******************************************************************************/

int
concrete_composite_box_rep::find_child (SI x, SI y, SI delta, bool force) {
  if (border_flag &&
      outside (x, delta, x1, x2) &&
      (is_accessible (ip) || force)) return -1;
  int i, n= subnr(), d= MAX_SI, m= -1;
  for (i=0; i<n; i++)
    if (distance (i, x, y, delta)< d)
      if (bs[i]->accessible () || force) {
	d= distance (i, x, y, delta);
	m= i;
      }
  return m;
}

/******************************************************************************
* User interface
******************************************************************************/

box
composite_box (path ip, array<box> bs, bool bfl) {
  return new concrete_composite_box_rep (ip, bs, bfl);
}

box
composite_box (path ip, array<box> bs, array<SI> x, array<SI> y, bool bfl) {
  return new concrete_composite_box_rep (ip, bs, x, y, bfl);
}
