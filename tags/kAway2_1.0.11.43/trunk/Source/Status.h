/**
 *  Status class
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

#include "stdafx.h"
#include "Helpers.h"
#include "NetList.h"
#include "Format.h"

struct sItemInfo {
  unsigned int net;
  unsigned int st;
  std::string info;

  sItemInfo(int _net = 0, int _st = 0, std::string _info = "") : 
    net(_net), st(_st), info(_info) { }
};

struct sStInfoMaxChars {
  unsigned int net;
  unsigned int length;

  sStInfoMaxChars(int _net = 0, int _length = 0) : 
    net(_net), length(_length) { }
};

struct sStReplacement {
  unsigned int net;
  unsigned int before;
  unsigned int after;

  sStReplacement(int _net, int _before, int _after) : 
    net(_net), before(_before), after(_after) { }
};

typedef std::list<sItemInfo> tItemInfos;
typedef std::map<int, sItemInfo> tLastInfos;
typedef std::list<sStReplacement> tStReplacements;
typedef std::list<sStInfoMaxChars> tStInfoMaxChars;

class Status {
  public:
    Status(NetList *lCtrl, int onHiddenCfgCol = 0, int dotsCfgCol = 0, std::string stInfoVar = "");
    ~Status();

  public:
    // Obcina status do maks. d³ugoœci
    std::string limitChars(std::string status, int net);
    // Formatuje status
    std::string parse(std::string status, int net);

    // Sprawdza czy sieæ 'nadaje siê do u¿ytku'
    bool isNetValid(int net);
    // Czy zmieniaæ przy statusie 'ukryty' ?
    bool chgOnHidden();
    // obs³uga akcji
    void actionHandle(sIMessage_base *msgBase);

    // "..." przy obcinanym opisie
    inline std::string getDots() {
      if (!this->dotsCfgCol) return("");

      if (Ctrl->DTgetType(DTCFG, this->dotsCfgCol) == DT_CT_STR) {
        return(GETSTRA(this->dotsCfgCol));
      }
      return(GETINT(this->dotsCfgCol) ? "…" : "");
    }

    inline bool isRemembered(int net = 0) {
      if (!net) {
        return(this->remember);
      } else {
        for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
          if (it->net == net) return(true);
        }
        return(false);
      }
    }

    inline int getStInfoMaxLength(int net = 0) {
      for (tStInfoMaxChars::iterator it = this->stInfoMaxChars.begin(); it != this->stInfoMaxChars.end(); it++) {
        if (it->net == net) return(it->length);
      }
      return(0);
    }

    // Zmienia status, txt - opis, st - id statusu
    void changeStatus(std::string info, int st = -1);
    void changeStatus(int st);
    // Zmienia status na wybranej sieci
    void changeStatus(int net, std::string info, int st = -1);
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
    std::string getActualInfo(int net);
    // Zwraca opis sieci net
    std::string getInfo(int net);

    // Zwraca aktualny status sieci net
    tStatus getActualStatus(int net);
    // Zwraca status sieci net
    int getStatus(int net);

    int applyReplacementSt(int net, int st);
    void addReplacementSt(int net, int before, int after);
    void removeReplacementSt(int net, int before);

    // lista z sieciami
    NetList *lCtrl;
    // formatowanie statusu
    Format *fCtrl;

  protected:
    // nazwa zmiennej do której wrzucany bêdzie aktualny opis statusu
    std::string stInfoVar;
    // kolumna konfiguracji ktora odpowiada za opcje 'zmieniaj status przy statusie ukryty'
    unsigned int onHiddenCfgCol;
    unsigned int dotsCfgCol;
    bool remember;

    tLastInfos lastSt;
    // zapamietane statusy i ich opisy
    tItemInfos rememberedSt;
    // zamienniki nieobs³ugiwanych statusów
    tStReplacements stReplacements;
    // maksymalne d³ugoœci statusów opisowych
    tStInfoMaxChars stInfoMaxChars;
};