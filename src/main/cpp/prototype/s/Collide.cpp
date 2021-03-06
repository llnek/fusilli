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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(prototype)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    clamp(dt);
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::clamp(float dt) {
  auto py= CC_GEC(f::CPixie, _player, "f/CPixie");
  auto loc= py->getPosition();
  auto sz= CC_CSIZE(py);
  auto wb= cx::visBox();

  if (loc.x <= wb.left + HWZ(sz)) {
    py->setPositionX(wb.left + HWZ(sz));
  }

  if (loc.x >= wb.right - HWZ(sz)) {
    py->setPositionX(wb.right - HWZ(sz));
  }

  if (loc.y <= wb.bottom + HHZ(sz)) {
    py->setPositionY(wb.bottom + HHZ(sz));
  }

  if (loc.y >= wb.top - HHZ(sz)) {
    py->setPositionY(wb.top - HHZ(sz));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
}

NS_END




