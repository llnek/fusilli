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
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"
#include "Ende.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {
  auto lbl= cx::reifySprite("gameover.png");
  auto wb= cx::visBox();

  centerImage("game.bg")->setOpacity(0.8 * 255);

  lbl->setPosition(wb.cx, wb.top * 0.8);
  addItem(lbl);

  auto t= cx::reifyMenuText("btns", "Try Again?");
  auto q= cx::reifyMenuText("btns", "Quit");

  q->setCallback([=](c::Ref*){ cx::prelude();  });
  t->setCallback([=](c::Ref*){
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });

  auto menu=cx::mkVMenu(s_vec<c::MenuItem*>{t,q},
      CC_CHT(t)/GOLDEN_RATIO);
  CC_POS2(menu,wb.cx, wb.top * 0.2);
  addItem(menu);

}


NS_END



