﻿// This library is distributed in  the hope that it will be useful but without
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
#include "Player.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
Player::~Player(){
  CC_DROP(_floatAnimation);
  CC_DROP(_rideAnimation);
}

//////////////////////////////////////////////////////////////////////////////
//
PlayerMotion::PlayerMotion() {
  maxSpeed.x = PLAYER_INITIAL_SPEED;
  speed.x = PLAYER_INITIAL_SPEED;
  _floatingTimerMax = 2;
  _floatingTimer = 0;
  nextPos.x=0;
  nextPos.y = cx::visBox().top * 0.6;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Player*> Player::create() {
  auto p= mc_new(Player);
  p->initWithSpriteFrameName("player_1.png");
  p->autorelease();
  return p;
}

void Player::update (float dt) {

    if (_speed + P_ACCELERATION <= _maxSpeed) {
        _speed += P_ACCELERATION;
    } else {
        _speed = _maxSpeed;
    }

    _vector.x = _speed;

  switch (_state) {
    case kPlayerMoving:
      _vector.y -= G_FORCE;
            if (_hasFloated) _hasFloated = false;
    break;

        case kPlayerFalling:
            if (_floating ) {
        _vector.y -= FLOATNG_GRAVITY;
        _vector.x *= FLOATING_FRICTION;

            } else {
        _vector.y -= G_FORCE;
        _vector.x *= AIR_FRICTION;
        _floatingTimer = 0;
      }
    break;
        case kPlayerDying:
            _vector.y -= G_FORCE;
            _vector.x = -_speed;
            this->setPositionX(this->getPositionX() + _vector.x);
        break;

  }

    if (_jumping) {
        _state = kPlayerFalling;
        _vector.y += PLAYER_JUMP * 0.25f;
        if (_vector.y > PLAYER_JUMP ) _jumping = false;
    }

    if (_vector.y < -TERMINAL_VELOCITY) _vector.y = -TERMINAL_VELOCITY;

    _nextPosition.y = this->getPositionY() + _vector.y;


  if (_vector.x * _vector.x < 0.01) _vector.x = 0;
  if (_vector.y * _vector.y < 0.01) _vector.y = 0;

  if (_floating) {
    _floatingTimer += dt;
    if (_floatingTimer > _floatingTimerMax) {
            _floatingTimer = 0;
            SimpleAudioEngine::getInstance()->playEffect("falling.wav");
      this->setFloating(false);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::setFloating(Player *py, bool value) {

  if (_floating == value ||
      (value && _hasFloated) { return; }

  py->stopAllActions();
  _floating = value;

  if (value) {
    py->setDisplayFrame(cx::getSpriteFrame("player_float.png"));
    py->animateFloat();
    _hasFloated = true;
    vel.y += HTV(PLAYER_JUMP);
    cx::sfxPlay("openUmbrella");
  } else {
    py->animateRide();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::animateFloat() { this->runAction(_floatAnimation); }
void Player::animateRide() { this->runAction(_rideAnimation); }

//////////////////////////////////////////////////////////////////////////////
//
bool Player::initSpriteWithFrameName(const sstr &fn) {
  auto rc= c::Sprite::initSpriteWithFrameName(fn);
  auto wb= cx::visBox();
  if (!rc) { retutn rc; }
  this->setPosition(wb.right * 0.2, wb.top* 0.6);
  this->setAnchorPoint(cx::anchorT());

  auto animation= c::Animation::create();
  animation->setRestoreOriginalFrame(false);
  animation->setDelayPerUnit(0.2 / 3.0);
  animation->setLoops(-1);
  for (auto i = 1; i <= 3; ++i) {
    animation->addSpriteFrame(
      cx::getSpriteFrame("player_" + FTOS(i) + ".png"));
  }
  _rideAnimation = c::Animate::create(animation);
  _rideAnimation->retain();

  _floatAnimation = c::RepeatForever::create(
      (ActionInterval*)
      c::Sequence::create(
             c::EaseInOut::create(c::RotateTo::create(0.8, -10), 2),
             c::EaseInOut::create(c::RotateTo::create(0.8, 10), 2),
             CC_NIL));
  _floatAnimation->retain();

  animateRide();
}



NS_END



