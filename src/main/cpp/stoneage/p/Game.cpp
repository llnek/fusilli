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
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  virtual void onTouchMotion(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual bool onTouchStart(c::Touch*);

  virtual void onMouseMotion(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);

  void buildGrid(GVars*);
  void addToScore();
  void startTimer();
  void tick(float);

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onInited();

  __decl_ptr(c::Sprite, _timeBar)
  __decl_ptr(ecs::Node, _shared)
  __decl_bf(_touchDown)

};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {

  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  if (!ss->enabled ) {
    return false;
  } else {
    this->_touchDown = true;
  }

  auto touched= findGemAtPos(ss, tap);
  if (touched.gem) {

    if (E_NIL(ss->selectedGem)) {
      selectStartGem(ss, touched);
    } else {
      if (isValidTarget(ss, touched.x, touched.y, tap)) {
        selectTargetGem(ss, touched);
      } else {
        if (ss->selectedGem) {
          ss->selectedGem->deselect();
        }
        ss->selectedGem = CC_NIL;
        selectStartGem (ss,touched);
      }
    }

  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  this->_touchDown = false;
  if (!MGMS()->isLive()) { return; }
  if (!ss->enabled) { return; }
  if (ss->selectedGem) { dropSelectedGem(ss); }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {

  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  if (!MGMS()->isLive() ||
      !ss->enabled ) {
  return; }

  //track to see if we have a valid target
  if (ss->selectedGem != CC_NIL && this->_touchDown ) {
    CC_POS2(ss->selectedGem,
      tap.x - ss->gemsContainer->getPositionX(),
      tap.y - ss->gemsContainer->getPositionY());
    auto touched= findGemAtPos(ss, tap);
    if (touched.gem != CC_NIL &&
        isValidTarget(ss, touched.x, touched.y, tap)) {
      selectTargetGem(ss,touched);
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared=_engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  ss->gemsContainer = c::Node::create();
  //ss->selectedGemPos = CC_ZPT;
  //ss->selectedIndex = f::Cell2I();
  //ss->targetIndex = f::Cell2I();
  //ss->selectedGem = CC_NIL;
  //ss->targetGem = CC_NIL;
  //ss->combos = 0;
  //ss->addingCombos = false;

  CC_POS2(ss->gemsContainer, 25, 80);
  addItem(ss->gemsContainer);

  auto frame = cx::createSprite("frame");
  auto sz= CC_CSIZE(frame);
  frame->setScale(wz.width/sz.width);
  CC_POS2(frame, wb.cx, wb.cy);
  addItem(frame);

  buildGrid(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {
  _engine= mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");
  regoAtlas("cc-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::buildGrid(GVars *ss) {
  auto TILE_GRID = TILE_SIZE + GRID_SPACE;
  ss->enabled = false;
  for (auto c = 1; c <= GRID_SIZE_X; ++c) {
    auto m= new f::FArrayPtr<Gem>(GRID_SIZE_Y);
    auto g= mc_new1(f::FArrInt,GRID_SIZE_Y);

    ss->gemsColMap.push_back(m);
    ss->grid.push_back(g);

    for (auto r = 1; r <= GRID_SIZE_Y; ++r) {
      auto gem = Gem::create();
      auto idx= c<3
        ? getVertUnique(ss,c-1,r-1)
        : getVertHorzUnique(ss,c-1,r-1);
      auto t= getGemType(idx);

      gem->setType(t);
      g->set(r-1,t); // zero based
      gem->inflate(c*TILE_GRID, r*TILE_GRID);

      ss->gemsContainer->addChild(gem);
      m->set(r-1,gem); // zero based
      ss->allGems.push_back(gem);
    }
  }

  animateIntro(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::addToScore() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  F__LOOP(it, ss->matchArray) {
    auto &pos= *it;
    auto gem = ss->gemsColMap[pos.x]->get(pos.y);
    if (gem->getType() == TYPE_GEM_WHITE) {
        //got a diamond
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startTimer() {

  auto timeBarBg = cx::reifySprite("timeBarBg.png");
  auto wb = cx::visBox();

  CC_POS2(timeBarBg, wb.cx, 40);
  addItem(timeBarBg);

  _timeBar = cx::reifySprite("timeBar.png");
  _timeBar->setAnchorPoint(cx::anchorL());
  CC_POS2(_timeBar, wb.cx - 290, 40);
  addItem(_timeBar);

  this->schedule(CC_SCHEDULE_SELECTOR(GLayer::tick), 1);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::tick(float dt) {
  auto scaleNow = _timeBar->getScaleX();
  auto speed = 0.007;
  auto wb= cx::visBox();

  if (scaleNow - speed > 0) {
    _timeBar->setScaleX(scaleNow - speed);
  } else {
    this->unschedule(CC_SCHEDULE_SELECTOR(GLayer::tick));
      //GameOver!!!
    _timeBar->setScaleX(0);
    cx::pauseAudio();
    MGMS()->stop();
    // show game over
    auto gameOver = cx::reifySprite("gameOver.png");
    CC_POS2(gameOver, wb.cx, wb.cy);
    addItem(gameOver);
  }
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if ("/game/start/timer" == topic) {
    y->startTimer();
  }
  else
  if ("/game/player/addscore" == topic) {
    y->addToScore();
  }
  else
  if ("/game/player/earnscore" == topic) {
    auto msg= ( j::json*)m;
    y->getHUD()->updateScore(
        JS_STR(msg->operator[]("type")),
        JS_INT(msg->operator[]("score")));
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}




NS_END


