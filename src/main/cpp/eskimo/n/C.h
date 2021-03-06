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
#include "Box2D/Box2D.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

enum {
  kBackground,
  kMiddleground,
  kForeground
};

enum {
  kGameIntro,
  kGamePlay,
  kGameOver
};

enum {
  kDirectionUp,
  kDirectionDown,
  kDirectionRight,
  kDirectionLeft
};

enum {
  kStateBox,
  kStateCircle
};

enum {
  kSpriteBlock,
  kSpritePlayer,
  kSpriteSwitch,
  kSpritePlatform,
  kSpriteIgloo,
  kSpriteBtnOn,
  kSpriteBtnOff,
  kSpriteBtnPlay,
  kSpriteBtnPause,
  kSpriteBtnReset,
  kSpriteBtnStart,
  kSpriteBtnMenu,
  kSpriteBtnAgain
};

#define NOTIFY_LEVEL_DONE  "NOTIFICATION_LEVEL_COMPLETED"
#define NOTIFY_GSWITCH  "NOTIFICATION_GRAVITY_SWITCH"

#define TUTORIAL_1 "tilt the device to move the eskimo"
#define TUTORIAL_2 "tap the screen to switch from snowball to ice cube"
#define TUTORIAL_3 "hit the gravity switches to help the eskimo reach the igloo"

#define ACCELEROMETER_MULTIPLIER 4.0
#define CIRCLE_MULTIPLIER 2.0
#define BOX_MULTIPLIER 4.0
#define PTM_RATIO 32.0
#define TILE 32
#define BALL_FRICTION 0.95
#define SWITCH_RADIUS 10.0
#define PLAYER_RADIUS 20.0
#define IGLOO_SQ_RADIUS TILE * TILE
#define FORCE_GRAVITY 9.0
#define PLATFORM_HEIGHT 16.0
#define PLAYER_SWITCH_RADII pow(SWITCH_RADIUS * 0.8, 2) + pow(PLAYER_RADIUS * 0.8, 2)
#define TILES_PER_PLATFORM 15

//////////////////////////////////////////////////////////////////////////////
//
class Igloo;
struct CC_DLL GVars : public ecs::Component {

  __decl_comp_tpid( "n/GVars" )

  __decl_ptr(c::ParticleSystem, smoke)
  __decl_ptr(b2World, world)
  __decl_md(CCT_PT, acc)

  __decl_fz(tutorialCounter)
  __decl_iz(tutorialStep)

  __decl_ptr(Igloo, igloo)
  __decl_iz(currentLevel)
  __decl_iz(gravity)

};

//////////////////////////////////////////////////////////////////////////////
//
const sstr fmtPng(const sstr& pfx, int);




NS_END



