/*
 *  kAway2 netList class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  netList::netList(const int cfgCol, const int cfgGroup, const int dynActGroup, const int actCreate, const int actDestroy) {
    this->configSaved = false;
    this->netsDrawn = false;

    this->cfgCol = cfgCol;
    this->cfgGroup = cfgGroup;
    this->dynActGroup = dynActGroup;
    this->actCreate = actCreate;
    this->actDestroy = actDestroy;
  }

  netList::~netList() {
    this->nets.clear();
  }

  void netList::loadNets() {
    std::string buff(GETSTRA(this->cfgCol));
    std::list<itemNet> nets;

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

        Control::Debug("[netList::loadNets().retroItem]: net = %i, use = %s",
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

          for (std::list<itemNet>::iterator it = nets.begin(); it != nets.end(); it++) {
            if ((*it).net == item.net && (*it).use) {
              item.use = true; break;
            }
          }
          this->nets.push_back(item);

          Control::Debug("[netList::loadNets().item]: id = %i, net = %i, name = %s, use = %s",
            item.id, item.net, item.name.c_str(), btoa(item.use));
        }
      }
    }
  }

  void netList::saveNets() {
    std::string buff;
    itemNet item;
    char v[10];

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      item = *it;
      buff += itoa(item.net, v, 10);
      buff += "|";
      buff += item.use ? "1" : "0";
      buff += "|";

      Control::Debug("[netList::saveNets().item]: name = %s, use = %s",
        item.name.c_str(), btoa(item.use));
    }

    SETSTR(this->cfgCol, buff.c_str());
  }

  void netList::actionHandle(const int id, const int code) {
    if (!this) return;

    if (id == this->cfgGroup && code == ACTN_SAVE) {
      this->configSaved = true;
    } else if (id == this->actCreate && code == ACTN_CREATEWINDOW) {
      this->netsDrawn = true;
      this->UISetState();
    } else if (id == this->actDestroy && code == ACTN_DESTROYWINDOW) {
      if (this->configSaved && this->netsDrawn) {
        Control::Debug("[netList::actionHandle()]: id = %i, code = %i", id, code);
        this->UIGetState();
        this->saveNets();
      }
      this->netsDrawn = false;
      this->configSaved = false;
    }
  }

  void netList::UIDraw(int colCount) {
    itemNet item;
    int c = 1, i = 0, ico;
    int count = this->nets.size();

    UIActionCfgAdd(this->cfgGroup, this->actDestroy, ACTT_HWND, 0, 0, 0, -20);

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++, i++, c++) {
      item = *it;
      ico = UIIcon(IT_LOGO, (int) item.net, 0, 0);
      i %= colCount; 

      Control::Debug("[netList::UIDraw().item]: name = %s, use = %s",
        item.name.c_str(), btoa(item.use));

      UIActionCfgAdd(this->cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, shared::Icon16(ico).c_str(), 0, (i > 0) ? 10 : 0);
      UIActionCfgAdd(this->cfgGroup, this->dynActGroup + item.id, (i != (colCount - 1) && c != count) ? ACTT_CHECK | ACTSC_INLINE : ACTT_CHECK, item.name.c_str());
    }

    UIActionCfgAdd(this->cfgGroup, this->actCreate, ACTT_HWND, 0, 0, 0, -20);
  }

  void netList::UIGetState() {
    itemNet item;
    char buff[16];
    bool v;

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      item = *it;
      UIActionCfgGetValue(sUIAction(this->cfgGroup, this->dynActGroup + item.id), buff, 16, true);
      v = (bool) atoi(buff);

      if (v != item.use) (*it).use = v;

      Control::Debug("[netList::UIGetState().item]: name = %s, use = %s [now: %s]",
        item.name.c_str(), btoa(item.use), btos(v).c_str());
    }
  }

  void netList::UISetState() {
    itemNet item;

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      item = *it;
      UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup + item.id), (item.use ? "1" : "0"), true);

      Control::Debug("[netList::UISetState().item]: name = %s, use = %s",
        item.name.c_str(), btoa(item.use));
    }
  }

  std::string netList::getNetName(int net) {
    std::string fake;
    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      if ((*it).net == net) return((*it).name);
    }
    return(fake);
  }

  bool netList::getNetState(int net) {
    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      if ((*it).net == net) return((*it).use);
    }
    return(false);
  }

  bool netList::setNetState(int net, bool use) {
    itemNet item;
    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      item = *it;

      if (item.net == net && item.use != use) {
        item.use = use;
        *it = item;
        return(true);
      }
    }
    return(false);
  }

  bool netList::isConnected(int net) {
    if (IMessage(IM_ISCONNECTED, net, IMT_PROTOCOL))
      return(true);
    else
      return(false);
  }

  bool netList::isIgnored(int net) {
    for (int i = 0; i < sizeof(ignoredNets) / sizeof(int); i++) {
      if (net == ignoredNets[i]) return(true);
    }
    return(false);
  }
}