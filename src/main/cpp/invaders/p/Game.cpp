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
#include "MMenu.h"
#include "Game.h"
#include "HUD.h"
#include "Ende.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)
BEGIN_NS_UNAMED

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*) getSceneX()->getLayer(3); }
  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onInited();
  void onPlayerKilled();
  void showMenu();
  void onEarnScore(int);

  DECL_PTR(ecs::Node, _player)

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  auto cannon = _engine->getNodes("n/CannonCtrl")[0];
  auto gun = CC_GEC(Cannon, cannon, "n/Cannon");
  auto lpr= CC_GEC(Looper, cannon, "n/Looper");
  auto cfg= MGMS()->getLCfg()->getValue();

  _player = _engine->getNodes("n/Ship")[0];

  // pre-population objects in pools
  SCAST(GEngine*,_engine)->reifyExplosions();
  SCAST(GEngine*,_engine)->reifyMissiles();
  SCAST(GEngine*,_engine)->reifyBombs();

  lpr->timer7 = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= []() { cx::pop(); };
  cx::pushEx(MMenu::reify(mc_new1(MCX, f)));
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  this->_engine= mc_new(GEngine);
  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  cx::sfxPlay("xxx-explode");
  if (getHUD()->reduceLives(1)) {
    MGMS()->stop();
    Ende::reify(MGMS(),4);
  } else {
    CC_GEC(Ship,_player,"n/Ship")->inflate();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(int score) {
  getHUD()->updateScore(score);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = SCAST(GLayer*, getLayer(2));

  if (topic == "/game/player/earnscore") {
    auto msg = (j::json*) m;
    auto n= JS_INT(msg->operator[]("score"));
    y->onEarnScore(n);
  }
  else
  if (topic == "/hud/showmenu") {
    y->showMenu();
  }
  else
  if (topic == "/hud/replay") {
  }
  else
  if (topic == "/game/player/killed") {
    y->onPlayerKilled();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {

  MGMS()->reifyPool("Explosions");
  MGMS()->reifyPool("Aliens");
  MGMS()->reifyPool("Missiles");
  MGMS()->reifyPool("Bombs");

  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);

  play();
}



NS_END



