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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(dttower)

// Number of path steps
#define NUM_PATH_STEPS 10
// Number of defenses
#define NUM_DEFENSES 30
// Number of enemies
#define NUM_ENEMIES 10
// Base number of enemies for each wave
#define WAVES_NUM_ENEMIES 10
// Number of waves
#define NUM_WAVES 10
// Waves interval
#define WAVES_INTERVAL 24

//////////////////////////////////////////////////////////////////////////////
//
class PathStep;
class Defense;
class Enemy;
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )

  s_vec<c::Sprite*> defensePositions;
  s_vec<PathStep*> pathSteps;
  s_vec<Defense*> defenses;
  s_vec<Enemy*> enemies;

  DECL_FZ(squareSize)
  DECL_IZ(waveNumber)
  DECL_IZ(countEnemies)
  DECL_IZ(lifePoints)

};



NS_END



