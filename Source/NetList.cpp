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
  String buff(GETSTR(cfgCol));
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

      logDebug("[NetList<%i>::load().saved-item]: net = %i, active = %s",
        this, item.getNet(), btoa(item.isActive()));

      loaded_nets.push_back(item);
    }
  }

  int plugs = Ctrl->IMessage(IMC_PLUG_COUNT);
  int id, type;

  Item item;

  for (int i = 1, n = 1; i < plugs; i++) {
    id = Ctrl->IMessage(IMC_PLUG_ID, 0, 0, i);
    type = Ctrl->IMessageDirect(IM_PLUG_TYPE, id);

    if ((type & IMT_NET) != IMT_NET) {
      continue;
    }
    item._net = (int) Ctrl->IMessageDirect(IM_PLUG_NET, id);
    item._name = (char*) Ctrl->IMessageDirect(IM_PLUG_NETNAME, id);

    if (item.getName().length() && !isIgnored(item.getNet())) {
      item._action_id = dynActGroup + n++;
      item._parent_id = cfgGroup;
      item._active = defaultUse;

      for (tItems::iterator it = loaded_nets.begin(); it != loaded_nets.end(); it++) {
        if (it->getNet() == item.getNet()) {
          item.setActive(it->isActive()); break;
        }
      }
      _items.push_back(item);

      logDebug("[NetList<%i>::load().item]: action id = %i, net = %i, name = %s, active = %s",
        this, item.getActionID(), item.getNet(), item.getName().c_str(), btoa(item.isActive()));
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

    logDebug("[NetList<%i>::save().item]: name = %s, active = %s",
      this, it->getName().c_str(), btoa(it->isActive()));
  }
  SETSTR(cfgCol, buff.c_str());
}

void NetList::_groupListener(ActionEvent& ev) {
  if (ev.getCode() == ACTN_CREATEWINDOW) {
    _opened = true;
    refreshUI();
  }
  if (ev.getCode() == ACTN_DESTROYWINDOW) {
    _opened = false;
  }
}

void NetList::_onAction(ActionEvent& ev) {
  if (ev.getCode() == ACTN_SAVE && _opened) {
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

    logDebug("[NetList<%i>::refreshFromUI().item]: name = %s, active = %s [now: %s]",
      this, it->getName().c_str(), btoa(it->isActive()), btoa(v));

    if (it->isActive() != v) it->setActive(v);
  }
}

void NetList::refreshUI() {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    UIActionCfgSetValue(sUIAction(cfgGroup, it->getActionID()), (it->isActive() ? "1" : "0"), true);

    logDebug("[NetList<%i>::refreshUI().item]: name = %s, active = %s",
      this, it->getName().c_str(), btoa(it->isActive()));
  }
}

bool NetList::hasItem(tNet net) const {
  for (tItems::const_iterator it = _items.begin(); it != _items.end(); it++) {
    if (it->getNet() == net) return true;
  }
  return false;
}

NetList::Item& NetList::getItem(tNet net) {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    if (it->getNet() == net) return *it;
  }
  throw ExceptionString("Item was not found");
}

bool NetList::isIgnored(tNet net) const {
  for (tIgnored::const_iterator it = _ignored.begin(); it != _ignored.end(); it++) {
    if (*it == net) return true;
  }
  return Ctrl->IMessage(IM::hidePresence, net);
}

void NetList::addIgnored(tNet net) {
  _ignored.push_back(net);
}

void NetList::removeIgnored(tNet net) {
  for (tIgnored::iterator it = _ignored.begin(); it != _ignored.end(); it++) {
    if (*it == net) {
      _ignored.erase(it); return;
    }
  }
}