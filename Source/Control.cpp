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
  Control::Control() {
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

  void Control::Log(enDebugLevel level, const char *format, va_list ap) {
    if (Ctrl && Ctrl->DebugLevel(level))
      Ctrl->IMLOG_(level, format, ap);
  }

  void Control::Enable(std::string msg) {
    if (!this->isOn) {
      if(this->sCtrl)
        this->sCtrl->RememberInfo();

      this->awayMsg = msg;
      this->awayTime->now();
      this->isOn = true;
    }
  }

  void Control::Disable(std::string msg) {
    if (this->isOn) {
      if(this->sCtrl)
        this->sCtrl->BackRemeberedInfo();

      this->awayMsg = "";
      this->awayTime->clear();
      this->ignoredUids = "";
      this->isOn = false;
    }
  }
}