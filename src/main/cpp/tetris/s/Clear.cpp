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
#include "core/CCSX.h"
#include "n/lib.h"
#include "Clear.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void Clear::preamble() {
  arena = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Clear::update(float dt) {

  if (MGMS()->isLive()) {
    auto ps= CC_GNLF(Pauser, arena, "pauser");
    if (ps->pauseToClear) {
      if (cx::timerDone(ps->timer)) {
        cx::undoTimer(ps->timer);
        ps->timer=nullptr;
        clearFilled();
        ps->pauseToClear=false;
      }
      return false;
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearFilled() {
  auto flines = CC_GNLF(FilledLines, arena, "flines");
  auto score= flines->lines.size();

  F__LOOP(it, flines->lines) {
    auto z = *it;
    clearOneRow( z);
    resetOneRow( z);
  }

  auto msg= j::json({
      { "score", score * 50 }
      });
  shiftDownLines();
  SENDMSGEX("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearOneRow( int r) {
  auto bks= CC_GNLF(BlockGrid, arena, "blocks");
  auto &row= bks->grid[r];
  auto end = row.size() -1;

  // skip the 2 side walls
  for (auto c=1; c < end; ++c) {
    auto z= row.get(c);
    if (NNP(z)) {
      z->dispose();
    }
    delete z;
    row.set(c, nullptr);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::resetOneRow( int r) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::shiftDownLines() {
  auto top= topLine(arena->head);
  // top down search
  auto f0= findFirstDirty();
  // no lines are touched
  if (f0==0) { return; }
  auto e0= findFirstEmpty(f0);
  if (e0==0) { return; }
  auto e= findLastEmpty(e0);
  auto f = e0 + 1; // last dirty
  if (f > f0) {
    throw "error while shifting lines down";
  }
  auto r= (e0 - e) + 1;  // number of empty lines
  auto s=f;  // last dirty
  auto t=e;
  while (s <= f0) {
    copyLine(s, t);
    ++t;
    ++s;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findFirstDirty() {
  auto t = topLine(arena->head);// - 1,

  for (auto r = t; r > 0; --r) {
    if (!isEmptyRow(r)) { return r; }
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findFirstEmpty( int t) {
  for (auto r=t; r > 0; --r) {
    if (isEmptyRow(r)) { return r; }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findLastEmpty( int t) {
  for (auto r=t; r >= 0; --r) {
    if (!isEmptyRow(r)) { return r+1; }
  }
  //should never get here
  throw "findLastEmpty has error";
  //return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Clear::isEmptyRow( int r) {
  auto co= CC_GNLF(BlockGrid, arena, "blocks");
  auto &row= co->grid[r];
  auto len= row.size() - 1;

  if (r==0) { return false; }

  for (auto c=1; c < len; ++c) {
    if (row.get(c) != nullptr) { return false; }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::copyLine( int from, int to) {
  auto bs = CC_GNLF(BlockGrid, arena, "blocks");
  auto tile = CC_CSV(c::Integer, "TILE");
  auto dlen= tile * (from - to);
  //TODO : fix dlen

  auto &g_f = bs->grid[from];
  auto &g_t = bs->grid[to];
  auto end= g_f.size() -1;

  // skip the 2 side walls
  for (auto c=1; c < end; ++c) {
    auto b= g_f.get(c);
    if (b) {
      auto pos = b->getPosition();
      b->setPosition(pos.x, pos.y - dlen);
    }
    auto old= g_t.swap(c, b);
    if (old) {
      old->dispose();
      delete old;
    }
    g_f.set(c, nullptr);
  }
}


NS_END(tetris)


