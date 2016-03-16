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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared = _engine->getNodes("n/GVars")[0];
  _bomb = _engine->getNodes("n/Bomb")[0];
  _ufo = _engine->getNodes("n/Ufo")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {

  auto bomb=CC_GEC(f::CPixie,_bomb,"f/CPixie");
  auto ufo=CC_GEC(f::CPixie,_ufo,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto wb=cx::visBox();

  ss->meteorTimer += dt;
  if (ss->meteorTimer > ss->meteorInterval) {
    ss->meteorTimer = 0;
    resetMeteor();
  }

  ss->ufoTimer += dt;
  if (ss->ufoTimer > ss->ufoInterval) {
    ss->ufoTimer = 0;
    resetUfo(_ufo);
  }

  ss->healthTimer += dt;
  if (ss->healthTimer > ss->healthInterval) {
    ss->healthTimer = 0;
    resetHealth();
  }

  ss->difficultyTimer += dt;
  if (ss->difficultyTimer > ss->difficultyInterval) {
    ss->difficultyTimer = 0;
    increaseDifficulty();
  }

  if (bomb->node->isVisible()) {
    if (bomb->node->getScale() > 0.3) {
      if (bomb->node->getOpacity() != 255) {
        bomb->node->setOpacity(255);
      }
    }
  }

  auto ray = CC_GCT(ufo->node,kSpriteRay);
  auto pos=ufo->pos();
  if (ufo->node->isVisible() &&
      ray->isVisible()) {
    if (pos.x > wb.right * 0.1 &&
        pos.x <= wb.right * 0.9) {
      changeEnergy(-0.25);
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetMeteor() {
  if (ss->fallingObjects.size() > 30) {
  return; }
  auto wb=cx::visBox();
  auto mtx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto mx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto ss = CC_GEC(GVars,_shared,"n/GVars");
  auto po= MGMS()->getPool("Meteors");
  auto e = po->take(true);
  auto meteor=CC_GEC(f::CPixie,e,"f/CPixie");
  auto sz= meteor->csize();

  meteor->node->setPosition(mx, wb.top + HHZ(sz));
  meteor->node->stopAllActions();

  auto rr = c::RepeatForever::create(
      c::RotateBy::create(0.5 , -90));
  auto seq= c::Sequence::create(
      c::MoveTo::create(ss->meteorSpeed,
        c::Vec2(mtx, wb.top * 0.15)),
      c::CallFunc::create(
        [=]() { this->fallingObjectDone(e); }),
      CC_NIL);

  meteor->show();
  meteor->node->runAction(rr);
  meteor->node->runAction(seq);

  ss->fallingObjects.insert(
      S__PAIR(ecs::Node*,ecs::Node*,e,e));
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetUfo(ecs::Node *node) {

  auto UFO_SPEED = CC_CSV(c::Integer,"UFO+SPEED");
  auto ufo=CC_GEC(f::CPixie,node,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb= cx::visBox();
  auto newY = cx::randFloat(wb.top * 0.3) + wb.top * 0.3;
  auto newX= cx::randSign() > 0 ? 0 : wb.right;

  if (ufo->node->isVisible()) { return; }

  if (newY > wb.top * 0.7) {
    newY = wb.top * 0.7;
  }

  ufo->node->stopAllActions();
  ufo->setPos(newX, newY);
  ufo->node->runAction(ss->ufoAnimation->clone());

  auto ray = CC_GCT(ufo->node,kSpriteRay);
  CC_HIDE(ray);
  ray->stopAllActions();
  ray->runAction(ss->blinkRay->clone());

  c::FiniteTimeAction *seq;
  if ((int)newX == 0) {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          c::Vec2(wb.right * 1.1, newY)),
        c::CallFuncN::create(
          [](c::Node *p) { CC_HIDE(p); }),
        CC_NIL);
  } else {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          c::Vec2(-wb.right * 0.1, newY)),
        c::CallFuncN::create(
          [](c::Node *p) { CC_HIDE(p); }),
        CC_NIL);
  }

  ufo->show();
  ufo->node->runAction(seq);
  ss->ufoKilled = false;
  cx::sfxPlay("pew");
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetHealth() {

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb= cx::visBox();

  if (ss->fallingObjects.size() > 30) {
  return; }

  auto htx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto hx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto hp= MGMS()->getPool("Healths");
  auto e = hp->take(true);
  auto health=CC_GEC(f::CPixie,e,"f/CPixie");
  auto sz= health->csize();

  health->setPos(hx, wb.top + HHZ(sz));
  health->node->stopAllActions();

  auto seq = c::Sequence::create(
         c::MoveTo::create(ss->healthSpeed,
           c::Vec2(htx, wb.top * 0.15)),
         c::CallFunc::create(
           [=]() { this->fallingObjectDone(e); }),
         CC_NIL);

  health->show();
  health->node->runAction( ss->swingHealth->clone());
  health->node->runAction(seq);

  ss->fallingObjects.insert(
      S__PAIR(ecs::Node*,ecs::Node*,e,e));
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::changeEnergy(float value) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  ss->energy += value;

  if (ss->energy <= 0) {
    ss->energy = 0;
    SENDMSG("/game/end");
  }

  if (ss->energy > 100) {
    ss->energy = 100;
  }

  SENDMSG("/game/hud/updateenergy");
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::increaseDifficulty () {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  ss->meteorInterval -= 0.15;
  if (ss->meteorInterval < 0.25) {
    ss->meteorInterval = 0.25;
  }

  ss->meteorSpeed -= 1;
  if (ss->meteorSpeed < 4) {
    ss->meteorSpeed = 4;
  }

  ss->ufoInterval -= 0.1;
  if (ss->ufoInterval < 5) {
    ss->ufoInterval = 5;
  }

  ss->healthSpeed -= 1;
  if (ss->healthSpeed < 8) {
    ss->healthSpeed = 8;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void AI::fallingObjectDone(ecs::Node *node) {

  auto ui= CC_GEC(f::CPixie,node,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  ss->fallingObjects.erase(node);

  ui->node->stopAllActions();
  ui->node->setRotation(0);

  if (ui->node->getTag() == kSpriteMeteor) {
    changeEnergy(-15);
    ui->node->runAction(ss->groundHit->clone() );
    //play explosion sound
    cx::sfxPlay("boom");
  } else {
    ui->hide();
    if ((int)ss->energy == 100) {
      auto msg= j::json({
          {"score", 25}
          });
      SENDMSGEX("/game/player/earnscore", &msg);
    } else {
      changeEnergy(10);
    }
    //play health bonus sound
    cx::sfxPlay("health");
  }

  cx::hibernate(node);
}




NS_END



