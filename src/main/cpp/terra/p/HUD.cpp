// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Menu.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::realize() {
  auto soff = CC_CSV(c::Integer, "S_OFF");
  auto tile = CC_CSV(c::Integer, "TILE");
  auto wz = cx::visRect();
  auto wb= cx::visBox();

  regoAtlas("game-pics");

  scoreLabel= cx::reifyBmfLabel("font.TinyBoxBB", "0");
  scoreLabel->setAnchorPoint(cx::anchorBR());
  scoreLabel->setScale(12.0f/72.0f);
  scoreLabel->setPosition( wz.width - tile - soff,
      wz.height - tile - soff - cx::getHeight(scoreLabel));
  addItem(scoreLabel);

  lives = f::reifyRefType<f::XLives>();
  lives->realize("ship01.png",3,
      tile +soff,
      wz.height - tile - soff, 0.4);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = cx::getHeight(b) * 0.5f;
  auto hw = cx::getWidth(b) * 0.5f;
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::showMenu));
  //b->setColor(this->color);
  auto menu = cx::mkMenu(b);

  menu->setPosition(wb.right - tile - hw, wb.bottom + tile  + hh);
  addItem(menu);

  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showMenu(c::Ref*) {
  MGMS()->sendMsg("/hud/showmenu");
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::resetAsNew() {
  reset();
  score=0;
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::reset() {

}

NS_END(terra)
