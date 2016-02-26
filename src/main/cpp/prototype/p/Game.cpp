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

  DECL_PTR(c::PhysicsWorld, pWorld);
  DECL_PTR(a::NodeList, shared)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

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
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  for (auto n = 0; n < 2;  ++n) {
    auto s = cx::createSprite("game.bg");
    ss->bgSprites[n]=s;
    s->setPosition(
        wb.cx,
        (-1 * wz.size.height * n) + wb.cy);
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
  pWorld = world;
  pWorld->setGravity(c::Vec2(0, 0));
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onContactBegin(c::PhysicsContact&) {
  setOpacity(255 * 0.1);
  cx::sfxPlay("crash");
  surcease();
  MGMS()->stop();
  Ende::reify(MGMS(), 4);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  auto ss= CC_GNLF(GVars, shared, "slots");
  ss->isTouching = true;
  ss->touchPt = touch->getLocation();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  auto ss= CC_GNLF(GVars, shared, "slots");
  ss->touchPt = touch->getLocation();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  auto ss= CC_GNLF(GVars, shared, "slots");
  ss->isTouching = false;
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  auto btn= cx::createMenuBtn("pause-std","pause-sel");
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

  engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
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



