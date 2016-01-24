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
#pragma once
//////////////////////////////////////////////////////////////////////////////
#include "core/OdinEvent.h"
#include "ash/System.h"
#include "GEngine.h"

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Net : public a::System {

  void onSocket(ws::OdinEvent*);
  void onSess(ws::OdinEvent*);
  void onNet(ws::OdinEvent*);
  void initOnline();
  void process();
  void sync();

public:

  MDECL_SYS_PRIORITY(a::NetPlay)
  MDECL_SYS_TPID( "n/Net")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  DECL_PTR(a::NodeList, board)
  DECL_PTR(a::NodeList, arena)

  Net(a::Engine *e)
  : System(e)
  {}

};


NS_END(tttoe)

