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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Stage.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Stage::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  PaddleNode p;
  BallNode b;
  ArenaNode a;

  fauxNode = e->getNodeList(f.typeId());
  ballNode=e->getNodeList(b.typeId());
  arenaNode = e->getNodeList(a.typeId());
  paddleNode=e->getNodeList(p.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  if (MGMS()->isLive()) {
    if (! inited) {
      onceOnly();
    }
  }
    return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly() {
  auto slots= CC_GNF(Slots, arenaNode->head, "slots");
  auto world = MGMS()->getEnclosureBox();
  auto ps = initPaddleSize();
  auto bs = initBallSize();
  auto cw= cx::center();

  // position of paddles
  // portrait
  auto p1y = world.top * 0.1f + bs.height + HHZ(ps);
  auto p2y = world.top * 0.9f - bs.height - HHZ(ps);
  // landscape
  auto p2x = world.right - bs.width - HWZ(ps);
  auto p1x = world.left + bs.width + HWZ(ps);

  slots->pz= c::Size( ps.width, ps.height);
  slots->bz= c::Size( bs.width, bs.height);
  slots->bp= c::Vec2( cw.x, cw.y);

  if (cx::isPortrait()) {
    slots->p1p= c::Vec2(cw.x, p1y);
    slots->p2p= c::Vec2(cw.x, p2y);
  } else {
    slots->p1p= c::Vec2(p1x, cw.y);
    slots->p2p= c::Vec2(p2x, cw.y);
  }

  CC_GNF(Ball,ballNode->head, "ball")->inflate(slots->bp.x, slots->bp.y);
  initPaddles(paddleNode, slots);
  initPaddles(fauxNode, slots);

  inited=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::initPaddles(a::NodeList *nl, Slots *slots) {
   for (auto node=nl->head;node;node=node->next) {
    auto p= CC_GNF(Paddle,node,"paddle");
    if (p->pnum == 2) { p->inflate(slots->p2p.x, slots->p2p.y); }
    if (p->pnum == 1) { p->inflate(slots->p1p.x, slots->p1p.y); }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size Stage::initPaddleSize() {
  return cx::calcSize("red_paddle.png");
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size Stage::initBallSize() {
  return cx::calcSize("pongball.png");
}


NS_END(pong)


