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

#if !defined(__XLIVES_H__)
#define __XLIVES_H__

#include "platform/CCCommon.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLives : public c::Node {
protected:

  s::vector<c::Node*> icons;
  c::Size lifeSize;
  c::Vec2 refPt;
  stdstr frameId;

  int totalLives;
  int curLives;
  int dir;
  float scale;

  NO__CPYASS(XLives)

public:

  virtual void realize( const stdstr& frame, int lives, float x, float y, float scale=1.0f, int dir= 1);

  bool isDead() { return curLives < 0; }
  int getLives() { return curLives; }

  void reduce(int cnt = 1);
  void reset();
  void resurrect();
  void drawLives();

  DECL_CTOR(XLives)
};



NS_END(fusii)
#endif

