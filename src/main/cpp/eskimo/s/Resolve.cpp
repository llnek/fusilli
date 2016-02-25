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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "Box2D/Box2D.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "n/Eskimo.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  players=engine->getNodeList(EskimoNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  auto py= CC_GNLF(Eskimo,players,"player");
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto player= (EskimoSprite*) py->node;
  auto wb= cx::visBox();
  auto pos= py->pos();

  //check for game over. player is off screen
  if (pos.y > wb.top || pos.y < 0 || pos.x > wb.right || pos.x < 0) {

    cx::sfxPlay("oops");
    MGMS()->stop();
    CC_HIDE(player);

    auto m2=j::json({ {"tag", kSpriteBtnReset } });
    auto m1=j::json({ {"tag", kSpriteBtnPause } });
    SENDMSGEX("/game/hud/togglebutton->off", &m2);
    SENDMSGEX("/game/hud/togglebutton->off", &m1);

    auto msg= j::json({ {"msg", "oops!"} });
    SENDMSGEX("/game/hud/showmsg", &msg);

    m2=j::json({ {"tag", kSpriteBtnAgain } });
    m1=j::json({ {"tag", kSpriteBtnMenu } });
    SENDMSGEX("/game/hud/togglebutton->on", &m2);
    SENDMSGEX("/game/hud/togglebutton->on", &m1);
  }

  //update text in tutorial level (currentLevel == 0)
  if (ss->currentLevel == 0) {
    ss->tutorialCounter += dt;
    if (ss->tutorialCounter > 15 && ss->tutorialStep == 1) {
      ss->tutorialStep = 2;
      auto msg= j::json({ {"msg", TUTORIAL_3 } });
      SENDMSGEX("/game/hud/showtut", &msg);
    }
    else
    if (ss->tutorialCounter > 5 && ss->tutorialStep == 0) {
      ss->tutorialStep = 1;
      auto msg= j::json({ {"msg", TUTORIAL_2 } });
      SENDMSGEX("/game/hud/showtut", &msg);
    }
  }

}


NS_END



