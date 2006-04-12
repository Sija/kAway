/*
 *  kAway2 Helpers
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: Helpers.h 26 2006-01-04 04:46:47Z sija $
 */

#pragma once

std::string itos(int i, int radix = 10) {
  char buff[64]; _itoa(i, buff, radix);
  return(buff);
}

char * btoa(bool value) {
  return(value ? "yes" : "no");
}

std::string btos(bool value) {
  return(btoa(value));
}

char * nullChk(const std::string& value) {
  return(value.length() ? value.c_str() : "(none)");
}

char * nullChk(char * value) {
  return(strlen(value) ? value : "(none)");
}

namespace kAway2 {
  namespace Helpers {
    std::string icon32(int ico) {
      std::string buff = "reg://IML32/" + itos(ico) + ".ico";
      return(buff);
    }

    std::string icon16(int ico) {
      std::string buff = "reg://IML16/" + itos(ico) + ".ico";
      return(buff);
    }

    std::string trim(std::string txt) {
      CStdString buff(txt); txt = buff.Trim();
      return(txt);
    }

    int altCfgVal(int cntId, int colId, bool isBool = true) {
      if (isBool)
        return((GETINT(colId) && (GETCNTI(cntId, colId) < 2) || 
          (!GETINT(colId) && (GETCNTI(cntId, colId) == 1))) ? true : false);
      else
        return((GETCNTI(cntId, colId) >= 0) ? GETCNTI(cntId, colId) : GETINT(colId));
    }

    const char * altCfgStrVal(int cntId, int colId) {
      return(strlen(GETCNTC(cntId, colId)) ? GETCNTC(cntId, colId) : GETSTRA(colId));
    }

    void UIActionCall(int group, int act, int cntID = 0) {
      Ctrl->ICMessage(IMI_ACTION_CALL, 
        (int) &sUIActionNotify_2params(sUIAction(group, act, cntID), ACTN_ACTION, 0, 0), 0);
    }

    bool isMsgWndOpen(int cntID) {
      return(Tabs::GetWindowState(cntID));
    }

    void showKNotify(char * text, int ico) {
      if (!GETINT(cfg::useKNotify)) return;
      Ctrl->IMessage(&KNotify::sIMessage_notify(text, ico));
    }

    void addItemToHistory(cMessage* msg, int cnt, const char * dir, std::string name, int session = 0) {
      sHISTORYADD item;

      item.cnt = cnt;
      item.m = msg;
      item.dir = dir;
      item.name = name.c_str();
      item.session = session;

      Ctrl->ICMessage(IMI_HISTORY_ADD, (int) &item);
    }

    int getPluginsGroup() {
      return(Ctrl->ICMessage(IMI_GETPLUGINSGROUP));
    }

    int findParentAction(int group, int id) {
      return(Ctrl->ICMessage(IMI_ACTION_FINDPARENT, (int) &sUIAction(group, id)));
    }

    void chgBtn(int group, int id, int cnt, const char * name = 0, int ico = 0, int flags = -1) {
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

    void chgBtn(int group, int id, const char * name, int ico = 0, int flags = 0) {
      UIActionSet(sUIActionInfo(group, id, 0, flags, (char*) name, ico));
    }

    void clearMru(const char * name) {
      sMRU mru;

      mru.name = name;
      mru.count = 0;
      mru.flags = MRU_GET_USETEMP;

      Ctrl->IMessage(&sIMessage_MRU(IMC_MRU_SET, &mru));
    }
  }
}