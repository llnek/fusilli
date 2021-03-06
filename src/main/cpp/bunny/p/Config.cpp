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

#include "Splash.h"
#include "Config.h"
NS_BEGIN(bunny)

//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c = mc_new(Config);
  c->initAssets();
  c->initLevels();
  c->initCsts();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({ });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  _game_id= "e5594a22-817e-459a-8218-aeba92c1dfc2";
  _app_id = "bunny";

  addCst("SHIP+SPEED", CC_INT(200));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addAtlas("explosion", CC_STR("misc/explosion.plist"));
  addAtlas("fire", CC_STR("misc/fire.plist"));
  addAtlas("shadow", CC_STR("misc/shadow.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game-pics", CC_STR("pics/images.png"));
  addImage("game.bg", CC_STR("pics/background.jpg"));

  addEffect("uh", CC_STR("sfx/uh.wav"));
  addEffect("bomb", CC_STR("sfx/bomb.mp3"));
  addEffect("button", CC_STR("sfx/button-click.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
  addFont("dft", CC_STR("fonts/en/font.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &rs) {
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("cc-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


