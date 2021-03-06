
/******************************************************************************
* MODULE     : glue_widget.cpp
* DESCRIPTION: Widgets for filling up space between a group of widgets.
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "gui.hpp"
#include "window.hpp"
#include "Widkit/layout.hpp"
#include "Widkit/basic_widget.hpp"

/******************************************************************************
* Glue widgets
******************************************************************************/

class glue_widget_rep: public basic_widget_rep {
  bool hflag, vflag;  // may be extended horizontally resp. vertically
  SI   minw, minh;    // minimal width and height in pixels

public:
  glue_widget_rep (bool hflag, bool vflag, SI minw, SI minh);
  operator tree ();

  void handle_get_size (get_size_event ev);
  void handle_repaint (repaint_event ev);
};

glue_widget_rep::glue_widget_rep (bool hflag2, bool vflag2, SI w2, SI h2):
  basic_widget_rep (),
  hflag (hflag2), vflag (vflag2), minw (w2), minh (h2) {}

glue_widget_rep::operator tree () {
  return "glue";
}

void
glue_widget_rep::handle_get_size (get_size_event ev) {
  if (ev->mode==0) {
    if (!hflag) ev->w= minw;
    if (!vflag) ev->h= minh;
  }
  if (ev->mode==-1) {
    ev->w= minw;
    ev->h= minh;
  }
  if (ev->mode==1) {
    gui_maximal_extents (ev->w, ev->h);
    if (!hflag) ev->w= minw;
    if (!vflag) ev->h= minh;
  }
}

void
glue_widget_rep::handle_repaint (repaint_event ev) {
  renderer ren= win->get_renderer ();
  layout_default (ren, ev->x1, ev->y1, ev->x2, ev->y2);
}

/******************************************************************************
* Interface
******************************************************************************/

wk_widget
glue_wk_widget (bool hflag, bool vflag, SI minw, SI minh) {
  return tm_new<glue_widget_rep> (hflag, vflag, minw, minh);
}
