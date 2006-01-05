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
  std::string __stdcall fGetAwayDateString(Format *fCtrl) {
    Stamina::Date64 *awayTime = pCtrl->getAwayTime();
    Stamina::Date64 today(true);

    if ((awayTime->day != today.day) || (awayTime->month != today.month) || (awayTime->year != today.year))
      return(awayTime->strftime(GETSTRA(cfg::dateFormat)));
    else
      return("");
  }

  Control::Control() {
    this->awayTime = new Stamina::Date64(false);
    this->pluginsGroup = Helpers::getPluginsGroup();

    this->isOn = false;
    this->muteStateSwitched = false;
    this->autoAway = false;
  }

  Control::~Control() {
    delete this->awayTime;
    this->awayTime = NULL;

    this->cntProps.clear();
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

    if (GETINT(cfg::muteOnEnable) && !GETINT(kSound::Cfg::mute)) {
      Helpers::UIActionCall(this->pluginsGroup, kSound::action::mute);
      this->muteStateSwitched = true;
    }

    this->switchBtns(true);

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 0; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onEnable) && !silent && !this->cntProp(i)->ignored) {
          this->sendMsgTpl(i, cfg::tpl::enable);
        }
      }
    }

    this->showKNotify("Tryb away zosta³ <b>w³¹czony<b>", ico::enable);
    this->Log("[Control::enable()]: msg = %s, silent = %s", 
      nullChk(msg), btoa(silent));
  }

  void Control::disable(std::string msg, bool silent) {
    if (!this->isOn) return;

    if (this->sCtrl) {
      this->sCtrl->fCtrl->clearVars();
      this->sCtrl->restoreInfo();
    }

    if (this->muteStateSwitched && GETINT(kSound::Cfg::mute)) {
      Helpers::UIActionCall(this->pluginsGroup, kSound::action::mute);
    }

    this->switchBtns(false);

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 0; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onDisable) && !silent && !this->cntProp(i)->ignored) {
          this->sendMsgTpl(i, cfg::tpl::disable, msg);
        }
      }
    }

    this->showKNotify("Tryb away zosta³ <b>wy³¹czony<b>", ico::disable);
    this->Log("[Control::disable()]: msg = %s, silent = %s", 
      nullChk(msg), btoa(silent));

    this->awayMsg = "";
    this->awayTime->clear();
    this->cntProps.clear();

    this->isOn = false;
    this->muteStateSwitched = false;
  }

  void Control::showKNotify(const char * text, int ico) {
    if (!GETINT(cfg::useKNotify) || !Helpers::pluginExists(plugsNET::knotify)) return;
    Helpers::showKNotify((char*) text, ico);
  }

  void Control::switchBtns(bool state) {
    int ico = (state) ? ico::disable : ico::enable;
    char * name = (state) ? "Wy³¹cz tryb away" : "W³¹cz tryb away";

    Helpers::chgBtn(IMIG_TRAY, ui::powerInTrayMenu, name, ico);
    Helpers::chgBtn(this->pluginsGroup, ui::powerInMainWnd, name, ico, 
      (state && (Helpers::findParentAction(IMIG_MAINWND, this->pluginsGroup) != IMIG_MAINTB)) ? ACTS_CHECKED : 0);
    Helpers::chgBtn(IMIG_MSGTB, ui::msgTbGrp, "kAway2", ico::logoSmall, 
      ACTR_INIT | ACTS_GROUP | (state ? ACTS_CHECKED : 0));

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 0; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        Helpers::chgBtn(IMIG_MSGTB, ui::msgTbGrp, i, "kAway2", ico::logoSmall, 
          ACTR_INIT | ACTS_GROUP | (state ? ACTS_CHECKED : 0));
      }
    }
    delete [] name;
  }

  void Control::sendMsgTpl(int cnt, int tplId, std::string msgVar) {
    int session, net = GETCNTI(cnt, CNT_NET);

    if (((sCtrl->getActualStatus(net) == ST_HIDDEN) && !Helpers::altCfgVal(cnt, cfg::reply::whenInvisible)) || 
      !lCtrl::reply->getNetState(net) || !lCtrl::reply->isConnected(net))
      return;

    std::string ext, uid(GETCNTC(cnt, CNT_UID));
    ext = SetExtParam(ext, "kA2AutoMsg", "true");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    Format *format = new Format;

    format->addVar("uid", uid);
    format->addVar("display", GETCNTC(cnt, CNT_DISPLAY));
    format->addVar("name", GETCNTC(cnt, CNT_NAME));
    format->addVar("nick", GETCNTC(cnt, CNT_NICK));
    format->addVar("surname", GETCNTC(cnt, CNT_SURNAME));
    format->addVar("date", fGetAwayDateString);
    format->addVar("time", this->awayTime->strftime(GETSTRA(cfg::timeFormat)));
    format->addVar("msg", (tplId == cfg::tpl::disable) ? msgVar : this->awayMsg);

    std::string body = Helpers::trim(format->parse(Helpers::altCfgStrVal(cnt, tplId)));
    cMessage msg = Message::prepare(uid, "", net, body, MT_MESSAGE, ext, 
      MF_SEND | (Helpers::altCfgVal(cnt, cfg::reply::useHtml) ? MF_HTML : 0));

    HWND hwndMsg = (HWND) UIGroupHandle(sUIAction(0, IMIG_MSGWND, cnt));
    if (!(session = (int) GetProp(hwndMsg, "MsgSession")))
      SetProp(hwndMsg, "MsgSession", (HANDLE) 1);

    Helpers::addItemToHistory(&msg, cnt, "messages", "", session);
    Message::send(&msg);
    /*
    if (Helpers::altCfgVal(cnt, cfg::reply::showInWnd))
       Message::insInMsgWnd(&msg, cnt);
    */

    this->Debug("[Control::sendMsgTpl()]: tpl.id = %i, msg.net = %i, msg.uid = %s", 
      tplId, net, uid.c_str());

    delete format; format = NULL;
  }
}