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

//#include "FwdController.h"

//#include "Forwarders/CntForwarder.h"
//#include "Forwarders/SMSForwarder.h"

namespace kAway2 {
  class Controller : public IMController<Controller>, signals::trackable {
  public:
    friend class IMController<Controller>;
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

      this->registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&Controller::onPrepare), this));
      this->registerObserver(IM_SETCOLS, bind(resolve_cast0(&Controller::onSetCols), this));
      this->registerObserver(IM_UIACTION, bind(resolve_cast0(&Controller::onAction), this));
      this->registerObserver(IM_MSG_RCV, bind(resolve_cast0(&Controller::onMsgRcv), this));
      this->registerObserver(IM_BEFOREEND, bind(resolve_cast0(&Controller::onEnd), this));
      this->registerObserver(IM_STATUSCHANGE, bind(resolve_cast0(&Controller::onStatusChange), this));
      this->registerObserver(IM_ALLPLUGINSINITIALIZED, bind(resolve_cast0(&Controller::onPluginsLoaded), this));
      this->registerObserver(IM_AWAY, bind(resolve_cast0(&Controller::onAutoAway), this));
      this->registerObserver(IM_BACK, bind(resolve_cast0(&Controller::onBack), this));

      /* API */
      this->registerObserver(api::isEnabled, bind(resolve_cast0(&Controller::apiEnabled), this));
      this->registerObserver(api::enable, bind(resolve_cast0(&Controller::apiEnable), this));
      this->registerObserver(api::disable, bind(resolve_cast0(&Controller::apiDisable), this));
      this->registerObserver(api::isIgnored, bind(resolve_cast0(&Controller::apiIgnored), this));
      this->registerObserver(api::isAutoAway, bind(resolve_cast0(&Controller::apiAutoAway), this));
      this->registerObserver(api::ignore, bind(resolve_cast0(&Controller::apiIgnore), this));
      this->registerObserver(api::showAwayWnd, bind(resolve_cast0(&Controller::apiShowAwayWnd), this));
    }

  public:
    /* IMessage callback methods */
    void onPrepare();
    void onSetCols();
    void onAction();
    void onMsgRcv();
    void onEnd();
    void onStatusChange();
    void onPluginsLoaded();
    void onAutoAway();
    void onBack();

    /* API callback methods */
    void apiEnabled();
    void apiEnable();
    void apiDisable();
    void apiIgnored();
    void apiAutoAway();
    void apiIgnore();
    void apiShowAwayWnd();

    /* strictly Controller methods */
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

    inline Date64 getAwayTime() {
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
    Date64 awayTime;

    void switchBtns(bool state);

  public:
    oNetList autoReplyList;
    oNetList statusList;
    oFormattedStatus statusCtrl;
    //oFwdController fwdCtrl;
    oAwayWnd wnd;
    oMRU mruList;
  };
}

#endif // __CONTROLLER_H__