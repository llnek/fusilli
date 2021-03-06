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
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
class Missile : public f::CDrawNode {

  __decl_md(CCT_PT, prevPos)
  __decl_ptr(GVars,ss)

public:

  static owner<Missile*> create(not_null<GVars*>, const CCT_PT &target, const CCT_PT &speed);

  virtual void update(float dt);
  virtual bool init();
  void explode(float dt = 0);

  __decl_md(CCT_PT, target)
  __decl_md(CCT_PT, speed)
  __decl_bf(mustBeRemoved)
  __decl_bf(hasExploded)

};


NS_END


