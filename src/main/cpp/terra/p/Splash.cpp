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
#include "s/utils.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
class CC_DLL UILayer : public f::XLayer {
protected:

  c::Sprite* flare;
  c::Sprite* ship;

public:

  STATIC_REIFY_LAYER(UILayer)

  NOCPYASS(UILayer)
  IMPLCZ(UILayer)

  virtual void update(float);
  virtual void decorate();

};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {
  auto wz = cx::visRect();
  auto wb = cx::visBox();
  auto cw = cx::center();

  centerImage("game.bg");

    flare = c::Sprite::create("pics/flare.jpg");
  flare->setVisible(false);
  ship = cx::reifySprite("ship03.png");
  ship->setPosition( cx::randFloat(wz.size.width), 0);
  addChild(flare, 15, 10);
  addChild(ship, 0, 4);

  auto f= [=]() { cx::runScene(XCFG()->startWith()); };
  auto b= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b);
  b->setCallback([=](c::Ref*) {
    btnEffect();
    flareEffect(flare, [=]() {
      cx::runScene(MainMenu::reify(mc_new_1(MContext, f)));
    });
  });

  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(menu);

  scheduleOnce(CC_SCHEDULE_SELECTOR(UILayer::update),0);
  cx::sfxMusic("mainMusic", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::update(float dt) {
  auto wz = cx::visRect();
  auto g= [=]() {
    this->ship->setPosition( cx::randFloat(wz.size.width), 10);
    this->update(0);
  };
  this->ship->runAction(c::Sequence::create(
        c::MoveBy::create(2, c::ccp(cx::randFloat(wz.size.width),
            wz.size.height + 100)),
        c::CallFunc::create(g), nullptr));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {
  UILayer::reify(this);
}

NS_END(terra)


