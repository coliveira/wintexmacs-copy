
/******************************************************************************
* MODULE     : QTMWidget.cpp
* DESCRIPTION: QT Texmacs widget class
* COPYRIGHT  : (C) 2008 Massimiliano Gubinelli and Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include <QtGui>

#include "QTMWidget.hpp"
#include "qt_renderer.hpp"
#include "qt_gui.hpp"
#include "converter.hpp"

#ifdef USE_CAIRO
#include "Cairo/cairo_renderer.hpp"
#if defined(Q_WS_X11)
#include <cairo-xlib.h>
#include <QX11Info>
extern Drawable qt_x11Handle(const QPaintDevice *pd);
extern const QX11Info *qt_x11Info(const QPaintDevice *pd);
#undef KeyPress  // conflict between QEvent::KeyPree and X11 defintion
#elif defined(Q_WS_MAC)
#define ID OTHER_ID
#include <cairo-quartz.h>
#undef ID
#endif
#endif

#include <QEvent>

#define PIXEL 256

extern bool qt_update_flag;
extern int time_credit;
extern int timeout_time;

hashmap<int,string> qtkeymap (0);

inline void
scale (QPoint& point) {	
  point.rx() *= PIXEL; point.ry() *= -PIXEL;
}

inline void
map (int code, string name) {
  qtkeymap(code) = name;
}

void
initkeymap () {
  map (Qt::Key_Space     , "space");
  map (Qt::Key_Return    , "return");
  map (Qt::Key_Tab       , "tab");
  map (Qt::Key_Backspace , "backspace");
  map (Qt::Key_Enter     , "enter");
  map (Qt::Key_Escape    , "escape");
  map (Qt::Key_Backspace , "backspace");
  map (Qt::Key_Up        , "up" );
  map (Qt::Key_Down      , "down" );
  map (Qt::Key_Left      , "left" );
  map (Qt::Key_Right     , "right" );
  map (Qt::Key_F1        , "F1" );
  map (Qt::Key_F2        , "F2" );
  map (Qt::Key_F3        , "F3" );
  map (Qt::Key_F4        , "F4" );
  map (Qt::Key_F5        , "F5" );
  map (Qt::Key_F6        , "F6" );
  map (Qt::Key_F7        , "F7" );
  map (Qt::Key_F8        , "F8" );
  map (Qt::Key_F9        , "F9" );
  map (Qt::Key_F10       , "F10" );
  map (Qt::Key_F11       , "F11" );
  map (Qt::Key_F12       , "F12" );
  map (Qt::Key_F13       , "F13" );
  map (Qt::Key_F14       , "F14" );
  map (Qt::Key_F15       , "F15" );
  map (Qt::Key_F16       , "F16" );
  map (Qt::Key_F17       , "F17" );
  map (Qt::Key_F18       , "F18" );
  map (Qt::Key_F19       , "F19" );
  map (Qt::Key_F20       , "F20" );
  map (Qt::Key_F21       , "F21" );
  map (Qt::Key_F22       , "F22" );
  map (Qt::Key_F23       , "F23" );
  map (Qt::Key_F24       , "F24" );
  map (Qt::Key_F25       , "F25" );
  map (Qt::Key_F26       , "F26" );
  map (Qt::Key_F27       , "F27" );
  map (Qt::Key_F28       , "F28" );
  map (Qt::Key_F29       , "F29" );
  map (Qt::Key_F30       , "F30" );
  map (Qt::Key_F31       , "F31" );
  map (Qt::Key_F32       , "F32" );
  map (Qt::Key_F33       , "F33" );
  map (Qt::Key_F34       , "F34" );
  map (Qt::Key_F35       , "F35" );
  map (Qt::Key_Insert    , "insert" );
  map (Qt::Key_Delete    , "delete" );
  map (Qt::Key_Home      , "home" );
  map (Qt::Key_End       , "end" );
  map (Qt::Key_PageUp    , "pageup" );
  map (Qt::Key_PageDown  , "pagedown" );
  map (Qt::Key_ScrollLock, "scrolllock" );
  map (Qt::Key_Pause     , "pause" );
  map (Qt::Key_SysReq    , "sysreq" );
  map (Qt::Key_Stop      , "stop" );
  map (Qt::Key_Menu      , "menu" );
  map (Qt::Key_Print     , "print" );
  map (Qt::Key_Select    , "select" );
  map (Qt::Key_Execute   , "execute" );
  map (Qt::Key_Help      , "help" );

  map (Qt::Key_Dead_Acute     , "acute");
  map (Qt::Key_Dead_Grave     , "grave");
  map (Qt::Key_Dead_Diaeresis , "umlaut");
  map (Qt::Key_Dead_Circumflex, "hat");
  map (Qt::Key_Dead_Tilde     , "tilde");

  // map (0x0003              , "K-enter");
  // map (Qt::Key_Begin       , "begin" );
  // map (Qt::Key_PrintScreen , "printscreen" );
  // map (Qt::Key_Break       , "break" );
  // map (Qt::Key_User        , "user" );
  // map (Qt::Key_System      , "system" );
  // map (Qt::Key_Reset       , "reset" );
  // map (Qt::Key_ClearLine   , "clear" );
  // map (Qt::Key_ClearDisplay, "cleardisplay" );
  // map (Qt::Key_InsertLine  , "insertline" );
  // map (Qt::Key_DeleteLine  , "deleteline" );
  // map (Qt::Key_InsertChar  , "insert" );
  // map (Qt::Key_DeleteChar  , "delete" );
  // map (Qt::Key_Prev        , "prev" );
  // map (Qt::Key_Next        , "next" );
  // map (Qt::Key_Undo        , "undo" );
  // map (Qt::Key_Redo        , "redo" );
  // map (Qt::Key_Find        , "find" );
  // map (Qt::Key_ModeSwitchFunctionKey, "modeswitch" );  
}

void
QTMWidget::postponedUpdate () {
  while (!is_nil (delayed_rects)) {
    update (delayed_rects->item);
    delayed_rects= delayed_rects->next;
  }
}

void
QTMWidget::paintEvent (QPaintEvent* event) {
  QRect rect = event->rect ();
  if (DEBUG_EVENTS) {
    QPainter p(this);
    QBrush brush (QColor ("red"));
    p.fillRect (rect, brush);
    p.end ();
  }

  if (qt_update_flag)
    delayed_rects= list<QRect> (rect, delayed_rects);
  else {
    //int start= texmacs_time ();
    basic_renderer_rep *r;

#ifdef USE_CAIRO
    r = the_cairo_renderer ();
    cairo_surface_t *surf;
#ifdef Q_WS_X11
    //const QX11Info & info = x11Info();//qt_x11Info(this);
    Display *dpy = x11Info().display();
    Drawable drawable = qt_x11Handle(this);
    Visual *visual = (Visual*)x11Info().visual();
    surf = cairo_xlib_surface_create(dpy, drawable, visual, width(), height());
#elif defined(Q_WS_MAC)
    surf = cairo_quartz_surface_create_for_cg_context ((CGContextRef)(this->macCGHandle()), width(), height());
#endif
    cairo_t *ct = cairo_create(surf);
    r->begin (ct);
    cairo_surface_destroy(surf);
    cairo_destroy(ct);
#else
    r = the_qt_renderer();
    r->begin (static_cast<QPaintDevice*>(this));
#endif

    tm_widget()->set_current_renderer(r);    

    r -> set_clipping
    (rect.x()*PIXEL, -(rect.y()+rect.height())*PIXEL, 
     (rect.x()+rect.width())*PIXEL, -rect.y()*PIXEL);
    tm_widget()->handle_repaint
    (rect.x()*PIXEL, -(rect.y()+rect.height())*PIXEL, 
     (rect.x()+rect.width())*PIXEL, -rect.y()*PIXEL);
    
    if (r->interrupted())
      qt_update_flag= true;
    
    r->end();
    
    tm_widget()->set_current_renderer(NULL);    
    //int end= texmacs_time ();
    //if (end > start) cout << "Repaint " << end - start << "\n";
  }

  if (qt_update_flag) {
    if (DEBUG_EVENTS)
      cout << "Postponed redrawing\n"; 
    QTimer::singleShot (1, this, SLOT (postponedUpdate ()));
  }
}

/*
void
QTMWidget::focusInEvent (QFocusEvent* event) {
  cout << "Got focus\n";
  QWidget::focusInEvent (event);
}

void
QTMWidget::focusOutEvent (QFocusEvent* event) {
  cout << "Lost focus\n";
  QWidget::focusOutEvent (event);
}
*/

