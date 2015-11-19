// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "XPool.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// Pre-populate a bunch of objects in the pool
void XPool::preset(s::function<ComObj* ()> f, int count) {
  for (int n=0; n < count; ++n) {
    auto rc= f();
    if (NNP(rc)) {
      objs.push_back(rc);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// Find an object by applying this filter
ComObj* XPool::select(s::function<bool (ComObj*)> f) {
  F__LOOP(it, objs) {
    auto e = *it;
    if (f(e)) {
      return e;
    }
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Get a free object from the pool and set it's status to true
ComObj* XPool::getAndSet() {
  auto rc= get();
  if (NNP(rc)) {
    rc->status=true;
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
ComObj* XPool::getAt(int pos) {
  return objs.at(pos);
}

//////////////////////////////////////////////////////////////////////////
// Get a free object from the pool.  More like a peek
ComObj* XPool::get() {
  F__LOOP(it, objs) {
    auto e= *it;
    if (! e->status) { return e; }
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void XPool::checkin(not_null<ComObj*> c) {
  objs.push_back(c);
}

//////////////////////////////////////////////////////////////////////////
//
void XPool::clearAll(bool del) {
  if (del) {
    F__LOOP(it, objs) { delete *it; }
  }
  objs.clear();
}

//////////////////////////////////////////////////////////////////////////
// Get the count of active objects
int XPool::countActives() {
  auto c=0;
  F__LOOP(it, objs) {
    if ((*it)->status) {
      ++c;
    }
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////
//
void XPool::foreach(s::function<void (ComObj*)> f) {
  F__LOOP(it, objs) {
    f(*it);
  }
}

//////////////////////////////////////////////////////////////////////////
// Hibernate (status off) all objects in the pool
void XPool::reset() {
  F__LOOP(it, objs) {
    (*it)->deflate();
  }
}

//////////////////////////////////////////////////////////////////////////
//
XPool::~XPool() {
  clearAll(true);
}

//////////////////////////////////////////////////////////////////////////
//
XPool::XPool() {
}



NS_END(fusii)

