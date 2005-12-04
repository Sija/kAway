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
  Status::Status(netList *lCtrl, int onHiddenCfgCol) {
    this->lCtrl = lCtrl;
    this->fCtrl = new Format;

    this->onHiddenCfgCol = onHiddenCfgCol;
  }

  Status::~Status() {
    delete this->fCtrl;
    this->fCtrl = NULL;

    this->info.clear();
  }

  void Status::ChangeStatus(const char * txt, int st) {
    for (std::list<itemNet>::iterator it = this->lCtrl->nets.begin(); it != this->lCtrl->nets.end(); it++) {
      this->ChangeStatus((*it).net, txt, st);
    }
  }

  bool Status::onHidden() {
    return((this->onHiddenCfgCol ? (bool) GETINT(this->onHiddenCfgCol) : true));
  }

  void Status::ChangeStatus(int net, const char * txt, int st) {
    std::string buff, status(txt);

    if (this->isNetUseful(net)) {
      buff = this->Parse(status, net);
      buff = this->LimitChars(buff, net);

      IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, st, (int) buff.c_str());

      Control::Debug("[Status::ChangeStatus().item]: net = %i, status = %i, info = %s",
        net, st, (buff.length() ? buff.c_str() : "(none)"));
    }
  }

  std::string Status::GetInfo(int net) {
    std::string fake;
    for (std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end(); it++) {
      if ((*it).net == net) return((*it).info);
    }
    return(fake);
  }

  void Status::AddInfo(char * info, int net) {
    itemInfo item;
    std::string txt(info);
    bool exists = false;

    for (std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end() && !exists; it++) {
      item = *it;
      if (item.net == net && item.info.compare(info)) {
        item.info = txt;
        *it = item;
        exists = true;
      }
    }

    if (!exists) {
      item.info = txt;
      item.net = net;
      this->info.push_back(item);
    }

    Control::Debug("[Status::AddInfo().item]: net = %i, info = %s",
      net, (info ? info : "(none)"));
  }

  void Status::RememberInfo() {
    for (std::list<itemNet>::iterator it = this->lCtrl->nets.begin(); it != this->lCtrl->nets.end(); it++) {
      this->RememberInfo((*it).net);
    }
  }

  void Status::RememberInfo(int net) {
    if (this->isNetUseful(net, false)) {
      char * info = NULL;

      info = (char*) IMessage(IM_GET_STATUSINFO, net);
      if (info) this->AddInfo(info, net);
    }
  }

  void Status::BackInfo() {
    for (std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end(); it++) {
      this->BackInfo((*it).net);
    }
  }

  void Status::BackInfo(int net) {
    if (this->isNetUseful(net, false)) {
      std::string info;

      info = this->GetInfo(net);
      IMessage(IM_CHANGESTATUS, net, IMT_PROTOCOL, -1 , (int) info.c_str());

      Control::Debug("[Status::BackInfo().item]: net = %i, info = %s",
        net, (info.length() ? info.c_str() : "(none)"));
    }
  }

  bool Status::isNetUseful(int net, bool onHidden) {
    if (this->lCtrl->getNetState(net) && this->lCtrl->isConnected(net)) {
      if (onHidden && !this->onHidden() && (ST_HIDDEN == IMessage(IM_GET_STATUS, net)))
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