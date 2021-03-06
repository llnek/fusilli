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

#include "spine/spine-cocos2dx.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::update(float dt) {
  _bg->sync();
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto roll= CC_CSV(c::Float, "BG+SCROLL");
  auto wb= cx::visBox();

  _bg = f::reifyRefType<ScrollingBgLayer>();
  _bg->set(roll);
  addItem(_bg);

  auto nameLabel = cx::reifyBmfLabel("title", "Ms.tinyBazooka");
  XCFG()->scaleBmfont(nameLabel, 64);
  CC_POS2(nameLabel, wb.cx, wb.top * 0.8);
  addItem(nameLabel);

  // jiggle the label
  moveXXX(nameLabel,20.0);

  auto skeletonNode = spine::SkeletonAnimation::createWithFile(
      "pics/player.json", "pics/player.atlas", 1.0f);
  skeletonNode->addAnimation(0,"runCycle",true,0);
  XCFG()->fit(skeletonNode);
  CC_POS2(skeletonNode,
      wb.right * .125 ,
      wb.top * 0.2 - HHZ(CC_CSIZE(skeletonNode)));
  addItem(skeletonNode);

  auto play= cx::reifyMenuBtn("play.png");
  XCFG()->fit(play);
  auto sz=CC_CSIZE(play);
  play->setCallback([=](c::Ref*) {
      cx::sfxPlay("pop");
      cx::runEx(Game::reify(mc_new(GameCtx)));
    });
  CC_POS2(play, wb.cx, wb.cy);
  addItem(cx::mkMenu(play), 10);

  auto newHighScoreLabel = cx::reifyBmfLabel("pixel", "CURRENT HIGH SCORE");
  auto k= XCFG()->scaleBmfont(newHighScoreLabel,50);
  CC_POS2(newHighScoreLabel, wb.cx, wb.top * 0.15);
  newHighScoreLabel->setScale(0.5 * k);
  addItem(newHighScoreLabel, 10);

  auto highScoreLabel = cx::reifyBmfLabel("pixel", "0");
  k= XCFG()->scaleBmfont(highScoreLabel, 50);
  CC_POS2(highScoreLabel, wb.cx, wb.top * 0.1);
  highScoreLabel->setScale(0.5 * k);
  addItem(highScoreLabel, 10);

  auto highScore = CC_APPDB()->getIntegerForKey("GameHighScore");
  highScoreLabel->setString(FTOS(highScore));

  this->scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::moveXXX(c::Node *ref, float delta) {
  auto actionMove = c::MoveTo::create(1,
      CCT_PT(ref->getPositionX(), ref->getPositionY() + delta));
  ref->runAction(
      c::Sequence::create(
        c::EaseSineInOut::create((c::ActionInterval*)actionMove),
        c::CallFunc::create([=]() {
          this->moveXXX(ref, -delta);
        }),
        CC_NIL));
}


NS_END



