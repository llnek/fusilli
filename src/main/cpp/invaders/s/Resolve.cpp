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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _aliens= _engine->getNodes("n/AlienSquad")[0];
  _player= _engine->getNodes("f/CGesture")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive() ) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  checkMissiles();
  checkBombs();
  checkAliens();
  checkShip();
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {

  auto mss = MGMS()->getPool("Missiles");
  auto wb = cx::visBox();
  auto &c = mss->ls();

  F__LOOP(it, c) {
    auto &e = *it;
    auto h= CC_GEC(f::CHealth,e,"f/CHealth");
    auto s= CC_GEC(f::CPixie,e,"f/CPixie");
    if (e->status()) {
      if (s->getPositionY() >= wb.top ||
          !h->alive()) {
        cx::hibernate((ecs::Node*)e);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {

  auto bbs = MGMS()->getPool("Bombs");
  auto &c = bbs->ls();
  auto wb= cx::visBox();

  F__LOOP(it, c) {
    auto &e = *it;
    auto h= CC_GEC(f::CHealth,e,"f/CHealth");
    auto s= CC_GEC(f::CPixie,e,"f/CPixie");
    if (e->status()) {
      if (!h->alive() ||
          s->getPositionY() <= wb.bottom) {
        cx::hibernate((ecs::Node*)e);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto sqad= CC_GEC(AlienSquad, _aliens, "n/AlienSquad");
  auto &c= sqad->aliens->ls();

  F__LOOP(it, c) {
    auto &e= *it;
    auto h= CC_GEC(f::CHealth,e,"f/CHealth");
    auto s= CC_GEC(f::CPixie,e,"f/CPixie");
    auto r= CC_GEC(Rank,e,"n/Rank");

    if (e->status()) {
      if (!h->alive()) {
        auto msg= j::json({
              {"score", r->value }
            });
        SENDMSGEX("/game/player/earnscore", &msg);
        cx::hibernate((ecs::Node*)e);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto h = CC_GEC(f::CHealth, _player, "f/CHealth");
  auto s = CC_GEC(Ship, _player, "f/CPixie");

  if (_player->status() && !h->alive()) {
    cx::hibernate((ecs::Node*)_player);
    SENDMSG("/game/player/killed");
  }
}


NS_END

