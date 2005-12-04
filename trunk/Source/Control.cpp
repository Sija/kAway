/*
 *  kAway2 Control class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  Control::Control() {
    // this->awayMsg = "";
    this->awayTime = new Stamina::Date64(false);
    // this->ignoredUids.clear();
    this->isOn = false;
  }

  Control::~Control() {
    delete this->awayTime;
    this->awayTime = NULL;

    this->awayMsg = "";
    this->ignoredUids.clear();
    this->isOn = false;
  }

  void Control::Log(enDebugLevel level, const char * format, va_list ap) {
    if (Ctrl && Ctrl->DebugLevel(level))
      Ctrl->IMLOG_(level, format, ap);
  }

  void Control::Enable(std::string msg) {
    if (!this->isOn) {
      if(this->sCtrl)
        this->sCtrl->RememberInfo();

      this->awayMsg = msg;
      this->awayTime->now();
      this->isOn = true;

      this->Debug("[Control::Enable()]: msg = %s, timestamp = %s",
        (msg.length() ? msg.c_str() : "(none)"), this->awayTime->strftime("%d/%m/%Y [%H:%M:%S]").c_str());
    }
  }

  void Control::Disable(std::string msg) {
    if (this->isOn) {
      if(this->sCtrl)
        this->sCtrl->BackInfo();

      this->Debug("[Control::Disable()]: msg = %s, timestamp = %s",
        (msg.length() ? msg.c_str() : "(none)"), this->awayTime->strftime("%d/%m/%Y [%H:%M:%S]").c_str());

      this->awayMsg = "";
      this->awayTime->clear();
      this->ignoredUids.clear();
      this->isOn = false;
    }
  }

  bool Control::isIgnoredUid(int net, std::string uid) {
    ignoredUid item;

    for (std::list<ignoredUid>::iterator it = this->ignoredUids.begin(); it != this->ignoredUids.end(); it++) {
      item = *it;
      if ((item.net == net) && (item.uid == uid)) return(true);
    }
    return(false);
  }

  void Control::addIgnoredUid(int net, std::string uid) {
    ignoredUid item;

    item.net = net;
    item.uid = uid;

    this->ignoredUids.push_back(item);
  }

  void Control::removeIgnoredUid(int net, std::string uid) {
    ignoredUid item;

    for (std::list<ignoredUid>::iterator it = this->ignoredUids.begin(); it != this->ignoredUids.end(); it++) {
      item = *it;

      if ((item.net == net) && (item.uid == uid)) {
        // removing unwanted item [how? ;>]
        // this->ignoredUids.pop_back();
      }
    }
  }
}