/**
 *  Status class
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

#ifndef __STATUS_H__
#define __STATUS_H__

#include "Helpers.h"
#include "NetList.h"

class Status : public iObject {
public:
  static const struct IM {
    static const unsigned int key = IM_USER + (668 * 1000) + 1;
    static const unsigned int infoCharLimit = key + 1;
  };

public:
  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(Status, iObject, Version(0,1,0,0));

  struct sInfo {
    unsigned int net;
    unsigned int st;
    String info;

    sInfo(int _net, int _st, const StringRef& _info = "") : 
      net(_net), st(_st), info(_info) { }
    sInfo() { }
  };

  struct sInfoCharLimit {
    unsigned int net;
    unsigned int length;

    sInfoCharLimit(int _net, int _length) : 
      net(_net), length(_length) { }
    sInfoCharLimit() { }
  };

  struct sStReplacement {
    unsigned int net;
    unsigned int before;
    unsigned int after;

    sStReplacement(int _net, int _before, int _after) : 
      net(_net), before(_before), after(_after) { }
  };

  typedef std::list<sInfo> tInfos;
  typedef std::map<int, sInfo> tLastInfos;
  typedef std::list<sStReplacement> tStReplacements;
  typedef std::list<sInfoCharLimit> tInfoCharLimits;

public:
  Status(NetList *lCtrl, int onHiddenCfgCol = 0, int dotsCfgCol = 0);

  // Obcina status do maks. d³ugoœci
  String limitChars(StringRef status, int net);

  // Sprawdza czy sieæ 'nadaje siê do u¿ytku'
  bool isNetValid(int net);
  // Czy zmieniaæ przy statusie 'ukryty' ?
  bool chgOnHidden();
  // obs³uga akcji
  void actionHandle(sIMessage_base *msgBase);

  // tekstowa nazwa statusu
  inline String labelById(int st) {
    String name = "?";

    switch (st) {
      case ST_ONLINE: name = "Dostêpny"; break;
      case ST_CHAT: name = "Pogadam"; break;
      case ST_AWAY: name = "Zaraz wracam"; break;
      case ST_NA: name = "Nieosi¹galny"; break;
      case ST_DND: name = "Nie przeszkadzaæ"; break;
      case ST_HIDDEN: name = "Ukryty"; break;
      case ST_OFFLINE: name = "Niedostêpny"; break;
    }
    return name;
  }

  // "..." przy obcinanym opisie
  inline String getDots() {
    if (!this->dotsCfgCol) return "";

    if (Ctrl->DTgetType(DTCFG, this->dotsCfgCol) == DT_CT_STR) {
      return GETSTRA(this->dotsCfgCol);
    }
    return GETINT(this->dotsCfgCol) ? "…" : "";
  }

  inline bool isRemembered(int net = 0) {
    if (!net) {
      return this->remember;
    } else {
      for (tInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
        if (it->net == net) return true;
      }
      return false;
    }
  }

  inline int getInfoCharLimit(int net = 0) {
    for (tInfoCharLimits::iterator it = this->infoCharLimits.begin(); it != this->infoCharLimits.end(); it++) {
      if (it->net == net) return it->length;
    }

    int limit = 0;
    if (net) {
      Ctrl->IMessage(IM::infoCharLimit, net, limit);
      if (Ctrl->getError() != IMERROR_NORESULT) {
        return limit;
      }
    }
    return 0;
  }

  // Formatuje status
  inline virtual String parseInfo(StringRef info, int net, int st = -1) {
    info = Helpers::trim(info);
    info = this->limitChars(info, net);

    return PassStringRef(info);
  }

  // Zmienia status, txt - opis, st - id statusu
  void changeStatus(const StringRef& info, int st = -1);
  void changeStatus(int st);
  // Zmienia status na wybranej sieci
  void changeStatus(int net, const StringRef& info, int st = -1);
  void changeStatus(int net, int st);

  // Zapamiêtuje aktualny opis na ka¿dej sieci
  void rememberInfo();
  // Zapamiêtuje aktualny opis na wybranej sieci
  void rememberInfo(int net);

  // Przywraca opis na ka¿dej sieci
  void restoreInfo();
  // Przywraca opis na wybranej sieci
  void restoreInfo(int net);

  // Zwraca aktualny opis sieci net
  String getActualInfo(int net);
  // Zwraca opis sieci net
  String getInfo(int net);

  // Zwraca aktualny status sieci net
  tStatus getActualStatus(int net);
  // Zwraca status sieci net
  int getStatus(int net);

  tStatus applyReplacementSt(int net, int st);
  void addReplacementSt(int net, int before, int after);
  void removeReplacementSt(int net, int before);

  // lista z sieciami
  NetList *lCtrl;

protected:
  // kolumna konfiguracji ktora odpowiada za opcje 'zmieniaj status przy statusie ukryty'
  unsigned int onHiddenCfgCol;
  unsigned int dotsCfgCol;
  bool remember;

  tLastInfos lastSt;
  // zapamietane statusy i ich opisy
  tInfos rememberedSt;
  // zamienniki nieobs³ugiwanych statusów
  tStReplacements stReplacements;
  // maksymalne d³ugoœci statusów opisowych
  tInfoCharLimits infoCharLimits;
};

#endif // __STATUS_H__