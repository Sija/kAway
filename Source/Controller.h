/**
  *  kAway2 Control class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2008 Sijawusz Pur Rahnama
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
#include "PlugController.h"
#include "MRU.h"
#include "NetList.h"
#include "Format.h"
#include "FormattedStatus.h"
#include "Message.h"
#include "AwayWnd.h"

// #include "FwdController.h"

namespace kAway2 {
  class Controller : public PlugController<Controller> {
  public:
    friend class PlugController<Controller>;

  public:
    /* Class version */
    STAMINA_OBJECT_CLASS_VERSION(Controller, PlugController, Version(1,2,0,0));

  public:
    typedef std::deque<cMessage*> tMsgQueue;

    enum enAutoMsgTpl {
      tplEnable = cfg::tpl::enable,
      tplDisable = cfg::tpl::disable,
      tplReply = cfg::tpl::reply
    };

    enum enAutoAwayType {
      typeDisabled,
      typeBasic,
      typeExtended
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
    Controller();

  protected:
    /* IMessage callback methods */
    void onPrepare(IMEvent& ev);
    void onAction(IMEvent& ev);
    void onMsgRcv(IMEvent& ev);
    void onEnd(IMEvent& ev);
    void onStatusChange(IMEvent& ev);
    void onPluginsLoaded(IMEvent& ev);
    void onExtAutoAway();
    void onAutoAway(IMEvent& ev);
    void onBack(IMEvent& ev);

    /* API callback methods */
    void apiEnabled(IMEvent& ev);
    void apiEnable(IMEvent& ev);
    void apiDisable(IMEvent& ev);
    void apiIgnored(IMEvent& ev);
    void apiAutoAway(IMEvent& ev);
    void apiIgnore(IMEvent& ev);
    void apiShowAwayWnd(IMEvent& ev);

  public:
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
      return this->autoAway != typeDisabled;
    }

    inline void setAutoAway(enAutoAwayType state) {
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
    enAutoAwayType autoAway;
    bool isFromWnd;
    int pluginsGroup;

    shared_ptr<TimerDynamic> extAutoAwayTimer;
    tCnts cntProps;
    String awayMsg;
    Date64 awayTime;

    void switchBtns(bool state);
    void changeStatus(int status, bool changeInfo = false);

  public:
    oNetList autoReplyList;
    oNetList statusList;
    oFormattedStatus statusCtrl;
    // oFwdController fwdCtrl;
    oAwayWnd wnd;
    oMRU mruList;
  };
}

#endif // __CONTROLLER_H__