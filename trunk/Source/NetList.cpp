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
#include "NetList.h"

NetList::NetList(int cfgCol, int cfgGroup, int dynActGroup, int actCreate, int actDestroy) {
  this->configSaved = false;
  this->netsDrawn = false;

  this->cfgCol = cfgCol;
  this->cfgGroup = cfgGroup;
  this->dynActGroup = dynActGroup;
  this->actCreate = actCreate;
  this->actDestroy = actDestroy;
}

NetList::~NetList() {
  this->ignoredNets.clear();
  this->nets.clear();
}

void NetList::loadNets() {
  std::string buff(GETSTRA(this->cfgCol));
  tItemNets nets;

  if (!buff.empty()) {
    sItemNet item;

    int pos = (int) buff.find("|");
    int off = 0;

    while (pos != std::string::npos) {
      item.net = atoi(buff.substr(off, pos - off).c_str());
      off = pos + 1;
      pos = (int) buff.find("|", off);
      item.use = atoi(buff.substr(off, pos - off).c_str()) ? true : false;
      off = pos + 1;
      pos = (int) buff.find("|", off);

      logDebug("[NetList<%i>::loadNets().retroItem]: net = %i, use = %s",
        this, item.net, btoa(item.use));

      nets.push_back(item);
    }
  }

  int id, type, net;
  int plugs = IMessage(IMC_PLUG_COUNT);

  std::string name;
  sItemNet item;

  for (int i = 1, n = 1; i < plugs; i++) {
    id = IMessage(IMC_PLUG_ID, 0, 0, i);
    type = IMessageDirect(IM_PLUG_TYPE, id);

    if ((type & IMT_NET) == IMT_NET) {
      net = (int) IMessageDirect(IM_PLUG_NET, id);
      name = (char*) IMessageDirect(IM_PLUG_NETNAME, id);

      if (name.length() && !this->isIgnored(net)) {
        item.net = net;
        item.id = n++;
        item.name = name;
        item.use = false;

        for (tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
          if (it->net == item.net && it->use) {
            item.use = true; break;
          }
        }
        this->nets.push_back(item);

        logDebug("[NetList<%i>::loadNets().item]: id = %i, net = %i, name = %s, use = %s",
          this, item.id, item.net, item.name.c_str(), btoa(item.use));
      }
    }
  }
}

void NetList::saveNets() {
  std::string buff;
  char v[10];

  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    buff += itoa(it->net, v, 10);
    buff += "|";
    buff += it->use ? "1" : "0";
    buff += "|";

    logDebug("[NetList<%i>::saveNets().item]: name = %s, use = %s",
      this, it->name.c_str(), btoa(it->use));
  }

  SETSTR(this->cfgCol, buff.c_str());
}

void NetList::actionHandle(int id, int code) {
  if (!this || !Ctrl || !Ctrl->isRunning()) return;

  if ((id == this->cfgGroup) && (code == ACTN_SAVE)) {
    this->configSaved = true;
  } else if ((id == this->actCreate) && (code == ACTN_CREATEWINDOW)) {
    this->netsDrawn = true;
    this->UISetState();
  } else if ((id == this->actDestroy) && (code == ACTN_DESTROYWINDOW)) {
    if (this->configSaved && this->netsDrawn) {
      logDebug("[NetList<%i>::actionHandle()]: id = %i, code = %i", this, id, code);
      this->UIGetState();
      this->saveNets();
    }
    this->netsDrawn = false;
    this->configSaved = false;
  }
}

void NetList::UIDraw(int colCount, char *groupTitle) {
  int i = 0, col = 0, ico;
  int netsCount = this->nets.size();

  if (!netsCount)
    return;

  if (groupTitle) {
    UIActionCfgAdd(this->cfgGroup, 0, ACTT_GROUP, groupTitle);
  }
  UIActionCfgAdd(this->cfgGroup, this->actDestroy, ACTT_HWND, 0, 0, 0, -20);

  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++, i++, col++) {
    ico = UIIcon(IT_LOGO, (int) it->net, 0, 0);
    col %= colCount;

    logDebug("[NetList<%i>::UIDraw().item]: name = %s, use = %s",
      this, it->name.c_str(), btoa(it->use));

    UIActionCfgAdd(this->cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico).c_str(), 0, (col > 0) ? 10 : 0);
    UIActionCfgAdd(this->cfgGroup, this->dynActGroup + it->id, ACTT_CHECK | 
      ((col != (colCount - 1) && i != (netsCount - 1)) ? ACTSC_INLINE : 0), it->name.c_str());
  }

  UIActionCfgAdd(this->cfgGroup, this->actCreate, ACTT_HWND, 0, 0, 0, -20);
  if (groupTitle) {
    UIActionCfgAdd(this->cfgGroup, 0, ACTT_GROUPEND);
  }
}

void NetList::UIGetState() {
  char buff[16];
  bool v;

  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    UIActionCfgGetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), buff, 16, true);
    v = (bool) atoi(buff);

    logDebug("[NetList<%i>::UIGetState().item]: name = %s, use = %s [now: %s]",
      this, it->name.c_str(), btoa(it->use), btos(v).c_str());

    if (it->use != v) it->use = v;
  }
}

void NetList::UISetState() {
  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), (it->use ? "1" : "0"), true);

    logDebug("[NetList<%i>::UISetState().item]: name = %s, use = %s",
      this, it->name.c_str(), btoa(it->use));
  }
}

std::string NetList::getNetName(int net) {
  std::string fake;
  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (it->net == net) return(it->name);
  }
  return(fake);
}

bool NetList::getNetState(int net) {
  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (it->net == net) return(it->use);
  }
  return(false);
}

bool NetList::setNetState(int net, bool use) {
  for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (it->net == net) {
      if (it->use != use) it->use = use;
      return(true);
    }
  }
  return(false);
}

bool NetList::isConnected(int net) {
  return(Ctrl->IMessage(IM_ISCONNECTED, net, IMT_PROTOCOL));
}

bool NetList::isIgnored(int net) {
  for (tIgnoredNets::iterator it = this->ignoredNets.begin(); it != this->ignoredNets.end(); it++) {
    if ((*it) == net) return(true);
  }
  return(false);
}

void NetList::addIgnored(int net) {
  this->ignoredNets.push_back(net);
}

void NetList::removeIgnored(int net) {
  for (tIgnoredNets::iterator it = this->ignoredNets.begin(); it != this->ignoredNets.end(); it++) {
    if ((*it) == net) {
      it = this->ignoredNets.erase(it); break;
    }
  }
}
