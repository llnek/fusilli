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
#include "ScrollingBgLayer.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }
  void fireRocket(Hero*);
  void onStop();

  DECL_PTR(ScrollingBgLayer, bgLayer)
  DECL_PTR(a::NodeList, players)
  DECL_PTR(a::NodeList, shared)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);

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
void GLayer::onStop() {
  MGMS()->stop();
  surcease();
  this->setOpacity(255 * 0.1);
  Ende::reify(MGMS(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  players = engine->getNodeList(PlayerNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  auto hero=CC_GNLF(Hero,players,"player");
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  // player setup
  hero->inflate(wb.right * 0.125, wb.top * 0.5);
  addItem(hero->node, 5);

  // player idle
  auto anim = c::Animation::create();
  for (auto n=1; n <= 4; ++n) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("player_idle_"+s::to_string(n)+".png"));
  }
  anim->setDelayPerUnit(0.25);

  hero->idle= c::RepeatForever::create(c::Animate::create(anim));
  CC_KEEP(hero->idle);

  // player boost
  anim = c::Animation::create();
  for (auto n=1; n <= 4; ++n) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("player_boost_"+s::to_string(n)+".png"));
  }
  anim->setDelayPerUnit(0.25);

  hero->boost= c::RepeatForever::create(c::Animate::create(anim));
  CC_KEEP(hero->boost);

  hero->node->runAction(hero->boost->clone());


  ss->gravity = c::Vec2(0, -5);
  ss->bgLayer=this->bgLayer;
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto roll= CC_CSV(c::Float, "BG+SCROLL");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  bgLayer = f::reifyRefType<ScrollingBgLayer>();
  bgLayer->set(roll);
  addItem(bgLayer);

  regoAtlas("game-pics");


  engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {

  auto hero= CC_GNLF(Hero, players, "player");
  auto wb= cx::visBox();

  if (MGMS()->isLive()) {
    if (loc.x < wb.cx) {
      // treat as left button pressed
      hero->jump=true;
    } else {
      fireRocket(hero);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {

  auto hero= CC_GNLF(Hero, players, "player");
  auto loc= touch->getLocation();
  auto wb= cx::visBox();

  if (MGMS()->isLive()) {
    if (loc.x < wb.cx) {
      // treat as left button pressed
      hero->jump=true;
    } else {
      fireRocket(hero);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::fireRocket(Hero *hero) {

  auto po= MGMS()->getPool("Rockets");
    auto sz= hero->csize();
  auto pos = hero->pos();
  auto r= (Projectile*) po->getAndSet(true);

  r->inflate( pos.x + HWZ(sz), pos.y - sz.height * 0.05);

  cx::sfxPlay("fireRocket");

  auto emitter = c::ParticleExplosion::create();
  emitter->setPosition(c::ccpAdd(pos, c::Vec2(HWZ(sz) ,0 )));
  emitter->setStartColor(c::ccc4f(1.0, 1.0, 1.0, 1.0));
  emitter->setEndColor(c::ccc4f(0.0, 0.0, 0.0, 0.0));
  emitter->setTotalParticles(10);
  emitter->setLife(0.25);
  emitter->setSpeed(2.0);
  emitter->setSpeedVar(50.0);
  emitter->setAutoRemoveOnFinish(true);
  addItem(emitter, 0);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/player/lose") {
    y->onStop();
  }
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



