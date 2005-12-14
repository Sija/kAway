/*
 *  Status class
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

    this->stReplacements.push_back( statusReplacement( plugsNET::gg, ST_CHAT, ST_ONLINE ) );
    this->stReplacements.push_back( statusReplacement( plugsNET::gg, ST_DND, ST_AWAY ) );
    this->stReplacements.push_back( statusReplacement( plugsNET::gg, ST_NA, ST_AWAY ) );
  }

  Status::~Status() {
    delete this->fCtrl;
    this->fCtrl = NULL;

    this->stReplacements.clear();
    this->info.clear();
  }

  void Status::changeStatus(std::string info, int st) {
    for (tItemNets::iterator it = this->lCtrl->nets.begin(); it != this->lCtrl->nets.end(); it++) {
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
        info = trim(info);

        if (dynSt) this->fCtrl->removeVar(this->stInfoVar);
      }

      st = st ? st : -1;
      if (st != -1) {
        for (tStReplacements::iterator it = this->stReplacements.begin(); it != this->stReplacements.end(); it++) {
          if (it->net == net && it->before == st) {
            st = it->after; break;
          }
        }
      }

      IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, info.length() ? (int) info.c_str() : 0);

      Control::Debug("[Status::changeStatus().item]: net = %i, status = %i, info = %s",
        net, st, (info.length() ? info.c_str() : "(none)"));
    }
  }

  int Status::getActualStatus(int net) {
    return(IMessage(IM_GET_STATUS, net));
  }

  int Status::getStatus(int net) {
    for (tItemInfos::iterator it = this->info.begin(); it != this->info.end(); it++) {
      if (it->net == net) return(it->st);
    }
  }

  std::string Status::getActualInfo(int net) {
    std::string status;

    status = (char*) IMessage(IM_GET_STATUSINFO, net);
    return(status);
  }

  std::string Status::getInfo(int net) {
    std::string fake;
    for (tItemInfos::iterator it = this->info.begin(); it != this->info.end(); it++) {
      if (it->net == net) return(it->info);
    }
    return(fake);
  }

  void Status::rememberInfo() {
    this->info.clear();
    for (tItemNets::iterator it = this->lCtrl->nets.begin(); it != this->lCtrl->nets.end(); it++) {
      this->rememberInfo(it->net);
    }
  }

  void Status::rememberInfo(int net) {
    if (this->isNetUseful(net)) {
      std::string info = this->getActualInfo(net);
      this->info.push_back(itemInfo(net, this->getActualStatus(net), info));

      Control::Debug("[Status::rememberInfo().item]: net = %i, info = %s", net, info.c_str());
    }
  }

  void Status::restoreInfo() {
    for (tItemInfos::iterator it = this->info.begin(); it != this->info.end(); it++) {
      this->restoreInfo(it->net);
    }
  }

  void Status::restoreInfo(int net) {
    if (this->isNetUseful(net)) {
      std::string info = this->getInfo(net);
      IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, this->getStatus(net), (int) info.c_str());

      Control::Debug("[Status::restoreInfo().item]: net = %i, info = %s",
        net, (info.length() ? info.c_str() : "(none)"));
    }
  }

  bool Status::onHidden() {
    return(this->onHiddenCfgCol ? (bool) GETINT(this->onHiddenCfgCol) : true);
  }

  bool Status::isNetUseful(int net) {
    if (this->lCtrl->getNetState(net) && this->lCtrl->isConnected(net)) {
      if (!this->onHidden() && (ST_HIDDEN == this->getActualStatus(net)))
        return(false);
      else
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