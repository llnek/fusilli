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
#include "GameScene.h"
NS_BEGIN(fusii)

static GameScene* _singleton;

//////////////////////////////////////////////////////////////////////////////
//
GameLayer* GameScene::get() {
  return _singleton->getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene* GameScene::self() {
  return _singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene::~GameScene() {
  F__LOOP(it, pools) { delete it->second; }
  mc_del_ptr(odin);
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene::GameScene() {
  mode = GMode::ONE;
  SNPTR(odin)
  level = 1;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* GameScene::getLCfg() {
  return XCFG()->getLevelCfg(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* GameScene::getPool(const stdstr& key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* GameScene::reifyPool(const stdstr& key) {
  auto it = pools.find(key);
  auto p= mc_new( XPool);
  if (it != pools.end()) {
    delete it->second;
    pools.erase(it);
  }
  pools.insert(S__PAIR(stdstr, XPool*, key, p));
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
void GameScene::resetPools() {
  F__LOOP(it, pools) {
    it->second->clearAll();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GameScene::setMode(GMode m) {
  this->mode= m;
}

//////////////////////////////////////////////////////////////////////////
//
GameScene* GameScene::reify(not_null<GameScene*> g, GMode mode,
    not_null<ws::OdinIO*> wsock) {
  g->setOnlineChannel( wsock);
  g->setMode(mode);
  g->realize();
  g->play();
  return g;
}

//////////////////////////////////////////////////////////////////////////
//
GameScene* GameScene::reify(not_null<GameScene*> g, GMode mode) {
  g->setMode(mode);
  g->realize();
  g->play();
  return g;
}

//////////////////////////////////////////////////////////////////////////
//
void GameScene::bind(not_null<GameScene*> m) {
  _singleton=m;
}





NS_END(fusii)
