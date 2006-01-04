/*
 *  kAway2 NetList class
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: netList.h 25 2005-12-26 18:00:47Z sija $
 */

#pragma once

namespace kAway2 {
  struct sItemNet {
    unsigned int id;
    unsigned int net;
    std::string name;
    bool use;

    sItemNet(int _id = 0, int _net = 0, std::string _name = "", bool _use = true) :
      id(_id), net(_net), name(_name), use(_use) { }
  };

  typedef std::list<sItemNet> tItemNets;
  typedef std::deque<int> tIgnoredNets;

  class NetList {
    public:
      NetList(int cfgCol, int cfgGroup, int dynActGroup, int actCreate, int actDestroy);
      ~NetList();

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
      void addIgnored(int net);
      void removeIgnored(int net);

    protected:
      unsigned int cfgCol;
      unsigned int cfgGroup;
      unsigned int dynActGroup;
      unsigned int actCreate;
      unsigned int actDestroy;

      tIgnoredNets ignoredNets;
      tItemNets nets;

      bool configSaved;
      bool netsDrawn;
  };
}