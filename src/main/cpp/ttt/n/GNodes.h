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
//////////////////////////////////////////////////////////////////////////////
#include "ash/NodeRego.h"
#include "ash/Node.h"
#include "CObjs.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoardNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
          //{"cellpos", "n/CellPos"},
          {"players", "n/Players" },
          //{"robot", "n/Robot"},
          {"grid", "n/Grid"},
          {"view", "n/PlayView" }
    });
  }

  MDECL_NFAC_TPID( "n/BoardNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL RobotNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "cellpos", "n/CellPos"},
        { "player", "n/Player"},
        { "robot", "n/Robot"}
    });
  }

  MDECL_NFAC_TPID( "n/RobotNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HumanNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "cellpos", "n/CellPos"},
        { "player", "n/Player"},
        { "human", "n/Human"}
    });
  }

  MDECL_NFAC_TPID( "n/HumanNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NetPlayNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "cellpos", "n/CellPos"},
        { "player", "n/Player"},
        { "netplay", "n/NetPlay"}
    });
  }

  MDECL_NFAC_TPID( "n/NetPlayNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CellPicNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "cell", "n/CellPic" }
    });
  }

  MDECL_NFAC_TPID( "n/CellPicNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ArenaNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "slots", "n/GVars" }
    });
  }

  MDECL_NFAC_TPID( "n/ArenaNode")
};



NS_END(tttoe)


