// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "GSL/gsl.h"
NS_USING(gsl)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
struct FS_DLL FDListItem {
  __decl_ptr(T , _prev)
  __decl_ptr(T , _next)
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
struct FS_DLL FDListAnchor {
  __decl_ptr(T , _head)
  __decl_ptr(T , _tail)
};

//////////////////////////////////////////////////////////////////////////////
// owns all items in this list
template <typename T>
struct FS_DLL FDList  {

  virtual bool isEmpty() { return _head==nullptr; }
  virtual void release(not_null<T*>);
  virtual void purge(not_null<T*>);
  virtual void add(not_null<T*> );

  const s_vec<T*> list();
  void clear() ;
  int size();

  __decl_ptr(T , _head)
  __decl_ptr(T , _tail)

  virtual ~FDList();
  FDList() {}
  __decl_nocpyass(FDList)
};

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::add(not_null<T*> e) {
  if (E_NIL(_head)) {
    assert(E_NIL(_tail) );
    e->_prev= P_NIL;
    e->_next= P_NIL;
    _head = _tail = e;
  } else {
    e->_prev = _tail;
    e->_next= P_NIL;
    _tail->_next = e;
    _tail = e;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FDList<T>::size() {
  auto n= _head;
  int c=0;
  while (N_NIL(n)) {
    n = n->_next;
    ++c;
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::release(not_null<T*> e) {
  if (_tail == e) { _tail = _tail->_prev; }
  if (_head == e) { _head = _head->_next; }
  if (N_NIL(e->_prev)) {
    e->_prev->_next = e->_next;
  }
  if (N_NIL(e->_next)) {
    e->_next->_prev = e->_prev;
  }
  e->_prev= P_NIL;
  e->_next= P_NIL;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::purge(not_null<T*> e) {
  release(e);
  delete e.get();
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::clear() {
  while (N_NIL(_head)) {
    auto e= _head;
    _head = _head->_next;
    delete e;
  }
  S__NIL(_head)
  S__NIL(_tail)
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
const s_vec<T*> FDList<T>::list() {
  s_vec<T*> v;
  for (auto p= _head; N_NIL(p); p=p->_next) {
    v.push_back(p);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
FDList<T>::~FDList() {
  //printf("FDList dtor\n");
  clear();
}


NS_END


