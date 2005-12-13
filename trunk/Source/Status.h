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

    itemInfo(int _net, int _st, std::string _info): 
      net(_net), st(_st), info(_info) { }
  };

  struct statusReplacement {
    unsigned int net;
    unsigned int before;
    unsigned int after;

    statusReplacement(int _net, int _before, int _after) : net(_net), before(_before), after(_after) { }
  };

  // definicje max d³ugoœci statusów poszcz. dla sieci
  enum enMaxLength {
    normal = 255,
    jabber = 255,
    tlen = 255,
    gg = 70
  };

  typedef std::list<itemInfo> tItemInfos;
  typedef std::list<statusReplacement> tStReplacements;

  class Status {
    public:
      Status(NetList *lCtrl, int onHiddenCfgCol = NULL, std::string stInfoVar = "");
      ~Status();

    public:
      // Obcina status do maks. d³ugoœci
      std::string limitChars(std::string status, int net, int s = 0);
      // Formatuje status
      std::string parse(std::string status, int net);

      // Sprawdza czy sieæ 'nadaje siê do u¿ytku'
      bool isNetUseful(int net);
      bool onHidden();

      // Zmienia status, txt - opis, st - id statusu
      void changeStatus(std::string info, int st = -1);
      // Zmienia status na wybranej sieci
      void changeStatus(int net, std::string info, int st = -1);

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
      int getActualStatus(int net);
      // Zwraca status sieci net
      int getStatus(int net);

      // lista z sieciami
      NetList *lCtrl;
      // formatowanie statusu
      Format *fCtrl;

    private:
      // nazwa zmiennej do której wrzucany bêdzie aktualny opis statusu
      std::string stInfoVar;
      // kolumna konfiguracji ktora odpowiada za opcje 'zmieniaj status przy statusie ukryty'
      unsigned int onHiddenCfgCol;
      // lista z opisami na sieciach
      tItemInfos info;
      // zamienniki nieobs³ugiwanych statusów
      tStReplacements stReplacements;
  };
}