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
  struct ignoredUid {
    unsigned int cntId;
    unsigned int net;
    std::string uid;

    ignoredUid(int _cntId, int _net, std::string _uid): cntId(_cntId), 
      net(_net), uid(_uid) { }
  };

  typedef std::list<ignoredUid> tIgnoredUids;

  class Control {
    public:
      Control();
      ~Control();

      void enable(std::string msg = "", int status = -1, bool silent = false);
      void disable(std::string msg = "", bool silent = false);

      bool isIgnoredUid(int cntId);
      void addIgnoredUid(int cntId);
      void removeIgnoredUid(int cntId);

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

      inline void setStatusCtrl(Status *handle) {
        this->sCtrl = handle;
      }

      inline std::string getAwayMsg() {
        return(this->awayMsg);
      }

      inline Stamina::Date64 *getAwayTime() {
        return(this->awayTime);
      }

      void sendMsgTpl(int cnt, int tplId, std::string msgVar = "");

      std::map<int, int> msgRcvTimes;

    protected:
      bool isOn;
      bool muteStateSwitched;
      bool autoAway;

      std::string awayMsg;
      Stamina::Date64 *awayTime;
      tIgnoredUids ignoredUids;

      static void Log(enDebugLevel level, const char * format, va_list ap);

      void checkBtn(int group, int id, bool state, bool check = false);
      void checkBtn(int group, int id, int cnt, bool state, bool check = true);

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