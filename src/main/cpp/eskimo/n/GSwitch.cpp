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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "GSwitch.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
owner<GSwitch*> GSwitch::create(not_null<GVars*> ss) {
  auto z= mc_new1(GSwitch, ss);
  z->initWithSpriteFrameName("blank.png");
  CC_HIDE(z);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
void GSwitch::initGSwitch(int dir, const CCT_PT &pos) {
  this->setDisplayFrame(
        cx::getSpriteFrame(fmtPng("switch_" , dir)));
  CC_POS1(this,pos);
  CC_SHOW(this);
  this->_direction = dir;
}


NS_END



