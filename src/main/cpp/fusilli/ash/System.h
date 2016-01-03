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

#if !defined(__SYSTEM_H__)
#define __SYSTEM_H__

#include "Ash.h"
NS_BEGIN(ash)

#define MDECL_SYS_PRIORITY(x) \
    virtual int priority() { return x; }

#define MDECL_SYS_TPID(x) \
    virtual const ash::SystemType typeId() { return x; }

class Engine;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL System {
protected:
  DECL_BT(active)
public:

  virtual void removeFromEngine(not_null<Engine*>);
  virtual void addToEngine(not_null<Engine*>);
  virtual bool update(float time) = 0;

  virtual const SystemType typeId() = 0;
  bool isa(const SystemType& );

  bool isActive() { return active; }
  virtual int priority() = 0;

  void restart();
  void suspend();

  DECL_PTR(System ,previous)
  DECL_PTR(System ,next)

  virtual ~System() {}
  System() {}
  NOCPYASS(System)
};

//////////////////////////////////////////////////////////////////////////////
//
struct FS_DLL SystemList {

  System* get(const SystemType&);
  void remove(not_null<System*>);
  void add(not_null<System*>);
  void removeAll();
  void clear();

  //owns the systems
  DECL_PTR(System ,head)
  DECL_PTR(System ,tail)

  virtual ~SystemList() {
    clear();
  }
  SystemList() {}
  NOCPYASS(SystemList)
};

NS_END(ash)
#endif



