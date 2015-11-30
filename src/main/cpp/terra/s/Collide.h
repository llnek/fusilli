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

#if !defined(__COLLIDE_H__)
#define __COLLIDE_H__

NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Collide : public f::BaseSystem<EFactory> {
protected:

  bool collide(f::ComObj*, f::ComObj*);
  virtual bool onUpdate(float);
  void checkMissilesBombs();
  void checkMissilesAliens();
  void checkShipBombs(a::Node*);
  void checkShipAliens(a::Node*);

  NO__CPYASS(Collide)
  Collide() = delete;

public:

  virtual const a::SystemType typeId() { return "n/Collide"; }

  Collide(not_null<a::Engine*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);

  virtual int priority() { return a::Collide; }

};



NS_END(terra)
#endif

