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

#include "ecs/System.h"
#include "GEngine.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Generate : public ecs::System {

  __decl_md(ShapeInfo, _nextShapeInfo)
  __decl_ptr(ecs::Node, _arena)
  __decl_ptr(Shape, _nextShape)

  const ShapeInfo randNextInfo();
  owner<Shape*> reifyNextShape();
  void previewNextShape();

public:

  virtual int priority() { return ecs::AI + 60; }
  __decl_sys_tpid( "s/Generate")
  __decl_sys_preamble()
  __decl_sys_update()

  Generate(ecs::Engine *e)
  : System(e)
  {}

};


NS_END


