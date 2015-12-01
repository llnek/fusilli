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

#include "base/CCEventKeyboard.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "x2d/GameScene.h"
#include "n/GNodes.h"
#include "Motions.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(f, d) {

  SNPTR(cannons)
  SNPTR(aliens)
  SNPTR(ships)
  right=false;
  left= false;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::addToEngine(not_null<a::Engine*> e) {
  //CCLOG("adding system: Motions");
  AlienMotionNode a;
  ShipMotionNode s;
  CannonCtrlNode c;

  aliens = e->getNodeList(a.TypeId());
  ships = e->getNodeList(s.TypeId());
  cannons = e->getNodeList(c.TypeId());
  //CCLOG("added system: Motions");
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::onUpdate(float dt) {
  auto enemy = aliens->head;
  auto ship=ships->head;
  auto cns= cannons->head;

  if (MGMS()->isLive()) {
    if (NNP(enemy)) {
      processAlienMotions(enemy,dt);
    }
    if (NNP(cns)) {
      controlCannon(cns,dt);
    }
    if (NNP(ship)) {
      scanInput(ship,dt);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::controlCannon(not_null<a::Node*> node, float dt) {

  auto gun = CC_GNF(Cannon, node, "cannon");
  auto lpr= CC_GNF(Looper, node, "looper");
  auto ship= CC_GNF(Ship, node, "ship");
  auto t= lpr->timer0;

  if (! gun->hasAmmo) {
    //throttle the cannon with timer
    if (cx::timerDone(t)) {
      ship->sprite->setSpriteFrame(ship->frame0);
      gun->hasAmmo=true;
      cx::undoTimer(t);
      SNPTR(lpr->timer0)
    }
  } else {
    //TODO:
    if (MGML()->keyPoll(c::EventKeyboard::KeyCode::KEY_SPACE)) {
      fireMissile(node,dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::fireMissile(not_null<a::Node*> node, float dt) {

  auto gun= CC_GNF(Cannon, node, "cannon");
  auto lpr= CC_GNF(Looper, node, "looper");
  auto ship= CC_GNF(Ship, node, "ship");

  auto p= MGMS()->getPool("missiles");
  auto top= cx::getTop(ship->sprite);
  auto pos= ship->pos();
  auto ent= p->get();

  if (ENP(ent)) {
    factory->reifyMissiles(36);
    ent= p->get();
  }

  ent->inflate(pos.x, top+4);

  lpr->timer0 = cx::reifyTimer( MGML(), gun->coolDownWindow);
  gun->hasAmmo=false;
  ship->sprite->setSpriteFrame(ship->frame1);

  cx::sfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::scanInput(not_null<a::Node*> node, float dt) {

  auto m= CC_GNF(Motion, node, "motion");
  auto s= CC_GNF(Ship, node, "ship");

  if (MGML()->keyPoll(
      c::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    m->right=true;
  } else {
    m->right=false;
  }
  if (MGML()->keyPoll(
      c::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    m->left=true;
  } else {
    m->left=false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processAlienMotions(not_null<a::Node*> node, float dt) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto lpr = CC_GNF(Looper, node, "looper");

  if (ENP(lpr->timer0)) {
    lpr->timer0= cx::reifyTimer(MGML(), 1);
  }

  if (ENP(lpr->timer1)) {
    lpr->timer1= cx::reifyTimer(MGML(), 2);
  }
}




NS_END(invaders)


