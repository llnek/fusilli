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
#include "s/GEngine.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(dttower)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void loadDefensePositions();
  void enemyReachedTower();
  void onEnd();

  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _player)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _player= _engine->getNodes("f/CHuman")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  loadDefensePositions();

  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadDefensePositions() {
  auto dict= cx::readXmlAsDict(XCFG()->getAtlas("defenses"));
  auto sz = cx::calcSize("defense_level_1.png");
  auto arr= f::dictVal<c::Array>(dict, "data");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto cnt= arr->count();
  auto sw= sz.width;
  auto gap = 19;

  ss->defensePositions.clear();

  for (auto i=0; i < cnt; ++i) {
    auto s = cx::reifySprite("defense_position.png");
    XCFG()->fit(s);
    auto d= (c::Dictionary*) arr->objectAtIndex(i);
    auto mX = f::dictVal<c::String>(d,"x")->floatValue();
    auto mY = f::dictVal<c::String>(d,"y")->floatValue();

    CC_POS2(s, sw * mX + gap, sw * mY);
    addAtlasItem("game-pics",s);
    ss->defensePositions.push_back(s);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  F__LOOP(it,ss->defensePositions) {
    auto &dp= *it;
    if (dp->boundingBox().containsPoint(tap)) {
      auto po=MGMS()->getPool("Defense1");
      auto e= po->take(true);
      auto pos= dp->getPosition();
      cx::resurrect((ecs::Node*)e, pos.x, pos.y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::enemyReachedTower() {
  auto h = CC_GEC(f::CHealth,_player,"f/CHealth");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  h->hurt();
  getHUD()->updateLifePts(h->curHP);

  if (!h->alive()) { onEnd(); }
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/hud/updatewave") {
    auto msg= (j::json*)m;
    y->getHUD()->updateWave(
        JS_INT(msg->operator[]("wave")));
  }

  if ("/game/player/enemyreachedtower" == topic) {
    y->enemyReachedTower();
  }

}


//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




