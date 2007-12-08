
/******************************************************************************
* MODULE     : ip_observer.cpp
* DESCRIPTION: Persistently attach inverse paths to trees
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* An inverse path observer maintains the inverse path of the position
* of the corresponding tree with respect to the global meta-tree.
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include "tree.hpp"
#include "path.hpp"

#define DETACHED (-5)

/******************************************************************************
* Definition of the ip_observer_rep class
******************************************************************************/

class ip_observer_rep: public observer_rep {
  path ip;
public:
  ip_observer_rep (path ip2): ip (ip2) {}
  ostream& print (ostream& out) { return out << " " << ip; }

  void notify_assign      (tree& ref, tree t);
  void notify_insert      (tree& ref, int pos, int nr);
  void notify_remove      (tree& ref, int pos, int nr);
  void notify_split       (tree& ref, int pos, tree prev);
  void notify_var_split   (tree& ref, tree t1, tree t2);
  void notify_join        (tree& ref, int pos, tree next);
  void notify_var_join    (tree& ref, tree t, int offset);
  void notify_assign_node (tree& ref, tree_label op);
  void notify_insert_node (tree& ref, int pos);
  void notify_remove_node (tree& ref, int pos);
  void notify_detach      (tree& ref, tree closest, bool right);

  bool get_ip (path& ip);
  bool set_ip (path ip);
};

/******************************************************************************
* Call back routines for modifications
******************************************************************************/

void
ip_observer_rep::notify_assign (tree& ref, tree t) {
  // cout << "Notify assign " << ref << ", " << t << "\n";
  path temp_ip= obtain_ip (ref);
  temp_ip= path (temp_ip->item, temp_ip->next); // prevents overriding temp_ip
  detach_ip (ref);
  attach_ip (t, temp_ip);
}

void
ip_observer_rep::notify_insert (tree& ref, int pos, int nr) {
  // cout << "Notify insert " << ref << ", " << pos << ", " << nr << "\n";
  (void) nr;
  if (is_compound (ref)) {
    int i, n= N(ref);
    for (i=pos; i<n; i++)
      attach_ip (ref[i], path (i, ip));
  }
}

void
ip_observer_rep::notify_remove (tree& ref, int pos, int nr) {
  // cout << "Notify remove " << ref << ", " << pos << ", " << nr << "\n";
  (void) nr;
  if (is_compound (ref)) {
    int i, n= N(ref);
    for (i=pos; i<(pos+nr); i++)
      detach_ip (ref[i]);
    for (; i<n; i++)
      attach_ip (ref[i], path (i-nr, ip));
  }
}

void
ip_observer_rep::notify_split (tree& ref, int pos, tree prev) {
  // cout << "Notify split " << ref << ", " << pos << ", " << prev << "\n";
  int i, n= N(ref);
  detach_ip (prev);
  for (i=pos; i<n; i++)
    attach_ip (ref[i], path (i, ip));
}

void
ip_observer_rep::notify_var_split (tree& ref, tree t1, tree t2) {
  (void) ref; (void) t1; (void) t2;
}

void
ip_observer_rep::notify_join (tree& ref, int pos, tree next) {
  // cout << "Notify join " << ref << ", " << pos << ", " << next << "\n";
  int i, n= N(ref);
  detach_ip (ref[pos]);
  detach_ip (ref[pos+1]);
  for (i=pos+2; i<n; i++)
    attach_ip (ref[i], path (i-1, ip));
  attach_ip (next, path (pos, ip));
}

void
ip_observer_rep::notify_var_join (tree& ref, tree t, int offset) {
  (void) ref; (void) t; (void) offset;
}

void
ip_observer_rep::notify_assign_node (tree& ref, tree_label op) {
  // cout << "Notify assign node " << ref << ", " << as_string (op) << "\n";
  (void) ref; (void) op;
}

void
ip_observer_rep::notify_insert_node (tree& ref, int pos) {
  // cout << "Notify insert node " << ref << ", " << pos << "\n";
  ip= path (pos, ip);
  attach_ip (ref[pos], ip); // updates children's ips
  attach_ip (ref, ip->next);
}

void
ip_observer_rep::notify_remove_node (tree& ref, int pos) {
  // cout << "Notify remove node " << ref << ", " << pos << "\n";
  for (int i=0; i<N(ref); i++)
    if (i != pos)
      detach_ip (ref[i]);
  if ((!nil (ip)) && (ip->item>=0)) attach_ip (ref[pos], ip);
  else detach_ip (ref[pos]);
  ip= DETACHED; // detach_ip (ref);
}

void
ip_observer_rep::notify_detach (tree& ref, tree closest, bool right) {
  (void) ref; (void) closest; (void) right;
}

/******************************************************************************
* Setting and getting inverse paths
******************************************************************************/

bool
ip_observer_rep::get_ip (path& ip2) {
  ip2= ip;
  return true;
}

bool
ip_observer_rep::set_ip (path ip2) {
  if (nil (ip) || nil (ip2))
    fatal_error ("cannot alter global root", "ip_observer_rep::set_ip");
  ip->item= ip2->item;
  ip->next= ip2->next;
  return true;
}

void
attach_ip (tree& ref, path ip) {
  // cout << "Set ip of " << ref << " to " << ip << "\n";
  if (nil (ref->obs) || !ref->obs->set_ip (ip)) {
    // cout << "Create ip observer " << ip << " for " << ref << "\n";
    ref->obs= list_observer (ip_observer (ip), ref->obs);
  }
  if (is_compound (ref)) {
    int i, n= N(ref);
    for (i=0; i<n; i++) {
      path old_ip= obtain_ip (ref[i]);
      if ((old_ip->item != i) || (!strong_equal (old_ip->next, ip))) {
	attach_ip (ref[i], path (i, ip));
      }
    }
  }
}

void
detach_ip (tree& ref) {
  // cout << "Detach ip of " << ref << "\n";
  if (!nil (ref->obs))
    (void) ref->obs->set_ip (DETACHED);
}

path
obtain_ip (tree& ref) {
  path ip;
  if (nil (ref->obs)) return DETACHED;
  if (!ref->obs->get_ip (ip)) return DETACHED;
  return ip;
}

/******************************************************************************
* Setting and getting inverse paths
******************************************************************************/

observer
ip_observer (path ip) {
  return new ip_observer_rep (ip);
}