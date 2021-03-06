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
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto soff= CC_CSV(c::Float, "S_OFF");
  auto tile= CC_CSV(c::Float, "TILE");
  auto tc= XCFG()->getColor("text");
  auto c= XCFG()->getColor("dft");
  auto wb= cx::visBox();

  regoAtlas("game-pics");

  //score1
  _score1= cx::reifyBmfLabel("dft", "0");
  _score1->setColor(c);
  _score1->setAnchorPoint(cx::anchorTL());
  XCFG()->scaleBmfont(_score1, 32);
  _score1->setPosition(tile+soff+2, wb.top-tile-soff);
  addItem(_score1);

  //score2
  _score2= cx::reifyBmfLabel("dft", "0");
  _score2->setColor(c);
  _score2->setAnchorPoint(cx::anchorTR());
  XCFG()->scaleBmfont(_score2, 32);
  _score2->setPosition(wb.right-tile-soff, wb.top-tile-soff);
  addItem(_score2);

  //title
  _title = cx::reifyBmfLabel("dft");
  _title->setAnchorPoint(cx::anchorT());
  _title->setColor(c);
  XCFG()->scaleBmfont(_title,24);
  _title->setPosition(wb.cx, wb.top - 2*tile);
  addItem(_title);

  //menu icon
  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = HTV(cx::getHeight(b));
  auto hw = HTV(cx::getWidth(b));
  b->setColor(c);
  b->setCallback(
      [=](c::Ref*) { SENDMSG("/hud/showmenu"); });
  auto menu = cx::mkMenu(b);
  menu->setPosition(wb.right-tile-hw, wb.bottom+tile+hh);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::setGridBox(const f::Box4 &grid) {

  auto tc= XCFG()->getColor("text");
  auto c= XCFG()->getColor("dft");
  auto wb= cx::visBox();
  auto pos= CCT_PT(wb.cx,
      grid.bottom - (grid.bottom-wb.bottom) * 1/4);

  // status
  _status= cx::reifyLabel("text", 20,
      "");
  _status->setColor(tc);
  _status->setPosition(pos);
  //_status->setPosition(wb.cx, wb.bottom + tile * 10);
  addItem(_status);

  // result
  _result= cx::reifyLabel("text", 20,
      "");
  _result->setColor(tc);
  _result->setPosition(pos);
  //_result->setPosition(wb.cx, wb.bottom + tile * 10);
  CC_HIDE(_result);
  addItem(_result);

  _countDown= cx::reifyBmfLabel("timer");
  //_countDown->setPosition(wb.cx, wb.top - 10*tile);
  _countDown->setPosition(
      wb.cx, wb.top - (wb.top-grid.top) * 3/4);
  _countDown->setAnchorPoint(cx::anchorC());
  XCFG()->scaleBmfont(_countDown, 24);
  _countDown->setColor(c);//XCFG()->getColor("text"));
  //CC_HIDE(_countDown);
  addItem(_countDown);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showTimer() {
  auto cfg= MGMS()->getLCfg()->getValue();
  auto ptt= JS_INT(cfg["HUMAN+THINK"]);
  auto tile= CC_CSV(c::Float,"TILE");
  //auto scale= XCFG()->getScale();
  auto c= XCFG()->getColor("dft");
  auto wb= cx::visBox();

  // timer is already showing, go away
  if (_countDownState) {
  return; }

  //CC_SHOW(_countDown);
  _countDownState= true;
  _countDownValue= ptt;

  showCountDown(FTOS(ptt));
  schedule(
      CC_SCHEDULE_SELECTOR(HUDLayer::updateTimer), 1);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateTimer(float dt) {
  if (!_countDownState) { return; } else {
    _countDownValue -= 1;
  }
  if (_countDownValue < 0) {
    killTimer();
    SENDMSG("/player/timer/expired");
  } else {
    showCountDown(FTOS(_countDownValue));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showCountDown(const sstr &msg) {
  if (_countDown) {
    _countDown->setString(msg);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::killTimer() {
  if (_countDownState) {
    unschedule(CC_SCHEDULE_SELECTOR(HUDLayer::updateTimer));
    showCountDown(" ");
  }
  _countDownState=false;
  _countDownValue=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int pnum, int value) {
  assert(pnum > 0 && pnum < _scores.size());
  _scores[pnum] += value;
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::draw(bool running, int category, int pnum) {
  if (running) {
    drawStatus(category, pnum);
  } else {
    drawResult(pnum);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame(int winner) {
  CC_HIDE(_status);
  CC_SHOW(_result);
  killTimer();
  drawResult(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawXXXText(c::Label *obj, const sstr &msg) {
  obj->setString(msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScores() {
  auto s2 = _scores[2];
  auto s1 = _scores[1];
  auto n2 = FTOS(s2);
  auto n1 = FTOS(s1);

  _score1->setString(n1);
  _score2->setString(n2);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawResult(int pnum) {
  auto msg = gets("whodraw");

  switch (pnum) {
    case 2:
      msg= gets("whowin", s_vec<sstr> { this->_p2Long});
      break;
    case 1:
      msg= gets("whowin", s_vec<sstr> { this->_p1Long});
      break;
  }

  drawXXXText(_result, msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawStatus(int category, int pnum) {
  if (pnum > 0) {
    auto pfx = pnum == 1 ? _p1Long : _p2Long;
    auto bot = CC_CSV(c::Integer, "BOT");
    sstr msg;
    if (category == bot) {
      msg= gets("whosthink", s_vec<sstr> { pfx });
    } else {
      msg= gets("whosturn", s_vec<sstr> { pfx });
    }
    drawXXXText(_status, msg);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::regoPlayers(const sstr &color1,
    const sstr &p1k, const sstr &p1n,
    const sstr &color2, const sstr &p2k, const sstr &p2n) {

  _title->setString(p1k + " / " + p2k);
  _scores= {0,0,0};
  _play2= color2;
  _play1= color1;
  _p2Long= p2n;
  _p1Long= p1n;
  _p2ID= p2k;
  _p1ID= p1k;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::resetAsNew() {
  reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::reset() {
  CC_HIDE(_result);
  CC_SHOW(_status);
  _scores = {0,0,0};
}



NS_END



