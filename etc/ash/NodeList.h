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

#include "Node.h"
NS_BEGIN(ecs)


//////////////////////////////////////////////////////////////////////////////
//
class Entity;
class Node;
class FS_DLL NodeList : public f::FDList<Node> {

  //owns the nodes
  DECL_TD(NodeType, nType)

public:

  const NodeType getType() { return nType; }

  bool containsWithin(not_null<Entity*>);
  bool isCompatible(not_null<Entity*>);
  void removeEntity(not_null<Entity*> );

  NodeList(const NodeType&);
  virtual ~NodeList() {}
  NODFT(NodeList)
  NOCPYASS(NodeList)

};



NS_END



