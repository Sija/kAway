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
  class netList {
    public:
      netList(const int cfgCol, const int cfgGroup, const int dynActGroup);
      ~netList();

      void loadNets();
      void saveNets();

      void UIDraw(int colCount = 3);
      void UIGetState();
      void UISetState();

      std::string getNetName(int net);
      bool setNetState(int net, bool use);
      bool getNetState(int net);

      inline std::list<itemNet> getNets() {
        return(this->nets);
      }
      bool isIgnored(int net);

      std::list<itemNet> nets;

    protected:
      int cfgCol;
      int cfgGroup;
      int dynActGroup;
  };
}