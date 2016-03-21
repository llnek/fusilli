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
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
class Missile : public f::CDrawNode {

  DECL_TV(c::Vec2, prevPos, CC_ZPT)
  DECL_PTR(GVars,ss)

public:

  static owner<Missile*> create(GVars*, const c::Vec2 &target, const c::Vec2 &speed);

  virtual void update(float dt);
  virtual bool init();
  void explode(float dt = 0);

  DECL_TV(c::Vec2, target, CC_ZPT)
  DECL_TV(c::Vec2, speed, CC_ZPT)
  DECL_BF(mustBeRemoved)
  DECL_BF(hasExploded)

};


NS_END

