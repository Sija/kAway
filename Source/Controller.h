/**
  *  kAway2 Controller class
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

#include <konnekt/contrib/iController.h>

#include "kAway2.h"

#include "Helpers.h"
#include "FwdController.h"
#include "MRUConfigurable.h"
#include "NetList.h"
#include "Format.h"
#include "FormattedStatus.h"
#include "Message.h"
#include "AwayWnd.h"

namespace kAway2 {
  class MyMessageHandler : public SharedObject<iMessageHandler> {
  public:
    inline static MyMessageHandler* getInstance() {
      if (!_instance.isValid()) {
        _instance = new MyMessageHandler;
      }
      return _instance;
    }

  protected:
    MyMessageHandler() { }

  public:
    inline bool handlingMessage(enMessageQueue queue, Message* msg) {
      if ((msg->getType() != Message::typeMessage) || msg->getOneFlag(Message::flagAutomated)) {
        return false;
      }
      return true;
    }
    tMsgResult handleMessage(Message* msg, enMessageQueue queue, enPluginPriority priority);

  public:
    inline void attach() {
      IM im (IM::imcRegisterMessageHandler, this, priorityHighest);
      Ctrl->IMessage(&im);
    }
    inline void detach() {
      IM im (IM::imcUnregisterMessageHandler, this, priorityHighest);
      Ctrl->IMessage(&im);
    }

  protected:
    static SharedPtr<MyMessageHandler> _instance;
  };

  class ItemFallback : public Config::Item {
  public:
    ItemFallback(tColId col, tRowId row): Item(tableContacts, col, row) { }

  public:
    inline Item getParentItem() const {
      return Item(tableConfig, getID());
    }

  public:
    inline int to_i(tRowId row) const {
      int value = Item::to_i(row);

      if (value < 0) {
        return getParentItem().to_i();
      }
      return value;
    }
    inline int to_i() const {
      return to_i(getRow());
    }

    inline bool to_b(tRowId row) const {
      int parent_value = getParentItem().to_i();
      int value = Item::to_i(row);

      if ((parent_value && value < 2) || value == 1) {
        return true;
      }
      return false;
    }
    inline bool to_b() const {
      return to_b(getRow());
    }

    inline String to_s(tRowId row) const {
      String value = Item::to_s(row);

      if (value.empty()) {
        return getParentItem().to_s();
      }
      return value;
    }
    inline String to_s() const {
      return to_s(getRow());
    }
  };

  class Controller : public iController {
  public:
    friend class MyMessageHandler;

  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(Controller, iController, Version(1,3,0,0));

  public:
    typedef std::deque<MessageOld*> tMsgQueue;

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
      // tMsgQueue msgQueue;

      // konstruktor
      sCnt(bool _ignored = false, int _historySess = 0, int _lastMsgTime = 0) : 
        ignored(_ignored), historySess(_historySess), lastMsgTime(_lastMsgTime) { }
    };

    typedef std::map<tCntId, sCnt> tCnts;

  public:
    Controller();

  protected:
    /* IMessage callback methods */
    void _prepare(IMEvent& ev);
    void _prepareUI(IMEvent& ev);

    void onEnd(IMEvent& ev);
    void onPluginsLoaded(IMEvent& ev);
    void onExtAutoAway();
    void onAutoAway(IMEvent& ev);
    void onBack(IMEvent& ev);

  public:
    // actions
    void _handleCntGroup(ActionEvent& ev);
    void _handleMsgTb(ActionEvent& ev);
    void _handlePowerBtns(ActionEvent& ev);
    void _handleIgnoreBtn(ActionEvent& ev);
    void _clearMRU(ActionEvent& ev);
    void _resetGlobalSettings(ActionEvent& ev);
    void _resetContactSettings(ActionEvent& ev);

  public:
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
      return this->_active;
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
      return this->_plugins_group;
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
      return &this->cntProps[Ctrl->DTgetID(tableContacts, id)];
    }

    /*
    inline void addMsg2CntQueue(int cnt, Message *msg) {
      this->cntProp(cnt)->msgQueue.push_front(messageDuplicate(msg));
    }
    */

    inline bool isCntSaved(int cnt) {
      return this->cntProps.find(cnt) != this->cntProps.end();
    }

  protected:
    bool _active;
    bool muteStateSwitched;
    enAutoAwayType autoAway;
    bool isFromWnd;
    int _plugins_group;

    shared_ptr<TimerDynamic> extAutoAwayTimer;
    tCnts cntProps;
    String awayMsg;
    Date64 awayTime;

    void switchBtns(bool state);
    void changeStatus(int status, bool changeInfo = false);
    tMsgResult _parseMsg(Message* m);

  public:
    oNetList autoReplyList;
    oNetList statusList;
    oFormattedStatus statusCtrl;
    // oFwdController fwdCtrl;
    oAwayWnd wnd;
    oMRU mruList;
    MyMessageHandler* mh;
  };
}

#endif // __CONTROLLER_H__