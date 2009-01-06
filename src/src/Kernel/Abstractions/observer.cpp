
/******************************************************************************
* MODULE     : observer.cpp
* DESCRIPTION: Observers of trees
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "modification.hpp"
#include "analyze.hpp"
#include "hashmap.hpp"

#define DETACHED (-5)

observer nil_observer;
extern tree the_et;

/******************************************************************************
* Debugging facilities
******************************************************************************/

static void
consistency_check (tree t, path ip) {
  if (obtain_ip (t) != ip)
    cout << "Wrong ip] " << t << " " << obtain_ip (t)
	 << " instead of " << ip << "\n";
  if (is_compound (t)) {
    int i, n= N(t);
    for (i=0; i<n; i++) {
      //if (!strong_equal (ip, obtain_ip (t[i])->next))
      if (obtain_ip (t) != obtain_ip (t[i])->next)
	cout << "Bad node] " << t << " " << obtain_ip (t) << " #" << i << "\n";
      consistency_check (t[i], path (i, ip));
    }
  }
}

void
consistency_check () {
  consistency_check (the_et, path ());
  cout << HRULE;
}

void
stretched_print (tree t, bool ips, int indent) {
  int i;
  for (i=0; i<indent; i++) cout << "  ";
  if (is_atomic (t)) {
    cout << raw_quote (t->label);
    if (ips) cout << " -- " << obtain_ip (t);
    cout << "\n";
  }
  else {
    cout << as_string (L(t));
    if (ips) cout << " -- " << obtain_ip (t);
    cout << "\n";    
    for (i=0; i<N(t); i++)
      stretched_print (t[i], ips, indent+1);
  }
}

ostream&
operator << (ostream& out, observer o) {
  out << "<observer";
  if (is_nil (o)) out << " null";
  else o->print (out);
  out << ">";
  return out;
}

/******************************************************************************
* Routines for modifying trees
*******************************************************************************
* 1) The "inserting modifications" (insert, split and insert_node) invoke
*    the observers call-back routines after the actual modification and
*    "assigning and deleting modifications" (assign, remove, join,
*    assign_node and remove_node)  before the actual modification.
* 2) The split and join modifications pass the joined tree
*    at position pos as an additional argument to the call-back routines.
* 3) They also admit variant call back routines for the split/join nodes.
******************************************************************************/

static void
simplify (observer& obs) {
  if (is_nil (obs)) return;
  observer& o1= obs->get_child (0);
  observer& o2= obs->get_child (1);  
  if (!is_nil (o1) || !is_nil (o2)) {
    simplify (o1);
    simplify (o2);
    obs= list_observer (o1, o2);
  }
}

static void
detach (tree& ref, tree closest, bool right) {
  if (!is_nil (ref->obs)) {
    ref->obs->notify_detach (ref, closest, right);
    simplify (ref->obs);
  }
  if (is_compound (ref)) {
    int i, n= N(ref);
    for (i=0; i<n; i++)
      detach (ref[i], closest, right);
  }
}

static pointer busy_root= NULL;
static hashmap<pointer,bool> busy_table (false);

bool
is_modifying (tree& ref) {
  return busy_table->contains (((void*) (&ref)));
}

static void
start_modifications (tree& ref) {
  //cout << "Start " << busy_root << ", " << ((void*) (&ref)) << "\n";
  if (busy_root == NULL) busy_root= ((void*) (&ref));
  busy_table (((void*) (&ref)))= true;
}

static void
end_modifications (tree& ref) {
  //cout << "End " << busy_root << ", " << ((void*) (&ref)) << "\n";
  if (busy_root != ((void*) (&ref))) return;
  busy_root = false;
  busy_table= hashmap<pointer,bool> (false);
}

