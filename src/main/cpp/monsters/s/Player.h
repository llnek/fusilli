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

#include "GEngine.h"

NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL PlayerLogic : public ecs::System {

  void handleMover(ecs::Node*, bool attacking);
  void process(float);
public:

  __decl_sys_tpid("n/PlayerLogic")
  __decl_sys_priority( 30)
  __decl_sys_preamble()
  __decl_sys_update()

  PlayerLogic(ecs::Engine *e)
  : System(e)
  {}

};


NS_END




