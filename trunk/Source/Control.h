/*
 *  kAway2 Control class
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005-2006 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  struct sCnt {
    bool ignored;
    int historySess;
    __int64 lastMsgTime;

    // konstruktor
    sCnt(bool _ignored = false, int _historySess = 0, int _lastMsgTime = 0) : 
      ignored(_ignored), historySess(_historySess), lastMsgTime(_lastMsgTime) { }
  };

  typedef std::map<int, sCnt> tCnts;

  class Control {
    public:
      Control();
      ~Control();

      bool enable(std::string msg = "", int status = 0, bool silent = false);
      bool disable(std::string msg = "", bool silent = false);

      void sendMsgTpl(int cnt, int tplId, std::string msgVar = "");
      void showKNotify(const char * text, int ico = ico::logoSmall);

      inline bool isMuteSwitched() {
        return(this->muteStateSwitched);
      }

      inline bool isEnabled() {
        return(this->isOn);
      }

      inline bool isAwayMsgSet() {
        return(this->awayMsg.length() ? true : false);
      }

      inline bool isAutoAway() {
        return(this->autoAway);
      }

      inline void setAutoAway(bool state) {
        this->autoAway = state;
      }

      inline void setStatusCtrl(Status *handle) {
        this->sCtrl = handle;
      }

      inline int getPluginsGroup() {
        return(this->pluginsGroup);
      }

      inline std::string getAwayMsg() {
        return(this->awayMsg);
      }

      inline Stamina::Date64 *getAwayTime() {
        return(this->awayTime);
      }

      inline sCnt *cntProp(int id) {
        return(&this->cntProps[Ctrl->DTgetID(DTCNT, id)]);
      }

    protected:
      bool isOn;
      bool muteStateSwitched;
      bool autoAway;
      int pluginsGroup;

      tCnts cntProps;
      std::string awayMsg;
      Stamina::Date64 *awayTime;

      void switchBtns(bool state);

    private:
      Status *sCtrl;
  };

  namespace lCtrl {
    NetList *status = NULL;

    NetList *reply = NULL;
    NetList *sms = NULL;
    NetList *email = NULL;
    NetList *forward = NULL;
  }

  Control *pCtrl = NULL;
  Status *sCtrl = NULL;
  AwayWnd *wCtrl = NULL;
}