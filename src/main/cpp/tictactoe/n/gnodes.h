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

#if !defined(__GNODES_H__)
#define __GNODES_H__

#include "cobjs.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ArenaNode : public a::NodeFactory {
public:

  virtual owner<a::Node*> ReifyNode() {
    return ReifyXXXNode(
        s::vector<stdstr> {
          "selection", "board", "robot", "grid", "view" },
        s::vector<a::COMType> {
          "n/UISelection", "n/Board", "n/SmartAlgo",
          "n/Grid", "n/PlayView" });
  }

  virtual const a::NodeType TypeId() {
    return "n/BoardNode";
  }

  NO__CPYASS(ArenaNode)
  IMPL_CTOR(ArenaNode)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GUINode : public a::NodeFactory {
public:

  virtual owner<a::Node*> ReifyNode() {
    return ReifyXXXNode(
        s::vector<stdstr> { "selection", "view" },
        s::vector<a::COMType> { "n/UISelection", "n/PlayView" });
  }

  virtual const a::NodeType TypeId() {
    return "n/GUINode";
  }

  NO__CPYASS(GUINode)
  IMPL_CTOR(GUINode)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL NetPlayNode : public a::NodeFactory {
public:

  virtual owner<a::Node*> ReifyNode() {
    return ReifyXXXNode(
        s::vector<stdstr> { "playcmd", "grid" },
        s::vector<a::COMType> { "n/NetPlay", "n/Grid" });
  }

  virtual const a::NodeType TypeId() {
    return "n/NetPlayNode";
  }

  NO__CPYASS(NetPlayNode)
  IMPL_CTOR(NetPlayNode)
};




NS_END(tttoe)
#endif


