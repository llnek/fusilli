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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

#define PLAYER_INITIAL_SPEED 8
#define PLAYER_JUMP 42
#define G_FORCE 1.5
#define FLOATNG_GRAVITY 0.4
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f
#define P_ACCELERATION 0.05

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
enum GameState {
  kGameIntro,
  kGamePlay,
  kGameOver,
  kGameTutorial,
  kGameTutorialReady,
  kGameTutorialEnd,
  kGameTutorialJump,
  kGameTutorialFloat,
  kGameTutorialDrop
};

enum {
  kBackground,
  kMiddleground,
  kForeground
};

enum {
  kSpritePlayer,
  kSpriteTerrain,
  kSpriteBlock,
  kSpriteChimney,
  kSpritePuff
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_ptr(c::Animate,jamAnimate)
  __decl_ptr(c::MoveTo,jamMove)
  __decl_ptr(c::Sprite,jam)
  __decl_ptr(c::Sprite,hat)

  __decl_ptr(c::Label,tutorialLabel)
  __decl_ptr(c::Sprite,background)
  __decl_ptr(c::Sprite,foreground)
  __decl_iz(state)

  __decl_iz(speedIncreaseInterval)
  __decl_iz(speedIncreaseTimer)

  __decl_comp_tpid( "n/GVars" )
};

void resetTerrain(not_null<ecs::Node*>);
void resetPlayer(not_null<ecs::Node*>);

NS_END


