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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "x2d/XScene.h"
#include "core/Odin.h"

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(pong)

typedef std::function<void (ws::OdinIO*, j::json)> NPCX_Yes;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NPCX : public f::SCTX {
  NPCX(NPCX_Yes yes, VOIDFN no) {
    this->yes=yes;
    this->no=no;
  }
  __decl_md(NPCX_Yes, yes)
  __decl_md(VOIDFN, no)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL NetPlay : public f::XScene {

  void networkEvent(ws::OdinEvent*);
  void sessionEvent(ws::OdinEvent*);
  void odinEvent(ws::OdinEvent*);
  void onPlayReply(ws::OdinEvent*);
  void showWaitOthers();
  void onStart(ws::OdinEvent*);
  void onCancel();
  void onLogin();

  __decl_ptr(ws::OdinIO, _odin)
  __decl_iz(_player)

public:

  __decl_create_scene_ctx(NetPlay)
  __decl_deco_ui()

  virtual ~NetPlay() {
    ws::disconnect(_odin);
    mc_del_ptr(_odin);
  }

};


NS_END


