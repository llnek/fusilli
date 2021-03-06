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

#include "x2d/GameLayer.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "MMenu.h"
#include "HUD.h"
#include "Ende.h"
#include "Game.h"
#include "s/GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  virtual bool onMouseStart(const CCT_PT&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  void onGUI(const CCT_PT&);
  void showMenu();
  void onEnd();
  void deco();

  // ----

  void initBlockMap(GVars*, BlockGrid*, const CCT_SZ&);
  void doPreview(GVars*);
  void doCtrl(GVars*);

  __decl_ptr(ecs::Node, _arena)
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::doPreview(GVars *ss) {
  auto tile = CC_CSV(c::Float, "TILE");
  auto wb= cx::visBox();
  auto xc= ss->cbox.right + HTV(wb.right - ss->cbox.right);
  auto yc= wb.top - tile * 5;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::doCtrl(GVars *ss) {
  auto cpad= CC_GEC(CtrlPad, _arena, "n/CtrlPad");
  auto sp= cx::reifySprite("shadedDark09.png");
  //sp= cx::reifySprite("shadedLight09.png");
  auto& hsps= cpad->hotspots;
  auto wb= cx::visBox();
  auto cz= CC_CSZ(sp);
  // view the pad as a 3x3 grid
  auto ch3= cz.height / 3.0;
  auto cw3= cz.width / 3.0;
  //x= cw.x + (wb.right - cw.x) * 0.5,
  //auto x= wb.right * 0.75;
  auto y= wb.top * 0.25;
    auto x= ss->cbox.right + HTV(wb.right - ss->cbox.right);

  CC_POS2(sp,x,y);
  addAtlasItem("game-pics", sp);

  auto cbx= cx::bbox4(sp);
  //calc hotspots for touch & mouse
  // rotate left right
  hsps["rr"] = f::Box4(
      cbx.top,
      cbx.right - cw3,
      cbx.top - ch3,
      cbx.left + cw3);

  hsps["rl"] = f::Box4(
      cbx.top- 2* ch3,
      cbx.right - cw3,
      cbx.bottom,
      cbx.left + cw3);

  // shifting left, right
  hsps["sl"] = f::Box4(
      cbx.top - ch3,
      cbx.left + cw3,
      cbx.top - 2 * ch3,
      cbx.left );

  hsps["sr"] = f::Box4(
      cbx.top - ch3,
      cbx.right,
      cbx.top - 2 * ch3,
      cbx.left + 2 * cw3);

  // fast drop down
  hsps["cd"] = f::Box4(
      cbx.top - ch3,
      cbx.right - cw3,
      cbx.top - 2 * ch3,
      cbx.left + cw3 );

  cpad->bbox=sp->getBoundingBox();
}

//////////////////////////////////////////////////////////////////////////
// Create our own collision map using cells.
void GLayer::initBlockMap(GVars *ss, BlockGrid *bks, const CCT_SZ &bz) {

  auto wlen= CC_CSV(c::Integer, "FIELD_W") + 2; // 2 side walls
  auto wz= MGMS()->getEnclosureRect();
  auto wb= MGMS()->getEnclosureBox();
  auto hlen = (int) floor(wz.size.height / bz.height);
  auto png= "gray.png";
  auto last= wlen-1;
  float x,y;

  ++hlen; // add one more to the top
  for (auto r = 1; r <= hlen; ++r) {
    FArrBrick rc(wlen);
    rc.fill(CC_NIL);
    auto y= wb.bottom + r * bz.height;
    auto x= wb.left;
    auto b= Brick::reify(CCT_PT(x,y), png);
    MGML()->addAtlasItem("game-pics", b->node);
    rc.setFirst(b);
    x += bz.width * (wlen-1);
    b= Brick::reify(CCT_PT(x,y), png);
    MGML()->addAtlasItem("game-pics", b->node);
    rc.setLast(b);
/* test to fill up entire box
    y= wb.bottom + r * bz.height;
    x= wb.left;
    for (int i=1; i < (wlen-1); ++i) {
      x += bz.width;
      b= Brick::reify( CCT_PT(x,y), "0.png");
      MGML()->addAtlasItem("game-pics", b->sprite);
      rc.set(i,b);
    }
*/
    bks->grid.push_back(rc);
  }

  //pop off the extra row count
  --hlen;

  // fill in first row == row[0]
  y = wb.bottom + bz.height;
  auto &rc=bks->grid[0];
  for (auto c = 1; c < last; ++c) {
    auto b= Brick::reify(
        CCT_PT(wb.left + c * bz.width, y), png);
    MGML()->addAtlasItem("game-pics", b->node);
    rc.set(c, b);
  }

  CCLOG("grid height = %d x tiles", hlen);

  ss->cbox= f::Box4(
    wb.bottom + hlen * bz.height,
    wb.left + wlen * bz.width,
    wb.bottom,
    wb.left);
  ss->hTiles= hlen;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *t) {
  return onMouseStart(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &pos) {
  auto motion= CC_GEC(Gesture, _arena, "f/CGesture");
  auto cpad= CC_GEC(CtrlPad, _arena, "n/CtrlPad");
  auto &hsps= cpad->hotspots;

  if (!cpad->bbox.containsPoint(pos)) { return false; }

  if (cx::pointInBox(hsps["rr"], pos)) {
    motion->rotr=true;
  }

  if (cx::pointInBox(hsps["rl"], pos)) {
    motion->rotl=true;
  }

  if (cx::pointInBox(hsps["sr"], pos)) {
    motion->right=true;
  }

  if (cx::pointInBox(hsps["sl"], pos)) {
    motion->left=true;
  }

  if (cx::pointInBox(hsps["cd"], pos)) {
    motion->down=true;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _arena= _engine->getNodes("n/BlockGrid")[0];

  auto blocks= CC_GEC(BlockGrid, _arena, "n/BlockGrid");
  auto fld_w = CC_CSV(c::Integer, "FIELD_W");
  auto ss=CC_GEC(GVars,_arena,"n/GVars");
  auto bz= cx::calcSize("gray.png");

  XCFG()->resetCst("TILE", CC_FLOAT( bz.width));
  initBlockMap(ss, blocks, bz);

  CCLOG("brick: w= %d, h= %d", (int)bz.width, (int)bz.height);
  CCLOG("tile size = %f", CC_CSV(c::Float,"TILE"));
  CCLOG("gridbox: t=%d, r=%d, b=%d, l=%d",
      (int)ss->cbox.top,(int)ss->cbox.right,
      (int)ss->cbox.bottom,(int)ss->cbox.left);
  CCLOG("collision tiles and blocks init'ed");

  doPreview(ss);
  doCtrl(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  this->_engine = mc_new(GEngine);

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= []() { cx::pop(); };
  auto m= MMenu::reify(mc_new1(MCX,f));
  cx::pushEx(m);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  getHUD()->endGame();
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {
  auto y = (GLayer*) getLayer(2);

  if ("/hud/score/update" == topic) {
    auto json= (j::json*) msg;
    auto v= JS_INT(json->operator[]("score"));
    y->getHUD()->updateScore(v);
  }

  if ("/hud/end" == topic) {
    y->onEnd();
  }

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END



