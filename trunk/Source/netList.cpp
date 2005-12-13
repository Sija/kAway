/*
 *  kAway2 NetList class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  NetList::NetList(int cfgCol, int cfgGroup, int dynActGroup, int actCreate, int actDestroy) {
    this->configSaved = false;
    this->netsDrawn = false;

    this->cfgCol = cfgCol;
    this->cfgGroup = cfgGroup;
    this->dynActGroup = dynActGroup;
    this->actCreate = actCreate;
    this->actDestroy = actDestroy;
  }

  NetList::~NetList() {
    this->nets.clear();
  }

  void NetList::loadNets() {
    std::string buff(GETSTRA(this->cfgCol));
    tItemNets nets;

    if (!buff.empty()) {
      itemNet item;

      int pos = (int) buff.find("|");
      int off = 0;

      while (pos != std::string::npos) {
        item.net = atoi(buff.substr(off, pos - off).c_str());
        off = pos + 1;
        pos = (int) buff.find("|", off);
        item.use = atoi(buff.substr(off, pos - off).c_str()) ? true : false;
        off = pos + 1;
        pos = (int) buff.find("|", off);

        Control::Debug("[NetList::loadNets().retroItem]: net = %i, use = %s",
          item.net, btoa(item.use));

        nets.push_back(item);
      }
    }

    int id, type, net;
    int plugs = IMessage(IMC_PLUG_COUNT);

    std::string name;
    itemNet item;

    for (int i = 1, n = 1; i < plugs; i++) {
      id = IMessage(IMC_PLUG_ID, 0, 0, i);
      type = IMessageDirect(IM_PLUG_TYPE, id);

      if ((type & IMT_NET) == IMT_NET) {
        net = (int) IMessageDirect(IM_PLUG_NET, id);
        name = (char*) IMessageDirect(IM_PLUG_NETNAME, id);

        if (name.length() && !this->isIgnored(net)) {
          item.net = net;
          item.id = n++;
          item.name = name;
          item.use = false;

          for (tItemNets::iterator it = nets.begin(); it != nets.end(); it++) {
            if (it->net == item.net && it->use) {
              item.use = true; break;
            }
          }
          this->nets.push_back(item);

          Control::Debug("[NetList::loadNets().item]: id = %i, net = %i, name = %s, use = %s",
            item.id, item.net, item.name.c_str(), btoa(item.use));
        }
      }
    }
  }

  void NetList::saveNets() {
    std::string buff;
    char v[10];

    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      buff += itoa(it->net, v, 10);
      buff += "|";
      buff += it->use ? "1" : "0";
      buff += "|";

      Control::Debug("[NetList::saveNets().item]: name = %s, use = %s",
        it->name.c_str(), btoa(it->use));
    }

    SETSTR(this->cfgCol, buff.c_str());
  }

  void NetList::actionHandle(int id, int code) {
    if (!this) return;

    if (id == this->cfgGroup && code == ACTN_SAVE) {
      this->configSaved = true;
    } else if (id == this->actCreate && code == ACTN_CREATEWINDOW) {
      this->netsDrawn = true;
      this->UISetState();
    } else if (id == this->actDestroy && code == ACTN_DESTROYWINDOW) {
      if (this->configSaved && this->netsDrawn) {
        Control::Debug("[NetList::actionHandle()]: id = %i, code = %i", id, code);
        this->UIGetState();
        this->saveNets();
      }
      this->netsDrawn = false;
      this->configSaved = false;
    }
  }

  void NetList::UIDraw(int colCount) {
    int c = 1, i = 0, ico;
    int count = this->nets.size();

    UIActionCfgAdd(this->cfgGroup, this->actDestroy, ACTT_HWND, 0, 0, 0, -20);

    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++, i++, c++) {
      ico = UIIcon(IT_LOGO, (int) it->net, 0, 0);
      i %= colCount; 

      Control::Debug("[NetList::UIDraw().item]: name = %s, use = %s",
        it->name.c_str(), btoa(it->use));

      UIActionCfgAdd(this->cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, shared::Icon16(ico).c_str(), 0, (i > 0) ? 10 : 0);
      UIActionCfgAdd(this->cfgGroup, this->dynActGroup + it->id, (i != (colCount - 1) && c != count) ? ACTT_CHECK | ACTSC_INLINE : ACTT_CHECK, it->name.c_str());
    }

    UIActionCfgAdd(this->cfgGroup, this->actCreate, ACTT_HWND, 0, 0, 0, -20);
  }

  void NetList::UIGetState() {
    char buff[16];
    bool v;

    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      UIActionCfgGetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), buff, 16, true);
      v = (bool) atoi(buff);

      Control::Debug("[NetList::UIGetState().item]: name = %s, use = %s [now: %s]",
        it->name.c_str(), btoa(it->use), btos(v).c_str());

      if (it->use != v) it->use = v;
    }
  }

  void NetList::UISetState() {
    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), (it->use ? "1" : "0"), true);

      Control::Debug("[NetList::UISetState().item]: name = %s, use = %s",
        it->name.c_str(), btoa(it->use));
    }
  }

  std::string NetList::getNetName(int net) {
    std::string fake;
    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      if (it->net == net) return(it->name);
    }
    return(fake);
  }

  bool NetList::getNetState(int net) {
    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      if (it->net == net) return(it->use);
    }
    return(false);
  }

  bool NetList::setNetState(int net, bool use) {
    for (tItemNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      if (it->net == net) {
        if (it->use != use) it->use = use;
        return(true);
      }
    }
    return(false);
  }

  bool NetList::isConnected(int net) {
    if (IMessage(IM_ISCONNECTED, net, IMT_PROTOCOL))
      return(true);
    else
      return(false);
  }

  bool NetList::isIgnored(int net) {
    for (int i = 0; i < sizeof(ignoredNets) / sizeof(int); i++) {
      if (net == ignoredNets[i]) return(true);
    }
    return(false);
  }
}