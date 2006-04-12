/*
 *  Helpers
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005-2006 Sijawusz Pur Rahnama
 *
 *  $Id$
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

  bool isMsgWndOpen(int cntID) {
    return(Tabs::GetWindowState(cntID));
  }

  void showKNotify(char * text, int ico) {
    Ctrl->IMessage(&KNotify::sIMessage_notify(text, ico));
  }

  int findParentAction(int group, int id) {
    return(Ctrl->ICMessage(IMI_ACTION_FINDPARENT, (int) &sUIAction(group, id)));
  }

  void clearMru(const char * name) {
    sMRU mru;

    mru.name = name;
    mru.count = 0;
    mru.flags = MRU_GET_USETEMP;

    Ctrl->IMessage(&sIMessage_MRU(IMC_MRU_SET, &mru));
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