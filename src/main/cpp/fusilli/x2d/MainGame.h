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

#if !defined(__MAINGAME_H__)
#define __MAINGAME_H__

#include "platform/CCCommon.h"
#include "aeon/fusilli.h"
#include "core/XPool.h"
#include "core/Odin.h"
#include "XScene.h"
#include "XGameLayer.h"
#include <queue>

#define MGMS() fusii::MainGame::Self()
#define MGML() fusii::MainGame::Get()

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(fusii)

enum class GMode { ONE, TWO, NET };

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MainGame : public XScene {
protected:

  s::map<stdstr, XPool*> pools;
  s::queue<stdstr> msgQ;
  ws::WSockSS* odin;

  bool running;
  GMode mode;
  int level;

  //virtual void SetMode(GMode, c::Dictionary*);
  virtual void SetMode(GMode);
  virtual XGameLayer* GetGLayer() = 0;

  static void Bind(not_null<MainGame*>);

  NO__CPYASS(MainGame)
  MainGame();

public:

  //static MainGame* Reify(not_null<MainGame*>, GMode, not_null<c::Dictionary*> );
  static MainGame* Reify(not_null<MainGame*>, GMode);

  static XGameLayer* Get();
  static MainGame* Self();

  void SetOnlineChannel(owner<ws::WSockSS*> s) { odin= s; }

  virtual bool IsRunning() { return running; }
  virtual bool IsOnline() { return NNP(odin); }

  virtual void NetSend(const ws::Event&);

  virtual void Stop() = 0;
  virtual void Play() = 0;

  virtual void Resume() {}
  virtual void Pause() {}

  XPool* ReifyPool(const stdstr& n);
  XPool* GetPool(const stdstr& n);

  c::Dictionary* GetLCfg();
  void ResetPools();

  s::queue<stdstr>& MsgQueue() { return msgQ; }
  ws::WSockSS* WSOCK() { return odin; }

  virtual ~MainGame();
};


NS_END(fusii)
#endif

