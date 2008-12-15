
/******************************************************************************
* MODULE     : basic.hpp
* DESCRIPTION: see basic.cpp
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef BASIC_H
#define BASIC_H
#include "tm_configure.hpp"
#include <stdlib.h>
#include <math.h>

// g++ >= 3.2 requires
#include <iostream>
using std::ostream;
using std::cout;
using std::cerr;
// instead of include <iostream.h>

#ifdef HAVE_INTPTR_T
#ifdef OS_SUN
#include <inttypes.h>
#else
#include <stdint.h>
#endif
#else
typedef long intptr_t;
#endif

#ifdef OS_WIN32
#define LESSGTR
#else
#define LESSGTR <>
#endif

#define DEBUG(x)
typedef int SI;
typedef unsigned int SN;
typedef short HI;
typedef unsigned short HN;
typedef char QI;
typedef unsigned char QN;
#ifdef OS_WIN32
typedef __int64 DI;
#else
typedef long long int DI;
#endif
typedef void* pointer;

#ifndef NO_FAST_ALLOC
#ifdef OS_IRIX
void* operator new (register size_t s) throw(std::bad_alloc);
void  operator delete (register void* ptr) throw();
void* operator new[] (register size_t s) throw(std::bad_alloc);
void  operator delete[] (register void* ptr) throw();
#else
void* operator new (register size_t s);
void  operator delete (register void* ptr);
void* operator new[] (register size_t s);
void  operator delete[] (register void* ptr);
#endif
#endif // not defined NO_FAST_ALLOC
int   mem_used ();
void  mem_info ();

#if (defined OS_WIN32 || defined __SUNPRO_CC)
#define STACK_NEW_ARRAY(name,T,size) T* name= tm_new_array<T> (size)
#define STACK_DELETE_ARRAY(name) tm_delete_array (name)
#else
#define STACK_NEW_ARRAY(name,T,size) T name[size]
#define STACK_DELETE_ARRAY(name)
#endif

enum { DEBUG_FLAG_AUTO, DEBUG_FLAG_VERBOSE, DEBUG_FLAG_EVENTS,
       DEBUG_FLAG_STD, DEBUG_FLAG_IO, DEBUG_FLAG_BENCH };
bool debug (int which, bool write_flag= false);
int  debug_off ();
void debug_on (int status);
#define DEBUG_AUTO (debug (DEBUG_FLAG_AUTO))
#define DEBUG_VERBOSE (debug (DEBUG_FLAG_VERBOSE))
#define DEBUG_EVENTS (debug (DEBUG_FLAG_EVENTS))
#define DEBUG_STD (debug (DEBUG_FLAG_STD))
#define DEBUG_IO (debug (DEBUG_FLAG_IO))
#define DEBUG_BENCH (debug (DEBUG_FLAG_BENCH))

#ifdef DEBUG_ASSERT
#include <assert.h>
#define ASSERT(cond,msg) assert (cond); // msg
#else
#define ASSERT(cond,msg)
#endif

inline SI min (SI i, SI j) { if (i<j) return i; else return j; }
inline SI max (SI i, SI j) { if (i>j) return i; else return j; }
inline double min (double i, double j) { if (i<j) return i; else return j; }
inline double max (double i, double j) { if (i>j) return i; else return j; }
inline int hash (int i) { return i; }
inline int hash (pointer ptr) {
  return ((int) ((intptr_t) ptr)) + (((int) ((intptr_t) ptr)) % 19); }
inline int copy (int x) { return x; }
inline SI as_int (double x) { return (SI) floor (x + 0.5); }

enum display_control { INDENT, UNINDENT, HRULE, LF };
ostream& operator << (ostream& out, display_control ctrl);

extern int concrete_count;
struct concrete_struct {
  int ref_count;
  inline concrete_struct (): ref_count (1) { DEBUG(concrete_count++); }
  virtual inline ~concrete_struct () { DEBUG(concrete_count--); }
};

extern int abstract_count;
struct abstract_struct {
  int ref_count;
  inline abstract_struct (): ref_count (0) { DEBUG(abstract_count++); }
  virtual inline ~abstract_struct () { DEBUG(abstract_count--); }
};

template<typename T>
struct type_helper {
  static int id;
  static T   init;
};

int new_type_identifier ();
template<typename T> int type_helper<T>::id  = new_type_identifier ();
template<typename T> T   type_helper<T>::init= T ();

/******************************************************************************
* indirect structures
******************************************************************************/

