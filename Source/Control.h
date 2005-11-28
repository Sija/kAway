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

      inline bool isEnabled() {
        return(this->isOn ? true : false);
      }

      inline std::string getAwayMsg() {
        return(this->awayMsg);
      }

      inline Stamina::Date64 getAwayTime() {
        return(this->awayTime);
      }

    protected:
      // CtrlStatus *ctrlSt;

      bool isOn;
      std::string awayMsg;
      Stamina::Date64 *awayTime;
      std::string ignoredUids;

    private:
      static UINT_PTR m_Timer;
      static kAway2::Control *sCtrl;
  };

  Control *pCtrl = NULL;
}