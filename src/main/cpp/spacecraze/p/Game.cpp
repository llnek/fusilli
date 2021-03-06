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
#include "BackDrop.h"
#include "MMenu.h"
#include "HUD.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)
BEGIN_NS_UNAMED

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  void onAlienKilled(ecs::Node*);
  void onPlayerKilled();
  void onEnd();

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  __decl_ptr(ecs::Node, _aliens)
  __decl_ptr(ecs::Node,_ship)
  __decl_ptr(ecs::Node, _shared)

};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto ui= CC_GEC(Ship,_ship,"f/CPixie");
  return ui->boundingBox().containsPoint(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *t) {
  return onMouseStart(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {
  auto ui= CC_GEC(Ship,_ship,"f/CPixie");
  auto bx= MGMS()->getEnclosureBox();
  auto pos= ui->getPosition();
  auto loc= cx::clamp(tap, CC_CSIZE(ui), bx);
  CC_POS2(ui, loc.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *t) {
  onMouseMotion(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(),4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {

  auto sp= CC_GEC(Ship,_ship,"f/CPixie");
  c::CallFunc *d2;

  _ship->yield();

  if (getHUD()->reduceLives(1)) {
    d2=c::CallFunc::create([=]() {
        this->onEnd();
    });
  } else {
    d2=c::CallFunc::create([=]() {
        spawnPlayer(_ship);
    });
  }

  sp->runAction(
      c::Sequence::create(
        c::Spawn::create(
          c::FadeOut::create(0.5),
          c::ScaleTo::create(0.5, 1.5),CC_NIL),
        c::CallFunc::create(
          [=]() { cx::hibernate(_ship); }),
        d2,
        CC_NIL));

  auto exp= c::ParticleSystemQuad::create(
        XCFG()->getAtlas("explosions"));
  auto pos= sp->getPosition();

  exp->setAutoRemoveOnFinish(true);
  exp->setPosition(pos);
  MGML()->addItem(exp);

  cx::sfxPlay("blast_player");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAlienKilled(ecs::Node *node) {

  auto cs=CC_GEC(f::CStats,node,"f/CStats");
  auto ui=CC_GEC(Alien,node,"f/CPixie");
  auto pos= ui->getPosition();

  node->yield();
  ui->stopAllActions();
  ui->runAction(
      c::Sequence::create(
        c::ScaleTo::create(0.25, 0.0),
        c::CallFunc::create(
          [=]() { cx::hibernate(node); }),
        CC_NIL));

  auto pc= c::ccc4f(0.9569, 0.2471, 0.3373, 1);
  auto exp = c::ParticleSystemQuad::create( XCFG()->getAtlas("explosions"));
  exp->setAutoRemoveOnFinish(true);
  CC_POS1(exp, pos);
  exp->setStartColor(pc);
  exp->setEndColor(pc);
  MGML()->addItem(exp);

  getHUD()->updateScore(cs->value);
  cx::sfxPlay("blast_enemy");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {
  auto c= loadLevel(MGMS()->getLevel());
  regoAtlas("game-pics");
  _engine= mc_new1(GEngine,c);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _aliens = _engine->getNodes("n/AlienSquad")[0];
  _ship= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  spawnPlayer(_ship);

  auto lpr= CC_GEC(f::Loopers,_shared,"f/Loopers");
  // timers for bullets
  lpr->tms[0].timer= cx::reifyTimer(MGML(), 1000);
  lpr->tms[0].duration= 1000;
  lpr->tms[1].timer= cx::reifyTimer(MGML(), 1000);
  lpr->tms[1].duration= 1000;
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = (GLayer*) getLayer(2);

  if ("/game/player/killed" == topic) {
    y->onPlayerKilled();
  }
  else
  if ("/game/alien/killed" == topic) {
    y->onAlienKilled((ecs::Node*) m);
  }
  else
  if ("/game/hud/earnscore" == topic) {
    auto msg= (j::json*) m;
    y->getHUD()->updateScore(
      JS_INT(msg->operator[]("score")));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {
  BackDrop::reify(this, 1);
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}


NS_END



