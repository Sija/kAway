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
    this->muteStateSwitched = false;
    this->autoAway = false;
  }

  Control::~Control() {
    delete this->awayTime;
    this->awayTime = NULL;

    this->ignoredUids.clear();
    this->msgRcvTimes.clear();
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
      Helpers::UIActionCall(ICMessage(IMI_GETPLUGINSGROUP), kSound::action::mute);
      this->muteStateSwitched = true;
    }

    this->checkBtn(ICMessage(IMI_GETPLUGINSGROUP), ui::powerInMainWnd, true);
    this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, true, true);
    this->checkBtn(IMIG_TRAY, ui::powerInTrayMenu, true);

    int count = IMessage(IMC_CNT_COUNT);
    for (int i = 0; i < count; i++) {
      if (IMessage(IMI_MSG_WINDOWSTATE, 0, 0, i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onEnable) && !silent && !this->isIgnoredUid(i)) {
          this->sendMsgTpl(i, cfg::tpl::enable);
        }
        this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, i, true);
      }
    }

    Helpers::showKNotify("Tryb away zosta³ <b>w³¹czony<b>", ico::enable);
    this->Log("[Control::enable()]: msg = %s, silent = %s", 
      (msg.length() ? msg.c_str() : "(none)"), btoa(silent));
  }

  void Control::disable(std::string msg, bool silent) {
    if (!this->isOn) return;

    if (this->sCtrl) {
      this->sCtrl->fCtrl->clearVars();
      this->sCtrl->restoreInfo();
    }

    if (this->muteStateSwitched) {
      Helpers::UIActionCall(ICMessage(IMI_GETPLUGINSGROUP), kSound::action::mute);
    }

    this->checkBtn(ICMessage(IMI_GETPLUGINSGROUP), ui::powerInMainWnd, false);
    this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, false, true);
    this->checkBtn(IMIG_TRAY, ui::powerInTrayMenu, false);

    int count = IMessage(IMC_CNT_COUNT);
    for (int i = 0; i < count; i++) {
      if (IMessage(IMI_MSG_WINDOWSTATE, 0, 0, i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onDisable) && !silent && !this->isIgnoredUid(i)) {
          this->sendMsgTpl(i, cfg::tpl::disable, msg);
        }
        this->checkBtn(IMIG_MSGTB, ui::powerInCntWnd, i, false);
      }
    }

    Helpers::showKNotify("Tryb away zosta³ <b>wy³¹czony<b>", ico::disable);
    this->Log("[Control::disable()]: msg = %s, silent = %s", 
      (msg.length() ? msg.c_str() : "(none)"), btoa(silent));

    this->awayMsg = "";
    this->awayTime->clear();
    this->ignoredUids.clear();
    this->msgRcvTimes.clear();
    this->isOn = false;
    this->muteStateSwitched = false;
  }

  void Control::sendMsgTpl(int cnt, int tplId, std::string msgVar) {
    int net = GETCNTI(cnt, CNT_NET);

    if ((sCtrl->getActualStatus(net) == ST_HIDDEN && !Helpers::altCfgVal(cnt, cfg::reply::whenInvisible)) || 
      !lCtrl::reply->getNetState(net) || !lCtrl::reply->isConnected(net))
      return;

    std::string ext, uid(GETCNTC(cnt, CNT_UID));
    ext = SetExtParam(ext, "kA2AutoMsg", "true");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    bool isReply = (tplId == cfg::tpl::reply);
    Format *format = new Format;

    format->addVar("display", GETCNTC(cnt, CNT_DISPLAY));
    format->addVar("name", GETCNTC(cnt, CNT_NAME));
    format->addVar("nick", GETCNTC(cnt, CNT_NICK));
    format->addVar("surname", GETCNTC(cnt, CNT_SURNAME));
    format->addVar("date", this->awayTime->strftime(GETSTRA(cfg::dateFormat)));
    format->addVar("time", this->awayTime->strftime(GETSTRA(cfg::timeFormat)));
    format->addVar("msg", (tplId == cfg::tpl::disable) ? msgVar : this->awayMsg);

    this->Debug("[Control::sendMsgTpl()]: tpl.id = %i, msg.net = %i, msg.uid = %s", 
      tplId, net, uid.c_str());

    Message::send(cnt, Helpers::trim(format->parse(Helpers::altCfgStrVal(cnt, tplId))), 
      MT_MESSAGE, ext, Helpers::altCfgVal(cnt, cfg::reply::useHtml), 
      (/*!isReply && */Helpers::altCfgVal(cnt, cfg::reply::showInWnd)));

    /*
    if (Helpers::altCfgVal(cnt, cfg::reply::showInWnd) && isReply) {
      Message::send("", uid, net, Helpers::trim(format->parse(GETSTRA(tplId))), 
        MT_MESSAGE, ext, Helpers::altCfgVal(cnt, cfg::reply::useHtml));
    }*/
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

    ICMessage(IMI_ACTION_SET, (int)&ai);
  }

  void Control::checkBtn(int group, int id, bool state, bool check) {
    sUIActionInfo ai(group, id, 0, (state && check) ? ACTS_CHECKED : 0, 
      (state) ? "Wy³¹cz tryb away" : "W³¹cz tryb away", (state) ? ico::disable : ico::enable);
    UIActionSet(ai);
  }

  bool Control::isIgnoredUid(int cntId) {
    for (tIgnoredUids::iterator it = this->ignoredUids.begin(); it != this->ignoredUids.end(); it++) {
      if (it->cntId == cntId) return(true);
    }
    return(false);
  }

  void Control::addIgnoredUid(int cntId) {
    std::string uid(GETCNTC(cntId, CNT_UID));
    int net = GETCNTI(cntId, CNT_NET);

    this->ignoredUids.push_back(ignoredUid(cntId, net, uid));
  }

  void Control::removeIgnoredUid(int cntId) {
    for (tIgnoredUids::iterator it = this->ignoredUids.begin(); it != this->ignoredUids.end(); it++) {
      if (it->cntId == cntId) {
        it = this->ignoredUids.erase(it); break;
      }
    }
  }
}