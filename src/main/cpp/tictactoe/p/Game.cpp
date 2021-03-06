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
#include "MMenu.h"
#include "Ende.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
BEGIN_NS_UNAMED
class CC_DLL GLayer : public f::GameLayer {

  virtual void onMouseClick(const CCT_PT&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  void onGUIXXX(const CCT_PT&);
  const f::Box4 showGrid();

public:

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _arena)
  __decl_ptr(ecs::Node, _board)

  void onEnd(int winner);
  void playTimeExpired();
  void showMenu();

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onInited();

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _arena = _engine->getNodes("n/CSquares")[0];
  _board = _engine->getNodes("n/Grid")[0];

  auto ps = CC_GEC(Players,_board,"n/Players");
  auto ss = CC_GEC(GVars,_arena,"n/GVars");
  auto human= CC_CSV(c::Integer,"HUMAN");
  // random start?
  auto pnum= cx::randSign() > 0 ? 2 : 1;
  auto box=showGrid();

  getHUD()->setGridBox(box);

  if (MGMS()->isOnline()) {
    ss->pnum= 0;
    return;
  } else {
    ss->pnum= pnum;
  }

  if (ps->parr[pnum]->category == human) {
    SENDMSG("/hud/timer/show");
  }

  auto msg= j::json({
      {"category", ps->parr[pnum]->category},
      {"running", true },
      {"pnum", pnum}
      });
  SENDMSGEX("/hud/update",&msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::playTimeExpired() {
  MGMS()->msgQueue().push("forfeit");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd(int winner) {
  getHUD()->setOpacity(0.1 * 255);
  getHUD()->endGame(winner);
  this->setOpacity(0.1 *255);
  MGMS()->stop();
  surcease();
  Ende::reify(getSceneX(),
    mc_new1(ECX, _arena),
    4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= [=]() { cx::pop(); };
  auto m= MMenu::reify(mc_new1(MCX,f));
  cx::pushEx(m);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *t) {
  onGUIXXX(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {
  onGUIXXX(loc);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUIXXX(const CCT_PT &pos) {

  auto css= CC_GEC(CSquares, _arena,"n/CSquares");
  auto sel= CC_GEC(CellPos, _board, "n/CellPos");
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto cur = ss->pnum;

  sel->cell =  -1;
  sel->px= pos.x;
  sel->py= pos.y;

  if (cur <=  0) {
    CCLOG("onGUIXXX: OOOPS, cur ============== %d", cur);
    return;
  }

  //which cell did he click on?
  auto n=0;
  F__LOOP(it, css->boxes) {
    auto &bx = *it;
    if (sel->px >= bx.left && sel->px <= bx.right &&
        sel->py >= bx.bottom && sel->py <= bx.top) {
      sel->cell= n;
      break;
    }
    ++n;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  f::emptyQueue<sstr>( MGMS()->msgQueue() );

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

  auto ctx = (GCXX*) getSceneX()->getCtx();
  auto pnum= JS_INT(ctx->_data["pnum"]);
  auto ppids= ctx->_data["ppids"];
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  sstr p1k;
  sstr p2k;
  sstr p1n;
  sstr p2n;

  CCLOG("seed =\n%s", ctx->_data.dump(2).c_str());
  _engine = mc_new1(GEngine,pnum);

  J__LOOP(it, ppids) {
    auto &arr= it.value();
    if (JS_INT(arr[0]) == 1) {
      p1n= JS_STR(arr[1]);
      p1k= it.key();
    } else {
      p2n= JS_STR(arr[1]);
      p2k= it.key();
    }
  }

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 GLayer::showGrid() {
  auto css= CC_GEC(CSquares, _arena, "n/CSquares");
  auto gps= mapGridPos(1);
  auto &b0= gps[0];
  auto &bN= gps[gps.size()-1];

  for (auto i=0; i < gps.size(); ++i) {
    auto s= css->sqs[i];
    auto &bx= gps[i];
    assert(s->cell == i);
    s->setPosition(cx::vboxMID(bx));
    addAtlasItem("game-pics", s);
  }

  return f::Box4(b0.top,bN.right,bN.bottom,b0.left);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
  else
  if ("/hud/timer/show" == topic) {
    y->getHUD()->showTimer();
  }
  else
  if ("/net/restart" == topic) {
    y->getHUD()->killTimer();
  }
  else
  if ("/net/stop" == topic) {
    auto msg= (j::json*) m;
    y->onEnd(
        JS_BOOL(msg->operator[]("status")));
  }
  else
  if ("/hud/timer/hide" == topic) {
    y->getHUD()->killTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto msg = (j::json*) m;
    y->getHUD()->updateScore(
        JS_INT(msg->operator[]("pnum")),
        JS_INT(msg->operator[]("score")));
  }
  else
  if ("/hud/end" == topic) {
    auto msg = (j::json*) m;
    y->onEnd(
        JS_INT(msg->operator[]("winner")));
  }
  else
  if ("/hud/update" == topic) {
    auto msg= (j::json*) m;
      y->getHUD()->draw(
        JS_BOOL(msg->operator[]("running")),
        JS_INT(msg->operator[]("category")),
        JS_INT(msg->operator[]("pnum")));
  }
  else
  if ("/player/timer/expired" == topic) {
    y->playTimeExpired();
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




