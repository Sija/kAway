/*
 *  kAway2 Control class
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: Control.h 26 2006-01-04 04:46:47Z sija $
 */

#pragma once

namespace kAway2 {
  struct sCnt {
    bool ignored;
    int historySess;
    int lastMsgRcvTime;

    // konstruktor
    sCnt(bool _ignored = false, int _historySess = 0, int _lastMsgRcvTime = 0) : 
      ignored(_ignored), historySess(_historySess), lastMsgRcvTime(_lastMsgRcvTime) { }
  };

  typedef std::map<int, sCnt> tCnts;

  class Control {
    public:
      Control();
      ~Control();

      void enable(std::string msg = "", int status = -1, bool silent = false);
      void disable(std::string msg = "", bool silent = false);

      void sendMsgTpl(int cnt, int tplId, std::string msgVar = "");

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

      static inline void Log(enDebugLevel level, const char * format, ...) {
        va_list ap;
        va_start(ap, format);
        Control::Log(level, format, ap);
        va_end(ap);
      }

      static inline void Log(const char * format, ...) {
        va_list ap;
        va_start(ap, format);
        Control::Log(DBG_LOG, format, ap);
        va_end(ap);
      }

      static inline void Debug(const char * format, ...) {
        if(!debug) return;

        va_list ap;
        va_start(ap, format);
        Control::Log(DBG_DEBUG, format, ap);
        va_end(ap);
      }

    protected:
      bool isOn;
      bool muteStateSwitched;
      bool autoAway;
      int pluginsGroup;

      tCnts cntProps;
      std::string awayMsg;
      Stamina::Date64 *awayTime;

      static void Log(enDebugLevel level, const char * format, va_list ap);

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