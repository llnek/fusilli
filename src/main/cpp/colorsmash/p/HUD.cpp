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
#include "MMenu.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();

  _time = 0;
  _score=0;

  _scoreLabel = cx::reifyLabel("dft", 18, "");
  _scoreLabel->setPosition(wb.right * 0.33, wb.top * 0.875);
  addItem(_scoreLabel);

  _timeLabel = cx::reifyLabel("dft", 18, "");
  _timeLabel->setPosition(wb.right * 0.66, wb.top * 0.875);
  addItem(_timeLabel);

  auto pause= cx::reifyMenuBtn("pause_button.png");
  pause->setCallback([=](c::Ref*){
      cx::sfxPlay("button");
      cx::pushEx(MMenu::reify());
  });
  pause->setPosition(wb.right * 0.9, wb.top * 0.95);
  auto menu= cx::mkMenu(pause);
  addItem(menu,1);

  updateTimer(0);
  updateScore(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::flashAlert() {
  auto seq= c::Sequence::create(
      c::EaseSineIn::create(c::ScaleTo::create(0.125, 1.1)),
      c::EaseSineOut::create(c::ScaleTo::create(0.125, 1)),
                                CC_NIL);
  _timeLabel->runAction(c::RepeatForever::create(seq));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::pause() {
  _timeLabel->stopAllActions();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateTimer(int n) {
  _time = n;
  _timeLabel->setString("Time: " + FTOS(_time));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString("Score: " + FTOS(_score));
  _scoreLabel->runAction(
      c::Sequence::create(
        c::EaseSineIn::create( c::ScaleTo::create(0.125, 1.1)),
        c::EaseSineOut::create(c::ScaleTo::create(0.125, 1)),
        CC_NIL));
}


NS_END



