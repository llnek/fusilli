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

#include "support/XConfig.h"
#include "support/CCSX.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "2d/XLayer.h"
#include "MsgBox.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
MsgBox* MsgBox::CreateWithAction(c::CallFunc* cb,
    const stdstr& msg) {

  auto m= MsgBox::create();
  m->SetAction(cb);
  m->SetMsg(msg);
  m->Realize();
  return m;
}

//////////////////////////////////////////////////////////////////////////////
//
MsgBox* MsgBox::CreateWithMsg(const stdstr& msg) {

  auto cb= c::CallFunc::create([](){
      c::Director::getInstance()->popScene();
      });
  return CreateWithAction(cb, msg);
}

//////////////////////////////////////////////////////////////////////////
//
void MsgBox::SetAction(c::CallFunc* cb) {
  action = cb;
  cb->retain();
}

//////////////////////////////////////////////////////////////////////////
//
void MsgBox::SetMsg(const stdstr& msg) {
  textMsg= msg;
}

//////////////////////////////////////////////////////////////////////////
//
void MsgBox::OnYes(c::Ref* rr) {
 action->execute();
}

//////////////////////////////////////////////////////////////////////////
//
void MsgBox::~MsgBox() {
 action->release();
}

//////////////////////////////////////////////////////////////////////////
//
void MsgBox::MsgBox()
  : action(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL MsgBoxLayer : public XLayer {
private:

  NO__COPYASSIGN(MsgBoxLayer)
  MsgBoxLayer();

public:
  virtual int GetIID() { return 1; }
  virtual XLayer* Realize();
  virtual ~MsgBoxLayer();
  CREATE_FUNC(MsgBoxLayer)
};

//////////////////////////////////////////////////////////////////////////
//
XLayer* MsgBoxLayer::Realize() {
  auto par = SCAST(MsgBox*, getParent());
  auto cfg = XConfig::GetInstance();

  auto fnt = cfg->GetFont("font.OCR");
  auto qn= c::Label::createWithBMFont(
      fnt, par->GetMsg());

  auto cw= cx::Center();
  auto wz= cx::VisRect();
  auto wb = cx::VisBox();

  CenterImage("game.bg");
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(cfg->GetScale() * 0.25);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto b1= cx::CreateMenuBtn("#ok.png");
  b1->setTarget(par,
      CC_MENU_SELECTOR(MsgBox::OnYes));
  auto menu= c::Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  AddItem(menu);
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
XScene* MsgBox::Realize() {
  auto y = MsgBoxLayer::create();
  AddLayer(y);
  y->Realize();
  return this;
}





NS_END(fusilli)

