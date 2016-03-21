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
#include "PowerUp.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
class MissileLauncher : public PowerUp {

  const s_vec<c::Vec2> generateTargets();
  DECL_PTR(GVars,ss)

public:

  static MissileLauncher* create(GVars *ss) {
    auto r= new MissileLauncher();
    r->ss=ss;
    r->init();
    r->autorelease();
    return r;
  }

  virtual bool init();

  virtual void update();
  virtual void activate();

};


NS_END

