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

#include "ecs/System.h"
#include "n/C.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Collide : public ecs::System {

  void onBrick(Brick*);
  bool onPlayerKilled();
  void check();
  void checkNodes();
  void checkBricks();

public:

  __decl_sys_priority(ecs::Collide)
  __decl_sys_tpid("s/Collide")
  __decl_sys_update()
  __decl_sys_preamble()

  Collide(ecs::Engine *e)
  : System(e)
  {}

  __decl_ptr(ecs::Node, _paddle)
  __decl_ptr(ecs::Node, _ball)
  __decl_ptr(ecs::Node, _fence)

};


NS_END


