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

  typedef std::list<itemNet> tItemNets;

  class netList {
    public:
      netList(int cfgCol, int cfgGroup, int dynActGroup, int actCreate, int actDestroy);
      ~netList();

      void loadNets();
      void saveNets();

      void actionHandle(int id, int code);

      void UIDraw(int colCount = 3);
      void UIGetState();
      void UISetState();

      std::string getNetName(int net);
      bool setNetState(int net, bool use);
      bool getNetState(int net);

      inline tItemNets getNets() {
        return(this->nets);
      }
      bool isConnected(int net);
      bool isIgnored(int net);

      tItemNets nets;

    protected:
      unsigned int cfgCol;
      unsigned int cfgGroup;
      unsigned int dynActGroup;
      unsigned int actCreate;
      unsigned int actDestroy;

      bool configSaved;
      bool netsDrawn;
  };
}