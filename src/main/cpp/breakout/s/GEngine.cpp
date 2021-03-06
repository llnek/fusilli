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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"
#include "Move.h"
#include "p/Config.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyNode("Arena", true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);

  createBricks();
  createPaddle();
  createBall();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(Move,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBricks() {
  auto cfg= MGMS()->getLCfg()->getValue();
  auto tile = CC_CSV(c::Float, "TILE");
  auto cg= (Config*) XCFG();
  auto wz = cx::visSize();
  auto wb = cx::visBox();

  auto csz= XCFG()->fit(cx::calcSize("red_candy.png"));
  auto loff= CC_CSV(c::Integer,"LEFT+OFF");
  j::json rows= JS_ARR(cfg["ROWS"]);
  auto cols= JS_INT(cfg["COLS"]);
  auto tr= JS_INT(cfg["TOP+ROW"]);
  auto y= wb.top - tr*tile;
  auto x=0.0f;
  auto bf= mc_new(BrickFence);

  J__LOOP(it, rows) {
    auto &rr = *it;
    auto r= JS_INT(rr);
    auto cn= cg->getCandy(r);
    x= HWZ(csz) + tile + loff;
    for (auto c=0; c < cols; ++c) {
      auto v= JS_INT(cfg["CDS"][cn]);
      auto b = Brick::create(cn, v,r);
      MGML()->addAtlasItem("game-pics", b);
      bf->bricks.push_back(b);
      b->inflate(x, y);
      x += csz.width + 1;
    }
    y -= csz.height - 2;
  }

  auto ent= this->reifyNode("BrickFence", true);
  ent->checkin(bf);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::bornPaddle(not_null<ecs::Node*> p, not_null<ecs::Node*> b) {
  auto pad= CC_GEC(f::CPixie, p, "f/CPixie");
  auto mv= CC_GEC(f::CMove, b, "f/CMove");
  auto ba= CC_GEC(f::CPixie, b, "f/CPixie");
  auto wb= cx::visBox();

  ba->inflate(wb.cx, wb.cy);
  pad->inflate();
  mv->vel.y = mv->speed.y * cx::randSign();
  mv->vel.x = mv->speed.x * cx::randSign();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createPaddle() {
  auto cfg= MGMS()->getLCfg()->getValue();
    auto sp = f::CPixie::reifyFrame("paddle.png");
  XCFG()->fit(sp);
  
  auto mv= mc_new(f::CMove);
  auto wb= cx::visBox();

  mv->speed.x = JS_INT(cfg["PADDLE+SPEED"]);
  MGML()->addAtlasItem("game-pics", sp);
  sp->inflate(wb.cx, 56);

  auto ent= this->reifyNode("Paddle", true);
  ent->checkin(mv);
  ent->checkin(sp);
  ent->checkin(mc_new(f::CHuman));
  ent->checkin(mc_new(f::CGesture));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBall() {
  auto cfg = MGMS()->getLCfg()->getValue();
    auto sp = f::CPixie::reifyFrame("ball.png");
  XCFG()->fit(sp);
  
  auto mv= mc_new(f::CMove);
  auto wb= cx::visBox();

  mv->speed.x = JS_INT(cfg["BALL+SPEED"]);
  mv->speed.y=mv->speed.x;
  MGML()->addAtlasItem("game-pics", sp);
  mv->vel.x= mv->speed.x * cx::randSign();
  mv->vel.y= mv->speed.y * cx::randSign();
  sp->inflate(wb.cx, wb.cy);

  auto ent= this->reifyNode("Ball", true);
  ent->checkin(mv);
  ent->checkin(sp);
  ent->checkin(mc_new(f::CAutoma));
}


NS_END



