/**
  *  Status class
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
#include "Status.h"

Status::Status(NetList* _netList, int _onHiddenCfgCol, int _dotsCfgCol) : netList(_netList), 
  onHiddenCfgCol(_onHiddenCfgCol), dotsCfgCol(_dotsCfgCol), remember(false) 
{
  this->infoCharLimits[plugsNET::dwutlenek] = 255;
  this->infoCharLimits[plugsNET::kaqq] = 255;
  this->infoCharLimits[plugsNET::gg] = 70;
}

tStatus Status::applyReplacementSt(int net, int st) {
  for (tStReplacements::iterator it = this->stReplacements.begin(); it != this->stReplacements.end(); it++) {
    if (it->net == net && it->before == st) {
      st = it->after; break;
    }
  }
  return st;
}

void Status::addReplacementSt(int net, int before, int after) {
  this->stReplacements.push_back(sStReplacement(net, before, after));
}

void Status::removeReplacementSt(int net, int before) {
  for (tStReplacements::iterator it = this->stReplacements.begin(); it != this->stReplacements.end(); it++) {
    if ((it->net == net) && (it->before == before)) {
      it = this->stReplacements.erase(it); break;
    }
  }
}

void Status::changeStatus(int st) {
  NetList::tItems& nets = this->netList->getItems();
  for (NetList::tItems::iterator it = nets.begin(); it != nets.end(); it++) {
    if (this->isNetValid(it->getNet())) this->changeStatus(it->getNet(), st);
  }
}

void Status::changeStatusInfo(const StringRef& info, int st) {
  NetList::tItems& nets = this->netList->getItems();
  for (NetList::tItems::iterator it = nets.begin(); it != nets.end(); it++) {
    if (this->isNetValid(it->getNet())) this->changeStatusInfo(it->getNet(), info, st);
  }
}

void Status::changeStatus(int net, int st) {
  if (st == -1 || st == this->getActualStatus(net)) return;

  st = this->applyReplacementSt(net, st);
  if (this->isRemembered()) {
    this->omittedSt[net].push_back(Item(net, st));
  }

  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, 0);

  logDebug("[Status<%i>::changeStatus().item]: net = %i, status = %i",
    this, net, st);
}

void Status::changeStatusInfo(int net, const StringRef& info, int st) {
  if (info == this->getActualInfo(net) && st != ST_OFFLINE) {
    return this->changeStatus(net, st);
  }

  String new_info(info);

  if (new_info.length()) {
    new_info = this->parseInfo(new_info, net, st);
  }

  if (st != -1) {
    st = this->applyReplacementSt(net, st);
  }
  if (this->isRemembered()) {
    this->omittedSt[net].push_back(Item(net, st, new_info));
  }

  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) new_info.c_str());

  logDebug("[Status<%i>::changeStatus().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(new_info));
}

tStatus Status::getActualStatus(int net) {
  return Ctrl->IMessage(IM_GET_STATUS, net);
}

int Status::getStatus(int net) {
  for (tItems::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    if (it->getNet() == net) return it->getStatus();
  }
  return -1;
}

String Status::getActualInfo(int net) {
  return (char*) Ctrl->IMessage(IM_GET_STATUSINFO, net);
}

String Status::getInfo(int net) {
  String fake;
  for (tItems::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    if (it->getNet() == net) return it->getInfo();
  }
  return fake;
}

bool Status::isRemembered(int net) {
  if (!net) {
    return this->remember;
  } else {
    for (tItems::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
      if (it->getNet() == net) return true;
    }
    return false;
  }
}

void Status::rememberInfo(int net) {
  if (!this->isNetValid(net)) return;

  int st = this->getActualStatus(net);
  String info = this->getActualInfo(net);
  this->rememberedSt.push_back(Item(net, st, info));

  logDebug("[Status<%i>::rememberInfo().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(info));
}

void Status::rememberInfo() {
  this->remember = true;
  this->omittedSt.clear();
  this->rememberedSt.clear();

  NetList::tItems& nets = this->netList->getItems();
  for (NetList::tItems::iterator it = nets.begin(); it != nets.end(); it++) {
    this->rememberInfo(it->getNet());
  }
}

void Status::restoreInfo(int net) {
  if (!this->isRemembered(net)) return;

  int st = !this->getActualStatus(net) ? ST_OFFLINE : this->getStatus(net);
  String info = this->getInfo(net);
  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) info.c_str());

  logDebug("[Status<%i>::restoreInfo().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(info));
}

void Status::restoreInfo() {
  for (tItems::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    this->restoreInfo(it->getNet());
  }
  this->remember = false;
}

void Status::actionHandle(IMEvent& ev) {
  sIMessage_StatusChange *st = static_cast<sIMessage_StatusChange*>((sIMessage_base*) ev.getIMessage());
  int net = Ctrl->IMessageDirect(IM_PLUG_NET, st->plugID);

  if (!this->isRemembered(net) || this->omittedSt.find(net) == this->omittedSt.end()) {
    return;
  }

  if (st->status != ST_CONNECTING && st->status != ST_OFFLINE) {
    bool r = false, r2 = false;
    for (tItems::iterator it = this->omittedSt[net].begin(); it != this->omittedSt[net].end(); it++) {
      if (it->getStatus() == st->status) r = true;
      if (it->getInfo() == st->info) r2 = true;
    }

    if (!r || !r2) {
      for (tItems::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
        if (it->getNet() != net) continue;

        if (!r) it->_status = st->status;
        if (!r2) it->_info = st->info;
      }
    }

    logDebug("[Status<%i>::actionHandle()]: net = %i, status = %i, info = %s", 
      this, net, st->status, nullChk(st->info));
  }
}

bool Status::changeOnHidden() {
  return this->onHiddenCfgCol ? GETINT(this->onHiddenCfgCol) : true;
}

String Status::getStatusLabel(int status) {
  String name = "?";

  switch (status) {
    case ST_ONLINE:   name = "Dost�pny";          break;
    case ST_CHAT:     name = "Pogadam";           break;
    case ST_AWAY:     name = "Zaraz wracam";      break;
    case ST_NA:       name = "Nieosi�galny";      break;
    case ST_DND:      name = "Nie przeszkadza�";  break;
    case ST_HIDDEN:   name = "Ukryty";            break;
    case ST_OFFLINE:  name = "Niedost�pny";       break;
  }
  return name;
}

int Status::getInfoCharLimit(int net) {
  for (tNetInfoLimits::iterator it = this->infoCharLimits.begin(); it != this->infoCharLimits.end(); it++) {
    if (it->first == net) return it->second;
  }
  return Ctrl->IMessage(IM::infoCharLimit, net);
}

String Status::limitChars(StringRef status, int net) {
  return Helpers::trunc(status, this->getInfoCharLimit(net), this->getDots());
}

String Status::parseInfo(StringRef info, int net, int st) {
  info = Helpers::trim(info);
  info = this->limitChars(info, net);

  return PassStringRef(info);
}

String Status::getDots() {
  if (!this->dotsCfgCol) return "";

  if (Ctrl->DTgetType(DTCFG, this->dotsCfgCol) == DT_CT_STR) {
    return GETSTR(this->dotsCfgCol);
  }
  return GETINT(this->dotsCfgCol) ? "�" : "";
}

bool Status::isNetValid(int net) {
  if (this->netList->getItem(net).isActive() && this->netList->getItem(net).isConnected()) {
    if (!this->changeOnHidden() && (ST_HIDDEN == this->getActualStatus(net))) {
      return false;
    } else {
      return true;
    }
  }
  return false;
}