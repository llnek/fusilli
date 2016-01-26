// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.


#include "core/XConfig.h"
#include "core/CCSX.h"
#include "GameScene.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

static GameScene *_singleton;

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
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene::GameScene() {
  binds(this);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* GameScene::getCurLevel() {
  return XCFG()->getLevel(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
f::JsonObj* GameScene::getLCfg() {
  return XCFG()->getLevelCfg(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* GameScene::getPool(const sstr &key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* GameScene::reifyPool(const sstr &key) {
  auto it = pools.find(key);
  auto p= mc_new( XPool);
  if (it != pools.end()) {
    delete it->second;
    pools.erase(it);
  }
  pools.insert(S__PAIR(sstr, XPool*, key, p));
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
void GameScene::binds(not_null<GameScene*> m) {
  _singleton=m;
}

//////////////////////////////////////////////////////////////////////////////
//
ws::OdinIO* GameScene::wsock() {
  return static_cast<GCX*>(context)->odin;
}

//////////////////////////////////////////////////////////////////////////////
//
GMode GameScene::getMode() {
  return static_cast<GCX*>(context)->mode;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameScene::isOnline() {
  return static_cast<GCX*>(context)->odin != nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Rect GameScene::getEnclosureRect() {
  return cx::visRect();
}

//////////////////////////////////////////////////////////////////////////////
const Box4 GameScene::getEnclosureBox() {
  return cx::visBox();
}

//////////////////////////////////////////////////////////////////////////////
//
void GameScene::reifyPools(const s_vec<sstr> &names) {
  F__LOOP(it,names) {
    reifyPool(*it);
  }
}



NS_END


