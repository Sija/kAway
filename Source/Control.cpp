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

kAway2::Control *kAway2::Control::sCtrl = NULL;
UINT_PTR kAway2::Control::m_Timer = NULL;

namespace kAway2 {
  Control::Control() {
    // this->ctrlSt = new kAway2::CtrlStatus();
    // this->ctrlSt->GetNets(kAway2::cfg::reply::netChange);

    this->awayMsg = "";
    this->awayTime = new Stamina::Date64(false);
    this->ignoredUids = "";
    this->isOn = false;
  }

  Control::~Control() {
    delete this->awayTime;
    this->awayTime = NULL;

    this->awayMsg = "";
    this->ignoredUids = "";
    this->isOn = false;
  }

  void Control::Enable(std::string msg) {
    if (!this->isOn) {
      // this->ctrlSt->RememberInfo();

      this->awayMsg = msg;
      this->awayTime->now();
      this->isOn = true;
    }
  }

  void Control::Disable(std::string msg) {
    if (this->isOn) {
      // this->ctrlSt->BackRemeberedInfo();

      this->awayMsg = "";
      this->awayTime->clear();
      this->ignoredUids = "";
      this->isOn = false;
    }
  }
}