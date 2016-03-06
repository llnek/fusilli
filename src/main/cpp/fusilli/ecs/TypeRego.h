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
//
#include "Ecs.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL TypeRegistry {

  s_map<COMType, CompoCache*> rego;
  NOCPYASS(TypeRegistry)

public:

  void unbind(not_null<Component*>, not_null<Entity*>);
  void unbind(const COMType&, not_null<Entity*>);

  void bind(not_null<Component*>, not_null<Entity*>);

  CompoCache* getCache(const COMType&);

  virtual ~TypeRegistry();
  TypeRegistry() {}

};


NS_END


