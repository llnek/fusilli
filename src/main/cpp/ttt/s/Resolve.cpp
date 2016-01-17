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
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  arena= engine->getNodeList(ArenaNode().typeId());
  board= engine->getNodeList(BoardNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    sync();
    doIt(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::sync() {
  auto css= CC_GNLF(CSquares, arena, "squares");
  auto grid= CC_GNLF(Grid, board, "grid");
  auto nil= CC_CSV(c::Integer, "CV_Z");

  for (int i=0; i < grid->vals.size(); ++i) {
    auto v= grid->vals[i];
    if (v != nil) {
      auto sq= css->sqs[i];
      if (sq->value == nil) {
        sq->toggle(v);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::doIt(float dt) {

  auto ps = CC_GNLF(Players, board, "players");
  auto gd = CC_GNLF(Grid, board, "grid");
  auto winner= -1;
  ArrDim combo;

  for (int i=1; i < ps->parr.size(); ++i) {
    if (checkWin(ps->parr[i], gd, combo)) {
      winner=i;
      break;
    }
  }

  if (winner > 0) {
    doWin(ps->parr[winner], combo);
  }
  else
  if (checkDraw(gd)) {
    doDraw();
  }
  else {
    auto &q= MGMS()->msgQueue();
    if (q.size() > 0) {
      auto s= q.front();
      q.pop();
      if ("forfeit" == s) {
        doForfeit();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doWin(Player *winner, const ArrDim &combo) {

  auto msg= j::json({
    {"pnum", winner->pnum },
    {"score", 1}
  });

  SENDMSGEX("/hud/score/update", &msg);
  showWinningIcons(combo);
  doDone(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doDraw() {
  auto ps= CC_GNLF(Players, board, "players");
  Player dummy;
  doDone(&dummy);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doForfeit() {
  auto css= CC_GNLF(CSquares, arena, "squares");
  auto ps= CC_GNLF(Players, board, "players");
  auto ss= CC_GNLF(GVars,arena,"slots");
  auto cur=ss->pnum;
  auto other= cur == 1 ? 2 : cur == 2 ? 1 : 0;

  auto loser = ps->parr[cur];
  auto win= ps->parr[other];

  auto msg = j::json({
    {"pnum", win->pnum },
    {"score", 1}
  });

  SENDMSGEX("/hud/score/update", &msg);

  //gray out the losing icons
  F__LOOP(it, css->sqs) {
    auto z = *it;
    if (z->value == loser->value) {
      z->flip();
    }
  }

  doDone(win);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::showWinningIcons(const ArrDim &combo) {
  auto css= CC_GNLF(CSquares, arena, "squares");
  auto nil = CC_CSV(c::Integer, "CV_Z");

  //flip the losing cells to gray
  for (int i=0; i < css->sqs.size(); ++i) {
    if (! (s::find(s::begin(combo), s::end(combo), i)
          != combo.end())) {
      auto z= css->sqs[i];
      if (z->value != nil) {
        z->flip();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doDone(Player *pobj) {

  auto ss= CC_GNLF(GVars,arena,"slots");
  auto msg= j::json({
    {"winner", pobj->pnum  }
  });

  ss->lastWinner= pobj->pnum;

  SENDMSG("/hud/timer/hide");
  SENDMSGEX("/hud/end", &msg);

  cx::sfxPlay("game_end");
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkDraw(Grid *gd) {

  auto nil= CC_CSV(c::Integer, "CV_Z");

  return ! (s::find(s::begin(gd->vals), s::end(gd->vals), nil)
    != gd->vals.end());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkWin(Player *p, Grid *game, ArrDim &combo) {

  CCLOG("checking win for %s", p->color.c_str());

  F__LOOP(it, game->GOALS) {

    auto &g = *it;
    int cnt=0;

    for (int i=0; i < g.size(); ++i) {
      auto pos = g[i];
      if (game->vals[pos] == p->value) {
        ++cnt;
      }
    }
    if (cnt == g.size()) {
       // found a winning combo, this guy wins!
      S__COPY(g, combo);
      return true;
    }
  }
  return false;
}



NS_END(tttoe)

