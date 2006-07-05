/**
 *  kAway2 Control class
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @link         svn://kplugins.net/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once
#include "Control.h"

namespace kAway2 {
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

  bool Control::enable(std::string msg, int status, bool silent) {
    if (this->isOn) return(false);

    this->awayMsg = msg;
    this->awayTime->now();
    this->isOn = true;

    if (this->sCtrl) {
      int chgStatus = GETINT(this->isFromWnd ? cfg::wnd::changeOnEnable : cfg::status::changeOnEnable);
      int chgInfo = GETINT(this->isFromWnd ? cfg::wnd::changeInfoOnEnable : cfg::status::changeInfoOnEnable);

      if (chgInfo) {
        this->sCtrl->fCtrl->addVar("date", this->awayTime->strftime(GETSTRA(cfg::dateFormat)));
        this->sCtrl->fCtrl->addVar("time", this->awayTime->strftime(GETSTRA(cfg::timeFormat)));
        this->sCtrl->fCtrl->addVar("msg", msg);
      }

      if (chgStatus || chgInfo) {
        this->sCtrl->rememberInfo();
        if (chgInfo)
          this->sCtrl->changeStatus(GETSTRA(cfg::tpl::status), 
            chgStatus ? (status ? status : GETINT(this->isFromWnd ? cfg::wnd::onEnableSt : cfg::status::onEnableSt)) : -1);
        else if (chgStatus)
          this->sCtrl->changeStatus(status ? status : GETINT(this->isFromWnd ? cfg::wnd::onEnableSt : cfg::status::onEnableSt));
      }
    }

    if (GETINT(this->isFromWnd ? cfg::wnd::muteOnEnable : cfg::muteOnEnable) && !GETINT(kSound::Cfg::mute)) {
      Helpers::UIActionCall(this->pluginsGroup, kSound::action::mute);
      this->muteStateSwitched = true;
    }

    this->switchBtns(true);
    this->fromWnd(false);

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 1; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onEnable) && !silent && !this->cntProp(i)->ignored) {
          this->sendMsgTpl(i, tplEnable);
        }
      }
    }

    this->showKNotify("Tryb away zosta³ <b>w³¹czony<b>", ico::enable);
    log("[Control::enable()]: msg = %s, silent = %s", nullChk(msg), btoa(silent));
    Ctrl->IMessage(api::isAway, NET_BROADCAST, -1, (int) msg.c_str(), status);

    fCtrl->onEnable();
    return(true);
  }

  bool Control::disable(std::string msg, bool silent) {
    if (!this->isOn) return(false);

    if (this->sCtrl) {
      this->sCtrl->fCtrl->clearVars();
      this->sCtrl->restoreInfo();
    }

    if (this->muteStateSwitched && GETINT(kSound::Cfg::mute)) {
      Helpers::UIActionCall(this->pluginsGroup, kSound::action::mute);
    }

    this->switchBtns(false);

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 1; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onDisable) && !silent && !this->cntProp(i)->ignored) {
          this->sendMsgTpl(i, tplDisable, msg);
        }
      }
    }

    for (tCnts::iterator it = this->cntProps.begin(); it != this->cntProps.end(); it++) {
      logDebug("[Control::disable().saved]: cnt = %i, ignored = %s", it->first, btoa(it->second.ignored));
      for (tMsgQueue::iterator it2 = it->second.msgQueue.begin(); it2 != it->second.msgQueue.end(); it2++) {
        logDebug("[Control::disable().saved.item]: msg = %s", (*it2)->body);
        messageFree((*it2), true);
      }
    }

    this->awayMsg = "";
    this->awayTime->clear();
    this->cntProps.clear();

    this->isOn = false;
    this->muteStateSwitched = false;

    this->showKNotify("Tryb away zosta³ <b>wy³¹czony<b>", ico::disable);
    log("[Control::disable()]: msg = %s, silent = %s", nullChk(msg), btoa(silent));
    Ctrl->IMessage(api::isBack, NET_BROADCAST, -1, (int) msg.c_str());

    fCtrl->onDisable();
    return(true);
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
    for (int i = 1; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        Helpers::chgBtn(IMIG_MSGTB, ui::msgTbGrp, i, "kAway2", ico::logoSmall, 
          ACTR_INIT | ACTS_GROUP | (state ? ACTS_CHECKED : 0));
      }
    }
    delete [] name;
  }

  void Control::sendMsgTpl(int cnt, enAutoMsgTpl tpl, std::string msgVal) {
    int session, net = GETCNTI(cnt, CNT_NET);

    if (((sCtrl->getActualStatus(net) == ST_HIDDEN) && !Helpers::altCfgVal(cnt, cfg::reply::whenInvisible)) || 
      !lCtrl::reply->getNetState(net) || !lCtrl::reply->isConnected(net))
      return;

    std::string ext, uid(GETCNTC(cnt, CNT_UID));
    ext = SetExtParam(ext, cfg::extParamName, itos(tpl));
    ext = SetExtParam(ext, MEX_ADDINFO, "kAway2");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    Format format;
    format.addVar("uid", uid);
    format.addVar("display", GETCNTC(cnt, CNT_DISPLAY));
    format.addVar("name", GETCNTC(cnt, CNT_NAME));
    format.addVar("nick", GETCNTC(cnt, CNT_NICK));
    format.addVar("surname", GETCNTC(cnt, CNT_SURNAME));
    format.addVar("date", Helpers::isToday(this->awayTime) ? "" : this->awayTime->strftime(GETSTRA(cfg::dateFormat)));
    format.addVar("time", this->awayTime->strftime(GETSTRA(cfg::timeFormat)));
    format.addVar("msg", (tpl == tplDisable) ? msgVal : this->awayMsg);

    /*
    std::string body = Helpers::trim(format.parse(Helpers::altCfgStrVal(cnt, tpl)));
    cMessage msg = Message::prepare(uid, "", net, body, MT_MESSAGE, ext, 
      MF_SEND | (Helpers::altCfgVal(cnt, cfg::reply::useHtml) ? MF_HTML : 0));

    HWND hwndMsg = (HWND) UIGroupHandle(sUIAction(0, IMIG_MSGWND, cnt));
    if (!(session = (int) GetProp(hwndMsg, "MsgSession")))
      SetProp(hwndMsg, "MsgSession", (HANDLE) 1);

    Helpers::addItemToHistory(&msg, cnt, "messages", "", session);
    Message::send(&msg);

    if (Helpers::altCfgVal(cnt, cfg::reply::showInWnd))
       Message::insInMsgWnd(&msg, cnt);
    */
    Message::send(cnt, Helpers::trim(format.parse(Helpers::altCfgStrVal(cnt, tpl))), 
      MT_MESSAGE, ext, Helpers::altCfgVal(cnt, cfg::reply::useHtml));

    logDebug("[Control::sendMsgTpl()]: msg.tpl = %i", tpl);
  }
}