void
QTMWidget::keyPressEvent (QKeyEvent* event) {
  time_credit= 25;
  timeout_time= texmacs_time () + time_credit;
  static bool fInit = false;
  if (!fInit) {
    if (DEBUG_EVENTS)
      cout << "Initializing keymap\n";
    initkeymap();
    fInit= true;
  }
	
  if (DEBUG_EVENTS)
    cout << "keypressed\n";
  simple_widget_rep *wid =  tm_widget();
  if (!wid) return;
  
  {
    // char str[256];
    int key = event->key();
    QString nss = event->text();
    Qt::KeyboardModifiers mods = event->modifiers();
    if (DEBUG_EVENTS) {
      cout << "key  : " << key << LF;
      cout << "text : " << nss.toAscii().data() << LF;
      cout << "count: " << nss.count() << LF;
      if (mods & Qt::ShiftModifier) cout << "shift\n";
      if (mods & Qt::MetaModifier) cout << "meta\n";
      if (mods & Qt::ControlModifier) cout << "control\n";
      if (mods & Qt::KeypadModifier) cout << "keypad\n";
      if (mods & Qt::AltModifier) cout << "alt\n";
    }

    bool flag= true;
    string r;
    if (qtkeymap->contains (key)) {
      r = qtkeymap[key];
      if (mods & Qt::ShiftModifier) r= "S-" * r;
    }
    else {
      QByteArray buf= nss.toUtf8();
      string rr (buf.constData(), buf.count());
      r= utf8_to_cork (rr);
      if (r == "<less>") r= "<";
      if (r == "<gtr>") r= ">";
      unsigned short unic= nss.data()[0].unicode();
      //cout << "unic= " << unic << "\n";
      if (unic < 32 && key < 128) {
	if ((mods & Qt::ShiftModifier) == 0)
	  if (((char) key) >= 'A' && ((char) key) <= 'Z')
	    key= (int) (key + ((int) 'a') - ((int) 'A'));
	r= string ((char) key);
      }
      else {
	if (unic == 168) r= "umlaut";
	if (unic == 96) {
	  if ((mods & Qt::AltModifier) != 0) r= "grave";
	  else r= "`";
	}
	if (unic == 180) r= "acute";
	if (unic == 710) r= "hat";
	if (unic == 732) r= "tilde";
	flag= false;
      }
    }

#ifdef Q_WS_MAC
    if (mods & Qt::MetaModifier) r= "C-" * r;
    if (mods & Qt::ControlModifier) r= "Mod1-" * r;
    //if (mods & Qt::KeypadModifier) r= "Mod3-" * r;
    if (flag && ((mods & Qt::AltModifier) != 0)) r= "Mod4-" * r;
#else
    if (mods & Qt::ControlModifier) r= "C-" * r;
    if (mods & Qt::AltModifier) r= "Mod1-" * r;
    //if (mods & Qt::KeypadModifier) r= "Mod3-" * r;
    if (mods & Qt::MetaModifier) r= "Mod4-" * r;
#endif

    if (r == "") return;
    if (DEBUG_EVENTS)
      cout << "key press: " << r << LF;
    //int start= texmacs_time ();
    wid -> handle_keypress (r, texmacs_time());        
    //int end= texmacs_time ();
    //if (end > start) cout << "Keypress " << end - start << "\n";
    the_gui->update (); // FIXME: remove this line when
                        // edit_typeset_rep::get_env_value will be faster
  }
}

