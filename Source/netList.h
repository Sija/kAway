/*
 *  kAway2 netList class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  struct itemNet {
    unsigned int id;
    unsigned int net;
    std::string name;
    bool use;
  };

  class netList {
    public:
      netList(const int cfgCol, const int cfgGroup, const int dynActGroup, const int actCreate, const int actDestroy);
      ~netList();

      void loadNets();
      void saveNets();

      void actionHandle(const int id, const int code);

      void UIDraw(int colCount = 3);
      void UIGetState();
      void UISetState();

      std::string getNetName(int net);
      bool setNetState(int net, bool use);
      bool getNetState(int net);

      inline std::list<itemNet> getNets() {
        return(this->nets);
      }
      bool isConnected(int net);
      bool isIgnored(int net);

      std::list<itemNet> nets;

    protected:
      int cfgCol;
      int cfgGroup;
      int dynActGroup;
      int actCreate;
      int actDestroy;

      bool configSaved;
      bool netsDrawn;
  };
}