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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

#include "AIStateCounter.h"
#include "AIStateDefend.h"
#include "AIStateMass.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AIStateMass::enter() {
  cx::sfxPlay("mass");
}

//////////////////////////////////////////////////////////////////////////////
//
void AIStateMass::update(ecs::Entity *e, AILogic *sys) {

  auto player = CC_GEC(Stash,e,"n/Stash");
  auto team = CC_GEC(Team,e,"n/Team");
  auto ai = CC_GEC(Automa,e,"n/Automa");

  if (!team || !player || !ai) { return; }

  auto enemies = entsWithinRange(sys->getEngine(),e,200, OTHER_TEAM(team->team));
  if (enemies.size() > 0) {
    sys->changeStateForEntity(e, mc_new(AIStateDefend));
    return;
  }
  else if (sys->aiTotalValue + player->coins >= COST_MUNCH + COST_ZAP*2) {
    sys->changeStateForEntity(e, mc_new(AIStateCounter));
    return;
  }

  player->attacking = false;
}



NS_END




