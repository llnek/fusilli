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

#if !defined(__COMOBJ_H__)
#define __COMOBJ_H__

#include "2d/CCSprite.h"
#include "Primitives.h"
#include "ash/Ash.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ComObj : public ash::Component {
protected:

  void init(c::Sprite*, int health, int score);

  NOCPYASS(ComObj)

public:

  virtual const a::COMType typeId() { return "fusii::ComObj"; }

  virtual void inflate(not_null<c::Dictionary*> options);
  virtual void inflate(float x, float y);
  virtual void inflate();
  virtual void deflate();
  virtual void hurt(int damage=1);

  void setPos(float x, float y);
  const c::Size csize();
  const c::Vec2 pos();

  float height();
  int pid();
  float width();

  virtual void updatePosition(float x, float y);

  ComObj(not_null<c::Sprite*>, int health, int score);
  ComObj(not_null<c::Sprite*>);
  virtual ~ComObj() {}
  ComObj();

  c::Sprite* sprite=nullptr;
  int origHealth=0;
  bool status=false;
  int health=0;
  int score=0;

  c::Vec2 lastPos;
  c::Vec2 vel;

};



NS_END(fusii)
#endif

