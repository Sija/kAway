/**
 *  Helpers
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
#include "Helpers.h"

/*
 *  Integer -> String conversion
 */

std::string itos(int i, int radix) {
  char buff[64]; _itoa(i, buff, radix);
  return(buff);
}

/*
 *  Bool -> Human readable string
 */

char * btoa(bool value) {
  return(value ? "yes" : "no");
}

std::string btos(bool value) {
  return(btoa(value));
}

/*
 *  NULL -> Human readable string
 */

char * nullChk(const std::string& value) {
  return(value.length() ? value.c_str() : "(none)");
}

char * nullChk(char * value) {
  return(strlen(value) ? value : "(none)");
}

/*
 *  Logging helpers, just a little short cut ;>
 */

void log(enDebugLevel level, const char * format, va_list ap) {
  if (Ctrl && Ctrl->DebugLevel(level))
    Ctrl->IMLOG_(level, format, ap);
}

void log(const char * format, ...) {
  va_list ap;
  va_start(ap, format);
	log(DBG_LOG, format, ap);
  va_end(ap);
}

void logDebug(const char * format, ...) {
  va_list ap;
  va_start(ap, format);
	log(DBG_DEBUG, format, ap);
  va_end(ap);
}

/*
 *  Various helpers
 */

namespace Helpers {
  std::string icon16(int ico) {
    return("reg://IML16/" + itos(ico) + ".ico");
  }

  std::string icon32(int ico) {
    return("reg://IML32/" + itos(ico) + ".ico");
  }

  std::string trim(std::string txt) {
    CStdString buff(txt); txt = buff.Trim();
    return(txt);
  }

  int altCfgVal(int cntId, int colId, bool isBool) {
    if (isBool)
      return((GETINT(colId) && (GETCNTI(cntId, colId) < 2) || 
        (!GETINT(colId) && (GETCNTI(cntId, colId) == 1))) ? true : false);
    else
      return((GETCNTI(cntId, colId) >= 0) ? GETCNTI(cntId, colId) : GETINT(colId));
  }

  const char * altCfgStrVal(int cntId, int colId) {
    return(strlen(GETCNTC(cntId, colId)) ? GETCNTC(cntId, colId) : GETSTRA(colId));
  }

  int getPluginsGroup() {
    return(Ctrl->ICMessage(IMI_GETPLUGINSGROUP));
  }

  int pluginExists(int net, int type) {
    return(Ctrl->ICMessage(IMC_FINDPLUG, net, type));
  }

  const char * getPlugName(int plugID) {
    return(SAFECHAR((char*) Ctrl->IMessageDirect(IM_PLUG_NAME, plugID)));
  }

  void UIActionCall(int group, int act, int cntID) {
    Ctrl->ICMessage(IMI_ACTION_CALL, 
      (int) &sUIActionNotify_2params(sUIAction(group, act, cntID), ACTN_ACTION, 0, 0), 0);
  }

#ifdef SHARED_TABLETKA_H
  bool isMsgWndOpen(int cntID) {
    return(Tabs::GetWindowState(cntID));
  }
#endif

  void showKNotify(char * text, int ico) {
    Ctrl->IMessage(&KNotify::sIMessage_notify(text, ico));
  }

#ifdef __STAMINA_TIME64__
  bool isToday(Stamina::Date64 date) {
    Stamina::Date64 today(true);

    if ((date.day != today.day) || (date.month != today.month) || (date.year != today.year)) {
      return(false);
    } else {
      return(true);
    }
  }
#endif

  int findParentAction(int group, int id) {
    return(Ctrl->ICMessage(IMI_ACTION_FINDPARENT, (int) &sUIAction(group, id)));
  }

  int subclassAction(int group, int id, int mask) {
    sUIActionInfo nfo(group, id);
    int prevOwner;

    nfo.mask = mask;
    nfo.txt = new char[100];
    nfo.txtSize = 99;

    UIActionGet(nfo);
    if (!(prevOwner = Ctrl->ICMessage(IMI_ACTION_GETOWNER, (int)&nfo.act))) {
      prevOwner = Ctrl->ICMessage(IMC_PLUG_ID, 0);
    }

    Ctrl->ICMessage(IMI_ACTION_REMOVE, (int)&nfo.act);
    Ctrl->ICMessage(IMI_ACTION, (int)&nfo);
    delete [] nfo.txt;

    return(prevOwner);
  }

  void addItemToHistory(cMessage* msg, int cnt, const char * dir, std::string name, int session) {
    sHISTORYADD item;

    item.cnt = cnt;
    item.m = msg;
    item.dir = dir;
    item.name = name.c_str();
    item.session = session;

    Ctrl->ICMessage(IMI_HISTORY_ADD, (int) &item);
  }

  void chgBtn(int group, int id, int cnt, const char * name, int ico, int flags) {
    sUIActionInfo ai;

    if (name) ai.txt = (char*) name;
    if (ico) ai.p1 = ico;
    if (flags >= 0) {
      ai.status = flags;
      ai.statusMask = -1;
    }

    ai.act = sUIAction(group, id, cnt);
    ai.mask = (name ? UIAIM_TXT : 0) | (ico ? UIAIM_P1 : 0) | (flags >= 0 ? UIAIM_STATUS : 0);

    UIActionSet(ai);
  }

  void chgBtn(int group, int id, const char * name, int ico, int flags) {
    UIActionSet(sUIActionInfo(group, id, 0, flags, (char*) name, ico));
  }
}