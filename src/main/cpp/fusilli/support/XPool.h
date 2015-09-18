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

#if !defined(__XPOOL_H__)
#define __XPOOL_H__

#include "deprecated/CCDeprecated.h"
#include "platform/CCCommon.h"
#include "core/fusilli.h"
#include "2d/ComObj.h"
#include <vector>

NS_ALIAS(s, std)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XPool {
private:

  DISALLOW_COPYASSIGN(XPool)
  XPool();

  s::vector<ComObj*> objs;

public:

  const s::vector<ComObj*>& Elements() { return objs; }
  void Preset(s::function<ComObj* (XPool*)>, int);
  ComObj* Select(s::function<bool (ComObj*)>);

  ComObj* GetAndSet();
  ComObj* Get();

  int Size() { return (int)objs.size(); }
  int CountActives();

  void Foreach(s::function<void (ComObj*)>);
  void Reset();

  virtual ~XPool();


  DEFCREATE_FUNC(XPool)
};





NS_END(fusilli)
#endif

