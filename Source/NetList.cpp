/**
  *  NetList class
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
#include "NetList.h"

void NetList::loadNets() {
  String buff(GETSTRA(this->cfgCol));
  tNets nets;

  if (!buff.empty()) {
    sNet item;

    int pos = (int) buff.find("|");
    int off = 0;

    while (pos != String::npos) {
      item.net = atoi(buff.substr(off, pos - off).a_str());
      off = pos + 1;
      pos = (int) buff.find("|", off);
      item.use = atoi(buff.substr(off, pos - off).a_str()) ? true : false;
      off = pos + 1;
      pos = (int) buff.find("|", off);

      logDebug("[NetList<%i>::loadNets().saved-item]: net = %i, use = %s",
        this, item.net, btoa(item.use));

      nets.push_back(item);
    }
  }

  int id, type, net;
  int plugs = IMessage(IMC_PLUG_COUNT);

  String name;
  sNet item;

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
        item.use = this->defaultUse;

        for (tNets::iterator it = nets.begin(); it != nets.end(); it++) {
          if (it->net == item.net) {
            item.use = it->use; break;
          }
        }
        this->nets.push_back(item);

        logDebug("[NetList<%i>::loadNets().item]: id = %i, net = %i, name = %s, use = %s",
          this, item.id, item.net, item.name.c_str(), btoa(item.use));
      }
    }
  }
}

void NetList::saveNets() {
  String buff;
  char v[10];

  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    buff += itoa(it->net, v, 10);
    buff += "|";
    buff += it->use ? "1" : "0";
    buff += "|";

    logDebug("[NetList<%i>::saveNets().item]: name = %s, use = %s",
      this, it->name.c_str(), btoa(it->use));
  }

  SETSTR(this->cfgCol, buff.a_str());
}

void NetList::actionHandle(int id, int code) {
  if (!this || !Ctrl || !Ctrl->isRunning()) return;

  if ((id == this->cfgGroup) && (code == ACTN_SAVE) && this->netsDrawn) {
    logDebug("[NetList<%i>::actionHandle()]: id = %i, code = %i", this, id, code);
    this->UIGetState();
    this->saveNets();
  } else if ((id == this->actCreate) && (code == ACTN_CREATEWINDOW)) {
    this->netsDrawn = true;
    this->UISetState();
  } else if ((id == this->actDestroy) && (code == ACTN_DESTROYWINDOW)) {
    this->netsDrawn = false;
  }
}

void NetList::UIDraw(int colCount, char *groupTitle) {
  int i = 0, col = 0, ico;
  int netsCount = this->nets.size();

  if (!netsCount)
    return;

  if (this->selection == typeRadiosEmpty) {
    this->nets.push_back(sNet(0, 0, (String) "Brak", 0));
    netsCount++;
  }

  if (groupTitle) {
    UIActionCfgAdd(this->cfgGroup, 0, ACTT_GROUP, groupTitle);
  }
  UIActionCfgAdd(this->cfgGroup, this->actDestroy, ACTT_HWND, 0, 0, 0, -20);

  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++, i++, col++) {
    ico = UIIcon(IT_LOGO, (int) it->net, 0, 0);
    col %= colCount;

    logDebug("[NetList<%i>::UIDraw().item]: name = %s, use = %s",
      this, it->name.c_str(), btoa(it->use));

    UIActionCfgAdd(this->cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico).a_str(), 0, (col > 0) ? 10 : 0);
    if (this->selection != typeCheckboxes) {
      String name = it->name + AP_VALUE + itos(it->net);
      UIActionCfgAdd(this->cfgGroup, this->dynActGroup + it->id, ACTT_RADIO | 
        ((col != (colCount - 1) && i != (netsCount - 1)) ? ACTSC_INLINE : 0) |
        (i != (netsCount - 1) ? 0 : ACTSRADIO_LAST), name.a_str());
    } else {
      UIActionCfgAdd(this->cfgGroup, this->dynActGroup + it->id, ACTT_CHECK | 
        ((col != (colCount - 1) && i != (netsCount - 1)) ? ACTSC_INLINE : 0), it->name.a_str());
    }
  }

  UIActionCfgAdd(this->cfgGroup, this->actCreate, ACTT_HWND, 0, 0, 0, -20);
  if (groupTitle) {
    UIActionCfgAdd(this->cfgGroup, 0, ACTT_GROUPEND);
  }

  if (this->selection == typeRadiosEmpty) {
    this->nets.pop_back();
  }
}

void NetList::UIGetState() {
  char buff[16];
  bool v;

  if (this->selection != typeCheckboxes) {
    UIActionCfgGetValue(sUIAction(this->cfgGroup, this->dynActGroup + this->nets.size()), buff, 16, true);
  }

  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (this->selection != typeCheckboxes) {
      v = (it->net == atoi(buff)) ? true : false;
    } else {
      v = *UIActionCfgGetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), 0, 0, true) == '1';
    }

    logDebug("[NetList<%i>::UIGetState().item]: name = %s, use = %s [now: %s]",
      this, it->name.c_str(), btoa(it->use), btoa(v));

    if (it->use != v) it->use = v;
  }
}

void NetList::UISetState() {
  bool isCheckedAny = false;

  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (this->selection != typeCheckboxes) {
      if (it->use) {
        UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), itos(it->net).a_str(), true);
        isCheckedAny = true;
      }
    } else {
      UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup + it->id), (it->use ? "1" : "0"), true);
    }

    logDebug("[NetList<%i>::UISetState().item]: name = %s, use = %s",
      this, it->name.c_str(), btoa(it->use));
  }

  if (this->selection == typeRadiosEmpty && !isCheckedAny) {
    UIActionCfgSetValue(sUIAction(this->cfgGroup, this->dynActGroup), "0", true);
  }
}

String NetList::getNetName(int net) {
  String fake;
  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (it->net == net) return it->name;
  }
  return fake;
}

bool NetList::getNetState(int net) {
  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (it->net == net) return it->use;
  }
  return false;
}

bool NetList::setNetState(int net, bool use) {
  for (tNets::iterator it = this->nets.begin(); it != this->nets.end(); it++) {
    if (it->net == net) {
      if (it->use != use) it->use = use;
      return true;
    }
  }
  return false;
}

bool NetList::isConnected(int net) {
  return Ctrl->IMessage(IM_ISCONNECTED, net, IMT_PROTOCOL);
}

bool NetList::isIgnored(int net) {
  for (tIgnoredNets::iterator it = this->ignoredNets.begin(); it != this->ignoredNets.end(); it++) {
    if ((*it) == net) return true;
  }

  int ignored = 0;
  Ctrl->IMessage(IM::hidePresence, net, ignored);

  if (Ctrl->getError() != IMERROR_NORESULT) {
    return ignored;
  }
  return false;
}

void NetList::addIgnored(int net) {
  this->ignoredNets.push_back(net);
}

void NetList::removeIgnored(int net) {
  for (tIgnoredNets::iterator it = this->ignoredNets.begin(); it != this->ignoredNets.end(); it++) {
    if ((*it) == net) {
      it = this->ignoredNets.erase(it); break;
    }
  }
}