
/******************************************************************************
* MODULE     : qt_other_widgets.hpp
* DESCRIPTION: some QT widgets class declarations
* COPYRIGHT  : (C) 2008  Massimiliano Gubinelli
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef QT_OTHER_WIDGETS_HPP
#define QT_OTHER_WIDGETS_HPP

#include "qt_widget.hpp"
#include "QTMInteractiveInputHelper.hpp"

#include <QLabel>
#include <QMainWindow>
#include <QScrollArea>
#include <QToolBar>

class qt_tm_widget_rep: public qt_view_widget_rep {
public:	
  QLabel *rightLabel;
  QLabel *leftLabel;

  QToolBar *mainToolBar;
  QToolBar *contextToolBar;
  QToolBar *userToolBar;
	  
  QTMInteractiveInputHelper helper;  
	
  qt_widget int_prompt;
  qt_widget int_input;

  bool visibility[5]; 

public:
  qt_tm_widget_rep (int mask = 0);
  ~qt_tm_widget_rep ();
	
  virtual void send (slot s, blackbox val);
  virtual blackbox query (slot s, int type_id);
  virtual widget read (slot s, blackbox index);
  virtual void write (slot s, blackbox index, widget w);
  // virtual void notify (slot s, blackbox new_val);	
  // virtual void connect (slot s, widget w2, slot s2);
  // virtual void deconnect (slot s, widget w2, slot s2);
  virtual widget plain_window_widget (string s);
	
  inline QMainWindow* tm_mainwindow () {
    return qobject_cast<QMainWindow*> (view); }
  inline QScrollArea* tm_scrollarea () {
    return qobject_cast<QScrollArea*> (tm_mainwindow()->centralWidget()); }
  inline QWidget* tm_canvas () {
    return tm_scrollarea()->widget(); }

  
  // void layout();
  void updateVisibility();
  void do_interactive_prompt ();
};

class qt_window_widget_rep: public widget_rep {
  QWidget *wid;
public:
  qt_window_widget_rep (QWidget* _wid);
  ~qt_window_widget_rep ();

  virtual void send (slot s, blackbox val);
  virtual blackbox query (slot s, int type_id);
  virtual widget read (slot s, blackbox index);
  virtual void write (slot s, blackbox index, widget w);
  virtual void notify (slot s, blackbox new_val);
  // virtual void connect (slot s, widget w2, slot s2);
  // virtual void deconnect (slot s, widget w2, slot s2);
};

#endif // defined QT_OTHER_WIDGETS_HPP
