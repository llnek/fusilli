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
#include "ash/Ash.h"
#include "Box2D/Box2D.h"
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

#define EVENT_DISPATCHER cocos2d::Director::getInstance()->getEventDispatcher()
#define ADD_NOTIFICATION( __target__, __notification__, __handler__) EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(cocos2d::EventListenerCustom::create (__notification__, __handler__), __target__);

#define NOTIFY_LEVEL_DONE  "NOTIFICATION_LEVEL_COMPLETED"
#define NOTIFY_GSWITCH  "NOTIFICATION_GRAVITY_SWITCH"

#define TUTORIAL_1 "tilt the device to move the eskimo."
#define TUTORIAL_2 "tap the screen to switch from snowball to ice cube."
#define TUTORIAL_3 "hit the gravity switches to help the eskimo reach the igloo."

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
struct CC_DLL GVars : public ash::Component {
  MDECL_COMP_TPID( "n/GVars" )

  DECL_PTR(c::ParticleSystem, smoke)
  DECL_PTR(b2World, world)
  DECL_TD(c::Vec2, acc)

  DECL_PTR(Igloo, igloo)
  DECL_IZ(currentLevel)
  DECL_IZ(gravity)
  DECL_IZ(gravity)

};

const sstr fmtPng(const sstr& pfx, int);

NS_END



