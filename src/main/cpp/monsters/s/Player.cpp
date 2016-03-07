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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Player.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)


//////////////////////////////////////////////////////////////////////////////
//
void PlayerLogic::preamble() {

}

//////////////////////////////////////////////////////////////////////////////
//
bool PlayerLogic::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

static float COIN_DROP_INTERVAL = 1.5;
static float COINS_PER_INTERVAL = 5;
//////////////////////////////////////////////////////////////////////////////
//
void PlayerLogic::process(float dt) {
  auto ents = engine->getEntities("n/Stash");
  auto time = cx::timeInMillis();
  F__LOOP(it,ents) {
    auto e = *it;
    auto player = CC_GEC(Stash,e,"n/Stash");
    auto team = CC_GEC(Team,e,"n/Team");
    auto melee = CC_GEC(Melee,e,"n/Melee");
    auto render = CC_GEC(f::CDraw,e,"f/CDraw");

    // Handle coins
    if (time - player->lastCoinDrop > COIN_DROP_INTERVAL) {
      player->lastCoinDrop = time;
      player->coins += COINS_PER_INTERVAL;
    }

    // Update player image
    if (render && team && melee) {
      auto png= melee->attacking ?
        "castle"+s::to_string(team->team)+"_atk.png"
        :
        "castle"+s::to_string(team->team)+"_def.png";
      render->node->setDisplayFrame(png);
    }

    // Update monster movement orders
    if (team) {
      auto movers = getEntOnTeam(engine,team->team,"f/CMove");
      F__LOOP(it,movers) {
        auto m= *it;
        handleMover(m,player->attacking);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerLogic::handleMover(ecs::Entity *mover, bool attacking) {

  auto moverRender = CC_GEC(f::CDraw,mover,"f/CDraw");
  auto moverTeam = CC_GEC(Team,mover,"n/Team");
  auto moverMove = CC_GEC(f::CMove,mover,"f/CMove");

  if (!moverTeam || !moverRender || !moverMove) {
  return; }

  if (attacking) {
    auto enemy = closestEntOnTeam(engine,mover,OTHER_TEAM(moverTeam->team));
    if (!enemy) {
      moverMove->moveTarget = moverRender->pos();
      return;
    }

    auto enemyRender = CC_GEC(f::CDraw,enemy,"f/CDraw");
    if (!enemyRender) { return; }
    moverMove->moveTarget = enemyRender->pos();

    auto gun = CC_GEC(Gun,e,"n/Gun");
    if (gun) {
      auto vector = c::ccpNormalize(c::ccpSub(moverRender->pos(), enemyRender->pos()));
      moverMove->moveTarget = c::ccpAdd(enemyRender->pos(), c::ccpMult(vector, gun->range));
    }

  } else {

    auto player = playerForTeam(engine,mover,moverTeam->team);
    auto playerRender = CC_GEC(f::CDraw,player,"f/CDraw");
    if (playerRender) {
      moverMove->moveTarget = playerRender->pos();
    }
  }

}


NS_END




