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
#include "Splash.h"
#include "Menu.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {

  STATIC_REIFY_LAYER(UILayer)

  virtual void decorate();
  void onPlay(c::Ref*);

  virtual ~UILayer() {}
  UILayer() {}
  NOCPYASS(UILayer)
};

//////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto b= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b);
  auto wb = cx::visBox();
  auto cw = cx::center();

  centerImage("game.bg");

  menu->setPosition(cw.x, wb.top * 0.1f);
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));

  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref*) {
  auto f= [=]() {
    cx::runScene(
        XCFG()->prelude(),
        CC_CSV(c::Float,"SCENE_DELAY"));
  };
  auto m= MMenu::reify(mc_new_1(MCX, f));
  cx::runScene(m,
      CC_CSV(c::Float, "SCENE_DELAY"));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {
  UILayer::reify(this);
}


NS_END(tetris)

