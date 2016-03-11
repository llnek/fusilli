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
//

#include "platform/CCCommon.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLives : public c::Node {
protected:

  DECL_TD(c::Size, _lifeSize)
  DECL_TD(c::Vec2, _refPt)
  DECL_TD(sstr, _frameId)

  s_vec<c::Node*> _icons;

  DECL_IZ(_totalLives)
  DECL_IZ(_curLives)
  DECL_TV(int, _dir, 1)
  DECL_TV(float, _scale, 1.0f)

public:

  virtual void decorate( const sstr &frame, int lives,
      float x, float y,
      float scale=1.0f, int dir= 1);

  bool isDead() { return _curLives < 0; }
  int getLives() { return _curLives; }

  void reduce(int cnt = 1);
  void reset();
  void resurrect();
  void drawLives();

  virtual ~XLives() {}
  XLives() {}
  NOCPYASS(XLives)
};



NS_END


