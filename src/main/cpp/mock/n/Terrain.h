﻿// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Block.h"

NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Terrain : public f::CPixie {
protected:

  c::Vector<Block*> _blockPool;
  c::Vector<Block*> _blocks;

  DECL_IZ(_blockPoolIndex)
  DECL_IZ(_lastBlockHeight)
  DECL_IZ(_lastBlockWidth)
  DECL_IZ(_minTerrainWidth)

  DECL_IZ(_currentPatternIndex)
  DECL_TV(int,_currentPatternCnt,1)

  DECL_IZ(_currentWidthIndex)
  DECL_IZ(_currentHeightIndex)

  DECL_IZ(_currentTypeIndex)

  DECL_IZ(_increaseGapInterval)
  DECL_FZ(_increaseGapTimer)
  DECL_IZ(_gapSize)

  DECL_TD(c::Rect, _visRect)

  DECL_BF(_startTerrain)
  DECL_BF(_showGap)

  void addBlocks(int currentWidth);
  void initTerrain();
  void distributeBlocks();
  void initBlock(Block*);


  float totalWidth () {
    auto w= 0;
    for (auto block : _blocks) {
      w += block->width();
    }
    return w;
  }

public:

  static owner<Terrain*> create(const c::Rect&);

  void checkCollision(ecs::Node*);
  void move (float xMove);
  void reset();

  virtual ~Terrain();
  Terrain() {}

};


NS_END




