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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XLayer.h"
#include "GameOver.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameOverLayer : public XLayer {
protected:
  NO__CPYASS(GameOverLayer)
public:

  virtual XLayer* Realize() {
    CC_PCAST(GameOver*)->DecoUI(this);
    return this;
  }

  IMPL_CTOR(GameOverLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
GameOver* GameOver::ReifyWithAction(not_null<GameOver*> s, VOIDFN fn) {

  s->Realize();
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
void GameOver::OnReplayBtn(c::Ref* rr) {
}

//////////////////////////////////////////////////////////////////////////
//
void GameOver::OnQuitBtn(c::Ref* rr) {
  cx::RunScene( XCFG()->StartWith() );
}

//////////////////////////////////////////////////////////////////////////
//
GameOver::~GameOver() {
}

//////////////////////////////////////////////////////////////////////////
//
GameOver::GameOver() {

}

//////////////////////////////////////////////////////////////////////////
//
XScene* GameOver::Realize() {
  auto y = ReifyRefType<GameOverLayer>();
  AddLayer(y);
  y->Realize();
  return this;
}





NS_END(fusii)
