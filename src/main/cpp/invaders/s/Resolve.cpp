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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  aliens= engine->getNodeList(AlienMotionNode().typeId());
  ship= engine->getNodeList(ShipMotionNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive() ) {
    checkMissiles();
    checkBombs();
    checkAliens();
    checkShip();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {

  auto mss = MGMS()->getPool("missiles");
  auto ht = cx::visRect().size.height;
  auto c = mss->list();

  F__LOOP(it, c) {
    auto &m = *it;
    if (m->status) {
      if (m->pos().y >= ht ||
          m->HP <= 0) {
        m->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {

  auto bbs = MGMS()->getPool("bombs");
  auto c = bbs->list();
  int bt = 0;

  F__LOOP(it, c) {

    auto &b = *it;
    if (b->status) {
      if (b->HP <= 0 ||
          b->pos().y <= bt) {
        b->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto sqad= CC_GNLF(AlienSquad, aliens, "aliens");
  auto c= sqad->list();

  F__LOOP(it, c) {

    auto &en= *it;
    if (en->status) {
      if (en->HP <= 0) {
        auto msg= j::json({
              {"score", en->score }
            });
        SENDMSGEX("/game/player/earnscore", &msg);
        en->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto sp = CC_GNLF(Ship, ship, "ship");

  if (sp->status &&
      sp->HP <= 0) {
    sp->deflate();
    SENDMSG("/game/player/killed");
  }
}


NS_END(invaders)

