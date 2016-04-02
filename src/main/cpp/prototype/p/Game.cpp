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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(prototype)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  bool onContactBegin(c::PhysicsContact&);
  void setPhysicsWorld(c::PhysicsWorld*);

  __decl_ptr(c::PhysicsWorld, _pWorld);
  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  for (auto n = 0; n < 2;  ++n) {
    auto s = cx::createSprite("game.bg");
    ss->bgSprites[n]=s;
    s->setPosition(
        wb.cx,
        (-1 * CC_ZH(wz.size) * n) + wb.cy);
    addItem(s, -2);
  }

  setPhysicsWorld(MGMS()->getPhysicsWorld());

  auto ln = c::EventListenerPhysicsContact::create();
  ln->onContactBegin = CC_CALLBACK_1(GLayer::onContactBegin, this);
  getEventDispatcher()->addEventListenerWithSceneGraphPriority(ln, this);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::setPhysicsWorld(c::PhysicsWorld *world) {
  _pWorld = world;
  _pWorld->setGravity(c::Vec2(0, 0));
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onContactBegin(c::PhysicsContact&) {
  this->setOpacity(0.1 * 255);
  cx::sfxPlay("crash");
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
  auto r= CC_GEC(f::CPixie,_player,"f/CPixie");
  r->setPos(loc.x,loc.y);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  auto r= CC_GEC(c::Node,_player,"f/CPixie");
  auto loc= touch->getLocation();
  return cx::isClicked(r,loc);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  auto r= CC_GEC(f::CPixie,_player,"f/CPixie");
  auto loc= touch->getLocation();
  r->setPos(loc.x, loc.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  auto btn= cx::reifyMenuBtn("pause-std.png","pause-sel.png");
  auto sz= CC_CSIZE(btn);
  auto gap= sz.width / 4;
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  btn->setPosition(
      wb.left + sz.width - gap,
      wb.top - sz.height + gap);
  btn->setCallback([=](c::Ref*){
    cx::sfxPlay("button");
    cx::pushEx(MMenu::reify());
  });
  auto menu = cx::mkMenu(btn);
  addItem(menu);

  _engine = mc_new(GEngine);
  cx::sfxMusic("background", true);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}

//////////////////////////////////////////////////////////////////////////////
//
Game::Game()
  : f::GameScene(true) {
}

NS_END




