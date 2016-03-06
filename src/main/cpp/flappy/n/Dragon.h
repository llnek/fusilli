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

#include "x2d/XNode.h"
#include "n/lib.h"
#include "n/C.h"

NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Dragon {

  DECL_PTR(f::XNode, parentNode)
  DECL_PTR(GVars,ss)

public:
  void dragonFlap();
  void dragonDeath();


  Dragon(GVars *ss, f::XNode *n) {
    this->ss=ss;
     parentNode=n;
  }

  void init();
  void onGameStart();
  void update(float);

  DECL_TV(c::Vec2, dragonPosition, c::Vec2(0,0))
  DECL_TV(c::Vec2, dragonSpeed, c::Vec2(0,0))
  DECL_PTR(c::Sprite,dragonSprite)
  DECL_BF(mustApplyGravity)

};


NS_END



