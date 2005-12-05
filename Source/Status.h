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
 *  - czy ustawianie zmiennej do ktorej wrzucony bedzie aktualny opis w konstruktorze to dobry pomysl ?
 *
 */

namespace kAway2 {
  struct itemInfo {
    unsigned int net;
    unsigned int st;
    std::string info;
  };

  typedef std::list<itemInfo> tItemInfos;

  class Status {
    public:
      Status(netList *lCtrl, int onHiddenCfgCol = NULL, std::string stInfoVar = "");
      ~Status();

    public:
      // Obcina status do maks. d³ugoœci
      std::string LimitChars(std::string status, int net, int s = 0);
      // Formatuje status
      std::string Parse(std::string status, int net);

      // Sprawdza czy sieæ 'nadaje siê do u¿ytku'
      bool isNetUseful(int net);
      bool onHidden();

      // Zmienia status, txt - opis, st - id statusu
      void ChangeStatus(std::string info, int st = -1);
      // Zmienia status na wybranej sieci
      void ChangeStatus(int net, std::string info, int st = -1);

      // Zapamiêtuje aktualny opis na ka¿dej sieci
      void RememberInfo();
      // Zapamiêtuje aktualny opis na wybranej sieci
      void RememberInfo(int net);

      // Przywraca opis na ka¿dej sieci
      void BackInfo();
      // Przywraca opis na wybranej sieci
      void BackInfo(int net);

      // Zwraca aktualny opis sieci net
      std::string GetActualInfo(int net);
      // Zwraca opis sieci net
      std::string GetInfo(int net);
      // Zwraca aktualny status
      int GetActualStatus(int net);
      // Dodaje info do listy
      void AddInfo(std::string info, int net);

      // lista z sieciami
      netList *lCtrl;
      // formatowanie statusu
      Format *fCtrl;

      // definicje max d³ugoœci statusów poszcz. dla sieci
      static const unsigned int default_status_length = 255;
      static const unsigned int jabber_status_length = 255;
      static const unsigned int tlen_status_length = 255;
      static const unsigned int gg_status_length = 70;

    protected:
      // virtual std::string Format(std::string txt, int net) = 0;

    private:
      // nazwa zmiennej do której wrzucany bêdzie aktualny opis statusu
      std::string stInfoVar;
      // kolumna konfiguracji ktora odpowiada za opcje 'zmieniaj status przy statusie ukryty'
      unsigned int onHiddenCfgCol;
      // lista z opisami na sieciach
      tItemInfos info;
  };
}