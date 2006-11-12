/**
 *  SMS Forwarder class
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once

#ifndef __SMSFORWARDER_H__
#define __SMSFORWARDER_H__

#include "../kAway2.h"

#include "../Forwarder.h"
#include "../Message.h"

namespace kAway2 {
  namespace ui {
    namespace sms {
      const unsigned int sms = ui + 30;

      const unsigned int cfgGroup = sms + 1;
      const unsigned int number = sms + 2;
      const unsigned int gates = sms + 3;
    }
  }

  namespace cfg {
    namespace sms {
      const unsigned int sms = cfg + 300;

      const unsigned int gate = sms + 2;
      const unsigned int number = sms + 3;
      const unsigned int sig = sms + 6;

      const unsigned int isSummaryActive = sms + 4;
      const unsigned int isForwardActive = sms + 5;
    }

    namespace tpl {
      const unsigned int smsForward = tpl + 10;
      const unsigned int smsSummary = tpl + 5;
    }
  }

  class SMSForwarder : public Forwarder {
  public:
    SMSForwarder();
    // ~SMSForwarder();

  public:
    inline bool preSummary() {
      return strlen(GETSTRA(cfg::sms::number)) && strlen(GETSTRA(cfg::sms::gate));
    }

    inline bool preForward(int cnt, cMessage *msg) {
      return this->preSummary();
    }

    inline void setCfgCols() {
      this->cfgCols["tplSummary"] = cfg::tpl::smsSummary;
      this->cfgCols["tplForward"] = cfg::tpl::smsForward;
      this->cfgCols["cfgGroup"] = ui::sms::cfgGroup;

      this->cfgCols["isSummaryActive"] = cfg::sms::isSummaryActive;
      this->cfgCols["isForwardActive"] = cfg::sms::isForwardActive;
    }

    void send(const StringRef& msg);
    void onISetCols(Controller* pCtrl);
    void onIPrepare(Controller* pCtrl);
    void onAction(int id, int code);
  };
}

#endif // __SMSFORWARDER_H__