void
assign (tree& ref, tree t) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Assign " << ref << " := " << t << "\n";
  modification mod= mod_assign (path (), t);
  if (!is_nil (ref->obs)) {
    ref->obs->announce (ref, mod);
    ref->obs->notify_assign (ref, t);
    simplify (ref->obs);
  }
  if (is_compound (ref)) {
    int i, n= N(ref), mid= (n+1)>>1;
    for (i=0; i<n; i++)
      detach (ref[i], t, i >= mid);
  }
  ref= t;
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
insert (tree& ref, int pos, tree t) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Insert " << ref << " += " << t << " at " << pos << "\n";
  modification mod= mod_insert (path (), pos, t);
  if (!is_nil (ref->obs))
    ref->obs->announce (ref, mod);
  if (is_atomic (ref) && is_atomic (t))
    ref->label= ref->label (0, pos) *t->label* ref->label (pos, N(ref->label));
  else {
    int i, n= N(ref), nr= N(t);
    AR(ref)->resize (n+nr);
    for (i=n-1; i>=pos; i--)
      ref[i+nr]= ref[i];
    for (i=0; i<nr; i++)
      ref[pos+i]= t[i];
  }
  if (!is_nil (ref->obs)) {
    ref->obs->notify_insert (ref, pos, is_atomic (t)? N(t->label): N(t));
    simplify (ref->obs);
  }
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
remove (tree& ref, int pos, int nr) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Remove " << ref << " -= " << nr << " at " << pos << "\n";
  modification mod= mod_remove (path (), pos, nr);
  if (!is_nil (ref->obs)) {
    ref->obs->announce (ref, mod);
    ref->obs->notify_remove (ref, pos, nr);
    simplify (ref->obs);
  }
  if (is_compound (ref)) {
    int i, n= N(ref), end= pos+nr, mid= (pos+end+1) >> 1;
    for (i=pos; i<mid; i++)
      if (pos == 0) detach (ref[i], ref, false);
      else detach (ref[i], ref[pos-1], true);
    for (; i<end; i++)
      if (end == n) detach (ref[i], ref, true);
      else detach (ref[i], ref[pos+nr], false);
  }

  if (is_atomic (ref))
    ref->label= ref->label (0, pos) * ref->label (pos+nr, N(ref->label));
  else {
    int i, n= N(ref)-nr;
    for (i=pos; i<n; i++)
      ref[i]= ref[i+nr];
    AR(ref)->resize (n);
  }
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
split (tree& ref, int pos, int at) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Split " << ref << " at " << pos << ", " << at << "\n";
  modification mod= mod_split (path (), pos, at);
  if (!is_nil (ref->obs))
    ref->obs->announce (ref, mod);
  tree t= ref[pos], t1, t2;
  if (is_atomic (ref[pos])) {    
    t1= ref[pos]->label (0, at);
    t2= ref[pos]->label (at, N(ref[pos]->label));
  }
  else {
    t1= ref[pos] (0, at);
    t2= ref[pos] (at, N(ref[pos]));
  }
  int i, n= N(ref);
  AR(ref)->resize (n+1);
  for (i=n; i>(pos+1); i--)
    ref[i]= ref[i-1];
  ref[pos  ]= t1;
  ref[pos+1]= t2;

  if (!is_nil (ref->obs)) {
    ref->obs->notify_split (ref, pos, t);
    simplify (ref->obs);
  }
  if (!is_nil (t->obs)) {
    t->obs->notify_var_split (t, t1, t2);
    simplify (t->obs);
  }
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
join (tree& ref, int pos) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Join " << ref << " at " << pos << "\n";
  // the following code is added for security
  if (is_atomic (ref[pos]) && (!is_atomic (ref[pos+1])))
    insert_node (ref[pos], 0, tree (L(ref[pos+1])));
  if (is_atomic (ref[pos+1]) && (!is_atomic (ref[pos])))
    insert_node (ref[pos+1], 0, tree (L(ref[pos])));
  // end security code

  modification mod= mod_join (path (), pos);
  if (!is_nil (ref->obs)) ref->obs->announce (ref, mod);
  tree t1= ref[pos], t2= ref[pos+1], t;
  int offset= is_atomic (ref)? N(t1->label): N(t1);
  if (is_atomic (t1) && is_atomic (t2)) t= t1->label * t2->label;
  else t= t1 * t2;
  if (!is_nil (ref->obs)) ref->obs->notify_join (ref, pos, t);
  if (!is_nil (t1->obs)) {
    t1->obs->notify_var_join (t1, t, 0);
    simplify (t1->obs);
  }
  if (!is_nil (t2->obs)) {
    t2->obs->notify_var_join (t2, t, offset);
    simplify (t2->obs);
  }
  ref[pos]= t;

  int i, n= N(ref)-1;
  for (i=pos+1; i<n; i++)
    ref[i]= ref[i+1];
  AR(ref)->resize (n);
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
assign_node (tree& ref, tree_label op) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Assign node " << ref << " : " << tree (op) << "\n";
  modification mod= mod_assign_node (path (), op);
  if (!is_nil (ref->obs)) {
    ref->obs->announce (ref, mod);
    ref->obs->notify_assign_node (ref, op);
    simplify (ref->obs);
  }
  LR (ref)= op;
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
insert_node (tree& ref, int pos, tree t) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Insert node " << ref << " : " << t << " at " << pos << "\n";
  modification mod= mod_insert_node (path (), pos, t);
  if (!is_nil (ref->obs)) ref->obs->announce (ref, mod);
  int i, n= N(t);
  tree r (t, n+1);
  for (i=0; i<pos; i++) r[i]= t[i];
  r[pos]= ref;
  for (i=pos; i<n; i++) r[i+1]= t[i];
  ref= r;
  if (!is_nil (ref[pos]->obs)) {
    ref[pos]->obs->notify_insert_node (ref, pos);
    simplify (ref[pos]->obs);
  }
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

