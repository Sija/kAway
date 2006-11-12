/**
  *  kAway2 Control class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "kAway2.h"

#include "Helpers.h"
#include "IMController.h"
#include "MRU.h"
#include "NetList.h"
#include "Format.h"
#include "FormattedStatus.h"
#include "Message.h"
#include "AwayWnd.h"

namespace kAway2 {
  class Controller : public IMController<Controller>, boost::signals::trackable {
    friend class IMController<Controller>;

  public:
    typedef std::deque<cMessage*> tMsgQueue;

    enum enAutoMsgTpl {
      tplEnable = cfg::tpl::enable,
      tplDisable = cfg::tpl::disable,
      tplReply = cfg::tpl::reply
    };

    struct sCnt {
      bool ignored;
      int historySess;
      __int64 lastMsgTime;
      tMsgQueue msgQueue;

      // konstruktor
      sCnt(bool _ignored = false, int _historySess = 0, int _lastMsgTime = 0) : 
        ignored(_ignored), historySess(_historySess), lastMsgTime(_lastMsgTime) { }
    };

    typedef std::map<tCntId, sCnt> tCnts;

  protected:
    inline Controller() : isOn(false), muteStateSwitched(false), autoAway(false), pluginsGroup(0) {
      this->addStaticValue(IM_PLUG_NET, net);
      this->addStaticValue(IM_PLUG_TYPE, IMT_UI | IMT_CONFIG | IMT_ALLMESSAGES);
      this->addStaticValue(IM_PLUG_VERSION, 0);
      this->addStaticValue(IM_PLUG_SIG, (int) sig);
      this->addStaticValue(IM_PLUG_CORE_V, (int) "W98");
      this->addStaticValue(IM_PLUG_UI_V, 0);
      this->addStaticValue(IM_PLUG_NAME, (int) "kAway2");
      this->addStaticValue(IM_PLUG_NETNAME, 0);
      this->addStaticValue(IM_PLUG_PRIORITY, PLUGP_HIGHEST);

      this->registerObserver(IM_UI_PREPARE, boost::bind(&Controller::onPrepare, this, _1));
      this->registerObserver(IM_SETCOLS, boost::bind(&Controller::onSetCols, this, _1));
      this->registerObserver(IM_UIACTION, boost::bind(&Controller::onAction, this, _1));
      this->registerObserver(IM_MSG_RCV, boost::bind(&Controller::onMsgRcv, this, _1));
      this->registerObserver(IM_BEFOREEND, boost::bind(&Controller::onEnd, this, _1));
      this->registerObserver(IM_STATUSCHANGE, boost::bind(&Controller::onStatusChange, this, _1));
      this->registerObserver(IM_ALLPLUGINSINITIALIZED, boost::bind(&Controller::onPluginsLoaded, this, _1));
      this->registerObserver(IM_AWAY, boost::bind(&Controller::onAway, this, _1));
      this->registerObserver(IM_BACK, boost::bind(&Controller::onBack, this, _1));

      /* API */
      this->registerObserver(api::isEnabled, boost::bind(&Controller::apiEnabled, this, _1));
      this->registerObserver(api::enable, boost::bind(&Controller::apiEnable, this, _1));
      this->registerObserver(api::disable, boost::bind(&Controller::apiDisable, this, _1));
      this->registerObserver(api::isIgnored, boost::bind(&Controller::apiIgnored, this, _1));
      this->registerObserver(api::isAutoAway, boost::bind(&Controller::apiAutoAway, this, _1));
      this->registerObserver(api::ignore, boost::bind(&Controller::apiIgnore, this, _1));
      this->registerObserver(api::showAwayWnd, boost::bind(&Controller::apiShowAwayWnd, this, _1));
    }

  public:
    /* IMessage events */
    void onPrepare(Controller* self);
    void onSetCols(Controller* self);
    void onAction(Controller* self);
    void onMsgRcv(Controller* self);
    void onEnd(Controller* self);

    void inline onStatusChange(Controller* self) {
      if (this->isEnabled() && !this->isAutoAway()) {
        statusCtrl->actionHandle(this->getIM());
      }
    }

    void inline onPluginsLoaded(Controller* self) {
      if (int oldId = Helpers::pluginExists(plugsNET::kaway)) {
        Ctrl->IMessage(&sIMessage_plugOut(oldId, "kAway2 jest nastêpc¹ wtyczki K.Away :)",
          sIMessage_plugOut::erNo, sIMessage_plugOut::euNowAndOnNextStart));
        return this->setFailure();
      }
      if (int ggCrypt = Helpers::pluginExists(plugsNET::ggcrypt)) {
        Ctrl->IMessage(&sIMessage_plugOut(ggCrypt, "Wtyczka GG Crypt jest przestarza³a, przy czym\n"
          "nie pozwala na poprawne dzia³anie wtyczki kAway2.",
          sIMessage_plugOut::erNo, sIMessage_plugOut::euNowAndOnNextStart));
        return this->setFailure();
      }
      this->setSuccess();
    }

    void inline onAway(Controller* self) {
      if (!this->isEnabled() && GETINT(cfg::autoAwaySync)) {
        this->setAutoAway(true);
        this->enable(GETSTRA(cfg::tpl::autoAway), GETINT(cfg::status::onAutoAwaySt), true);
      }
    }

    void inline onBack(Controller* self) {
      if (this->isEnabled() && this->isAutoAway()) {
        this->disable("", true);
        this->setAutoAway(false);
      }
    }

    /* API */

    void inline apiEnabled(Controller* self) {
      this->setReturnCode(this->isEnabled());
    }

    void inline apiEnable(Controller* self) {
      sIMessage_2params* msg = this->getIM();

      logDebug("Remote API Call [enable]: from = %s, msg = %s, status = %i", 
        Helpers::getPlugName(msg->sender), nullChk((char*)msg->p1), msg->p2);
      this->setReturnCode(this->enable((char*)msg->p1, msg->p2));
    }

    void inline apiDisable(Controller* self) {
      sIMessage_2params* msg = this->getIM();

      logDebug("Remote API Call [disable]: from = %s, msg = %s", 
        Helpers::getPlugName(msg->sender), nullChk((char*)msg->p1));
      this->setReturnCode(this->disable((char*)msg->p1));
    }

    void inline apiIgnored(Controller* self) {
      this->setReturnCode(this->cntProp(this->getIM()->p1)->ignored);
    }

    void inline apiAutoAway(Controller* self) {
      this->setReturnCode(this->isAutoAway());
    }

    void inline apiIgnore(Controller* self) {
      sIMessage_2params* msg = this->getIM();

      logDebug("Remote API Call [ignore]: from = %s, cnt = %i, ignore = %s", 
        Helpers::getPlugName(msg->sender), msg->p1, btoa((bool)msg->p2));
      if (this->isEnabled()) {
        this->cntProp(msg->p1)->ignored = (bool) msg->p2;
      }
    }

    void inline apiShowAwayWnd(Controller* self) {
      logDebug("Remote API Call [showAwayWnd]: from = %s",
        Helpers::getPlugName(this->getIM()->sender));
      wnd->show();
    }

    bool enable(const StringRef& msg = "", int status = 0, bool silent = false);
    bool disable(const StringRef& msg = "", bool silent = false);

    void sendMsgTpl(int cnt, enAutoMsgTpl tpl, const StringRef& msgVal = "");
    void showKNotify(const char * text, int ico = ico::logoSmall);

    inline void fromWnd(bool state) {
      this->isFromWnd = state;
    }

    inline bool isMuteSwitched() {
      return this->muteStateSwitched;
    }

    inline bool isEnabled() {
      return this->isOn;
    }

    inline bool isAwayMsgSet() {
      return this->awayMsg.length() ? true : false;
    }

    inline bool isAutoAway() {
      return this->autoAway;
    }

    inline void setAutoAway(bool state) {
      this->autoAway = state;
    }

    inline int getPluginsGroup() {
      return this->pluginsGroup;
    }

    const StringRef& getAwayMsg() const {
      return this->awayMsg;
    }

    inline Stamina::Date64 getAwayTime() {
      return this->awayTime;
    }

    inline tCnts getCnts() {
      return this->cntProps;
    }

    inline sCnt* cntProp(int id) {
      return &this->cntProps[Ctrl->DTgetID(DTCNT, id)];
    }

    inline void addMsg2CntQueue(int cnt, cMessage *msg) {
      this->cntProp(cnt)->msgQueue.push_front(messageDuplicate(msg));
    }

    inline bool isCntSaved(int cnt) {
      return this->cntProps.find(cnt) != this->cntProps.end();
    }

  protected:
    bool isOn;
    bool muteStateSwitched;
    bool autoAway;
    bool isFromWnd;
    int pluginsGroup;

    tCnts cntProps;
    String awayMsg;
    Stamina::Date64 awayTime;

    void switchBtns(bool state);

  public:
    oNetList autoReplyList;
    oNetList statusList;
    oFormattedStatus statusCtrl;
    oAwayWnd wnd;
    oMRU mruList;
  };
}

#endif // __CONTROLLER_H__