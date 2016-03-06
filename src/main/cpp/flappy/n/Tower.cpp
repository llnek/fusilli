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
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "Tower.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Tower::init() {
    // record size of the tower's sprite
  towerSpriteSize = cx::getSpriteFrame("opst_02")->getOriginalSize();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  // create the first pair of towers
  // they should be two whole screens away from the dragon
  auto initialPosition = c::Vec2(wz.size.width*2, wb.cy);
  firstTowerIndex = 0;
  createTower(initialPosition);
  // create the remaining towers
  lastTowerIndex = 0;
  createTower(getNextTowerPosition());
  lastTowerIndex = 1;
  createTower(getNextTowerPosition());
  lastTowerIndex = 2;
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::createTower(const c::Vec2 &position) {
  // create a new tower and add it to the array
  TowerBody tower(position);

  // create lower tower sprite & add it to GameWorld's batch node
  tower.lowerSprite = cx::reifySprite("opst_02");
  tower.lowerSprite->setPosition(
      position.x,
      position.y + VERT_GAP_BWN_TOWERS * -0.5 + towerSpriteSize.height * -0.5 );

  parentNode->addAtlasItem("dhtex", tower.lowerSprite, E_LAYER_TOWER);

  // create upper tower sprite & add it to GameWorld's batch node
  tower.upperSprite = cx::reifySprite("opst_01");
  tower.upperSprite->setPosition(
      position.x,
      position.y + VERT_GAP_BWN_TOWERS * 0.5 + towerSpriteSize.height * 0.5 );
  parentNode->addAtlasItem("dhtex", tower.upperSprite, E_LAYER_TOWER);

  towers.push_back(tower);
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::update(float dt) {
  auto n=0;
  F__LOOP(it, towers) {
    auto &tower = *it;
    // first update the position of the tower
    tower.position.x -= MAX_SCROLLING_SPEED;
    tower.lowerSprite->setPosition(tower.position.x, tower.lowerSprite->getPositionY());
    tower.upperSprite->setPosition(tower.position.x, tower.upperSprite->getPositionY());

    // if the tower has moved out of the screen, reposition them at the end
    if (tower.position.x < towerSpriteSize.width * -0.5) {
      repositionTower(n);
      // this tower now becomes the tower at the end
      lastTowerIndex = n;
      // that means some other tower has become first
      firstTowerIndex = ((n+1) >= towers.size()) ? 0 : (n+1);
    }

    ++n;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::repositionTower(int index) {
  auto &tower = towers[index];
  // update tower's position and sprites
  tower.position = getNextTowerPosition();
  tower.lowerSprite->setPosition(
      tower.position.x,
      tower.position.y + VERT_GAP_BWN_TOWERS * -0.5 + towerSpriteSize.height * -0.5);
  tower.upperSprite->setPosition(
      tower.position.x,
      tower.position.y + VERT_GAP_BWN_TOWERS * 0.5 + towerSpriteSize.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Vec2 Tower::getNextTowerPosition() {
  // randomly select either above or below last tower
  auto offset = cx::rand() * VERT_GAP_BWN_TOWERS * 0.75;
  auto isAbove = (cx::rand() > 0.5);
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  offset *= (isAbove) ? 1:-1;

  auto &tower = towers[lastTowerIndex];
  // new position calculated by adding to last tower's position
  auto newPositionX = tower.position.x + HWZ(wz.size);
  auto newPositionY = tower.position.y + offset;

  // limit the point to stay within 30-80% of the screen
  if (newPositionY >= wb.top  * 0.8) {
      newPositionY -= VERT_GAP_BWN_TOWERS;
  }
  else if (newPositionY <= wb.top * 0.3) {
      newPositionY += VERT_GAP_BWN_TOWERS;
  }

  // return the new tower position
  return c::Vec2(newPositionX, newPositionY);
}

//////////////////////////////////////////////////////////////////////////////
//
TowerBody& Tower::getFrontTower() {
  return towers[firstTowerIndex];
}




NS_END



