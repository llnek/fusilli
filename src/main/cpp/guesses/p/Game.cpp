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
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(guesses)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void checkTiles();

  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
static const s_arr<int,16> shuffle() {
  s_arr<int,16> src = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};
  S__MIX(src);
  S__MIX(src);
  return src;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto cells= shuffle();
  auto i=0;
  auto sz= cx::calcSize("cover.png");
  auto gap= sz.height * 2.0/64.0;
  auto ht= sz.height * 4 + gap * 3;
  auto wd= sz.width * 4 + gap * 3;

  auto x0= wb.cx - HTV(wd);
  auto y= wb.cy + HTV(ht);
  auto x=x0;

  for (auto r=0; r < 4; ++r) {
    for (auto c=0; c < 4; ++c) {
      auto n= cells[r*4 + c ];
      auto tile = MemoryTile::create(n);
      tile->setAnchorPoint(cx::anchorTL());
      addAtlasItem("game-pics",tile);
      CC_POS2(tile, x, y);
      ss->tiles.push_back(tile);
      x += sz.width + gap;
    }
    y -= (sz.height + gap);
    x= x0;
  }

/*
  F__LOOP(it, cells) {
    auto &n= *it;
    auto tile = MemoryTile::create(n);
    addAtlasItem("game-pics",tile);
    CC_POS2(tile, 49+i%4*74, 400-floor(i/4)*74);
    ss->tiles.push_back(tile);
    ++i;
  }
*/

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  MemoryTile *picked=CC_NIL;

  F__LOOP(it,ss->tiles) {
    auto &t= *it;
    if (!t->inflated &&
        t->getBoundingBox().containsPoint(tap)) {
      picked= t;
      break;
    }
  }

  if (ss->pickedTiles.size() < 2 &&
      picked != CC_NIL) {

    auto it= s::find(ss->pickedTiles.begin(),
                     ss->pickedTiles.end(), picked);
    //not found
    if (it == ss->pickedTiles.end()) {
      picked->setSpriteFrame("tile_"+ FTOS(picked->pvalue)+".png");
      ss->pickedTiles.push_back(picked);
      if (ss->pickedTiles.size()==2) {
        checkTiles();
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkTiles() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  ss->checkTimer= cx::reifyTimer(this,2000);
  ss->moves += 1;
  getHUD()->updateMoves(ss->moves);

}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto bg = c::LayerGradient::create(
      c::Color4B(0,0,0,255),
      c::Color4B(0x46,0x82,0xB4,255));

  addItem(bg,-1);
  regoAtlas("game-pics");

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