void
remove_node (tree& ref, int pos) {
  if (is_modifying (ref)) return;
  start_modifications (ref);
  // cout << "Remove node " << ref << " : " << pos << "\n";
  modification mod= mod_remove_node (path (), pos);
  if (!is_nil (ref->obs)) {
    ref->obs->announce (ref, mod);
    ref->obs->notify_remove_node (ref, pos);
    simplify (ref->obs);
  }
  for (int i=0; i<N(ref); i++)
    if (i < pos) detach (ref[i], ref[pos], false);
    else if (i > pos) detach (ref[i], ref[pos], true);
  ref= ref[pos];
  if (!is_nil (ref->obs)) ref->obs->done (ref, mod);
  // stretched_print (ref, true, 1);
  // consistency_check ();
  end_modifications (ref);
}

/******************************************************************************
* Wrappers for trees given by a path
******************************************************************************/

void
assign (path p, tree t) {
  assign (subtree (the_et, p), t);
}

void
insert (path p, tree ins) {
  insert (subtree (the_et, path_up (p)), last_item (p), ins);
}

void
remove (path p, int nr) {
  remove (subtree (the_et, path_up (p)), last_item (p), nr);
}

void
split (path p) {
  tree& st= subtree (the_et, path_up (path_up (p)));
  int   l1= last_item (path_up (p));
  int   l2= last_item (p);
  split (st, l1, l2);  
}

void
join (path p) {
  join (subtree (the_et, path_up (p)), last_item (p));
}

void
assign_node (path p, tree_label op) {
  assign_node (subtree (the_et, p), op);
}

void
insert_node (path p, tree ins) {
  insert_node (subtree (the_et, path_up (p)), last_item (p), ins);
}

void
remove_node (path p) {
  remove_node (subtree (the_et, path_up (p)), last_item (p));
}

/******************************************************************************
* Default virtual routines
******************************************************************************/

