/*
 *  kAway2
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  const unsigned int net = 667;
  const bool debug = true;
  const char poweredBy[64] = "Bonobo";

  namespace ui {
    const unsigned int ui = net * 1000;

    const unsigned int cfgGroup = ui + 1;
    const unsigned int cntCfgGroup = ui + 2;

    const unsigned int statusCfgGroup = ui + 3;
    const unsigned int replyCfgGroup = ui + 4;
    const unsigned int forwardCfgGroup = ui + 5;
    const unsigned int smsCfgGroup = ui + 6;
    const unsigned int emailCfgGroup = ui + 7;

    const unsigned int powerInMainWnd = ui + 8;
    const unsigned int powerInCntWnd = ui + 9;
    const unsigned int powerInTrayMenu = ui + 10;

    const unsigned int ignoreBtn = ui + 11;
    const unsigned int msgTbGrp = ui + 12;
  }

  namespace ico {
    const unsigned int ico = net * 1000 + 100;
 
    const unsigned int logoSmall = ico + 1;
    const unsigned int logoBig = ico + 2;

    const unsigned int enable = ico + 3;
    const unsigned int disable = ico + 4;

    const unsigned int ignore = ico + 5;
    const unsigned int unIgnore = ico + 6;

    const unsigned int status = ico + 7;
    const unsigned int reply = ico + 8;

    const unsigned int accept = ico + 9;
    const unsigned int cancel = ico + 10;

    const unsigned int help = ico + 11;
    const unsigned int trash = ico + 12;
  }

  namespace act {
    const unsigned int act = net * 1000 + 200;

    const unsigned int cfgGroupCheckCreate = act + 1;
    const unsigned int cfgGroupCheckDestroy = act + 2;

    const unsigned int replyCfgGroupCheckCreate = act + 3;
    const unsigned int replyCfgGroupCheckDestroy = act + 4;

    const unsigned int clearMru = act + 5;
    const unsigned int resetCntSettings = act + 6;
  }

  // dynamiczne akcje
  namespace dynAct {
    const unsigned int dynAct = net * 1000 + 300;

    const unsigned int reply = dynAct + 10;
    const unsigned int sms = dynAct + 20;
    const unsigned int email = dynAct + 30;
    const unsigned int forward = dynAct + 40;
    const unsigned int status = dynAct + 50;
  }

  namespace cfg {
    const unsigned int cfg = net * 1000 + 500;

    const unsigned int autoAwaySync = cfg + 1;
    const unsigned int useKNotify = cfg + 2;
    const unsigned int ircCmds = cfg + 3;
    const unsigned int disableOnMsgSend = cfg + 4;
    const unsigned int saveToHistory = cfg + 5;

    const unsigned int btnInMainWindow = cfg + 6;
    const unsigned int btnInCntWindow = cfg + 7;
    const unsigned int btnInTrayMenu = cfg + 8;

    const unsigned int dateFormat = cfg + 9;
    const unsigned int timeFormat = cfg + 10;

    const unsigned int mruSize = cfg + 11;
    const unsigned int muteOnEnable = cfg + 12;
    const unsigned int disableConfirmation = cfg + 13;

    const char historyFolder[16] = "kAway2";

    namespace tpl {
      const unsigned int tpl = cfg + 100;

      const unsigned int enable = tpl + 1;
      const unsigned int disable = tpl + 2;

      const unsigned int reply = tpl + 3;
      const unsigned int forward = tpl + 4;

      const unsigned int sms = tpl + 5;
      const unsigned int email = tpl + 6;

      const unsigned int status = tpl + 7;
      const unsigned int autoAway = tpl + 8;
    }

    namespace reply {
      const unsigned int reply = cfg + 200;

      const unsigned int onEnable = reply + 1;
      const unsigned int onDisable = reply + 2;
      const unsigned int onMsg = reply + 3;

      const unsigned int whenInvisible = reply + 4;
      const unsigned int minInterval = reply + 5;
      const unsigned int useHtml = reply + 6;

      const unsigned int magicKey = reply + 7;
      const unsigned int magicKeyNotifyMethod = reply + 8;

      const unsigned int netChange = reply + 9;
      const unsigned int showInWnd = reply + 10;
    }

    namespace sms {
      const unsigned int sms = cfg + 300;

      const unsigned int interval = sms + 1;
      const unsigned int gate = sms + 2;
      const unsigned int number = sms + 3;
    }

    namespace email {
      const unsigned int email = cfg + 400;

      const unsigned int interval = email + 1;
      const unsigned int useHtml = email + 2;
      const unsigned int address = email + 3;
      const unsigned int subject = email + 4;

      namespace smtp {
        const unsigned int smtp = cfg + 450;

        const unsigned int host = smtp + 1;
        const unsigned int port = smtp + 2;
        const unsigned int login = smtp + 3;
        const unsigned int password = smtp + 4;
      }
    }

    namespace forward {
      const unsigned int forward = cfg + 500;

      const unsigned int interval = forward + 1;
      const unsigned int net = forward + 2;
      const unsigned int to = forward + 3;
    }

    namespace status {
      const unsigned int status = cfg + 600;

      const unsigned int netChange = status + 1;
      const unsigned int whenInvisible = status + 2;

      const unsigned int changeOnEnable = status + 3;
      const unsigned int changeInfoOnEnable = status + 4;

      const unsigned int onEnableSt = status + 5;
      const unsigned int onAutoAwaySt = status + 6;
    }
  }
}