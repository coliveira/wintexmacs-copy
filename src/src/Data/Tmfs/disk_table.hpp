
/******************************************************************************
* MODULE     : disk_table.hpp
* DESCRIPTION: large size tables stored on disk
* COPYRIGHT  : (C) 2007  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#ifndef DISK_TABLE_H
#define DISK_TABLE_H
#include "url.hpp"
#include "hashmap.hpp"
#include "iterator.hpp"
#include <stdio.h>

typedef hashmap<string,int> collection;
typedef hashmap<string,collection> transaction;

class disk_table_rep: concrete_struct {
private:
  url         root;           // directory where the table is stored
  FILE*       pending_fp;     // file pointer for pending writes
  transaction pending_write;  // pending transactions for writing
  transaction pending_read;   // pending transactions for caching
  transaction read_cache;     // cache for reading

private:
  void open_pending_write ();
  void close_pending_write ();
  void flush_pending_write ();
  void flush_pending_read ();

public:
  disk_table_rep (url root2);
  ~disk_table_rep ();
  void write (transaction t);
  transaction read (collection keys);
  friend class disk_table;
};

class disk_table {
CONCRETE_NULL(disk_table);
  inline disk_table (url u):
    rep (new disk_table_rep (u)) {}
};
CONCRETE_NULL_CODE(disk_table);

#endif // defined DISK_TABLE_H