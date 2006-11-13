/**
  *  kAway2 Header
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

#ifndef __KAWAY2_H__
#define __KAWAY2_H__

namespace kAway2 {
  /*
   *  Values for {cfg::reply::minIntervalType} col
   */
  enum enMinIntType {
    intervalTypeRcvTime,
    intervalTypeSendTime,
    intervalTypeBoth
  };

  const char poweredBy[] = "planet boelex";
  const char sig[] = "KAWAY2";
  const unsigned int net = 667;

  namespace api {
    const unsigned int api = net * 1000 + IM_USER;

    const unsigned int isEnabled = api + 1;
    const unsigned int enable = api + 2;
    const unsigned int disable = api + 3;

    const unsigned int isIgnored = api + 4;
    const unsigned int ignore = api + 5;

    const unsigned int showAwayWnd = api + 6;

    const unsigned int isAway = api + 7;
    const unsigned int isBack = api + 8;
    const unsigned int isAutoAway = api + 9;
  }

  namespace ui {
    const unsigned int ui = net * 1000;

    const unsigned int cfgGroup = ui + 1;
    const unsigned int cntCfgGroup = ui + 2;

    const unsigned int statusCfgGroup = ui + 3;
    const unsigned int replyCfgGroup = ui + 4;

    const unsigned int powerInMainWnd = ui + 5;
    const unsigned int powerInCntWnd = ui + 6;
    const unsigned int powerInTrayMenu = ui + 7;

    const unsigned int ignoreBtn = ui + 8;
    const unsigned int msgTbGrp = ui + 9;
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

    const unsigned int msg = ico + 13;
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
    const unsigned int status = dynAct + 20;
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

    const char extParamName[] = "kA2AutoMsgTpl";
    const char historyFolder[] = "kAway2";
    const char mruName[] = "kA2_awayMsg";

    namespace wnd {
      const unsigned int wnd = cfg + 50;

      const unsigned int changeOnEnable = wnd + 1;
      const unsigned int changeInfoOnEnable = wnd + 2;
      const unsigned int muteOnEnable = wnd + 3;
      const unsigned int onEnableSt = wnd + 4;
    }

    namespace summary {
      const unsigned int summary = cfg + 75;

      const unsigned int interval = summary + 1;
      const unsigned int inAutoAway = summary + 2;
      const unsigned int minMsgCount = summary + 3;
    }

    namespace fwd {
      const unsigned int fwd = cfg + 85;

      const unsigned int inAutoAway = fwd + 1;
    }

    namespace tpl {
      const unsigned int tpl = cfg + 100;

      const unsigned int enable = tpl + 1;
      const unsigned int disable = tpl + 2;

      const unsigned int reply = tpl + 3;
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
      const unsigned int minIntervalType = reply + 11;
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

    namespace status {
      const unsigned int status = cfg + 600;

      const unsigned int netChange = status + 1;
      const unsigned int whenInvisible = status + 2;

      const unsigned int changeOnEnable = status + 3;
      const unsigned int changeInfoOnEnable = status + 4;

      const unsigned int onEnableSt = status + 5;
      const unsigned int onAutoAwaySt = status + 6;
      const unsigned int dotsAppend = status + 7;
      const unsigned int onAutoAwayChgOnlyIfOnline = status + 8;
    }
  }
}

#endif // __KAWAY2_H__