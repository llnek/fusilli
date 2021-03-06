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

//////////////////////////////////////////////////////////////////////////////

#include "p/ParticleLayer.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto hh=CC_GEC(f::CHealth,_player,"f/CHealth");
  auto hero=CC_GEC(Hero,_player,"f/CPixie");
  auto pr= MGMS()->getPool("Rockets");
  auto pe= MGMS()->getPool("Enemies");
  auto pb= MGMS()->getPool("Bullets");
  auto &rs= pr->ls();
  auto &es= pe->ls();
  auto &bs= pb->ls();
  bool gameOver=false;

  F__LOOP(it, rs) {
    auto &pe= *it;
    if (!pe->status()) { continue;  }
    auto h= CC_GEC(f::CHealth,pe,"f/CHealth");
    auto r= CC_GEC(Projectile,pe,"f/CPixie");
    F__LOOP(it2, es) {
      auto &e2= *it2;
      auto h2 = CC_GEC(f::CHealth,e2,"f/CHealth");
      auto e = CC_GEC(Enemy,e2,"f/CPixie");
      if (e2->status() && cx::collide(r,e)) {
        auto pLayer = mc_new1(ParticleLayer, e->pos());
        MGML()->addItem(pLayer);
        cx::sfxPlay("rocketExplode");
        cx::sfxPlay("enemyKill");
        h2->hurt();
        h->hurt();
        auto msg= j::json({
            {"score", 1}
            });
        SENDMSGEX("/game/player/earnscore", &msg);
        cx::hibernate((ecs::Node*)pe);
        cx::hibernate((ecs::Node*)e2);
        break;
      }
    }
  }

  F__LOOP(it, bs) {
    auto &e= *it;
    auto h= CC_GEC(f::CHealth,e,"f/CHealth");
    auto b= CC_GEC(Projectile,e,"f/CPixie");
    if (e->status() && cx::collide(b, hero)) {
      cx::sfxPlay("playerKill");
      hh->hurt();
      h->hurt();
      cx::hibernate((ecs::Node*)e);
      cx::hibernate(_player);
      gameOver=true;
      break;
    }
  }

  if (gameOver) {
    SENDMSG("/game/player/lose");
  }
}

NS_END




