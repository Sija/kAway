/**
 *  Status class
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

#include "stdafx.h"
#include "Status.h"

Status::Status(NetList* _netList, int _onHiddenCfgCol, int _dotsCfgCol) :
  SharedObject<iSharedObject>(), netList(_netList), onHiddenCfgCol(_onHiddenCfgCol), 
  dotsCfgCol(_dotsCfgCol), remember(false) 
{
  this->infoCharLimits.push_back(sInfoCharLimit(plugsNET::dwutlenek, 255));
  this->infoCharLimits.push_back(sInfoCharLimit(plugsNET::kaqq, 255));
  this->infoCharLimits.push_back(sInfoCharLimit(plugsNET::gg, 70));
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
  NetList::tNets nets = this->netList->getNets();
  for (NetList::tNets::iterator it = nets.begin(); it != nets.end(); it++) {
    if (this->isNetValid(it->net)) this->changeStatus(it->net, st);
  }
}

void Status::changeStatus(const StringRef& info, int st) {
  NetList::tNets nets = this->netList->getNets();
  for (NetList::tNets::iterator it = nets.begin(); it != nets.end(); it++) {
    if (this->isNetValid(it->net)) this->changeStatus(it->net, info, st);
  }
}

void Status::changeStatus(int net, int st) {
  if (st == -1 || st == this->getActualStatus(net)) return;

  st = this->applyReplacementSt(net, st);
  if (this->isRemembered()) {
    this->lastSt[net] = sInfo(net, st);
  }

  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, 0);

  logDebug("[Status<%i>::changeStatus().item]: net = %i, status = %i",
    this, net, st);
}

void Status::changeStatus(int net, const StringRef& info, int st) {
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
    this->lastSt[net] = sInfo(net, st, new_info);
  }

  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) new_info.c_str());

  logDebug("[Status<%i>::changeStatus().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(new_info));
}

tStatus Status::getActualStatus(int net) {
  return Ctrl->IMessage(IM_GET_STATUS, net);
}

int Status::getStatus(int net) {
  for (tInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    if (it->net == net) return it->st;
  }
  return -1;
}

String Status::getActualInfo(int net) {
  return (char*) Ctrl->IMessage(IM_GET_STATUSINFO, net);
}

String Status::getInfo(int net) {
  String fake;
  for (tInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    if (it->net == net) return it->info;
  }
  return fake;
}

bool Status::isRemembered(int net) {
  if (!net) {
    return this->remember;
  } else {
    for (tInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
      if (it->net == net) return true;
    }
    return false;
  }
}

void Status::rememberInfo(int net) {
  if (!this->isNetValid(net)) return;

  int st = this->getActualStatus(net);
  String info = this->getActualInfo(net);
  this->rememberedSt.push_back(sInfo(net, st, info));

  logDebug("[Status<%i>::rememberInfo().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(info));
}

void Status::rememberInfo() {
  this->remember = true;
  this->lastSt.clear();
  this->rememberedSt.clear();

  NetList::tNets nets = this->netList->getNets();
  for (NetList::tNets::iterator it = nets.begin(); it != nets.end(); it++) {
    this->rememberInfo(it->net);
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
  for (tInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    this->restoreInfo(it->net);
  }
  this->remember = false;
}

void Status::actionHandle(sIMessage_base *msgBase) {
  if (!this->isRemembered()) return;

  sIMessage_StatusChange *st = static_cast<sIMessage_StatusChange*>(msgBase);
  int net = Ctrl->IMessageDirect(IM_PLUG_NET, st->plugID);

  if ((this->getStatus(net) != -1) && (st->status != ST_CONNECTING) && (st->status != ST_OFFLINE)) {
    for (tInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
      if (it->net == net) {
        if (st->status != this->lastSt[net].st) it->st = st->status;
        if ((String) st->info != this->lastSt[net].info) it->info = st->info;
        break;
      }
    }
    logDebug("[Status<%i>::actionHandle()]: net = %i, status = %i, info = %s", 
      this, net, st->status, nullChk(st->info));
  }
}

bool Status::chgOnHidden() {
  return this->onHiddenCfgCol ? GETINT(this->onHiddenCfgCol) : true;
}

String Status::labelById(int st) {
  String name = "?";

  switch (st) {
    case ST_ONLINE: name = "Dostêpny"; break;
    case ST_CHAT: name = "Pogadam"; break;
    case ST_AWAY: name = "Zaraz wracam"; break;
    case ST_NA: name = "Nieosi¹galny"; break;
    case ST_DND: name = "Nie przeszkadzaæ"; break;
    case ST_HIDDEN: name = "Ukryty"; break;
    case ST_OFFLINE: name = "Niedostêpny"; break;
  }
  return name;
}

int Status::getInfoCharLimit(int net) {
  for (tInfoCharLimits::iterator it = this->infoCharLimits.begin(); it != this->infoCharLimits.end(); it++) {
    if (it->net == net) return it->length;
  }

  int limit = 0;
  if (net) {
    Ctrl->IMessage(IM::infoCharLimit, net, limit);
    if (Ctrl->getError() != IMERROR_NORESULT) {
      return limit;
    }
  }
  return 0;
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
    return GETSTRA(this->dotsCfgCol);
  }
  return GETINT(this->dotsCfgCol) ? "…" : "";
}

bool Status::isNetValid(int net) {
  if (this->netList->getNetState(net) && this->netList->isConnected(net)) {
    if (!this->chgOnHidden() && (ST_HIDDEN == this->getActualStatus(net))) {
      return false;
    } else {
      return true;
    }
  }
  return false;
}