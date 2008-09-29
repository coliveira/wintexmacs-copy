
/******************************************************************************
* MODULE     : qt_utilities.mm
* DESCRIPTION: Utilities for QT
* COPYRIGHT  : (C) 2007  Massimiliano Gubinelli
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include "qt_utilities.hpp"

QRect to_qrect(coord4 p)
{
  float c = 1.0/PIXEL;
  return QRect(p.x1*c, -p.x4*c, (p.x3-p.x1)*c, (p.x4-p.x2)*c);
}

QPoint to_qpoint(coord2 p)
{
  float c = 1.0/PIXEL;
  return QPoint(p.x1*c,-p.x2*c);
}

QSize to_qsize(coord2 p)
{
  float c = 1.0/PIXEL;
  return QSize(p.x1*c,p.x2*c);
}

coord4 from_qrect(QRect & rect)
{
  SI c1, c2, c3, c4;
	
  c1 = rect.x()*PIXEL;
  c2 = rect.y()*PIXEL;
  c3 = (rect.x()+rect.width())*PIXEL;
  c4 = (rect.y()+rect.height())*PIXEL;	
  return coord4 (c1, c2, c3, c4);
}

coord2 from_qpoint(QPoint & pt)
{
  SI c1, c2;
  c1 = pt.x()*PIXEL;
  c2 = -pt.y()*PIXEL;
  return coord2 (c1,c2)	;
}

coord2 from_qsize(QSize & s)
{
  SI c1, c2;
  c1 = s.width()*PIXEL;
  c2 = s.height()*PIXEL;
  return coord2 (c1,c2)	;
}

QString to_qstring(string s)
{
  char *p = as_charp(s);
  QString nss(p);
  delete [] p;	
  return nss;
}

string from_qstring(QString &s)
{
  QByteArray arr = s.toUtf8(); 
  const char *cstr = arr;
  return string((char*)cstr);
}