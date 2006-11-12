/**
  *  NetList class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __NETLIST_H__
#define __NETLIST_H__

#include "Helpers.h"

class NetList : public SharedObject<iSharedObject> {
public:
  static const struct IM {
    static const unsigned int key = IM_USER + (668 * 1000) + 50;
    static const unsigned int hidePresence = key + 1;
  };

public:
  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(NetList, iSharedObject, Version(0,1,0,0));

  struct sNet {
    unsigned int id;
    unsigned int net;
    String name;
    bool use;

    sNet(int _id, int _net, String _name, bool _use = true) :
      id(_id), net(_net), name(_name), use(_use) { }
    sNet() { }
  };

  enum enSelectionType {
    typeCheckboxes,
    typeRadiosEmpty,
    typeRadios
  };

  typedef std::list<sNet> tNets;
  typedef std::deque<int> tIgnoredNets;

public:
  NetList(int _cfgCol, int _cfgGroup, int _dynActGroup, int _actCreate, int _actDestroy, enSelectionType _selection = typeCheckboxes) :
    SharedObject<iSharedObject>(), cfgCol(_cfgCol), cfgGroup(_cfgGroup), dynActGroup(_dynActGroup), actCreate(_actCreate), actDestroy(_actDestroy), 
    defaultUse(true), netsDrawn(false), selection(_selection) { }

  void loadNets();
  void saveNets();

  void actionHandle(int id, int code);

  void UIDraw(int colCount = 3, char* groupTitle = NULL);
  void UIGetState();
  void UISetState();

  String getNetName(int net);
  bool setNetState(int net, bool use);
  bool getNetState(int net);

  inline tNets getNets() {
    return this->nets;
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
  tNets nets;

  enSelectionType selection;
  bool netsDrawn;
  bool defaultUse;
};

typedef SharedPtr<NetList> oNetList;

#endif // __NETLIST_H__