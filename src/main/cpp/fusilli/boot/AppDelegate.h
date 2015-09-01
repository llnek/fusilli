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

#ifndef  __APP_DELEGATE_H__
#define  __APP_DELEGATE_H__

#include "cocos2d.h"
#include "asterix.h"

class ZL_DLLEXPORT AppDelegate : private cocos2d::Application {

public:

  /**
  @brief    Implement Director and Scene init code here.
  @return true    Initialize success, app continue.
  @return false   Initialize failed, app terminate.
  */
  virtual bool applicationDidFinishLaunching();

  /**
  @brief  The function be called when the application enter background
  @param  the pointer of the application
  */
  virtual void applicationDidEnterBackground();

  /**
  @brief  The function be called when the application enter foreground
  @param  the pointer of the application
  */
  virtual void applicationWillEnterForeground();

  virtual void initGLContextAttrs();

  virtual ~AppDelegate();
  AppDelegate();

};

#endif
