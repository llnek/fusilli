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
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

BEGIN_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
protected:

  void onPlay(c::Ref*);

  NO__CPYASS(UILayer)
  UILayer()=delete;

public:

  virtual f::XLayer* realize();
  IMPL_CTOR(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {
  auto tt= cx::reifyBmfLabel( "font.JellyBelly",
      XCFG()->getL10NStr("mmenu"));
  auto tile = CC_CSV(c::Integer, "TILE");
  auto wb= cx::visBox();
  auto cw= cx::center();

  centerImage("gui.mmenus.menu.bg");

  tt->setPosition( cw.x, wb.top * 0.9f);
  tt->setColor(cx::white());
  tt->setScale(XCFG()->getScale());
  addItem(tt);

  auto b= cx::reifyMenuBtn("player1.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  auto menu= cx::mkMenu(b);
  menu->setPosition(cw);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  back->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onBack));
  back->setColor(c);

  auto quit= cx::reifyMenuBtn("icon_quit.png");
  quit->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onQuit));
  quit->setColor(c);

  auto m2= cx::mkMenu(s::vector<c::MenuItem*> {back, quit}, false, 10.0);
  auto sz= back->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  addItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::reifyAudioIcons(on, off);
  off->setColor(c);
  on->setColor(c);

  addAudioIcons((XLayer*) this,
      off, on,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref* ) {
  cx::runScene( Game::reify(f::GMode::ONE);
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::reifyWithBackAction(VOIDFN cb) {
  auto m = f::reifyRefType<MainMenu>();
  m->backAction= cb;
  m->realize();
  return m;
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* MainMenu::realize() {
  auto y = f::reifyRefType<UILayer>();
  addLayer(y);
  y->realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::~MainMenu() {
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu() {
  SNPTR(backAction)
}



NS_END(terra)
