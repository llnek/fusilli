﻿// This library is distributed in  the hope that it will be useful but without
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

#include "core/COMP.h"

NS_BEGIN(rocket)

enum {
  ROTATE_NONE,
  ROTATE_CLOCKWISE,
  ROTATE_COUNTER
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Rocket : public f::CPixie {

  Rocket() {}

public:

  static owner<Rocket*> create() {
    auto z=mc_new(Rocket);
    z->initWithSpriteFrameName("rocket.png");
    z->autorelease();
    return z;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL RocketMotion : public f::CMove {

  void setRotationFromVector() {
    targetRotation= CC_RADIANS_TO_DEGREES(atan2(-vel.y, vel.x));
  }

  __decl_fz(rotationDamping)
  __decl_fz(rotationSpring)
  __decl_fz(targetRotation)
  __decl_fz(dr)
  __decl_fz(ar)
  __decl_fz(vr)

  __decl_iz(rotationOrientation)
  __decl_md(CCT_PT, pivot)
  __decl_fz(angularSpeed)

  RocketMotion();
};

void rocketReset(not_null<Rocket*>, not_null<RocketMotion*>);
void rocketSelect(not_null<Rocket*>, bool flag);



NS_END


