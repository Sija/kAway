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
#include "stdafx.h"

/*
 *  Integer -> String conversion
 */

std::string itos(int i, int radix = 10);

/*
 *  Bool -> Human readable string
 */

char * btoa(bool value);
std::string btos(bool value);

/*
 *  NULL -> Human readable string
 */

char * nullChk(const std::string& value);
char * nullChk(char * value);

/*
 *  Logging helpers, just a little short cut ;>
 */

void log(enDebugLevel level, const char * format, va_list ap);
void log(const char * format, ...);
void logDebug(const char * format, ...);

/*
 *  Various helpers
 */

namespace Helpers {
  std::string icon16(int ico);
  std::string icon32(int ico);

  std::string trim(std::string txt);

  int altCfgVal(int cntId, int colId, bool isBool = true);
  const char * altCfgStrVal(int cntId, int colId);

  int getPluginsGroup();
  int pluginExists(int net, int type = IMT_ALL);
  const char * getPlugName(int plugID);

  void UIActionCall(int group, int act, int cntID = 0);
  bool isMsgWndOpen(int cntID);
  void showKNotify(char * text, int ico);
  int findParentAction(int group, int id);
  void clearMru(const char * name);

  void addItemToHistory(cMessage* msg, int cnt, const char * dir, std::string name, int session = 0);

  void chgBtn(int group, int id, int cnt, const char * name = 0, int ico = 0, int flags = -1);
  void chgBtn(int group, int id, const char * name, int ico = 0, int flags = 0);
}