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

#include "ash/System.h"
#include "GEngine.h"

NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AI : public a::System {

  void fallingObjectDone(f::ComObj*);
  void increaseDifficulty();
  void resetMeteor();
  void resetUfo(Ufo*);
  void resetHealth();
  void process(float);
  void changeEnergy(float);

public:

  MDECL_SYS_PRIORITY( a::AI)
  MDECL_SYS_TPID("n/AI")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  DECL_PTR(a::NodeList, shared)
  DECL_PTR(a::NodeList, bombs)
  DECL_PTR(a::NodeList, ufos)

  AI(a::Engine *e)
  : System(e)
  {}


};


NS_END


