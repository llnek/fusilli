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
#include "C.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// Calculate position of each individual cells in the grid,
// so that we can detect when a user clicks on the cell
const s_arr<f::Box4,GD_SZ> mapGridPos(float scale) {
  // memorize the co-ordinates of each cell on the board, so
  // we know which cell the user has clicked on
  auto csz = cx::scaleSize(cx::calcSize("z.png"), scale);
  auto ro= 8.0/72.0 * scale;
  auto gh = csz.height * ro;
  auto gw = csz.width * ro;
  auto zh= csz.height * BD_SZ + gh * (BD_SZ-1);
  auto zw= csz.width * BD_SZ + gw * (BD_SZ-1);

  //f::Box4 sq(y0,x0 + zw,y0-zh, x0);
  s_arr<f::Box4, GD_SZ> boxes;
  auto wb = cx::visBox();
  auto x0 = wb.cx - HTV(zw);
  auto y0 = wb.cy + HTV(zh);
  auto x1= x0;
  auto y1= y0;
  float x2, y2;

  for (auto r=0; r < BD_SZ; ++r) {
    for (auto c= 0; c < BD_SZ; ++c) {
      auto &bx= boxes[r*BD_SZ+c];
      y2 = y1 - csz.height;
      x2 = x1 + csz.width;
      bx.top= y1;
      bx.right= x2;
      bx.bottom= y2;
      bx.left= x1;
      x1= x2 + gw;
    }
    y1 = y2 - gh;
    x1 = x0;
  }

  return boxes;
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<ArrDim> mapGoalSpace() {

  s_vec<ArrDim> goals;
  ArrDim dx;
  ArrDim dy;

  for (auto r=0; r < BD_SZ; ++r) {

    ArrDim h;
    ArrDim v;

    for (auto c=0; c < BD_SZ; ++c) {
      h[c] = r * BD_SZ + c;
      v[c] = c * BD_SZ + r;
    }

    goals.push_back(h);
    goals.push_back(v);

    dx[r] = r * BD_SZ + r;
    dy[r] = (BD_SZ - r - 1) * BD_SZ + r;
  }

  goals.push_back(dx);
  goals.push_back(dy);

  return goals;
}

//////////////////////////////////////////////////////////////////////////////
//
j::json fmtGameData(f::GMode m) {

  if (m == f::GMode::TWO) {
    return j::json({
        {"ppids", j::json::object_t {
          { XCFG()->getL10NStr("p1"), j::json::array_t {
            1, XCFG()->getL10NStr("player1") }
          },
          { XCFG()->getL10NStr("p2"), j::json::array_t {
            2, XCFG()->getL10NStr("player2") }
          },
      } },
      {"pnum", 1 }
    });
  }

  if (m == f::GMode::ONE) {
    return j::json({
        {"ppids", j::json:: object_t {
          { XCFG()->getL10NStr("cpu"), j::json::array_t {
            2, XCFG()->getL10NStr("computer") }
          },
          { XCFG()->getL10NStr("p1"), j::json::array_t {
            1, XCFG()->getL10NStr("player1") }
          },
      } },
      {"pnum", 1 }
    });
  }

  if (m == f::GMode::NET) {
    return j::json({
        {"ppids", j::json::object_t { } },
      {"pnum", 0 }
    });
  }

  throw "bad game mode!";
}



NS_END


