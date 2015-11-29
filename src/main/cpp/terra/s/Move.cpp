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

#include "Move.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
Move::Move(not_null<a::Engine*> e, not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(e, d) {

  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Move::addToEngine(not_null<a::Engine*> e) {
  ShipMotionNode n;
  ships = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::onUpdate(float dt) {
  auto node= ships->head;
  if (MGMS()->isLive() && NNP(node)) {
    moveMissiles(dt);
    move(dt);
    onKeys(node,dt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(a::Node* node, float dt) {
  auto ssp = CC_CSV(c::Float, "SHIP_SPEED");
  auto ship = node->ship;
  auto wz= cx::visRect();
  auto mot= node->motion;
  auto sp = ship->sprite;
  auto ok = false;
  auto pos = ship->pos();
  auto x = pos.x;
  auto y = pos.y;

  if (mot->up && pos.y <= wz.height) {
    y = pos.y + dt * ssp;
    ok= true;
  }
  if (mot->down && pos.y >= 0) {
    y = pos.y - dt * ssp;
    ok= true;
  }
  if (mot->left && pos.x >= 0) {
    x = pos.x - dt * ssp;
    ok= true;
  }
  if (mot->right && pos.x <= wz.width) {
    x = pos.x + dt * ssp;
    ok= true;
  }

  if (ok) { ship->setPos(x,y); }

  mot->right= false;
  mot->left=false;
  mot->down=false;
  mot->up=false;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveOneBomb(f::ComObj* m, float dt) {
  auto pos = m->sprite->getPosition();
  m->sprite->setPosition(pos.x + m->vel.x * dt,
                       pos.y + m->vel.y * dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::move(float dt) {
  auto p = MGMS()->getPool("Bombs");
  p->foreach([=](f::ComObj* b) {
    if (b->status) {
      this->moveOneBomb(b,dt);
    }
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveOneMissile(f::ComObj* m, float dt) {
  auto pos = m->sprite->getPosition();
  m->sprite->setPosition(pos.x + m->vel.x * dt,
                       pos.y + m->vel.y * dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveMissiles(float dt) {
  auto p= MGMS()->getPool("Missiles");
  p->foreach([=](f::ComObj* v) {
    if (v->status) {
      this->moveOneMissile(v,dt);
    }
  });
}

NS_END(terra)


