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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveXXX(f::FPool *p, float dt) {
  p->foreach([=](f::Poolable *a) {
    auto e= (ecs::Node*) a;
    if (e->status()) { this->moveAstros(e, dt); }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveBBB(f::FPool *po, float dt) {
  po->foreach([=](f::Poolable *p) {
    auto e= (ecs::Node*)p;
    if (e->status()) {
      auto sp=CC_GEC(f::CPixie,e,"f/CPixie");
      auto mv=CC_GEC(f::CMove,e,"f/CMove");
      auto pos= sp->pos();
      auto y = pos.y + dt * mv->vel.y * mv->speed.y;
      auto x = pos.x + dt * mv->vel.x * mv->speed.x;
      CC_POS2(sp, x, y);
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    moveXXX(MGMS()->getPool("Astros3"), dt);
    moveXXX(MGMS()->getPool("Astros2"), dt);
    moveXXX(MGMS()->getPool("Astros1"), dt);
    processShipMotions(dt);
    moveBBB(MGMS()->getPool("Missiles"), dt);
    moveBBB(MGMS()->getPool("Lasers"), dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
float Move::rotateShip(float cur, float deg) {
  cur += deg;
  if (cur >= 360) {
    cur = cur - 360;
  }
  if (cur < 0) {
    cur = 360 + cur;
  }
  return cur;
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT Move::thrust(float angle, float power) {
  return cx::calcXY(angle, power);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processShipMotions(float dt) {
  auto mo= CC_GEC(f::CGesture, _ship, "f/CGesture");
  auto sp= CC_GEC(Ship, _ship, "f/CPixie");
  auto mv= CC_GEC(f::CMove, _ship, "f/CMove");
  auto pos = sp->pos();
  auto x= pos.x;
  auto y= pos.y;

  if (mo->right) {
    mv->angle= rotateShip(mv->angle, 3);
    sp->setRotation(mv->angle);
  }

  if (mo->left) {
    mv->angle= rotateShip(mv->angle, -3);
    sp->setRotation(mv->angle);
  }

  if (mo->up) {
    auto acc= thrust(mv->angle, mv->power);
    sp->setSpriteFrame(sp->frame1);
    mv->acc.x= acc.x;
    mv->acc.y= acc.y;
  } else {
    sp->setSpriteFrame(sp->frame0);
  }
  moveShip(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveShip(float dt) {
  auto mv = CC_GEC(f::CMove, _ship,"f/CMove");
  auto sp = CC_GEC(Ship, _ship,"f/CPixie");
  auto B = MGMS()->getEnclosureBox();
  auto sz = sp->csize();
  auto pos= sp->pos();
  float x,y;

  mv->vel.y = mv->vel.y + dt * mv->acc.y;
  mv->vel.x = mv->vel.x + dt * mv->acc.x;

  if (mv->vel.y > mv->maxSpeed.y) {
    mv->vel.y = mv->maxSpeed.y;
  }
  else
  if (mv->vel.y < - mv->maxSpeed.y) {
    mv->vel.y = - mv->maxSpeed.y;
  }

  if (mv->vel.x > mv->maxSpeed.x) {
    mv->vel.x = mv->maxSpeed.x;
  }
  else
  if (mv->vel.x < -mv->maxSpeed.x) {
    mv->vel.x = -mv->maxSpeed.x;
  }

  y = pos.y + dt * mv->vel.y;
  x = pos.x + dt * mv->vel.x;

  CC_POS2(sp, x,y);

  //wrap?
  auto r= cx::bbox4(sp);

  if (r.bottom > B.top) {
    if (mv->vel.y > 0) {
      y = B.bottom - sz.height;
    }
  }

  if (r.top < B.bottom) {
    if (mv->vel.y < 0) {
      y = B.top + sz.height;
    }
  }

  if (r.left > B.right) {
    if (mv->vel.x > 0) {
      x = B.left - sz.width;
    }
  }

  if (r.right < B.left) {
    if (mv->vel.x < 0) {
      x = B.right + sz.width;
    }
  }

  sp->setRotation(mv->angle);
  CC_POS2(sp, x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveAstros(ecs::Node *astro, float dt) {
  auto mv= CC_GEC(f::CMove,astro,"f/CMove");
  auto sp= CC_GEC(f::CPixie,astro,"f/CPixie");
  auto B= MGMS()->getEnclosureBox();
  auto rot= mv->angle;
  auto sz= sp->csize();
  auto pos= sp->pos();
  auto x = pos.x + dt * mv->vel.x;
  auto y = pos.y + dt * mv->vel.y;

  rot += 0.1f;
  if (rot > 360) { rot -= 360; }

  sp->setRotation(rot);
  mv->angle = rot;
  CC_POS2(sp, x,y);

  //wrap?
  auto r= cx::bbox4(sp);

  if (r.bottom > B.top) {
    if (mv->vel.y > 0) {
      y = B.bottom - sz.height;
    }
  }

  if (r.top < B.bottom) {
    if (mv->vel.y < 0) {
      y = B.top + sz.height;
    }
  }

  if (r.left > B.right) {
    if (mv->vel.x > 0) {
        x = B.left - sz.width;
      }
    }

  if (r.right < B.left) {
    if (mv->vel.x < 0) {
      x = B.right + sz.width;
    }
  }

  CC_POS2(sp, x,y);
}


NS_END



