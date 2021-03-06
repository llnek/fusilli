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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  drawScore();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {
  auto wz = cx::visSize();
  auto wb = cx::visBox();

  regoAtlas("game-pics");

  _scoreLabel = cx::reifyBmfLabel("dft", "0");
  _scoreLabel->setAnchorPoint(cx::anchorTR());
  XCFG()->scaleBmfont(_scoreLabel, 24);
  auto gap= CC_CHT(_scoreLabel)/GOLDEN_RATIO;
  CC_POS2(_scoreLabel,  wb.right - gap, wb.top - gap);
  addItem(_scoreLabel);

  _lives= f::reifyRefType<f::XLives>();
  _lives->initLives("paddle.png", 3,
      gap,
      wb.top - gap, 0.5f);
  addItem(_lives);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  gap= CC_CHT(b)/GOLDEN_RATIO;

  b->setCallback([=](c::Ref*) { SENDMSG("/hud/showmenu"); });
  CC_POS2(b, wb.right-gap, wb.bottom + gap);
  addItem(cx::mkMenu(b));

  _lives->resurrect();
  _score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScore() {
  _scoreLabel->setString(FTOS(_score));
}

//////////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::reduceLives(int n) {
  return _lives->reduce(n)->isDead();
}




NS_END


