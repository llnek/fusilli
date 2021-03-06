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

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {

  if (MGMS()->isLive()) {
    checkMissilesAliens();
    checkShipAliens();
    checkShipBombs();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesBombs() {
  cx::testCollide(
    MGMS()->getPool("Missiles"),
    MGMS()->getPool("Bombs"));
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesAliens() {
  cx::testCollide(
    MGMS()->getPool("Missiles"),
    MGMS()->getPool("Baddies"));
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {
  auto h=CC_GEC(f::CHealth,_ship,"f/CHealth");
  if (!h->isGod())
  cx::testCollide(MGMS()->getPool("Bombs"), _ship);
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens() {
  auto h=CC_GEC(f::CHealth,_ship,"f/CHealth");
  if (!h->isGod())
  cx::testCollide(MGMS()->getPool("Baddies"), _ship);
}


NS_END



