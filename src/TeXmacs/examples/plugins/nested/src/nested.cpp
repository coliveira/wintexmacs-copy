
/******************************************************************************
* MODULE     : nested.cpp
* DESCRIPTION: A plugin which demonstrates nested DATA_BEGIN/DATA_END blocks
* COPYRIGHT  : (C) 2003  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include <stdio.h>
#include <iostream.h>

#define DATA_BEGIN   ((char) 2)
#define DATA_END     ((char) 5)
#define DATA_ESCAPE  ((char) 27)

void
f (int i, int nr) {
  if (i >= nr) cout << nr;
  else {
    cout << "\\frac{1}{" << i << "+";
    f (i+1, nr);
    cout << "}\n";
  }
}

int
main () {
  cout << DATA_BEGIN << "verbatim:";
  cout << "Please enter a positive integer at each input";
  cout << DATA_END;
  fflush (stdout);

  while (true) {
    int i, nr;
    cin >> nr;
    cout << DATA_BEGIN << "verbatim:";

    cout << "A first formula\n";
    cout << DATA_BEGIN << "latex:";
    cout << "$";
    for (i=1; i<nr; i++)
      cout << "x_{" << i << "}+";
    cout << "x_{" << i << "}$";
    cout << DATA_END;

    cout << "\nA second formula\n";
    cout << DATA_BEGIN << "latex:";
    cout << "$\\displaystyle ";
    f (1, nr);
    cout << "$";
    cout << DATA_END;

    cout << DATA_END;
    fflush (stdout);
  }
  return 0;
}
