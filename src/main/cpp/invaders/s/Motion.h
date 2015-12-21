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

#if !defined(__MOTIONS_H__)
#define __MOTIONS_H__

#include "core/XSystem.h"
#include "EFactory.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motions : public f::XSystem<EFactory> {
public:

  virtual const a::SystemType typeId() { return "n/Motions"; }

  Motions(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);

  virtual int priority() { return a::Motion; }

  virtual bool update(float);

  void processAlienMotions(a::Node*,float dt);
  void controlCannon(a::Node*, float dt);
  void fireMissile(a::Node*, float dt);
  void scanInput(a::Node*, float dt);

  virtual ~Motions() {}
  NODFT(Motions)
  NOCPYASS(Motions)

  a::NodeList *cannons=nullptr;
  a::NodeList *ships=nullptr;
  a::NodeList *aliens=nullptr;
  bool right=false;
  bool left=false;
};


NS_END(invaders)
#endif

