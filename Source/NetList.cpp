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
  tItems nets;

  if (!buff.empty()) {
    Item item;

    int pos = (int) buff.find("|");
    int off = 0;

    while (pos != String::npos) {
      item.net = atoi(buff.substr(off, pos - off).a_str());
      off = pos + 1;
      pos = (int) buff.find("|", off);
      item.active = (bool) atoi(buff.substr(off, pos - off).a_str());
      off = pos + 1;
      pos = (int) buff.find("|", off);

      logDebug("[NetList<%i>::load().saved-item]: net = %i, active = %s",
        this, item.net, btoa(item.active));

      nets.push_back(item);
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
    item.net = (int) Ctrl->IMessageDirect(IM_PLUG_NET, id);
    item.name = (char*) Ctrl->IMessageDirect(IM_PLUG_NETNAME, id);

    if (item.name.length() && !isIgnored(item.net)) {
      item.action_id = dynActGroup + n++;
      item.active = defaultUse;

      for (tItems::iterator it = nets.begin(); it != nets.end(); it++) {
        if (it->net == item.net) {
          item.active = it->active; break;
        }
      }
      _items.push_back(item);

      logDebug("[NetList<%i>::load().item]: action id = %i, net = %i, name = %s, active = %s",
        this, item.action_id, item.net, item.name.c_str(), btoa(item.active));
    }
  }
}

void NetList::save() {
  string buff;
  char v[10];

  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    buff += itoa(it->net, v, 10);
    buff += "|";
    buff += it->active ? "1" : "0";
    buff += "|";

    logDebug("[NetList<%i>::save().item]: name = %s, active = %s",
      this, it->name.c_str(), btoa(it->active));
  }
  SETSTR(cfgCol, buff.c_str());
}

void NetList::_onCreate(ActionEvent& ev) {
  if (ev.getCode() == ACTN_CREATEWINDOW) {
    _opened = true;
    refreshUI();
  }
}

void NetList::_onDestroy(ActionEvent& ev) {
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

void NetList::_drawItem(NetList::Item& item, bool draw_inline, int x) {
  logDebug("[NetList<%i>::_drawItem()]: name = %s, active = %s",
    this, item.name.c_str(), btoa(item.active));

  UIActionCfgAdd(cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(item.getIconID()).a_str(), 0, x);
  UIActionCfgAdd(cfgGroup, item.action_id, ACTT_CHECK | (draw_inline ? ACTSC_INLINE : 0), item.name.c_str());
}

void NetList::drawItems(int columns) {
  int items_count = _items.size();
  if (!items_count) {
    return;
  }
  int i = 0, col = 0;

  // destructor action
  UIActionCfgAdd(cfgGroup, actDestroy, ACTT_HWND, 0, 0, 0, -20);

  for (tItems::iterator it = _items.begin(); it != _items.end(); it++, i++, col++) {
    col %= columns;
    _drawItem(*it, (col != columns - 1 && i != items_count - 1), (col > 0) ? 10 : 0);
  }

  // constructor action
  UIActionCfgAdd(cfgGroup, actCreate, ACTT_HWND, 0, 0, 0, -20);
}

void NetList::refreshFromUI() {
  bool v;

  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    v = *UIActionCfgGetValue(sUIAction(cfgGroup, it->action_id), 0, 0, true) == '1';

    logDebug("[NetList<%i>::refreshFromUI().item]: name = %s, active = %s [now: %s]",
      this, it->name.c_str(), btoa(it->active), btoa(v));

    if (it->active != v) it->active = v;
  }
}

void NetList::refreshUI() {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    UIActionCfgSetValue(sUIAction(cfgGroup, it->action_id), (it->active ? "1" : "0"), true);

    logDebug("[NetList<%i>::refreshUI().item]: name = %s, active = %s",
      this, it->name.c_str(), btoa(it->active));
  }
}

bool NetList::hasItem(int net) {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    if (it->net == net) return true;
  }
  return false;
}

NetList::Item& NetList::getItem(int net) {
  for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
    if (it->net == net) return *it;
  }
  throw new ExceptionString("Given net was not found");
}

bool NetList::isIgnored(int net) {
  for (tIgnored::iterator it = _ignored.begin(); it != _ignored.end(); it++) {
    if (*it == net) return true;
  }
  return Ctrl->IMessage(IM::hidePresence, net);
}

void NetList::addIgnored(int net) {
  _ignored.push_back(net);
}

void NetList::removeIgnored(int net) {
  for (tIgnored::iterator it = _ignored.begin(); it != _ignored.end(); it++) {
    if (*it == net) _ignored.erase(it); return;
  }
}