static unsigned int
mouse_state (QMouseEvent* event, bool flag) {
  unsigned int i= 0;
  Qt::MouseButtons bstate= event->buttons ();
  Qt::MouseButton  tstate= event->button ();
  Qt::KeyboardModifiers kstate= event->modifiers ();
  if (flag) bstate= bstate | tstate;
  if ((bstate & Qt::LeftButton     ) != 0) i += 1;
  if ((bstate & Qt::MidButton      ) != 0) i += 2;
  if ((bstate & Qt::RightButton    ) != 0) i += 4;
  if ((bstate & Qt::XButton1       ) != 0) i += 8;
  if ((bstate & Qt::XButton2       ) != 0) i += 16;
#ifdef Q_WS_MAC
  if ((kstate & Qt::AltModifier    ) != 0) i = 2;
  if ((kstate & Qt::MetaModifier   ) != 0) i = 4;
  if ((kstate & Qt::ShiftModifier  ) != 0) i += 256;
  if ((kstate & Qt::ControlModifier) != 0) i += 2048;
#else
  if ((kstate & Qt::ShiftModifier  ) != 0) i += 256;
  if ((kstate & Qt::ControlModifier) != 0) i += 512;
  if ((kstate & Qt::AltModifier    ) != 0) i += 2048;
  if ((kstate & Qt::MetaModifier   ) != 0) i += 16384;
#endif
  return i;
}

