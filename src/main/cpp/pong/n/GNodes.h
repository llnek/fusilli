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

#include "ash/NodeRego.h"
#include "ash/Node.h"
#include "CObjs.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BallNode : public a::NodeFactory {

  virtual const NodeType typeId() { return "n/BallNode"; }
  virtual owner<Node*> reifyNode() {
    return reifyXXXNode({
        { "velocity", "n/Velocity" },
        {"ball", "n/Ball" }
    });
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL FauxPaddleNode : public a::NodeFactory {

  virtual const NodeType typeId() { return "n/FauxPaddleNode"; }
  virtual owner<Node*> reifyNode() {
    return reifyXXXNode({
        {"lastpos", "n/Position" },
        {"paddle", "n/Paddle" },
        {"faux", "n/Faux" },
        {"player" , "n/Player" }
    });
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL PaddleNode : public a::NodeFactory {

  virtual const NodeType typeId() { return "n/PaddleNode"; }
  virtual owner<Node*> reifyNode() {
    return reifyXXXNode({
      {"lastpos" , "n/Position" },
      {"motion", "n/Motion" },
      {"paddle" , "n/Paddle" },
      {"player" , "n/Player" }
    });
};


NS_END(pong)
#endif

