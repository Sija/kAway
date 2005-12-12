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

  void Control::enable(std::string msg, int status) {
    if (!this->isOn) {
      this->awayMsg = msg;
      this->awayTime->now();
      this->isOn = true;

      if (this->sCtrl) {
        int chgStatus = GETINT(cfg::status::changeOnEnable);
        int chgInfo = GETINT(cfg::status::changeInfoOnEnable);

        if (chgInfo) {
          this->sCtrl->fCtrl->addVar("date", this->awayTime->strftime("%d/%m/%Y"));
          this->sCtrl->fCtrl->addVar("time", this->awayTime->strftime("%H:%M:%S"));
          this->sCtrl->fCtrl->addVar("msg", msg);
        }

        if (chgStatus || chgInfo) {
          this->sCtrl->rememberInfo();
          this->sCtrl->changeStatus(chgInfo ? GETSTRA(cfg::tpl::status) : "", chgStatus ? status : -1);
        }
      }

      this->Log("[Control::enable()]: msg = %s", (msg.length() ? msg.c_str() : "(none)"));
    }
  }

  void Control::disable(std::string msg) {
    if (this->isOn) {
      if (this->sCtrl) {
        this->sCtrl->fCtrl->clearVars();
        this->sCtrl->restoreInfo();
      }

      this->Log("[Control::disable()]: msg = %s", (msg.length() ? msg.c_str() : "(none)"));

      this->awayMsg = "";
      this->awayTime->clear();
      this->ignoredUids.clear();
      this->isOn = false;
    }
  }

  bool Control::isIgnoredUid(int net, std::string uid) {
    for (tIgnoredUids::iterator it = this->ignoredUids.begin(); it != this->ignoredUids.end(); it++) {
      if ((it->net == net) && (it->uid == uid)) return(true);
    }
    return(false);
  }

  void Control::addIgnoredUid(int net, std::string uid) {
    this->ignoredUids.push_back(ignoredUid(net, uid));
  }

  void Control::removeIgnoredUid(int net, std::string uid) {
    for (tIgnoredUids::iterator it = this->ignoredUids.begin(); it != this->ignoredUids.end(); it++) {
      if ((it->net == net) && (it->uid == uid)) {
        it = this->ignoredUids.erase(it); break;
      }
    }
  }
}