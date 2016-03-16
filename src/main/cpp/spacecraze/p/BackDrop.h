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

#include "x2d/XLayer.h"
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL BackDrop : public f::XLayer {

DECL_TD(s_vec<c::Sprite*>, _stars)
  DECL_TD(c::Size, _sz)

public:

  STATIC_REIFY_LAYER(BackDrop)
  MDECL_UPDATE()
  MDECL_DECORATE()

};



NS_END


