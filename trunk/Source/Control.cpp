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

      this->checkBtn(ICMessage(IMI_GETPLUGINSGROUP), ui::powerInMainWnd, true);
      this->checkBtn(IMIG_TRAY, ui::powerInTrayMenu, true);

      int count = IMessage(IMC_CNT_COUNT);
      for (int i = 0; i < count; i++) {
        int net = GETCNTI(i, CNT_NET);
        if (IMessage(IMI_MSG_WINDOWSTATE, 0, 0, i)) {
          this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, i, true);

          if (GETINT(cfg::reply::onEnable) && lCtrl::reply->getNetState(net)) {
            // wysylanie powiadomienia
          }
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

      this->checkBtn(ICMessage(IMI_GETPLUGINSGROUP), ui::powerInMainWnd, false);
      this->checkBtn(IMIG_TRAY, ui::powerInTrayMenu, false);

      int count = IMessage(IMC_CNT_COUNT);
      for (int i = 0; i < count; i++) {
        int net = GETCNTI(i, CNT_NET);
        if (IMessage(IMI_MSG_WINDOWSTATE, 0, 0, i)) {
          this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, i, false);

          if (GETINT(cfg::reply::onDisable) && lCtrl::reply->getNetState(net)) {
            // wysylanie powiadomienia
          }
        }
      }

      this->Log("[Control::disable()]: msg = %s", (msg.length() ? msg.c_str() : "(none)"));

      this->awayMsg = "";
      this->awayTime->clear();
      this->ignoredUids.clear();
      this->isOn = false;
    }
  }

  void Control::checkBtn(int group, int id, int cnt, bool val, bool check) {
    sUIAction act(group, id, cnt);
    sUIActionInfo ai;

    ai.mask = UIAIM_P1 | UIAIM_TXT | UIAIM_STATUS;
    ai.act = act;
    ai.txt = (val) ? "Wy³¹cz tryb away" : "W³¹cz tryb away";
    ai.p1 = (val) ? ico::disable : ico::enable;
    ai.status = (val && check) ? ACTS_CHECKED : 0;

    ICMessage(IMI_ACTION_SET , (int)&ai , 0);
  }

  void Control::checkBtn(int group, int id, bool val, bool check) {
    sUIActionInfo ai(group, id, 0, (val && check) ? ACTS_CHECKED : 0, 
      (val) ? "Wy³¹cz tryb away" : "W³¹cz tryb away", (val) ? ico::disable : ico::enable);
    UIActionSet(ai);
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