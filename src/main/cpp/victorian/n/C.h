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
#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)


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


typedef enum {
    kGameIntro,
    kGamePlay,
    kGameOver,
    kGameTutorial,
    kGameTutorialJump,
    kGameTutorialFloat,
    kGameTutorialDrop
} GameState;


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Widget : public f::ComObj {

  Widget(not_null<c::Sprite*> s)
    : ComObj(s) {
  }

  virtual float radius() {
    return _width * 0.5f;
  }

  virtual float height() { return _height; }
  virtual float width() { return _width; }

  void setSize() {
    _height = sprite->boundingBox().size.height;
    _width = sprite->boundingBox().size.width;
  }

  DECL_FZ(_height)
  DECL_FZ(_width)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cloud : public f::ComObj {
  Cloud(not_null<c::Sprite*> s) :
    ComObj(s) {
  }
  MDECL_COMP_TPID("n/Cloud")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  DECL_PTR(c::Animate,jamAnimate)
  DECL_PTR(c::MoveTo,jamMove)
  DECL_PTR(c::Sprite,jam)
  DECL_PTR(c::Sprite,hat)

  DECL_PTR(c::Sprite,background)
  DECL_PTR(c::Sprite,foreground)
  DECL_IZ(state)

  DECL_IZ(speedIncreaseInterval)
  DECL_IZ(speedIncreaseTimer)

  MDECL_COMP_TPID( "n/GVars" )
};



NS_END


