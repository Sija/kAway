/**
 *  NetList class
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

struct sItemNet {
  unsigned int id;
  unsigned int net;
  std::string name;
  bool use;

  sItemNet(int _id = 0, int _net = 0, std::string _name = "", bool _use = true) :
    id(_id), net(_net), name(_name), use(_use) { }
};

enum enCfgDraw {
  checkboxes,
  radiosEmpty,
  radios
};

typedef std::list<sItemNet> tItemNets;
typedef std::deque<int> tIgnoredNets;

class NetList {
  public:
    NetList(int cfgCol, int cfgGroup, int dynActGroup, 
      int actCreate, int actDestroy, enCfgDraw uiDraw = checkboxes);
    ~NetList();

    void loadNets();
    void saveNets();

    void actionHandle(int id, int code);

    void UIDraw(int colCount = 3, char *groupTitle = NULL);
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

    enCfgDraw uiDraw;
    bool netsDrawn;
};