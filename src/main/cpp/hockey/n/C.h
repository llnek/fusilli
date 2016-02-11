// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Widget : public f::ComObj {
  virtual void setPos(float x, float y) {
    auto pos= c::Vec2(x,y);
    if (!nextPos.equals(pos)) {
      nextPos = pos;
    }
    f::ComObj::setPos(x,y);
  }
  void syncPos() {
    f::ComObj::setPos(nextPos.x,nextPos.y);
  }

  Widget(not_null<c::Sprite*> s)
  : ComObj(s) {}

  DECL_TD(c::Vec2,nextPos)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Mallet : public Widget {
  Mallet(not_null<c::Sprite*> s, int pnum)
    : Widget(s) {
    this->pnum=pnum;
  }
  DECL_PTR(c::Touch, tap)
  DECL_IZ(pnum)
  MDECL_COMP_TPID("n/Mallet")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Puck : public Widget {
  Puck(not_null<c::Sprite*> s)
    : Widget(s) {
  }
  MDECL_COMP_TPID("n/Puck")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public a::Component {
  MDECL_COMP_TPID("n/Player")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public a::Component {
  MDECL_COMP_TPID("n/Players")
  s_arr<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
  DECL_BF(right)
  DECL_BF(left)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  MDECL_COMP_TPID( "n/GVars" )
  DECL_FZ(goalWidth)
  DECL_FZ(sq_radii)
  DECL_TD(c::Vec2,ballNextPos)
  DECL_TD(c::Vec2,ballVec)
};



NS_END



