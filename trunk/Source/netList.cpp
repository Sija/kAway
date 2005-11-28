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
  netList::netList(const int cfgCol, const int cfgGroup, const int dynActGroup) {
    this->cfgCol = cfgCol;
    this->cfgGroup = cfgGroup;
    this->dynActGroup = dynActGroup;
  }

  netList::~netList() {
    this->nets.clear();
  }

  void netList::loadNets() {
    std::string buff(GETSTRA(this->cfgCol));
    std::list<itemNet> nets;

    if (!buff.empty()) {
      itemNet cfg;

      int pos = (int) buff.find("|");
      int off = 0;

      while (pos != std::string::npos) {
        cfg.net = atoi(buff.substr(off, pos - off).c_str());
        off = pos + 1;
        pos = (int) buff.find("|", off);
        cfg.use = atoi(buff.substr(off, pos - off).c_str()) ? true : false;
        off = pos + 1;
        pos = (int) buff.find("|", off);
        nets.push_back(cfg);
      }
    }

    int id, type, net;
    int plugs = IMessage( IMC_PLUG_COUNT );
    std::string name;
    itemNet cfg;

    for (int i = 1; i < plugs ; i++) {
      id = IMessage(IMC_PLUG_ID, 0, 0, i);
      type = IMessageDirect(IM_PLUG_TYPE, id);

      if ((type & IMT_NET) == IMT_NET) {
        net = (int)IMessageDirect(IM_PLUG_NET, id);
        name = (char*)IMessageDirect(IM_PLUG_NETNAME, id);

        if (name.length() && !this->isIgnored(net)) {
          cfg.id = id;
          cfg.net = net;
          cfg.name = name;
          cfg.use = false;

          for ( std::list<itemNet>::const_iterator it = nets.begin(); it != nets.end(); it++) {
            if ((*it).net == net && (*it).use) cfg.use = true; break;
          }
          this->nets.push_back(cfg);
        }
      }
    }
  }

  void netList::saveNets() {
    std::string buff;
    itemNet net;
    char v[10];

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      net = *it;
      buff += itoa(net.net, v, 10);
      buff += "|";
      buff += net.use ? "1" : "0";
      buff += "|";
    }

    SETSTR(this->cfgCol, buff.c_str());
  }

  void netList::UIdraw(int colCount) {
    itemNet item;
    int c = 1, i = 0, ico;
    int count = this->nets.size();

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++, i++, c++) {
      item = *it;
      ico = UIIcon(IT_LOGO, (int) item.net, 0, 0);
      i %= colCount; 

      UIActionCfgAdd(this->cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, shared::Icon16(ico).c_str(), 0, (i > 0) ? 10 : 0);
      UIActionCfgAdd(this->cfgGroup, this->dynActGroup + item.id, (i != (colCount - 1) && c != count) ? ACTT_CHECK | ACTSC_INLINE : ACTT_CHECK, item.name.c_str());
    }
  }

  void netList::UIgetState() {
    itemNet item;
    char buff[16];
    bool v;

    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      item = *it;
      UIActionCfgGetValue(sUIAction(this->cfgGroup, this->dynActGroup + item.id), buff, 16, true);
      v = (bool) atoi(buff);

      if (v != item.use) {
        item.use = v;
        *it = item;
      }
    }
  }

  void netList::UIsetState() {
    itemNet item;
    for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
      item = *it;
      // Wszystkie nowo utworzone checkboxy s� zaznaczone, wi�c tylko odznaczamy :)
      if (!item.use)
        UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup + item.id), "0", true);
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

  bool netList::isIgnored(int net) {
    for (int i = 0; i < sizeof(kAway2::ignoredNets) / sizeof(int); i++) {
      if (net == kAway2::ignoredNets[i]) return(true);
    }
    return(false);
  }
}