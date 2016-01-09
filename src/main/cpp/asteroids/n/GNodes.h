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

NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CannonCtrlNode : public a::NodeFactory {

  MDECL_NFAC_TPID( "n/CannonCtrlNode")

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "looper", "n/Looper" },
        { "cannon", "n/Cannon" },
        { "ship", "n/Ship" }
    });
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShipMotionNode : public a::NodeFactory {

  MDECL_NFAC_TPID( "n/ShipMotionNode")

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "motion", "n/Motion" },
        { "ship", "n/Ship" }
    });
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ArenaNode : public a::NodeFactory {

  MDECL_NFAC_TPID( "n/ArenaNode")

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "slots", "n/GVars" }
    });
  }

};



NS_END(asteroids)
#endif

