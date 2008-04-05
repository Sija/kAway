/**
  *  NetList class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2008 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#include "stdafx.h"
#include "NetList.h"

void NetList::load() {
  String buff = Config::get(cfgCol);
  tItems loaded_nets;

  if (!buff.empty()) {
    Item item;

    int pos = (int) buff.find("|");
    int off = 0;

    while (pos != String::npos) {
      item._net = atoi(buff.substr(off, pos - off).a_str());
      off = pos + 1;
      pos = (int) buff.find("|", off);
      item._active = (bool) atoi(buff.substr(off, pos - off).a_str());
      off = pos + 1;
      pos = (int) buff.find("|", off);

      Ctrl->log(logMisc, "NetList", "load-from-config", "net = %i, active = %s",
        item.getNet(), btoa(item.isActive()));

      loaded_nets.push_back(item);
    }
  }

  int plugs = Ctrl->ICMessage(IMC_PLUG_COUNT);
  int id, type;

  Item item;

  for (int i = 1, n = 1; i < plugs; i++) {
    id = Ctrl->ICMessage(IMC_PLUG_ID, i);
    type = Ctrl->IMessageDirect(IM_PLUG_TYPE, (tPluginId) id);

    if ((type & imtNet) != imtNet) {
      continue;
    }
    item._net = (int) Ctrl->IMessageDirect(IM_PLUG_NET, (tPluginId) id);
    item._name = (char*) Ctrl->IMessageDirect(IM_PLUG_NETNAME, (tPluginId) id);

    if (item.getNet() && item.getName().length() && !isIgnored(item.getNet())) {
      item._action_id = dynActGroup + n++;
      item._parent_id = cfgGroup;
      item._active = defaultUse;

      for (tItems::iterator it = loaded_nets.begin(); it != loaded_nets.end(); it++) {
        if (it->getNet() == item.getNet()) {
          item.setActive(it->isActive()); break;
        }
      }
      _items.push_back(item);

      Ctrl->log(logMisc, "NetList", "load", "action id = %i, net = %i, name = %s, active = %s",
        item.getActionID(), item.getNet(), item.getName().c_str(), btoa(item.isActive()));
    }
  }
}

void NetList::save() {
  string buff;
  char v[10];

  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    buff += itoa(it->getNet(), v, 10);
    buff += "|";
    buff += it->isActive() ? "1" : "0";
    buff += "|";

    Ctrl->log(logMisc, "NetList", "save", "name = %s, active = %s",
      it->getName().c_str(), btoa(it->isActive()));
  }
  Config::get(cfgCol).set(buff);
}

void NetList::_groupListener(ActionEvent& ev) {
  if (ev.withCode(ACTN_CREATEWINDOW)) {
    _opened = true;
    refreshUI();
  }
  if (ev.withCode(ACTN_DESTROYWINDOW)) {
    _opened = false;
  }
}

void NetList::_onAction(ActionEvent& ev) {
  if (ev.withCode(ACTN_SAVE) && _opened) {
    refreshFromUI();
    save();
  }
}

void NetList::drawGroup(int columns, const string& title) {
  if (!_items.size()) {
    return;
  }
  UIActionCfgAdd(cfgGroup, 0, ACTT_GROUP, title.c_str());
  drawItems(columns);
  UIActionCfgAdd(cfgGroup, 0, ACTT_GROUPEND);
}

void NetList::drawItems(int columns) {
  int items_count = _items.size();
  if (!items_count) {
    return;
  }
  int i = 0, col = 0;

  for (tItems::iterator it = _items.begin(); it != _items.end(); it++, i++, col++) {
    col %= columns;
    it->draw((col != columns - 1 && i != items_count - 1), (col > 0) ? 10 : 0);
  }

  // add config group listener - to intercept ACTN_CREATE/DESTROYWINDOW msgs
  UIActionCfgAdd(cfgGroup, actCreate, ACTT_HWND, 0, 0, 0, -20);
}

void NetList::refreshFromUI() {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    bool v = *UIActionCfgGetValue(sUIAction(cfgGroup, it->getActionID()), 0, 0, true) == '1';

    Ctrl->log(logMisc, "NetList", "refreshFromUI", "name = %s, active = %s [now: %s]",
      it->getName().c_str(), btoa(it->isActive()), btoa(v));

    if (it->isActive() != v) it->setActive(v);
  }
}

void NetList::refreshUI() {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    UIActionCfgSetValue(sUIAction(cfgGroup, it->getActionID()), (it->isActive() ? "1" : "0"), true);

    Ctrl->log(logMisc, "NetList", "refreshUI", "name = %s, active = %s",
      it->getName().c_str(), btoa(it->isActive()));
  }
}

bool NetList::hasItem(unsigned int net) const {
  for (tItems::const_iterator it = _items.begin(); it != _items.end(); it++) {
    if (it->getNet() == net) return true;
  }
  return false;
}

NetList::Item& NetList::getItem(unsigned int net) {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    if (it->getNet() == net) return *it;
  }
  throw ExceptionString("Item was not found");
}

bool NetList::isIgnored(unsigned int net) const {
  for (tIgnored::const_iterator it = _ignored.begin(); it != _ignored.end(); it++) {
    if (*it == net) return true;
  }
  return Ctrl->IMessage(IM::hidePresence, (tNet) net);
}

void NetList::addIgnored(unsigned int net) {
  _ignored.push_back(net);
}

void NetList::removeIgnored(unsigned int net) {
  for (tIgnored::iterator it = _ignored.begin(); it != _ignored.end(); it++) {
    if (*it == net) {
      _ignored.erase(it); return;
    }
  }
}