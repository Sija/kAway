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
    unsigned int net;
    std::string uid;
  };

  class Control {
    public:
      Control();
      ~Control();

      void Enable(std::string msg = "");
      void Disable(std::string msg = "");

      bool isIgnoredUid(int net, std::string uid);
      void addIgnoredUid(int net, std::string uid);
      void removeIgnoredUid(int net, std::string uid);

      inline bool isEnabled() {
        return(this->isOn ? true : false);
      }

      inline bool isAwayMsgSet() {
        return(this->awayMsg.length() ? true : false);
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

      inline Stamina::Date64 getAwayTime() {
        return(this->awayTime);
      }

    protected:
      bool isOn;
      std::string awayMsg;
      Stamina::Date64 *awayTime;
      std::list<ignoredUid> ignoredUids;

      static void Control::Log(enDebugLevel level, const char * format, va_list ap);

    private:
      Status *sCtrl;
      static UINT_PTR m_Timer;
  };

  namespace lCtrl {
    netList *status = NULL;

    netList *reply = NULL;
    netList *sms = NULL;
    netList *email = NULL;
    netList *forward = NULL;
  }

  Control *pCtrl = NULL;
  Status *sCtrl = NULL;
}