static string
mouse_decode (unsigned int mstate) {
  if      (mstate & 1 ) return "left";
  else if (mstate & 2 ) return "middle";
  else if (mstate & 4 ) return "right";
  else if (mstate & 8 ) return "up";
  else if (mstate & 16) return "down";
  return "unknown";
}

void
QTMWidget::mousePressEvent (QMouseEvent* event) {
  simple_widget_rep *wid= tm_widget ();
  if (!wid) return;
  QPoint point = event->pos();
  scale (point);
  unsigned int mstate= mouse_state (event, false);
  string s= "press-" * mouse_decode (mstate);
  wid -> handle_mouse (s, point.x (), point.y (), mstate, texmacs_time ());
  if (DEBUG_EVENTS)
    cout << "mouse event: " << s << " at "
	 << point.x () << ", " << point.y () << LF;
}

void
QTMWidget::mouseReleaseEvent (QMouseEvent* event) {
  simple_widget_rep *wid = tm_widget();
  if (!wid) return;
  QPoint point = event->pos();
  scale (point);
  unsigned int mstate= mouse_state (event, true);
  string s= "release-" * mouse_decode (mstate);
  wid -> handle_mouse (s, point.x (), point.y (), mstate, texmacs_time ());
  if (DEBUG_EVENTS)
    cout << "mouse event: " << s << " at "
	 << point.x () << ", " << point.y () << LF;
}

void
QTMWidget::mouseMoveEvent (QMouseEvent* event) {
  simple_widget_rep *wid = tm_widget();
  if (!wid) return;
  QPoint point = event->pos();
  scale (point);
  unsigned int mstate= mouse_state (event, false);
  string s= "move";
  wid -> handle_mouse (s, point.x (), point.y (), mstate, texmacs_time ());
  if (DEBUG_EVENTS)
    cout << "mouse event: " << s << " at "
	 << point.x () << ", " << point.y () << LF;
}


bool
QTMWidget::event (QEvent* event) {
  if (event->type() 
== 
QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent*> (event);
    keyPressEvent (ke);
    return true;
  }  
  return QWidget::event (event);
}