#define INC_COUNT(R)      { (R)->ref_count++; }
#define DEC_COUNT(R)      { if(0==--((R)->ref_count)) tm_delete (R); }
#define INC_COUNT_NULL(R) { if ((R)!=NULL) (R)->ref_count++; }
#define DEC_COUNT_NULL(R) { if ((R)!=NULL && 0==--((R)->ref_count)) tm_delete (R); }

// concrete
#define CONCRETE(PTR)               \
  PTR##_rep *rep;                   \
public:                             \
  inline PTR (const PTR&);          \
  inline ~PTR ();                   \
  inline PTR##_rep* operator -> (); \
  inline PTR& operator = (PTR x)
#define CONCRETE_CODE(PTR)                       \
  inline PTR::PTR (const PTR& x):                \
    rep(x.rep) { INC_COUNT (this->rep); }        \
  inline PTR::~PTR () { DEC_COUNT (this->rep); } \
  inline PTR##_rep* PTR::operator -> () {        \
    return rep; }                                \
  inline PTR& PTR::operator = (PTR x) {          \
    INC_COUNT (x.rep); DEC_COUNT (this->rep);    \
    this->rep=x.rep; return *this; }

// definition for 1 parameter template classes
#define CONCRETE_TEMPLATE(PTR,T)      \
  PTR##_rep<T> *rep;                  \
public:                               \
  inline PTR (const PTR<T>&);         \
  inline ~PTR ();                     \
  inline PTR##_rep<T>* operator -> (); \
  inline PTR<T>& operator = (PTR<T> x)
#define CONCRETE_TEMPLATE_CODE(PTR,TT,T)                          \
  template<TT T> inline PTR<T>::PTR (const PTR<T>& x):            \
    rep(x.rep) { INC_COUNT (this->rep); }                         \
  template<TT T> inline PTR<T>::~PTR() { DEC_COUNT (this->rep); } \
  template<TT T> inline PTR##_rep<T>* PTR<T>::operator -> () {    \
    return this->rep; }                                           \
  template<TT T> inline PTR<T>& PTR<T>::operator = (PTR<T> x) {   \
    INC_COUNT (x.rep); DEC_COUNT (this->rep);                     \
    this->rep=x.rep; return *this; }

// definition for 2 parameter template classes
#define CONCRETE_TEMPLATE_2(PTR,T1,T2)     \
  PTR##_rep<T1,T2> *rep;                   \
public:                                    \
  inline PTR (const PTR<T1,T2>&);          \
  inline ~PTR ();                          \
  inline PTR##_rep<T1,T2>* operator -> (); \
  inline PTR<T1,T2>& operator = (PTR<T1,T2> x)
#define CONCRETE_TEMPLATE_2_CODE(PTR,TT1,T1,TT2,T2)                           \
  template<TT1 T1,TT2 T2> inline PTR<T1,T2>::PTR (const PTR<T1,T2>& x):       \
    rep(x.rep) { INC_COUNT (this->rep); }                                     \
  template<TT1 T1,TT2 T2> inline PTR<T1,T2>::~PTR () { DEC_COUNT(this->rep);} \
  template<TT1 T1,TT2 T2> inline PTR##_rep<T1,T2>* PTR<T1,T2>::operator -> () \
    { return this->rep; }                                                     \
  template <TT1 T1,TT2 T2>                                                    \
  inline PTR<T1,T2>& PTR<T1,T2>::operator = (PTR<T1,T2> x) {                  \
    INC_COUNT (x.rep); DEC_COUNT (this->rep);                                 \
    this->rep=x.rep; return *this; }
// end concrete

