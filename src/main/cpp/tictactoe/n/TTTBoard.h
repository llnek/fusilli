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

#if !defined(__TTTBOARD_H__)
#define __TTTBOARD_H__

#include "algos/NegaMax.h"
#include "cobjs.h"
#include <tuple>
NS_ALIAS(ag, fusii::algos)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// A Tic Tac Toe board
class FS_DLL TTTBoard : public ag::GameBoard<BD_SZ> {
protected:

  bool TestWin(const ArrCells&, int actor, const ArrDim& combo);

  const s::vector<ArrDim>& GOALS;
  s::array<int,3> actors;
  ArrCells grid;
  int CV_Z;

  NO__CPYASS(TTTBoard)
  TTTBoard() = delete;

public:

  TTTBoard(int nil,  int p1v,  int p2v);
  virtual ~TTTBoard();

  int GetOtherPlayer(int pv);
  bool IsNil(int cellv);
  int GetFirstMove();

  int GetWinner(not_null<ag::FFrame<BD_SZ>*>, ArrDim& combo);
  void SyncState(const ArrCells& seed, int actor);

////
//
//
  virtual const s::vector<int> GetNextMoves(not_null<ag::FFrame<BD_SZ>*>);
  virtual int EvalScore(not_null<ag::FFrame<BD_SZ>*>);

  virtual bool IsStalemate(not_null<ag::FFrame<BD_SZ>*>);
  virtual bool IsOver(not_null<ag::FFrame<BD_SZ>*>);

  virtual void UndoMove(not_null<ag::FFrame<BD_SZ>*>, int move);
  virtual void MakeMove(not_null<ag::FFrame<BD_SZ>*>, int move);

  virtual void SwitchPlayer(not_null<ag::FFrame<BD_SZ>*>);
  virtual ag::FFrame<BD_SZ> TakeFFrame();

};


NS_END(tttoe)
#endif
