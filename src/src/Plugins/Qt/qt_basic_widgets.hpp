
/******************************************************************************
* MODULE     : qt_basic_widgets.h
* DESCRIPTION: Basic widgets
* COPYRIGHT  : (C) 2008  Massimiliano Gubinelli
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef QT_BASIC_WIDGETS_HPP
#define QT_BASIC_WIDGETS_HPP
#include "qt_widget.hpp"

class qt_text_widget_rep: public qt_widget_rep {
public:
  string str, lan;
  color col;
  bool tsp;

  inline qt_text_widget_rep (string _s, color _col, bool _tsp, string _lan):
    str (_s), lan (_lan), col (_col), tsp (_tsp) {}
  virtual QAction* as_qaction ();
};

class qt_image_widget_rep: public qt_widget_rep {
public:
  url image;
  
  inline qt_image_widget_rep (url _image): image(_image) {}
  virtual QAction *as_qaction();
};

class qt_balloon_widget_rep: public qt_widget_rep {
public:
  widget text, hint;
  
  inline qt_balloon_widget_rep (widget _text, widget _hint):
    text (_text), hint (_hint) {}
  virtual QAction* as_qaction ();
};

class qt_input_text_widget_rep: public qt_widget_rep {
public:
  command cmd;
  string type;
  array<string> def;
  string text;

  inline qt_input_text_widget_rep
    (command _cmd, string _type, array<string> _def):
      cmd (_cmd), type (_type), def (_def), text ("") {}
};

#endif // QT_BASIC_WIDGETS_HPP
