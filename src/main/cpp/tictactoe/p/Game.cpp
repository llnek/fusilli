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

#include "core/Odin.h"
#include "Game.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(f, fusii)
NS_BEGIN(tttoe)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : f::XLayer {
public:

  virtual f::XLayer* Realize() {
    CenterImage("game.bg");
    return this;
  }

  NO__CPYASS(BGLayer)
  IMPL_CTOR(BGLayer)
};

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::InizGame() {
  CCLOG("about to init-game");

  for (auto it= atlases.begin(); it != atlases.end(); ++it) {
    it->second->removeAllChildren();
  }

  if (atlases.empty()) {
    RegoAtlas("game-pics");
    RegoAtlas("lang-pics");
  }

  f::EmptyQueue( MGMS()->MsgQueue() );
  MkAsh();

  auto p1c= CC_CSV(c::String, "P1_COLOR")->getCString();
  auto p2c= CC_CSV(c::String, "P2_COLOR")->getCString();
  auto seed = XCFG()->GetSeedData();
  auto ppids = seed["ppids"];
  stdstr p1k;
  stdstr p2k;
  stdstr p1n;
  stdstr p2n;

  for (auto it=ppids.begin(); it != ppids.end(); ++it) {
    auto arr= it->second;
    if (arr[0].int_value() == 1) {
      p1k= it->first;
      p1n= arr[1].string_value();
    } else {
      p2k= it->first;
      p2n= arr[1].string_value();
    }
  }

  GetHUD()->RegoPlayers(p1c, p1k, p1n,
                        p2c, p2k, p2n);
  GetHUD()->Reset();

  options->setObject(CC_INT(0), "lastWinner");

  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::MkAsh() {
  auto e = a::Engine::Reify();
  auto d = CC_DICT();
  auto f = new EFactory(e, d);

  this->factory=f;
  this->engine = e;
  this->options= d;

  e->RegoSystem(new Resolve(f, d));
  e->RegoSystem(new Stager(f, d));
  e->RegoSystem(new Motion(f, d));
  e->RegoSystem(new Logic(f, d));
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::Replay() {
  if (MGMS()->IsOnline()) {
    // request server to restart a new game
    ws::Send(MGMS()->WSOCK(), ws::Event(
      ws::MType::SESSION,
      ws::EType::REPLAY
    ));
  } else {
    InizGame();
    Reset(false);
    InitAsh();
    GetScene()->Resume();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::UpdateHUD() {
  if (MGMS()->IsRunning()) {
    GetHUD()->DrawStatus(CC_GDV(c::Integer, options, "pnum"));
  } else {
    GetHUD()->DrawResult(CC_GDV(c::Integer, options, "lastWinner"));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::PlayTimeExpired() {
  MGMS()->MsgQueue().push("forfeit");
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::OverAndDone(int winner) {
  GetHUD()->EndGame(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::SendMsg(const stdstr& topic, void* msg) {
  auto par = CC_PCAST(Game*);

  if ("/hud/showmenu" == topic) {
    ShowMenu();
  }

  if ("/hud/replay" == topic) {
    Replay();
  }

  if ("/hud/timer/show" == topic) {
    GetHUD()->ShowTimer();
  }

  if ("/net/restart" == topic) {
    GetHUD()->KillTimer();
    Play(false);
  }

  if ("/net/stop" == topic) {
    OverAndDone(msg.status);
  }

  if ("/hud/timer/hide" == topic) {
    GetHUD()->KillTimer();
  }

  if ("/hud/score/update" == topic) {
    GetHUD()->UpdateScore(msg.color, msg.score);
  }

  if ("/hud/end" == topic) {
    OverAndDone(msg.winner);
  }

  if ("/hud/update" == topic) {
    GetHUD()->Draw(msg.running, msg.pnum);
  }

  if ("/player/timer/expired" == topic) {
    PlayTimeExpired(msg);
  }

}

/* scene */


//////////////////////////////////////////////////////////////////////////////
//
void Game::Play() {
  running= true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Stop() {
  running= false;
}

//////////////////////////////////////////////////////////////////////////
//
f::XGameLayer* Game::GetGLayer() {
  return (f::XGameLayer*) GetLayer(2);
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::Realize() {

  auto g = (f::XLayer*) f::ReifyRefType<GameLayer>();
  auto b = (f::XLayer*) f::ReifyRefType<BGLayer>();
  auto h = (f::XLayer*) f::ReifyRefType<HUDLayer>();

  AddLayer(b, 1)->Realize();
  AddLayer(g, 2);
  AddLayer(h, 3)->Realize();

  // set this to be THE main game
  Bind(this);

  // realize game layer last
  g->Realize();

  return this;
}


NS_END(tttoe)

