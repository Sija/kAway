/**
 *  Status class
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
#include "Status.h"

Status::Status(NetList *lCtrl, int onHiddenCfgCol, int dotsCfgCol, std::string stInfoVar) {
  this->lCtrl = lCtrl;
  this->fCtrl = new Format;

  this->stInfoVar = stInfoVar;
  this->onHiddenCfgCol = onHiddenCfgCol;
  this->dotsCfgCol = dotsCfgCol;
  this->remember = false;

  this->stInfoMaxChars.push_back(sStInfoMaxChars(plugsNET::dwutlenek, 255));
  this->stInfoMaxChars.push_back(sStInfoMaxChars(plugsNET::gg, 70));
}

Status::~Status() {
  delete this->fCtrl;
  this->fCtrl = NULL;

  this->stReplacements.clear();
  this->rememberedSt.clear();
  this->stInfoMaxChars.clear();
  this->lastSt.clear();
}

int Status::applyReplacementSt(int net, int st) {
  for (tStReplacements::iterator it = this->stReplacements.begin(); it != this->stReplacements.end(); it++) {
    if (it->net == net && it->before == st) {
      st = it->after; break;
    }
  }
  return(st);
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
  NetList::tItemNets nets = this->lCtrl->getNets();
  for (NetList::tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
    if (this->isNetValid(it->net)) this->changeStatus(it->net, st);
  }
}

void Status::changeStatus(std::string info, int st) {
  NetList::tItemNets nets = this->lCtrl->getNets();
  for (NetList::tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
    if (this->isNetValid(it->net)) this->changeStatus(it->net, info, st);
  }
}

void Status::changeStatus(int net, int st) {
  if (st == -1 || st == this->getActualStatus(net)) return;

  st = this->applyReplacementSt(net, st);
  if (this->isRemembered())
    this->lastSt[net] = sItemInfo(net, st);

  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, 0);

  logDebug("[Status<%i>::changeStatus().item]: net = %i, status = %i",
    this, net, st);
}

void Status::changeStatus(int net, std::string info, int st) {
  if (info == this->getActualInfo(net) && st != ST_OFFLINE)
    return(this->changeStatus(net, st));

  if (info.length()) {
    bool dynSt = (this->stInfoVar.length() && 
      this->fCtrl->addVar(this->stInfoVar, this->getInfo(net), false));

    info = this->fCtrl->parse(info);
    info = Helpers::trim(info);
    info = this->limitChars(info, net);

    if (dynSt) this->fCtrl->removeVar(this->stInfoVar);
  }

  if (st != -1) 
    st = this->applyReplacementSt(net, st);

  if (this->isRemembered())
    this->lastSt[net] = sItemInfo(net, st, info);

  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) info.c_str());

  logDebug("[Status<%i>::changeStatus().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(info));
}

tStatus Status::getActualStatus(int net) {
  return(Ctrl->IMessage(IM_GET_STATUS, net));
}

int Status::getStatus(int net) {
  for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    if (it->net == net) return(it->st);
  }
  return(-1);
}

std::string Status::getActualInfo(int net) {
  std::string status((char*) Ctrl->IMessage(IM_GET_STATUSINFO, net));
  return(status);
}

std::string Status::getInfo(int net) {
  std::string fake;
  for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    if (it->net == net) return(it->info);
  }
  return(fake);
}

void Status::rememberInfo() {
  this->remember = true;
  this->lastSt.clear();
  this->rememberedSt.clear();

  NetList::tItemNets nets = this->lCtrl->getNets();
  for (NetList::tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
    this->rememberInfo(it->net);
  }
}

void Status::rememberInfo(int net) {
  if (!this->isNetValid(net)) return;

  int st = this->getActualStatus(net);
  std::string info = this->getActualInfo(net);
  this->rememberedSt.push_back(sItemInfo(net, st, info));

  logDebug("[Status<%i>::rememberInfo().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(info));
}

void Status::restoreInfo() {
  for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
    this->restoreInfo(it->net);
  }
  this->remember = false;
}

void Status::restoreInfo(int net) {
  if (!this->isRemembered(net)) return;

  int st = !this->getActualStatus(net) ? ST_OFFLINE : this->getStatus(net);
  std::string info = this->getInfo(net);
  Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) info.c_str());

  logDebug("[Status<%i>::restoreInfo().item]: net = %i, status = %i, info = %s",
    this, net, st, nullChk(info));
}

void Status::actionHandle(sIMessage_base *msgBase) {
  if (!this->isRemembered()) return;

  sIMessage_StatusChange *st = static_cast<sIMessage_StatusChange*>(msgBase);
  int net = Ctrl->IMessageDirect(IM_PLUG_NET, st->plugID);

  if ((this->getStatus(net) != -1) && (st->status != ST_CONNECTING) && (st->status != ST_OFFLINE)) {
    for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
      if (it->net == net) {
        if (st->status != this->lastSt[net].st) it->st = st->status;
        if (st->info != this->lastSt[net].info) it->info = st->info;
        break;
      }
    }
    logDebug("[Status<%i>::actionHandle()]: net = %i, status = %i, info = %s", 
      this, net, st->status, nullChk(st->info));
  }
}

bool Status::chgOnHidden() {
  return(this->onHiddenCfgCol ? (bool) GETINT(this->onHiddenCfgCol) : true);
}

bool Status::isNetValid(int net) {
  if (this->lCtrl->getNetState(net) && this->lCtrl->isConnected(net)) {
    if (!this->chgOnHidden() && (ST_HIDDEN == this->getActualStatus(net))) {
      return(false);
    } else {
      return(true);
    }
  }
  return(false);
}

std::string Status::limitChars(std::string status, int net) {
  int limit = this->getStInfoMaxLength(net);
  if (limit && (status.length() > limit)) {
    // truncating string
    status.resize(limit);

    // dots magic ;)
    std::string dots = this->getDots();
    if (int dotsSize = dots.length()) {
      status.replace(status.length() - dotsSize, dotsSize, dots);
    }
  }
  return(status);
}