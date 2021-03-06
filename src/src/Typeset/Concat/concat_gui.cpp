
/******************************************************************************
* MODULE     : concat_gui.cpp
* DESCRIPTION: Typesetting GUI markup
* COPYRIGHT  : (C) 2007  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "concater.hpp"
#include "formatter.hpp"
#include "analyze.hpp"
#include "Concat/canvas_properties.hpp"

SI resize (edit_env env, SI old, SI minimum, SI maximum, tree new_size);

/******************************************************************************
* Scrollable canvases
******************************************************************************/

canvas_properties
get_canvas_properties (edit_env env, tree t) {
  bool printed= (env->get_string (PAGE_PRINTED) == "true");
  SI   border = env->get_length (ORNAMENT_BORDER);
  SI   pixel  = env->get_int (SFACTOR) * PIXEL;
  if (!printed)
    border= max (pixel, ((border + pixel/2) / pixel) * pixel);

  canvas_properties props;
  props->env        = env;
  props->type       = env->get_string (CANVAS_TYPE);
  props->x1         = env->exec (t[0]);
  props->y1         = env->exec (t[1]);
  props->x2         = env->exec (t[2]);
  props->y2         = env->exec (t[3]);
  props->xt         = env->expand (t[4]);
  props->yt         = env->expand (t[5]);
  props->scx        = env->exec (props->xt);
  props->scy        = env->exec (props->yt);
  props->hpadding   = env->get_length (CANVAS_HPADDING);
  props->vpadding   = env->get_length (CANVAS_VPADDING);
  props->border     = border;
  props->bg         = env->read (CANVAS_COLOR);
  props->sunny      = env->get_color (ORNAMENT_SUNNY_COLOR);
  props->shadow     = env->get_color (ORNAMENT_SHADOW_COLOR);
  props->bar_width  = env->get_length (CANVAS_BAR_WIDTH);
  props->bar_padding= env->get_length (CANVAS_BAR_PADDING);
  props->bar_bg     = env->read (CANVAS_BAR_COLOR);
  props->bar_button = env->read (ORNAMENT_COLOR);
  return props;
}

void
get_canvas_horizontal (canvas_properties props,
		       SI bx1, SI bx2, SI& x1, SI& x2, SI& scx)
{
  edit_env env   = props->env;
  string   type  = props->type;
  SI       bwid  = props->bar_width;
  SI       bpad  = props->bar_padding;
  SI       border= props->border;
  x1= resize (env, bx1, bx1, bx2, props->x1);
  x2= resize (env, bx2, bx1, bx2, props->x2);
  if (type != "plain") {
    if (ends (type, "w") || ends (type, "e"))
      x2= max (x1, x2 - (bwid + bpad));
    x1 += (border + props->hpadding);
    x2 -= (border + props->hpadding);
  }
  if (is_atomic (props->scx) && ends (props->scx->label, "%")) {
    double p= as_double (props->scx->label (0, N(props->scx->label)-1))/100.0;
    SI d = ((x2 - x1) - (bx2 - bx1));
    SI dx= (d >= 0? 0: (SI) (p * d));
    scx  = dx + x1 - bx1;
  }
  else scx= -env->as_length (props->scx);
}

void
get_canvas_vertical (canvas_properties props,
		     SI by1, SI by2, SI& y1, SI& y2, SI& scy)
{
  edit_env env   = props->env;
  string   type  = props->type;
  SI       bwid  = props->bar_width;
  SI       bpad  = props->bar_padding;
  SI       border= props->border;
  y1= resize (env, by1, by1, by2, props->y1);
  y2= resize (env, by2, by1, by2, props->y2);
  if (type != "plain") {
    if (starts (type, "n") || starts (type, "s"))
      y2= max (y1, y2 - (bwid + bpad));
    y1 += (border + props->vpadding);
    y2 -= (border + props->vpadding);
  }
  if (is_atomic (props->scy) && ends (props->scy->label, "%")) {
    double p= as_double (props->scy->label (0, N(props->scy->label)-1))/100.0;
    SI d = ((y2 - y1) - (by2 - by1));
    SI dy= (d >= 0? d: (SI) (p * d));
    scy  = dy + y1 - by1;
  }
  else scy= -env->as_length (props->scy);
}

box
make_hor_bar (canvas_properties props, path ip, SI x1, SI x2, SI X1, SI X2) {
  SI    h     = props->bar_width;
  SI    border= props->border;
  tree  c1    = props->bar_bg;
  color c2    = props->sunny;
  color c3    = props->shadow;
  tree  c4    = props->bar_button;
  box   mask1 = empty_box (ip, x1, border, x2, h-border);
  box   mask2 = empty_box (ip, X1, 2*border, X2, h-2*border);
  box   hl1   = highlight_box (ip, mask1, border, 0, 0, c1, c3, c2);
  box   hl2   = highlight_box (ip, mask2, border, 0, 0, c4, c2, c3);
  array<box> bs (2);
  array<SI>  xs (2);
  array<SI>  ys (2);
  bs[0]= hl1; bs[1]= hl2;
  xs[0]= xs[1]= 0;
  ys[0]= ys[1]= 0;
  return composite_box (ip, bs, xs, ys);
}

