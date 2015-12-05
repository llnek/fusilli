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

#include "core/Primitives.h"
#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "n/GNodes.h"
#include "EFactory.h"
#include "Stager.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Stager::Stager(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(f, d) {

  initAlienSize();
  initShipSize();

  f->reifyExplosions();
  f->reifyMissiles();
  f->reifyBombs();
  f->reifyAliens();
  f->reifyShip();

  SNPTR(cannons)
  inited=false;
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::addToEngine(not_null<a::Engine*> e) {
  CannonCtrlNode c;
  cannons = e->getNodeList(c.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::initAlienSize() {
  auto z= cx::calcSize("purple_bug_0.png");
  //pick purple since it is the largest
  state->setObject(f::Size2::create(z.width,z.height), "alienSize");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::initShipSize() {
  auto z= cx::calcSize("ship_0.png");
  state->setObject(f::Size2::create(z.width, z.height), "shipSize");
}

//////////////////////////////////////////////////////////////////////////
//
bool Stager::onUpdate(float dt) {
  if (cx::isTransitioning()) { return false; }
  if (!inited) {
    onceOnly(cannons->head, dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::onceOnly(a::Node* node, float) {
  auto gun = CC_GNF(Cannon, node, "cannon");
  auto lpr= CC_GNF(Looper, node, "looper");
  auto ship= CC_GNF(Ship, node, "ship");
  lpr->timer7 = cx::reifyTimer( MGML(), gun->coolDownWindow);
  gun->hasAmmo=false;
  inited=true;
}

/*
static void _fire(const stdstr& t, void* evt) {
  if ("/touch/one/move" == t ||
      "/mouse/move" == t) {} else {
    return;
  }
  if (MGMS()->isLive() && NNP(ships->head)) {
    auto ship = (f::ComObj*) ships->head->get("ship");
    auto pos = ship->pos();
    auto x=pos.x;
    auto y=pos.y;
    auto wz= cx::visRect();
    //TODO:
    //pos.add(c::Vec2(evt->getDelta().x, 0));
    pos.clamp(c::Vec2(0, 0),
              c::Vec2(wz.size.width, wz.size.height));
    ship->setPos(pos.x, pos.y);
  }
}
*/


NS_END(invaders)

