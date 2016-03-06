// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "FPool.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// Pre-populate a bunch of objects in the pool
void FPool::preset(s::function<Poolable* ()> f, int count) {
  for (auto n=0; n < count; ++n) {
    auto rc= f();
    if (NNP(rc)) {
      objs.push_back(rc);
    }
  }
  batch=count;
  ctor=f;
}

//////////////////////////////////////////////////////////////////////////
// Find an object by applying this filter
Poolable* FPool::select(s::function<bool (Poolable*)> f) {
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
Poolable* FPool::getAndSet(bool create) {
  auto rc= get(create);
  if (NNP(rc)) {
    rc->take();
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
Poolable* FPool::getAt(int pos) {
  return objs.at(pos);
}

//////////////////////////////////////////////////////////////////////////
// Get a free object from the pool.  More like a peek
Poolable* FPool::get(bool create) {
  F__LOOP(it, objs) {
    auto e= *it;
    if (! e->status()) { return e; }
  }
  if (create &&  ctor) {
    preset(ctor,batch);
    return get();
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void FPool::checkin(not_null<Poolable*> c) {
  objs.push_back(c);
}

//////////////////////////////////////////////////////////////////////////
//
void FPool::clearAll() {
  if (ownObjects) {
    F__LOOP(it, objs) { delete *it; }
  }
  objs.clear();
}

//////////////////////////////////////////////////////////////////////////
// Get the count of active objects
int FPool::countActives() {
  auto c=0;
  F__LOOP(it, objs) {
    auto z= *it;
    if (z->status()) {
      ++c;
    }
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////
//
void FPool::foreach(s::function<void (Poolable*)> f) {
  F__LOOP(it, objs) {
    f(*it);
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool FPool::some(s::function<bool (Poolable*)> f) {
  F__LOOP(it, objs) { if (f(*it)) { return true;} }
  return false;
}

//////////////////////////////////////////////////////////////////////////
// Hibernate (status off) all objects in the pool
void FPool::reset() {
  F__LOOP(it, objs) {
    auto z= *it;
    z->yield();
  }
}



NS_END



