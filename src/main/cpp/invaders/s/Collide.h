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

#if !defined(__COLLIDE_H__)
#define __COLLIDE_H__

#include "x2d/GSystem.h"
#include "n/GNodes.h"
#include "EFactory.h"

NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Collide : public f::GSystem<GEngine> {

  virtual bool update(float);
  virtual void onAdd();

  MDECL_SYS_PRIORITY( a::Collide)
  MDECL_SYS_TPID( "n/Collide")

  bool maybeCollide(f::ComObj*, f::ComObj*);
  void checkMissilesBombs();
  void checkMissilesAliens(a::Node*);
  void checkShipBombs(a::Node*);
  void checkShipAliens(a::Node*, a::Node*);

  DECL_PTR(a::NodeList, aliens)
  DECL_PTR(a::NodeList, ships)

};



NS_END(invaders)
#endif

