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

#if !defined(__GAMELAYER_H__)
#define __GAMELAYER_H__

#include "base/CCEventKeyboard.h"
#include "core/ComObj.h"
#include "ash/Engine.h"
#include "XLayer.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public XLayer {
protected:

  void addListener(c::EventListener*);
  void disableListeners();
  void enableListeners();

  virtual void initMouse();
  virtual void initKeys();
  virtual void initTouch();

  s::array<bool, 256> keyboard;
  c::Dictionary* options;
  a::Engine* engine;

  c::EventListenerKeyboard* keys;
  c::EventListenerMouse* mouse;
  c::EventListener* touch;

  NO__CPYASS(GameLayer)
  GameLayer();

public:

  virtual void sendMsg(const stdstr& topic, void* msg) = 0;
  void sendMsg(const stdstr& topic) {
    sendMsg(topic, nullptr);
  }

  bool keyPoll(c::EventKeyboard::KeyCode );
  virtual const Box4 getEnclosureBox();
  virtual int getIID() { return 2; }

  //virtual void update(float);
  //virtual XLayer* realize();
  virtual ~GameLayer();

};





NS_END(fusii)
#endif
