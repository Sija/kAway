/*
 *  kAway2
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: $
 */

#pragma once

namespace kAway2 {
  class Control {
    public:
      Control();
      ~Control();

      void Enable(std::string msg = "");
      void Disable(std::string msg = "");

      inline bool kAway2::Control::isEnabled() {
        return(this->isOn ? true : false);
      }

      inline std::string kAway2::Control::getAwayMsg() {
        return(this->awayMsg);
      }

      inline Stamina::Date64 kAway2::Control::getAwayTime() {
        return(this->awayTime);
      }

      void netListInit(int colCount = 3, int cfgGroup = kAway2::ui::cfgGroup, int dynActGroup = kAway2::dynAct::dynAct);
      void netListSetState(int cfgGroup = kAway2::ui::cfgGroup, int dynActGroup = kAway2::dynAct::dynAct);
      void netListGetState(int cfgGroup = kAway2::ui::cfgGroup, int dynActGroup = kAway2::dynAct::dynAct);

    protected:
      CtrlStatus *ctrlSt;

      bool isOn;
      std::string awayMsg;
      Stamina::Date64 awayTime;
      std::string ignoredUids;

    private:
      static UINT_PTR m_Timer;
      static kAway2::Control *sCtrl;

    public:
      inline void RememberInfo() {
        this->ctrlSt->RememberInfo();
      }

      inline void BackInfo() {
        this->ctrlSt->BackInfo();
      }

      inline std::list<itemNet> GetNets() {
        return(this->ctrlSt->nets);
      }

      inline std::string GetInfo(int net) {
        return(this->ctrlSt->GetInfo(net));
      }

      inline std::string LimitChars(std::string txt, int net, int s = 0) {
        return(this->ctrlSt->LimitChars(txt, net, s));
      }

      inline void ChangeStatus(int net, std::string txt) {
        this->ctrlSt->ChangeStatus(net, txt.c_str());
      }
  };
}