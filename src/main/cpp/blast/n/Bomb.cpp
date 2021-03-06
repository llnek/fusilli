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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Bomb.h"
#include "Blast.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
bool Bomb::init() {

  if (!PowerUp::init()) {
  return false; }

  // draw a triangle with a green border
  // get vertices for a triangle
  s_vec<CCT_PT> vs;
  getRegularPolygonVertices(vs, 3, POWERUP_ICON_INNER_RADIUS);
  drawPolygon(&vs[0], 3,
      c::ccc4f(0, 0, 0, 0), 3, c::ccc4f(0, 1, 0, 1));

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Bomb::update() {
  if (! isActive) {
    PowerUp::update();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Bomb::activate() {
  // must activate only once
  if (isActive) {
  return; }

  // first call parent function
  PowerUp::activate();

  // create a blast 8 times the size of the player that should last for 2 seconds
  auto blast = Blast::create(PLAYER_RADIUS * 8, 2);
  // position blast over bomb
  CC_POS1(blast, getPosition());
  addBlast(ss, blast);
  cx::sfxPlay("big_blast");

  PowerUp::deactivate();
}


NS_END

