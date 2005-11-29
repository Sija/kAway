/*
 *  Status class
 *  v. 0.1.2.2
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2005 Winthux
 *
 *  $Id$
 */

#pragma once

#define GG_STATUS_LENGTH 70
#define TLEN_STATUS_LENGTH 255
#define JABBER_STATUS_LENGTH 255
#define DEFAULT_STATUS_LENGTH 255

#include <list>

/*
 *  TODO:
 *  - niech zapamiêtuje status i opis (czy warto status?), ¿eby póŸniej przywróciæ
 *
 */

namespace kAway2 {
  struct itemInfo {
    std::string info;
    int net;
  };

  struct StatInfo {
    std::string info;
    int st;

    bool operator == (const StatInfo a) const {
      return (a.st == this->st && !a.info.compare(this->info)) ? true : false;
    }

    bool operator != (const StatInfo a) const {
      return (a.st != this->st || a.info.compare(this->info)) ? true : false;
    }

    bool operator == (const int _st) const {
      return (this->st == _st) ? true : false;
    }

    bool operator != (const int _st) const {
      return (this->st != _st) ? true : false;
    }

    bool operator == (std::string _info) const {
      return (!_info.compare(this->info)) ? true : false;
    }

    bool operator != (std::string _info) const {
      return (_info.compare(this->info)) ? true : false;
    }
  };

  class Status {
    public:
      Status(netList *lCtrl);
      ~Status();

    public:
      // Obcina status do maks. d³ugoœci
      std::string LimitChars(std::string Status, int net, int s = 0);

      // Zmienia status, txt - opis, st - id statusu
      void ChangeStatus(const char *txt, int st = -1);
      // Zmienia status na wybranej sieci
      void ChangeStatus(int net, const char *txt, int st = -1);

      // Zapamiêtuje aktualny opis na ka¿dej sieci
      void RememberInfo();
      // Zapamiêtuje aktualny opis na wybranej sieci
      void RememberInfo(int net);

      // Przywraca opis na ka¿dej sieci
      void BackInfo();
      // Przywraca opis na wybranej sieci
      void BackInfo(int net);
      // Przywraca zapamiêtane opisy
      void BackRemeberedInfo();

      // lista z sieciami
      netList *lCtrl;

    protected:
      virtual std::string Format(std::string txt, int net) = 0;

    public:
      /* Opisy */
      // Dodaje info do listy
      void AddInfo(char *info, int net);
      // Zwraca opis sieci net
      std::string GetInfo(int net);

    private:
      // Czy ma zmieniaæ opis przy Ukrytym?
      bool onHidden;
      // lista z opisami na sieciach
      std::list<itemInfo> info;
      // poprzedni opis
      StatInfo prevStat;
  };

  class CtrlStatus : public Status {
    public:
      CtrlStatus(netList *lCtrl) : Status(lCtrl) { }
    protected:
      std::string Format(std::string txt, int net);
  };
}