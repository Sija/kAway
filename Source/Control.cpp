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

  void Control::enable(std::string msg, int status, bool silent) {
    if (this->isOn) return;

    this->awayMsg = msg;
    this->awayTime->now();
    this->isOn = true;

    if (this->sCtrl) {
      int chgStatus = GETINT(cfg::status::changeOnEnable);
      int chgInfo = GETINT(cfg::status::changeInfoOnEnable);

      if (chgInfo) {
        this->sCtrl->fCtrl->addVar("date", this->awayTime->strftime(GETSTRA(cfg::dateFormat)));
        this->sCtrl->fCtrl->addVar("time", this->awayTime->strftime(GETSTRA(cfg::timeFormat)));
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
      if (IMessage(IMI_MSG_WINDOWSTATE, 0, 0, i)) {
        if (GETINT(cfg::reply::onEnable) && !silent) {
          this->sendMsgTpl(i, cfg::tpl::enable);
        }
        this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, i, true);
      }
    }

    this->showKNotify("Tryb away zosta³ <b>w³¹czony<b>", ico::enable);
    this->Log("[Control::enable()]: msg = %s, silent = %s", 
      (msg.length() ? msg.c_str() : "(none)"), btoa(silent));
  }

  void Control::disable(std::string msg, bool silent) {
    if (!this->isOn) return;

    if (this->sCtrl) {
      this->sCtrl->fCtrl->clearVars();
      this->sCtrl->restoreInfo();
    }

    this->checkBtn(ICMessage(IMI_GETPLUGINSGROUP), ui::powerInMainWnd, false);
    this->checkBtn(IMIG_TRAY, ui::powerInTrayMenu, false);

    int count = IMessage(IMC_CNT_COUNT);
    for (int i = 0; i < count; i++) {
      if (IMessage(IMI_MSG_WINDOWSTATE, 0, 0, i)) {
        if (GETINT(cfg::reply::onDisable) && !silent) {
          this->sendMsgTpl(i, cfg::tpl::disable, msg);
        }
        this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, i, false);
      }
    }

    this->showKNotify("Tryb away zosta³ <b>wy³¹czony<b>", ico::disable);
    this->Log("[Control::disable()]: msg = %s, silent = %s", 
      (msg.length() ? msg.c_str() : "(none)"), btoa(silent));

    this->awayMsg = "";
    this->awayTime->clear();
    this->ignoredUids.clear();
    this->isOn = false;
  }

  void Control::showKNotify(char * text, int ico) {
    if (!GETINT(cfg::useKNotify)) return;
    Ctrl->IMessage(&KNotify::sIMessage_notify(text, ico));
  }

  void Control::sendMsgTpl(int cnt, int tplId, std::string msgVar) {
    int net = GETCNTI(cnt, CNT_NET);

    if ((sCtrl->getActualStatus(net) == ST_HIDDEN && !GETINT(cfg::reply::whenInvisible)) || 
      !lCtrl::reply->getNetState(net) || !lCtrl::reply->isConnected(net))
      return;

    std::string ext;
    ext = SetExtParam(ext, "kA2AutoMsg", "true");

    Format *format = new Format;

    format->addVar("display", GETCNTC(cnt, CNT_DISPLAY));
    format->addVar("name", GETCNTC(cnt, CNT_NAME));
    format->addVar("nick", GETCNTC(cnt, CNT_NICK));
    format->addVar("surname", GETCNTC(cnt, CNT_SURNAME));
    format->addVar("date", this->awayTime->strftime(GETSTRA(cfg::dateFormat)));
    format->addVar("time", this->awayTime->strftime(GETSTRA(cfg::timeFormat)));
    format->addVar("msg", (tplId == cfg::tpl::disable) ? msgVar : this->awayMsg);

    this->Debug("[Control::sendMsg()]: net = %i, uid = %s", net, GETCNTC(cnt, CNT_UID));

    Message::SendMsg(GETCNTC(cnt, CNT_UID), net, format->parse(GETSTRA(tplId)), 
      MT_MESSAGE, ext, GETINT(cfg::reply::useHtml));
    delete format; format = NULL;
  }

  void Control::checkBtn(int group, int id, int cnt, bool state, bool check) {
    sUIAction act(group, id, cnt);
    sUIActionInfo ai;

    ai.act = act;
    ai.mask = UIAIM_P1 | UIAIM_TXT | UIAIM_STATUS;
    ai.p1 = (state) ? ico::disable : ico::enable;
    ai.txt = (state) ? "Wy³¹cz tryb away" : "W³¹cz tryb away";
    ai.status = (state && check) ? ACTS_CHECKED : 0;

    ICMessage(IMI_ACTION_SET , (int)&ai , 0);
  }

  void Control::checkBtn(int group, int id, bool state, bool check) {
    sUIActionInfo ai(group, id, 0, (state && check) ? ACTS_CHECKED : 0, 
      (state) ? "Wy³¹cz tryb away" : "W³¹cz tryb away", (state) ? ico::disable : ico::enable);
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