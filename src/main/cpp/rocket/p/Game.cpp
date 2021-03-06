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
#include "Ende.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(rocket)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onInited();

  void createParticles();
  void resetStar();
  void killPlayer();
  void createStarGrid();

  __decl_ptr(ecs::Node, _drawing)
  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _rocket)

};

static s_arr<CCT_PT,7> PPOS = {
  CCT_PT(0.25,0.8),
  CCT_PT(0.8,0.45),
  CCT_PT(0.75,0.8),
  CCT_PT(0.5,0.5),
  CCT_PT(0.18,0.45),
  CCT_PT(0.8,0.15),
  CCT_PT(0.18,0.1)
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _rocket= _engine->getNodes("f/CGesture")[0];
  _drawing= _engine->getNodes("n/RPath")[0];
  _shared = _engine->getNodes("n/GVars")[0];

  auto mv=CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto dw= CC_GEC(RPath,_drawing,"n/RPath");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto po= MGMS()->getPool("Planets");
  auto wz=cx::visSize();
  auto wb=cx::visBox();

  ss->minLineLength = wz.width * 0.07;
  ss->drawing = false;
  ss->state = kGamePlay;//kGameIntro;

  //add planets
  for (auto i=0; i < PPOS.size(); ++i) {
    auto s= Planet::create(i+1);
    auto &pos= PPOS[i];
    auto e= _engine->reifyNode("Planet", true);
    CC_POS2(s, wb.right * pos.x, wb.top * pos.y);
    addAtlasItem("game-pics",s,kBackground,kSpritePlanet);
    e->checkin(s);
    po->checkin(e);
  }

  createParticles();
  createStarGrid();

  CC_POS2(rock, wb.cx, wb.top * 0.1);
  rock->setOpacity(255);
  CC_SHOW(rock);
  rocketReset(rock, mv);

  ss->timeBetweenPickups = 0.0;
  ss->cometInterval = 4;
  ss->cometTimer = 0;

  //getHUD()->updateScore(0);

  dw->reset();

  //shuffle grid cells
  S__MIX(ss->grid);
  ss->gridPos = 0;

  resetStar();
  ss->warp->stopSystem();

  cx::sfxPlay("rocket");
  cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto mv=CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto dw=CC_GEC(RPath,_drawing,"n/RPath");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto rpos=rock->getPosition();
  auto dx = rpos.x - tap.x;
  auto dy = rpos.y - tap.y;

  if (dx * dx + dy * dy <= pow(rock->radius(), 2)) {
    mv->rotationOrientation= ROTATE_NONE;
    dw->lineType= LINE_NONE;
    ss->drawing = true;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (ss->drawing) {
    auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
    auto dw=CC_GEC(RPath,_drawing,"n/RPath");
    auto rpos=rock->getPosition();
    auto dx = rpos.x - tap.x;
    auto dy = rpos.y - tap.y;

    if (dx * dx + dy * dy > pow(ss->minLineLength, 2)) {
      dw->lineType= LINE_TEMP;
      dw->pivot= tap;
      rocketSelect(rock,true);
    } else {
      dw->lineType= LINE_NONE;
      rocketSelect(rock, false);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
  auto mv=CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto dw= CC_GEC(RPath,_drawing,"n/RPath");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  //track if tapping on ship
  ss->drawing = false;
  rocketSelect(rock, false);

  //if we are showing a temp line
  if (dw->lineType == LINE_TEMP) {
    //set up dashed line
    dw->pivot= tap;
    dw->lineLength= rock->getPosition().distance(tap);

    //set up rocket
    auto circlelen = dw->lineLength * 2 * M_PI;
    auto iters = floor(circlelen / mv->speed.x);
    mv->pivot=tap;
    mv->angularSpeed= 2 * M_PI / iters;

    auto diff = rock->getPosition();
    diff.subtract(mv->pivot);
    auto clkwise = diff.getRPerp();
    auto dot =clkwise.dot(mv->vel);

    if (dot > 0) {
      mv->rotationOrientation= ROTATE_CLOCKWISE;
      mv->angularSpeed= - mv->angularSpeed;
      mv->targetRotation=
        CC_RADIANS_TO_DEGREES(atan2(clkwise.y, clkwise.x));
    } else {
      mv->rotationOrientation= ROTATE_COUNTER;
      mv->targetRotation=
        CC_RADIANS_TO_DEGREES(atan2(-clkwise.y, -clkwise.x));
    }
    dw->lineType= LINE_DASHED;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {
  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");
  //regoAtlas("cc-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createParticles() {
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto wb=cx::visBox();

  ss->jet = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("jet"));
  ss->jet->setSourcePosition(CCT_PT(-rock->radius() * 0.8,0));
  ss->jet->setAngle(180);
  ss->jet->stopSystem();
  addItem(ss->jet, kBackground);

  ss->boom = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("boom"));
  ss->boom->stopSystem();
  addItem(ss->boom, kForeground);

  ss->comet = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("comet"));
  ss->comet->stopSystem();
  ss->comet->setPosition(CCT_PT(0, wb.top * 0.6));
  CC_HIDE(ss->comet);
  addItem(ss->comet, kForeground);

  ss->pickup = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("plink"));
  ss->pickup->stopSystem();
  addItem(ss->pickup, kMiddleground);

  ss->warp = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("warp"));
  ss->warp->setPosition(rock->pos());
  addItem(ss->warp, kBackground);

  ss->star = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("star"));
  ss->star->stopSystem();
  CC_HIDE(ss->star);
  addItem(ss->star, kBackground, kSpriteStar);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createStarGrid() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto po=MGMS()->getPool("Planets");
  auto wz=cx::visSize();
  auto ps= po->ls();
  auto tile = 32;
  auto gridFrame = CC_ZW(wz) * 0.1;
  auto rows = (CC_ZH(wz) - 4 * gridFrame)/tile;
  auto cols = (CC_ZW(wz) - 2 * gridFrame)/tile;

  for (auto r = 0; r < rows; ++r) {
    for (auto c = 0; c < cols; ++c) {
      auto cell = CCT_PT(gridFrame + c * tile, 2 * gridFrame + r * tile);
      auto overlaps = false;
      F__LOOP(it,ps) {
        auto e= *it;
        auto pt=CC_GEC(Planet,e,"f/CPixie");
        auto pos= pt->getPosition();
        if (pow(pos.x - cell.x, 2) + pow(pos.y - cell.y, 2) <=
            pow(pt->radius() * 1.2, 2)) {
          overlaps = true;
        }
      }
      if (!overlaps) { ss->grid.push_back(cell); }
    }
  }

  CCLOG("POSSIBLE STARS: %i", (int)ss->grid.size());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::resetStar() {
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto rpos=rock->getPosition();

  while (true) {
    auto pos= ss->grid[ss->gridPos];
    if (++ss->gridPos >= ss->grid.size()) {
    ss->gridPos=0; }
    if (pow(pos.x - rpos.x, 2) +
        pow(pos.y - rpos.y, 2) > rock->radius() * 6) {
      CC_POS1(ss->star, pos);
      CC_SHOW(ss->star);
      ss->star->resetSystem();
      return;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::killPlayer() {
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto dw=CC_GEC(RPath,_drawing,"n/RPath");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto wb=cx::visBox();

  cx::pauseAudio();
  cx::sfxPlay("shipBoom");

  CC_POS1(ss->boom, rock->getPosition());
  ss->boom->resetSystem();
  CC_HIDE(rock);
  ss->jet->stopSystem();
  dw->lineType= LINE_NONE;

  ss->state = kGameOver;
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/player/killed") {
    y->killPlayer();
  }
  else
  if (topic == "/game/resetstar") {
    y->resetStar();
  }
  else
  if (topic == "/game/player/earnscore") {
    auto msg= (j::json*) m;
    y->getHUD()->updateScore(JS_INT(msg->operator[]("score")));
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




