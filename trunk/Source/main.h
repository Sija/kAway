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
  const unsigned int net = 667;
  const char poweredBy[64] = "Louis Armstrong and The Presage";

  namespace ui {
    const unsigned int ui = net * 1000;

    const unsigned int cfgGroup = ui + 1;
    const unsigned int cntCfgGroup = ui + 2;

    const unsigned int replyCfgGroup = ui + 3;
    const unsigned int forwardCfgGroup = ui + 3;
    const unsigned int smsCfgGroup = ui + 4;
    const unsigned int emailCfgGroup = ui + 5;
  }

  namespace ico {
    const unsigned int ico = net * 1000 + 100;
 
    const unsigned int logoSmall = ico + 1;
    const unsigned int logoBig = ico + 2;

    const unsigned int enabled = ico + 3;
    const unsigned int disabled = ico + 4;
  }

  namespace act {
    const unsigned int act = net * 1000 + 200;
  }

  namespace cfg {
    const unsigned int cfg = net * 1000 + 300;

    const unsigned int autoAwaySync = cfg + 1;
    const unsigned int useKNotify = cfg + 2;
    const unsigned int ircCmds = cfg + 3;
    const unsigned int disableOnMsgSend = cfg + 4;
    const unsigned int saveToHistory = cfg + 5;
    // const unsigned int notifyWhenInvisible = cfg + 6;

    const unsigned int notifyOnEnable = cfg + 6;
    const unsigned int notifyOnDisable = cfg + 7;
    const unsigned int notifyOnReply = cfg + 8;

    const unsigned int btnInMainWindow = cfg + 9;
    const unsigned int btnInCntWindow = cfg + 10;
    const unsigned int btnInTrayMenu = cfg + 11;

    const unsigned int replyMinInterval = cfg + 12;
    const unsigned int replyUseHtml = cfg + 13;
    const unsigned int replyMagicKey = cfg + 14;
    const unsigned int replyMagicKeyNotifyMethod = cfg + 15;

    const unsigned int smsInterval = cfg + 16;
    const unsigned int smsGate = cfg + 17;
    const unsigned int smsNumber = cfg + 18;

    const unsigned int emailInterval = cfg + 19;
    const unsigned int emailUseHtml = cfg + 20;
    const unsigned int emailAddress = cfg + 21;
    const unsigned int emailSubject = cfg + 22;

    const unsigned int smtpHost = cfg + 23;
    const unsigned int smtpPort = cfg + 24;
    const unsigned int smtpLogin = cfg + 25;
    const unsigned int smtpPassword = cfg + 26;

    const unsigned int forwardInterval = cfg + 27;
    const unsigned int forwardNet = cfg + 28;
    const unsigned int forwardTo = cfg + 29;

    const unsigned int netChange = cfg + 30;

    namespace tpl {
      const unsigned int tpl = cfg + 100;

      const unsigned int enable = tpl + 1;
      const unsigned int disable = tpl + 2;

      const unsigned int reply = tpl + 3;
      const unsigned int forward = tpl + 4;

      const unsigned int sms = tpl + 5;
      const unsigned int email = tpl + 6;

      const unsigned int status = tpl + 7;
    }
  }

  // dynamiczne akcje
  namespace dynAct {
    const int dynAct = net * 1000 + 500;
  }
}