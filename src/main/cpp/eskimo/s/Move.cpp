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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "Box2D/Box2D.h"
#include "core/CCSX.h"
#include "n/Eskimo.h"
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
// some friction
static void applyForce(b2Body *body, float acc, float mult, bool x) {
  b2Vec2 bv;
  if (x) {
    bv= b2Vec2(acc * mult, 0);
  } else {
    bv= b2Vec2(0, acc * mult);
  }
  body->ApplyForce( bv, body->GetWorldCenter(), true);
  auto vec = body->GetLinearVelocity();
  if (x) {
    vec.x *= BALL_FRICTION;
  } else {
    vec.y *= BALL_FRICTION;
  }
  body->SetLinearVelocity(vec);
}

//////////////////////////////////////////////////////////////////////////////
// no friction
static void applyLinearImpulse(b2Body *body, float acc, float mult, bool x) {
  b2Vec2 bv;
  if (x) {
    bv= b2Vec2(acc * mult, 0);
  } else {
    bv= b2Vec2(0, acc * mult);
  }
  body->ApplyLinearImpulse( bv, body->GetWorldCenter(), true);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto player= CC_GEC(Eskimo,_player,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  player->update();

  //if player is touching something,
  //update it with accelerometer data

  if (player->getBody()->GetContactList()) {
    switch (ss->gravity) {
    //update X axis
      case kDirectionDown:
      case kDirectionUp:
        if (player->getShape() == kStateCircle) {
          applyForce(player->getBody(), ss->acc.x,CIRCLE_MULTIPLIER,true);
        } else {
          applyLinearImpulse(player->getBody(), ss->acc.x,BOX_MULTIPLIER, true);
        }
      break;
      //update Y axis
      case kDirectionRight:
      case kDirectionLeft:
        if (player->getShape() == kStateCircle) {
          applyForce(player->getBody(), ss->acc.y, CIRCLE_MULTIPLIER, false);
        } else {
          applyLinearImpulse(player->getBody(), ss->acc.y, BOX_MULTIPLIER, false);
        }
      break;
    }
  }
}

NS_END




