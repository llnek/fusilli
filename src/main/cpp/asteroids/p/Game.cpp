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
#include "s/GEngine.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  MDECL_GET_LAYER(HUDLayer,getHUD,3)
  STATIC_REIFY_LAYER(GLayer)
  MDECL_GET_IID(2)
  MDECL_DECORATE()

  DECL_PTR(a::NodeList, arenaNode)
  DECL_PTR(a::NodeList, shipNode)

  virtual void postReify();
  void spawnPlayer();
  void showMenu();
  void onDone();
  void onPlayerKilled();
  void onEarnScore(j::json*);

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  centerImage("game.bg");
  incIndexZ();
  regoAtlas("game-pics");

  this->engine = mc_new(GEngine);
  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {

  shipNode= engine->getNodeList(ShipMotionNode().typeId());
  arenaNode= engine->getNodeList(ArenaNode().typeId());

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnPlayer() {
    auto ship = CC_GNLF(Ship, shipNode, "ship");
  SCAST(GEngine*,engine)->bornShip(ship);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    spawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
  getHUD()->reset();
  MGMS()->stop();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(j::json *msg) {
  getHUD()->updateScore(JS_INT(msg->operator[]("score")));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {
  auto y = (GLayer*) getLayer(2);

  if (t== "/game/missiles/killed") {
    //y->onMissileKilled(msg);
  }

  if (t == "/game/ufos/killed") {
    //y->onUfoKilled(msg);
  }

  if (t == "/game/players/shoot") {
    //y->onFireMissile(msg);
  }

  if (t == "/game/players/killed") {
    y->onPlayerKilled();
  }

  if (t == "/game/ufos/shoot") {
    //y->onFireLaser(msg);
  }

  if (t == "/game/stones/create") {
    //y->onCreateStones(msg);
  }

  if (t == "/game/rocks/create") {
    //y->onCreateRocks(msg);
  }

  if (t == "/game/players/earnscore") {
    auto msg = (j::json*) m;
    y->onEarnScore(msg);
  }

  if (t == "/hud/showmenu") {
    y->showMenu();
  }

  if (t == "/hud/replay") {
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
}

NS_END(asteroids)


