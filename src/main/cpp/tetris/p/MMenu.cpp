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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "MMenu.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {

  auto tt= cx::reifyBmfLabel("JellyBelly", gets("mmenu"));
  auto tile = CC_CSV(c::Float, "TILE");
  auto c= XCFG()->getColor("dft");
  auto wb= cx::visBox();

  centerImage("gui.mmenu.menu.bg");

  tt->setPosition(wb.cx, wb.top * 0.9f);
  tt->setScale(XCFG()->getScale());
  tt->setColor(c);
  addItem(tt);

  auto b= cx::reifyMenuBtn("player1.png");
  auto menu= cx::mkMenu(b);
  b->setCallback(
      [=](c::Ref*) {
        cx::runEx( Game::reify(mc_new(f::GCX))); });
  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto quit= cx::reifyMenuBtn("icon_quit.png");
  auto ctx= (MCX*) getCtx();
  auto sz= back->getContentSize();

  quit->setColor(c);
  back->setColor(c);

  quit->setCallback([=](c::Ref*) { cx::prelude();  });
  back->setCallback([=](c::Ref*) { ctx->back(); });

  s_vec<c::MenuItem*> btns {back, quit};
  auto m2= cx::mkHMenu(btns);
  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  addItem(m2);

  auto audios= cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons( audios,
    cx::anchorBR(),
    c::Vec2(wb.right - tile, wb.bottom + tile));
}

NS_END(tetris)