// abstract
#define ABSTRACT(PTR) \
  CONCRETE(PTR);      \
  inline PTR (PTR##_rep*)
#define ABSTRACT_CODE(PTR) \
  CONCRETE_CODE(PTR) ;     \
  inline PTR::PTR (PTR##_rep* rep2): rep(rep2) { INC_COUNT (this->rep); }
#define ABSTRACT_TEMPLATE(PTR,T) \
  CONCRETE_TEMPLATE(PTR,T);      \
  inline PTR (PTR##_rep<T>*)
#define ABSTRACT_TEMPLATE_CODE(PTR,TT,T)                  \
  CONCRETE_TEMPLATE_CODE(PTR,TT,T);                       \
  template<TT T> inline PTR<T>::PTR (PTR##_rep<T>* rep2): \
    rep(rep2) { INC_COUNT (this->rep); }
// end abstract

/******************************************************************************
* null indirect structures
******************************************************************************/

// concrete_null
#define CONCRETE_NULL(PTR) \
  CONCRETE(PTR);           \
  inline PTR();            \
  friend bool is_nil /*LESSGTR*/ (PTR x)
#define CONCRETE_NULL_CODE(PTR)                         \
  inline PTR::PTR (): rep(NULL) {}                      \
  inline PTR::PTR (const PTR& x):                       \
    rep(x.rep) { INC_COUNT_NULL (this->rep); }          \
  inline PTR::~PTR() { DEC_COUNT_NULL (this->rep); }    \
  inline PTR##_rep* PTR::operator -> () {               \
    return this->rep; }                                 \
  inline PTR& PTR::operator = (PTR x) {                 \
    INC_COUNT_NULL (x.rep); DEC_COUNT_NULL (this->rep); \
    this->rep=x.rep; return *this; }                    \
  inline bool is_nil (PTR x) { return x.rep==NULL; }
#define CONCRETE_NULL_TEMPLATE(PTR,T) \
  CONCRETE_TEMPLATE(PTR,T);           \
  inline PTR();                       \
  friend bool is_nil LESSGTR (PTR<T> x)
#define CONCRETE_NULL_TEMPLATE_CODE(PTR,TT,T)                           \
  template<TT T> inline PTR<T>::PTR (): rep(NULL) {}                    \
  template<TT T> inline PTR<T>::PTR (const PTR<T>& x):                  \
    rep(x.rep) { INC_COUNT_NULL (this->rep); }                          \
  template<TT T> inline PTR<T>::~PTR () { DEC_COUNT_NULL (this->rep); } \
  template<TT T> inline PTR##_rep<T>* PTR<T>::operator -> () {          \
    return this->rep; }                                                 \
  template<TT T> inline PTR<T>& PTR<T>::operator = (PTR<T> x) {         \
    INC_COUNT_NULL (x.rep); DEC_COUNT_NULL (this->rep);                 \
    this->rep=x.rep; return *this; }                                    \
  template<TT T> inline bool is_nil (PTR<T> x) { return x.rep==NULL; }

#define CONCRETE_NULL_TEMPLATE_2(PTR,T1,T2) \
  CONCRETE_TEMPLATE_2(PTR,T1,T2);           \
  inline PTR();                             \
  friend bool is_nil LESSGTR (PTR<T1,T2> x)
#define CONCRETE_NULL_TEMPLATE_2_CODE(PTR,TT1,T1,TT2,T2)                  \
  template<TT1 T1, TT2 T2> inline PTR<T1,T2>::PTR (): rep(NULL) {}        \
  template<TT1 T1, TT2 T2> inline PTR<T1,T2>::PTR (const PTR<T1,T2>& x):  \
    rep(x.rep) { INC_COUNT_NULL (this->rep); }                            \
  template<TT1 T1, TT2 T2> inline PTR<T1,T2>::~PTR () {                   \
    DEC_COUNT_NULL (this->rep); }                                         \
  template<TT1 T1, TT2 T2> PTR##_rep<T1,T2>* PTR<T1,T2>::operator -> () { \
    return this->rep; }                                                   \
  template<TT1 T1, TT2 T2>                                                \
  inline PTR<T1,T2>& PTR<T1,T2>::operator = (PTR<T1,T2> x) {              \
    INC_COUNT_NULL (x.rep); DEC_COUNT_NULL (this->rep);                   \
    this->rep=x.rep; return *this; }                                      \
  template<TT1 T1, TT2 T2> inline bool is_nil (PTR<T1,T2> x) {               \
    return x.rep==NULL; }
// end concrete_null

// abstract_null
#define ABSTRACT_NULL(PTR) \
  CONCRETE_NULL (PTR);     \
  inline PTR (PTR##_rep*)
#define ABSTRACT_NULL_CODE(PTR)      \
  CONCRETE_NULL_CODE (PTR);          \
  inline PTR::PTR (PTR##_rep* rep2): \
    rep(rep2) { INC_COUNT_NULL (this->rep); }
#define ABSTRACT_NULL_TEMPLATE(PTR,T) \
  CONCRETE_NULL_TEMPLATE (PTR,T);     \
  inline PTR (PTR##_rep<T>*)
#define ABSTRACT_NULL_TEMPLATE_CODE(PTR,TT,T)              \
  CONCRETE_NULL_TEMPLATE_CODE (PTR,TT,T);                  \
  template<TT T> inline PTR<T>::PTR (PTR##_rep<T>* rep2):  \
    rep(rep2) { INC_COUNT (this->rep); }
#define ABSTRACT_NULL_TEMPLATE_2(PTR,T1,T2) \
  CONCRETE_NULL_TEMPLATE_2 (PTR,T1,T2);     \
  inline PTR (PTR##_rep<T1,T2>*)
#define ABSTRACT_NULL_TEMPLATE_2_CODE(PTR,TT1,T1,TT2,T2)                    \
  CONCRETE_NULL_TEMPLATE_2_CODE (PTR,TT1,T1,TT2,T2);                          \
  template<TT1 T1,TT2 T2> inline PTR<T1,T2>::PTR (PTR##_rep<T1,T2>* rep2):  \
    rep(rep2) { INC_COUNT (this->rep); }
// end abstract_null

/******************************************************************************
* extensions
******************************************************************************/

#define EXTEND(BASE,PTR) \
  ABSTRACT(PTR);         \
  inline PTR(BASE&);     \
  inline operator BASE ()
#define EXTEND_CODE(BASE,PTR)             \
  ABSTRACT_CODE(PTR);                     \
  inline PTR::PTR(BASE& x):               \
    rep(static_cast<PTR##_rep*>(x.rep)) { \
    INC_COUNT (this->rep); }              \
  inline PTR::operator BASE () { return BASE (this->rep); }
// end extend

// extend_null
#define EXTEND_NULL(BASE,PTR) \
  ABSTRACT_NULL(PTR);         \
  inline PTR(BASE&);          \
  inline operator BASE ()
#define EXTEND_NULL_CODE(BASE,PTR)        \
  ABSTRACT_NULL_CODE(PTR);                \
  inline PTR::PTR(BASE& x):               \
    rep(static_cast<PTR##_rep*>(x.rep)) { \
    INC_COUNT_NULL(this->rep); }          \
  inline PTR::operator BASE () { return BASE (this->rep); }

#define EXTEND_NULL_TEMPLATE(BASE,PTR,T) \
  ABSTRACT_NULL_TEMPLATE(PTR,T);         \
  inline PTR<T>(BASE&);                  \
  inline operator BASE ()
#define EXTEND_NULL_TEMPLATE_CODE(BASE,PTR,TT,T) \
  ABSTRACT_NULL_TEMPLATE_CODE(PTR,TT,T);         \
  template<TT T> inline PTR<T>::PTR(BASE& x):    \
    rep(static_cast<PTR##_rep<T>*>(x.rep)) {     \
    INC_COUNT_NULL(this->rep); }                 \
  template<TT T> inline PTR<T>::operator BASE () { return BASE (this->rep); }
// end extend_null

/******************************************************************************
* Fast new and delete
******************************************************************************/

#ifdef OLD_GNU_COMPILER
inline void* operator new   (register size_t s, void* loc) { return loc; }
inline void* operator new[] (register size_t s, void* loc) { return loc; }
#else
#include <new>
#endif

extern void* fast_alloc (register size_t s);
extern void  fast_free (register void* ptr, register size_t s);

template<typename C> inline C*
tm_new () {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C ();
  return (C*) ptr;
}

template<typename C, typename A1> inline C*
tm_new (const A1& a1) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (const A1& a1, const A2& a2) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (const A1& a1, A2& a2) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (A1& a1, const A2& a2) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (A1& a1, A2& a2) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (const A1& a1, A2& a2, A3& a3) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (A1& a1, A2& a2, const A3& a3) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (A1& a1, A2& a2, A3& a3) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4> inline C*
tm_new (const A1& a1, A2& a2, A3& a3, A4& a4) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (const A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6> inline C*
tm_new (A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, A6& a6) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6, typename A7> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6, typename A7> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	A4& a4, const A5& a5, const A6& a6,
	const A7& a7) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
		      a11, a12, a13);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13, typename A14> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13, const A14& a14) {
  void* ptr= fast_alloc (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
		      a11, a12, a13, a14);
  return (C*) ptr;
}

template<typename C> inline void
tm_delete (C* ptr) {
  ptr -> ~C ();
  fast_free ((void*) ptr, sizeof (C));
}

template<typename C> inline C*
tm_new_array (int n) {
  void* ptr= fast_alloc (n * sizeof (C) + 8);
  *((int*) ptr)= n;
  ptr= (void*) (((char*) ptr) + 8);
  C* ctr= (C*) ptr;
  for (int i=0; i<n; i++, ctr++)
    (void) new ((void*) ctr) C ();
  return (C*) ptr;
}

template<typename C> inline void
tm_delete_array (C* Ptr) {
  void* ptr= (void*) Ptr;
  ptr= (void*) (((char*) ptr) - 8);
  int n= *((int*) ptr);
  C* ctr= Ptr+n-1;
  for (int i=0; i<n; i++, ctr--) ctr -> ~C();
  fast_free (ptr, n * sizeof (C) + 8);
}

#endif // defined BASIC_H
