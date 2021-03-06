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
//
class CC_DLL AILogic : public ecs::System {

  __decl_ptr(ecs::Node, _enemy)
  void process(float);

public:

  void spawnMonster(ecs::Node*, int cost, int count);
  void changeStateForEntity(ecs::Node*, AIState*);
  void spawnQuirkForEntity(ecs::Node*);
  void spawnMunchForEntity(ecs::Node*);
  void spawnZapForEntity(ecs::Node*);

  __decl_fz(humanQuirkValue)
  __decl_fz(humanZapValue)
  __decl_fz(humanMunchValue)
  __decl_fz(humanTotalValue)
  __decl_fz(aiQuirkValue)
  __decl_fz(aiZapValue)
  __decl_fz(aiMunchValue)
  __decl_fz(aiTotalValue)

  __decl_sys_tpid("n/AILogic")
  __decl_sys_priority( 60)
  __decl_sys_preamble()
  __decl_sys_update()

  AILogic(ecs::Engine *e)
  : System(e)
  {}

};


NS_END



