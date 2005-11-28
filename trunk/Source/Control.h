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
  class Control {
    public:
      Control();
      ~Control();

      void Enable(std::string msg = "");
      void Disable(std::string msg = "");

      void Control::Log(enDebugLevel level, const char *format, ...);
      void Control::Log(const char *format, ...);
      void Control::Debug(const char *format, ...);

      inline bool isEnabled() {
        return(this->isOn ? true : false);
      }

      inline void setStatusCtrl(CtrlStatus *handle) {
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
      std::string ignoredUids;

      void Control::Log(enDebugLevel level, const char *format, va_list ap);

    private:
      CtrlStatus *sCtrl;
      static UINT_PTR m_Timer;
  };

  Control *pCtrl = NULL;
  CtrlStatus *sCtrl = NULL;
}