box
make_ver_bar (canvas_properties props, path ip, SI y1, SI y2, SI Y1, SI Y2) {
  SI    w     = props->bar_width;
  SI    border= props->border;
  tree  c1    = props->bar_bg;
  color c2    = props->sunny;
  color c3    = props->shadow;
  tree  c4    = props->bar_button;
  box   mask1 = empty_box (ip, border, y1, w-border, y2);
  box   mask2 = empty_box (ip, 2*border, Y1, w-2*border, Y2);
  box   hl1   = highlight_box (ip, mask1, border, 0, 0, c1, c3, c2);
  box   hl2   = highlight_box (ip, mask2, border, 0, 0, c4, c2, c3);
  array<box> bs (2);
  array<SI>  xs (2);
  array<SI>  ys (2);
  bs[0]= hl1; bs[1]= hl2;
  xs[0]= xs[1]= 0;
  ys[0]= ys[1]= 0;
  return composite_box (ip, bs, xs, ys);
}

box
put_scroll_bars (canvas_properties props, box b, path ip,
		 box inner, SI scx, SI scy)
{
  string type  = props->type;
  SI     hpad  = props->hpadding;
  SI     vpad  = props->vpadding;
  SI     bwid  = props->bar_width;
  SI     bpad  = props->bar_padding;
  SI     border= props->border;
  tree   ccol  = props->bg;
  color  csun  = props->sunny;
  color  cshad = props->shadow;
  path   dip   = decorate (ip);
  int    hor   = 0;
  int    ver   = 0;
  if (type != "plain") {
    if (starts (type, "s")) hor= -1;
    if (starts (type, "n")) hor=  1;
    if (ends   (type, "w")) ver= -1;
    if (ends   (type, "e")) ver=  1;
  }
  array<box> bs (1);
  array<SI>  xs (1);
  array<SI>  ys (1);
  b     = resize_box (dip, b, b->x1-hpad, b->y1-vpad, b->x2+hpad, b->y2+vpad);
  bs[0] = highlight_box (dip, b, border, 0, 0, ccol, cshad, csun);
  xs[0] = (ver < 0? bwid + bpad: 0) - border;
  ys[0] = (hor < 0? bwid + bpad: 0) + border;
  if (hor != 0 && inner->w() > b->w() + 4 - 2*hpad) { 
    SI dx= b->x1 + hpad - inner->x1 - scx;
    double start= 0.0, end= 1.0;
    start= ((double) dx) / ((double) inner->w());
    end  = start + ((double) b->w() - 2*hpad) / ((double) inner->w());
    SI X1= b->x1 + border + ((SI) (start * (b->w() - 2*border)));
    SI X2= b->x1 + border + ((SI) (end   * (b->w() - 2*border)));
    box hor_bar= make_hor_bar (props, dip, b->x1, b->x2, X1, X2);
    hor_bar= scrollbar_box (dip, hor_bar, false, X2-X1, props->xt);
    bs << hor_bar;
    xs << (ver < 0? bwid + bpad: 0) - border;
    ys << (hor < 0? b->y1: b->y2 + bpad + 2*border);
  }
  if (ver != 0 && inner->h() > b->h() + 4 - 2*vpad) {
    SI dy= b->y1 + vpad - inner->y1 - scy;
    double start= 0.0, end= 1.0;
    start= ((double) dy) / ((double) inner->h());
    end  = start + ((double) b->h() - 2*vpad) / ((double) inner->h());
    SI Y1= b->y1 + border + ((SI) (start * (b->h() - 2*border)));
    SI Y2= b->y1 + border + ((SI) (end   * (b->h() - 2*border)));
    box ver_bar= make_ver_bar (props, dip, b->y1, b->y2, Y1, Y2);
    ver_bar= scrollbar_box (dip, ver_bar, true, Y2-Y1, props->yt);
    bs << ver_bar;
    xs << (ver < 0? b->x1 - 2*border: b->x2 + bpad);
    ys << (hor < 0? bwid + bpad: 0) + border;
  }
  return composite_box (ip, bs, xs, ys);
}

void
concater_rep::typeset_canvas (tree t, path ip) {
  // IDEA: set left, right, bottom, top environment variables
  //       and allow doing computations with them
  canvas_properties props= get_canvas_properties (env, t);
  box b= typeset_as_concat (env, t[6], descend (ip, 6));
  SI x1, y1, x2, y2, scx, scy;
  get_canvas_horizontal (props, b->x1, b->x2, x1, x2, scx);
  get_canvas_vertical (props, b->y1, b->y2, y1, y2, scy);
  string type= env->get_string (CANVAS_TYPE);
  path dip= (type == "plain"? ip: decorate (ip));
  box cb= clip_box (dip, b, x1, y1, x2, y2, props->xt, props->yt, scx, scy);
  if (type != "plain") cb= put_scroll_bars (props, cb, ip, b, scx, scy);
  print (STD_ITEM, cb);
}

/******************************************************************************
* Highlighting
******************************************************************************/

void
concater_rep::typeset_ornament (tree t, path ip) {
  SI    w     = env->get_length (ORNAMENT_BORDER);
  SI    xpad  = env->get_length (ORNAMENT_HPADDING);
  SI    ypad  = env->get_length (ORNAMENT_VPADDING);
  tree  bg    = env->read       (ORNAMENT_COLOR);
  color sunny = env->get_color  (ORNAMENT_SUNNY_COLOR);
  color shadow= env->get_color  (ORNAMENT_SHADOW_COLOR);
  box   b     = typeset_as_concat (env, t[0], descend (ip, 0));
  print (STD_ITEM, highlight_box (ip, b, w, xpad, ypad, bg, sunny, shadow));
}
