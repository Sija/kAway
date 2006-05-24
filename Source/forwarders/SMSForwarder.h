/**
 *  SMS Forwarder class
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @link         svn://kplugins.net/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once

#include "../stdafx.h"
#include "../main.h"
#include "../Helpers.h"

#include "../Control.h"
#include "../Format.h"

#include <konnekt/sms.h>
#include <stamina/timer.h>

namespace kAway2 {
  typedef std::deque<std::string> tMsgSenders;

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

      const unsigned int interval = sms + 1;
      const unsigned int gate = sms + 2;
      const unsigned int number = sms + 3;

      const unsigned int minMsgCount = sms + 4;
    }

    namespace tpl {
      const unsigned int sms = tpl + 5;
    }
  }

  class SMSForward : public Forwarder {
    protected:
      boost::shared_ptr<Stamina::TimerDynamic> timer;
      unsigned int receivedMsgCount;
      std::string lastMsgFrom;
      tMsgSenders msgSenders;

    public:
      SMSForward();
      ~SMSForward();

    public:
      void onISetCols();
      void onIPrepare();
      void onAction(int id, int code);

      void onEnable();
      void onDisable();
      void onNewMsg(int cnt, cMessage *msg);

      void send();
  };
}