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
  Status::Status(netList *lCtrl, int onHiddenCfgCol, std::string stInfoVar) {
    this->lCtrl = lCtrl;
    this->fCtrl = new Format;

    this->stInfoVar = stInfoVar;
    this->onHiddenCfgCol = onHiddenCfgCol;
  }

  Status::~Status() {
    delete this->fCtrl;
    this->fCtrl = NULL;

    this->info.clear();
  }

  void Status::ChangeStatus(std::string info, int st) {
    for (tItemNets::iterator it = this->lCtrl->nets.begin(); it != this->lCtrl->nets.end(); it++) {
      this->ChangeStatus(it->net, info, st);
    }
  }

  bool Status::onHidden() {
    return(this->onHiddenCfgCol ? (bool) GETINT(this->onHiddenCfgCol) : true);
  }

  void Status::ChangeStatus(int net, std::string info, int st) {
    if (this->isNetUseful(net)) {
      bool dynSt = (this->stInfoVar.length() && !this->fCtrl->VarExists(this->stInfoVar)) ? true : false;

      if (dynSt)
        this->fCtrl->AddVar(this->stInfoVar, this->GetInfo(net));

      info = this->Parse(info, net);
      info = this->LimitChars(info, net);

      if (dynSt)
        this->fCtrl->RemoveVar(this->stInfoVar);

      IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) info.c_str());

      Control::Debug("[Status::ChangeStatus().item]: net = %i, status = %i, info = %s",
        net, st, (info.length() ? info.c_str() : "(none)"));
    }
  }

  std::string Status::GetActualInfo(int net) {
    std::string status;
    status = (char*) IMessage(IM_GET_STATUSINFO, net);

    return(status);
  }

  int Status::GetActualStatus(int net) {
    return(IMessage(IM_GET_STATUS, net));
  }

  std::string Status::GetInfo(int net) {
    std::string fake;
    for (tItemInfos::iterator it = this->info.begin(); it != this->info.end(); it++) {
      if (it->net == net) return(it->info);
    }
    return(fake);
  }

  void Status::AddInfo(std::string info, int net) {
    bool exists = false;

    for (tItemInfos::iterator it = this->info.begin(); it != this->info.end() && !exists; it++) {
      if (it->net == net) {
        it->info = info;
        exists = true;
      }
    }

    if (!exists) {
      itemInfo item;

      item.info = info;
      item.net = net;
      this->info.push_back(item);
    }

    Control::Debug("[Status::AddInfo().item]: net = %i, info = %s",
      net, (info.length() ? info.c_str() : "(none)"));
  }

  void Status::RememberInfo() {
    for (tItemNets::iterator it = this->lCtrl->nets.begin(); it != this->lCtrl->nets.end(); it++) {
      this->RememberInfo(it->net);
    }
  }

  void Status::RememberInfo(int net) {
    if (this->isNetUseful(net)) {
      std::string info = this->GetActualInfo(net);
      if (info.length()) this->AddInfo(info, net);

      Control::Debug("[Status::RememberInfo().item]: net = %i, info = %s",
        net, (info.length() ? info.c_str() : "(none)"));
    }
  }

  void Status::BackInfo() {
    for (tItemInfos::iterator it = this->info.begin(); it != this->info.end(); it++) {
      this->BackInfo(it->net);
    }
  }

  void Status::BackInfo(int net) {
    if (this->isNetUseful(net)) {
      std::string info = this->GetInfo(net);
      IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, -1 , (int) info.c_str());

      Control::Debug("[Status::BackInfo().item]: net = %i, info = %s",
        net, (info.length() ? info.c_str() : "(none)"));
    }
  }

  bool Status::isNetUseful(int net) {
    if (this->lCtrl->getNetState(net) && this->lCtrl->isConnected(net)) {
      if (!this->onHidden() && (ST_HIDDEN == this->GetActualStatus(net)))
        return(false);
      else
        return(true);
    }
    return(false);
  }

  std::string Status::Parse(std::string status, int net) {
    Control::Debug("[Status::Parse().item]: net = %i, info = %s",
      net, (status.length() ? status.c_str() : "(none)"));

    return(this->fCtrl->Parse(status));
  }

  std::string Status::LimitChars(std::string status, int net, int s) {
    UINT iLimit;

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
        iLimit = jabber_status_length;
        break;
      case plugsNET::dwutlenek:
        iLimit = tlen_status_length;
        break;
      case plugsNET::gg:
        iLimit = gg_status_length;
        break;
      default:
        iLimit = default_status_length;
        break;
    }

    iLimit = iLimit - s;
    if (status.length() > iLimit) {
      status.resize(iLimit);
      status.replace(status.length() - 3, 3, "...");
    }
    return(status);
  }
}