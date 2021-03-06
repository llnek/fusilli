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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(p2048)

#define CELL_SPACE 10

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Card : public f::ColorLayer {

  bool inix(const CCT_SZ&, const CCT_PT&);
  Card(int v) { _value=v; }

  __decl_ptr(c::Label, _numLabel)
  __decl_iz(_value)

public:

  static owner<Card*> create(int, const CCT_SZ&, const CCT_PT&);
  __declapi_setr(int, _value, Number)
  __decl_getr(int,_value,Number)
  void runNewNumberAction();

};

typedef f::FArrayPtr<Card> CardArr;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_vec(CardArr*, cardArr)
  __decl_fz(cellSize)
  __decl_bf(enabled)
  __decl_bf(swiped)
  virtual ~GVars() {
     F__LOOP(it, cardArr) { delete *it; }
  }
};



NS_END




