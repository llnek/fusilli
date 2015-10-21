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

#if !defined(__EFACTORY_H__)
#define __EFACTORY_H__

#include "core/Primitives.h"
#include "core/Factory.h"
NS_ALIAS(a,ash)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL EFactory {
protected:

  s::vector<f::FArray<int>> goals;
  c::Dictionary* state;
  a::Engine* engine;

  void MapGoalSpace(int );

  NO__CPYASS(EFactory)
  EFactory() = delete;

public:

  EFactory(not_null<a::Engine*>, not_null<c::Dictionary*> options);
  virtual ~EFactory() {}

  a::Entity* ReifyBoard(not_null<c::Node*> layer);

};


NS_END(tttoe)
#endif

