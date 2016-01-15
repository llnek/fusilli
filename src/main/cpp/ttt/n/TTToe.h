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

#include "algos/NegaMax.h"
#include "s/utils.h"

NS_ALIAS(ag, fusii::algos)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// A Tic Tac Toe board
struct CC_DLL TTToe : public ag::GameBoard<BD_SZ> {

  virtual const s_vec<int> getNextMoves(not_null<ag::FFrame<BD_SZ>*>);
  virtual int evalScore(not_null<ag::FFrame<BD_SZ>*>);

  virtual bool isStalemate(not_null<ag::FFrame<BD_SZ>*>);
  virtual bool isOver(not_null<ag::FFrame<BD_SZ>*>);

  virtual void undoMove(not_null<ag::FFrame<BD_SZ>*>, int move);
  virtual void makeMove(not_null<ag::FFrame<BD_SZ>*>, int move);

  virtual void switchPlayer(not_null<ag::FFrame<BD_SZ>*>);
  virtual owner<ag::FFrame<BD_SZ>*> takeFFrame();

  int getWinner(not_null<ag::FFrame<BD_SZ>*>, ArrDim &combo);
  void syncState(const ArrCells &seed, int actor);

  TTToe(int nil,  int p1v,  int p2v);
  virtual ~TTToe() {}

  int getOtherPlayer(int pv);
  bool isNil(int cellv);
  int getFirstMove();

private:

  bool testWin(const ArrCells& , int actor, const ArrDim&);

  DECL_TD(ArrCells, grid)
  DECL_IZ(CV_Z)

  s_vec<ArrDim> GOALS;
  s_arr<int,3> actors;
};

NS_END(tttoe)


