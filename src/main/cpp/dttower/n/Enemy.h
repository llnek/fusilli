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

#include "core/COMP.h"

#define NUM_DEFENSES 30
#define SPEED 5

NS_BEGIN(dttower)
//////////////////////////////////////////////////////////////////////////////
//
class PathStep;
class Defense;
class GVars;
struct CC_DLL Enemy : public f::CPixie {

  void outOfRangeFromDefense(Defense*);
  void addAttackingDefense(not_null<Defense*> );
    void removeEnemyFromScene();
    
  static owner<Enemy*> create(GVars*, not_null<PathStep*> );
  virtual void update(float);

  s_vec<Defense*> attackingDefenses;
  DECL_PTR(PathStep, pathStep)
  DECL_PTR(GVars,ss)
  DECL_TV(int, lifePoints,1)

};


NS_END


