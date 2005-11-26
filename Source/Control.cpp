/*
 *  kAway2 Control class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: $
 */

#pragma once

kAway2::Control *kAway2::Control::sCtrl = NULL;
UINT_PTR kAway2::Control::m_Timer = NULL;

kAway2::Control::Control() {
  this->ctrlSt = new kAway2::CtrlStatus(kAway2::cfg::netChange);
  this->ctrlSt->Init();

  this->awayMsg = "";
  this->awayTime = Stamina::Date64(false);
  this->ignoredUids = "";
  this->isOn = false;
}

kAway2::Control::~Control() {
  delete this->ctrlSt, this->awayTime;
  this->ctrlSt, this->awayTime = NULL;

  this->awayMsg = "";
  this->ignoredUids = "";
  this->isOn = false;
}

void kAway2::Control::Enable(std::string msg) {
  if (!this->isOn) {
    this->ctrlSt->RememberInfo();

    this->awayMsg = msg;
    this->awayTime.now();
    this->isOn = true;
  }
}

void kAway2::Control::Disable(std::string msg) {
  if (this->isOn) {
    this->ctrlSt->BackRemeberedInfo();

    this->awayMsg = "";
    this->awayTime = NULL;
    this->ignoredUids = "";
    this->isOn = false;
  }
}

void kAway2::Control::netListInit(int colCount, int cfgGroup, int dynActGroup) {
  itemNet item;
  int c = 1, i = 0, ico;
  int count = this->ctrlSt->nets.size();
  char *netName = NULL;

  for (std::list<itemNet>::iterator it = this->ctrlSt->nets.begin(); it != this->ctrlSt->nets.end(); it++, i++, c++) {
    item = *it;
    ico = UIIcon(IT_LOGO, (int) item.net, 0, 0);
    netName = (char*) Ctrl->IMessage(IM_PLUG_NETNAME, item.net); 
    i %= colCount; 

    UIActionCfgAdd(cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, shared::Icon16(ico).c_str(), 0, (i > 0) ? 10 : 0);
    UIActionCfgAdd(cfgGroup, dynActGroup + item.net, (i != (colCount - 1) && c != count) ? ACTT_CHECK | ACTSC_INLINE : ACTT_CHECK, netName);
    netName = NULL;
  }
}

void kAway2::Control::netListSetState(int cfgGroup, int dynActGroup) {
  itemNet item;
  for (std::list<itemNet>::iterator it = this->ctrlSt->nets.begin(); it != this->ctrlSt->nets.end(); it++) {
    item = *it;
    // Wszystkie nowo utworzone checkboxy s¹ zaznaczone
    // wiêc tylko odznaczamy :)
    if (!item.use)
      UIActionCfgSetValue(sUIAction(cfgGroup, dynActGroup + item.net), "0", true);
  }
}

void kAway2::Control::netListGetState(int cfgGroup, int dynActGroup) {
  itemNet item;
  char buff[16];
  int v;

  for (std::list<itemNet>::iterator it = this->ctrlSt->nets.begin(); it != this->ctrlSt->nets.end(); it++) {
    item = *it;
    UIActionCfgGetValue(sUIAction(cfgGroup, dynActGroup + item.net), buff, 16, true);
    v = atoi(buff);

    if ((v ? true : false) != item.use) {
      item.use = v ? true : false;
      *it = item;
    }
  }
}