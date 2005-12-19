/*
 *  kAway2 Status class
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2005 Winthux
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  Status::Status(NetList *lCtrl, int onHiddenCfgCol, std::string stInfoVar) {
    this->lCtrl = lCtrl;
    this->fCtrl = new Format;

    this->stInfoVar = stInfoVar;
    this->onHiddenCfgCol = onHiddenCfgCol;
    this->remember = false;
  }

  Status::~Status() {
    delete this->fCtrl;
    this->fCtrl = NULL;

    this->stReplacements.clear();
    this->rememberedSt.clear();
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
    this->stReplacements.push_back(statusReplacement(net, before, after));
  }

  void Status::removeReplacementSt(int net, int before) {
    for (tStReplacements::iterator it = this->stReplacements.begin(); it != this->stReplacements.end(); it++) {
      if ((it->net == net) && (it->before == before)) {
        it = this->stReplacements.erase(it); break;
      }
    }
  }

  void Status::changeStatus(std::string info, int st) {
    tItemNets nets = this->lCtrl->getNets();
    for (tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
      this->changeStatus(it->net, info, st);
    }
  }

  void Status::changeStatus(int net, std::string info, int st) {
    if (this->isNetUseful(net)) {
      if (info.length()) {
        bool dynSt = (this->stInfoVar.length() && !this->fCtrl->varExists(this->stInfoVar)) ? true : false;
        if (dynSt) this->fCtrl->addVar(this->stInfoVar, this->getInfo(net));

        info = this->fCtrl->parse(info);
        info = this->limitChars(info, net);
        info = Helpers::trim(info);

        if (dynSt) this->fCtrl->removeVar(this->stInfoVar);
      }

      st = st ? st : -1;
      if (st != -1) 
        st = this->applyReplacementSt(net, st);

      if (this->isRemembered())
        this->lastSt[net] = itemInfo(net, st, info);

      Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, info.length() ? (int) info.c_str() : 0);

      Control::Debug("[Status::changeStatus().item]: net = %i, status = %i, info = %s",
        net, st, (info.length() ? info.c_str() : "(none)"));
    }
  }

  int Status::getActualStatus(int net) {
    return(Ctrl->IMessage(IM_GET_STATUS, net));
  }

  int Status::getStatus(int net) {
    for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
      if (it->net == net) return(it->st);
    }
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

    tItemNets nets = this->lCtrl->getNets();
    for (tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
      this->rememberInfo(it->net);
    }
  }

  void Status::rememberInfo(int net) {
    if (this->isNetUseful(net)) {
      int st = this->getActualStatus(net);
      std::string info = this->getActualInfo(net);
      this->rememberedSt.push_back(itemInfo(net, st, info));

      Control::Debug("[Status::rememberInfo().item]: net = %i, status = %i, info = %s",
        net, st, (info.length() ? info.c_str() : "(none)"));
    }
  }

  void Status::restoreInfo() {
    for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
      this->restoreInfo(it->net);
    }
    this->remember = false;
  }

  void Status::restoreInfo(int net) {
    if (this->isNetUseful(net)) {
      int st = this->getStatus(net);
      std::string info = this->getInfo(net);
      Ctrl->IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) info.c_str());

      Control::Debug("[Status::restoreInfo().item]: net = %i, status = %i, info = %s",
        net, st, (info.length() ? info.c_str() : "(none)"));
    }
  }

  void Status::actionHandle(sIMessage_base *msgBase) {
    if (!this->isRemembered()) return;

    sIMessage_StatusChange *st = static_cast<sIMessage_StatusChange*>(msgBase);
    int net = Ctrl->IMessageDirect(IM_PLUG_NET, st->plugID);

    if (this->getStatus(net)) {
      for (tItemInfos::iterator it = this->rememberedSt.begin(); it != this->rememberedSt.end(); it++) {
        if (it->net == net) {
          if (st->status != this->lastSt[net].st) it->st = st->status;
          if (st->info != this->lastSt[net].info) it->info = st->info;
          break;
        }
      }
      Control::Debug("[Status::actionHandle()]: net = %i, status = %i, info = %s", 
        net, st->status, st->info);
    }
  }

  bool Status::onHidden() {
    return(this->onHiddenCfgCol ? (bool) GETINT(this->onHiddenCfgCol) : true);
  }

  bool Status::isNetUseful(int net) {
    if (this->lCtrl->getNetState(net) && this->lCtrl->isConnected(net)) {
      if (!this->onHidden() && (ST_HIDDEN == this->getActualStatus(net))) {
        if (this->isRemembered() && (this->lastSt[net].st == ST_HIDDEN))
          return(true);
        return(false);
      }
      return(true);
    }
    return(false);
  }

  std::string Status::limitChars(std::string status, int net, int s) {
    enMaxLength limit;

    switch (net) {
      case plugsNET::kjabber:
      case plugsNET::kjabber1:
      case plugsNET::kjabber2:
      case plugsNET::kjabber3:
      case plugsNET::kjabber4:
      case plugsNET::kjabber5:
      case plugsNET::kjabber6:
      case plugsNET::kjabber7:
      case plugsNET::kjabber8:
      case plugsNET::kjabber9:
      case plugsNET::kjabber10:
        limit = jabber;
        break;
      case plugsNET::dwutlenek:
        limit = tlen;
        break;
      case plugsNET::gg:
        limit = gg;
        break;
      default:
        limit = normal;
        break;
    }

    UINT iLimit = limit - s;
    if (status.length() > iLimit) {
      status.resize(iLimit);
      status.replace(status.length() - 3, 3, "...");
    }
    return(status);
  }
}