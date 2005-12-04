/*
 *  Status class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2005 Winthux
 *
 *  $Id$
 */

#pragma once

/*
 *  TODO:
 *  - niech zapamiêtuje status i opis (czy warto status?), ¿eby póŸniej przywróciæ
 *  - rozprawic sie z char * w metodach
 *
 */

namespace kAway2 {
  struct itemInfo {
    std::string info;
    int net;
  };

  struct statInfo {
    std::string info;
    int st;

    bool operator == (const statInfo a) const {
      return (a.st == this->st && !a.info.compare(this->info)) ? true : false;
    }

    bool operator != (const statInfo a) const {
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
      Status(netList *lCtrl, int onHiddenCfgCol = 0);
      ~Status();

    public:
      // Obcina status do maks. d³ugoœci
      std::string LimitChars(std::string status, int net, int s = 0);
      // Formatuje status
      std::string Parse(std::string status, int net);

      // Sprawdza czy sieæ 'nadaje siê do u¿ytku'
      bool Status::isNetUseful(int net, bool onHidden = true);
      bool Status::onHidden();

      // Zmienia status, txt - opis, st - id statusu
      void ChangeStatus(const char * txt, int st = -1);
      // Zmienia status na wybranej sieci
      void ChangeStatus(int net, const char * txt, int st = -1);

      // Zapamiêtuje aktualny opis na ka¿dej sieci
      void RememberInfo();
      // Zapamiêtuje aktualny opis na wybranej sieci
      void RememberInfo(int net);

      // Przywraca opis na ka¿dej sieci
      void BackInfo();
      // Przywraca opis na wybranej sieci
      void BackInfo(int net);

      // Dodaje info do listy
      void AddInfo(char * info, int net);
      // Zwraca opis sieci net
      std::string GetInfo(int net);

      // lista z sieciami
      netList *lCtrl;
      // formatowanie statusu
      Format *fCtrl;

      // definicje max d³ugoœci statusów poszcz. dla sieci
      static const int default_status_length = 255;
      static const int jabber_status_length = 255;
      static const int tlen_status_length = 255;
      static const int gg_status_length = 70;

    protected:
      // virtual std::string Format(std::string txt, int net) = 0;

    private:
      int onHiddenCfgCol;
      // lista z opisami na sieciach
      std::list<itemInfo> info;
  };
}