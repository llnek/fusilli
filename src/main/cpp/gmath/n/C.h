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
#include "UIScale9Sprite.h"

#define KeyFinishedTutorial "keyFinishedTutorial"
#define KeySound "keySound"
#define KeyMusic "keyMusic"
#define DataHighScores "highScores"
#define DictTotalScore "totalScore"
#define DictTurnsSurvived "turnsSurvived"
#define DictUnitsKilled "unitsKilled"
#define DictHighScoreIndex "hsIndex"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(gmath)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_iz(tutorial)
  __decl_md(c::Size,unitSize)
  __decl_ptr(c::Array, friends)
  __decl_ptr(c::Array, enemies)
  GVars() {
    friends=c::Array::create();
    enemies=c::Array::create();
    CC_KEEP(friends);
    CC_KEEP(enemies);
  }
  virtual ~GVars() {
    friends->removeAllObjects();
    enemies->removeAllObjects();
    CC_DROP(friends);
    CC_DROP(enemies);
  }
};


const c::Vec2 getPosAsGrid(GVars*, const f::Cell2I&);





NS_END