void
observer_rep::announce (tree& ref, modification mod) {
  // cout << "Modify: " << mod << "\n";
  switch (mod->k) {
  case MOD_ASSIGN:
    announce_assign (ref, mod->p, mod->t);
    break;
  case MOD_INSERT:
    announce_insert (ref, mod->p, mod->t);
    break;
  case MOD_REMOVE:
    announce_remove (ref, path_up (mod->p), last_item (mod->p));
    break;
  case MOD_SPLIT:
    announce_split (ref, mod->p);
    break;
  case MOD_JOIN:
    announce_join (ref, mod->p);
    break;
  case MOD_ASSIGN_NODE:
    announce_assign_node (ref, mod->p, L(mod->t));
    break;
  case MOD_INSERT_NODE:
    announce_insert_node (ref, mod->p, mod->t);
    break;
  case MOD_REMOVE_NODE:
    announce_remove_node (ref, mod->p);
    break;
  }
}

void
observer_rep::done (tree& ref, modification mod) {
  (void) ref; (void) mod;
}

void
observer_rep::announce_assign (tree& ref, path p, tree t) {
  (void) ref; (void) p; (void) t;
}

void
observer_rep::announce_insert (tree& ref, path p, tree ins) {
  (void) ref; (void) p; (void) ins;
}

void
observer_rep::announce_remove (tree& ref, path p, int nr) {
  (void) ref; (void) p; (void) nr;
}

void
observer_rep::announce_split (tree& ref, path p) {
  (void) ref; (void) p;
}

void
observer_rep::announce_join (tree& ref, path p) {
  (void) ref; (void) p;
}

void
observer_rep::announce_assign_node (tree& ref, path p, tree_label op) {
  (void) ref; (void) p; (void) op;
}

void
observer_rep::announce_insert_node (tree& ref, path p, tree ins) {
  (void) ref; (void) p; (void) ins;
}

void
observer_rep::announce_remove_node (tree& ref, path p) {
  (void) ref; (void) p;
}

void
observer_rep::notify_assign (tree& ref, tree t) {
  (void) ref; (void) t;
}

void
observer_rep::notify_insert (tree& ref, int pos, int nr) {
  (void) ref; (void) pos; (void) nr;
}

void
observer_rep::notify_remove (tree& ref, int pos, int nr) {
  (void) ref; (void) pos; (void) nr;
}

void
observer_rep::notify_split (tree& ref, int pos, tree prev) {
  (void) ref; (void) pos; (void) prev;
}

void
observer_rep::notify_var_split (tree& ref, tree t1, tree t2) {
  (void) ref; (void) t1; (void) t2;
}

void
observer_rep::notify_join (tree& ref, int pos, tree next) {
  (void) ref; (void) pos; (void) next;
}

void
observer_rep::notify_var_join (tree& ref, tree t, int offset) {
  (void) ref; (void) t; (void) offset;
}

void
observer_rep::notify_assign_node (tree& ref, tree_label op) {
  (void) ref; (void) op;
}

void
observer_rep::notify_insert_node (tree& ref, int pos) {
  (void) ref; (void) pos;
}

void
observer_rep::notify_remove_node (tree& ref, int pos) {
  (void) ref; (void) pos;
}

void
observer_rep::notify_detach (tree& ref, tree closest, bool right) {
  (void) ref; (void) closest; (void) right;
}

bool
observer_rep::get_ip (path& ip) {
  (void) ip;
  return false;
}

bool
observer_rep::set_ip (path ip) {
  (void) ip;
  return false;
}

bool
observer_rep::get_position (tree& t, int& index) {
  (void) t; (void) index;
  return false;
}

bool
observer_rep::set_position (tree t, int index) {
  (void) t; (void) index;
  return false;
}

observer&
observer_rep::get_child (int which) {
  (void) which;
  return nil_observer;
}

list<observer>
observer_rep::get_tree_pointers () {
  return list<observer> ();
}

bool
observer_rep::get_tree (tree& t) {
  (void) t;
  return false;
}
