/**
  *  Contact Forwarder class
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
#include "CntForwarder.h"

namespace kAway2 {
  CntForwarder::CntForwarder() : Forwarder("forward", "Lustereczko", ico::forward, true, true) {
    //fCtrl->setEvtOnISetCols(boost::bind(&CntForwarder::onISetCols, this));
    //fCtrl->setEvtOnIPrepare(boost::bind(&CntForwarder::onIPrepare, this));
    //fCtrl->setEvtOnAction(boost::bind(&CntForwarder::onAction, this, _1, _2));
    //fCtrl->setEvtOnNewMsg(boost::bind(&CntForwarder::onNewMsg, this, _1, _2));
    //fCtrl->setEvtOnEnable(boost::bind(&CntForwarder::onEnable, this));
    //fCtrl->setEvtOnDisable(boost::bind(&CntForwarder::onDisable, this));
  }

  void CntForwarder::send(const StringRef& msg) {
    String ext;
    ext = SetExtParam(ext, cfg::extParamName, itos(cfg::tpl::forward));
    ext = SetExtParam(ext, MEX_ADDINFO, "kAway2");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    if (GETINT(cfg::forward::type) == toCnt) {
      Message::send(GETINT(cfg::forward::cnt), msg, MT_MESSAGE, ext, true);
    } else {
      Message::send(GETSTRA(cfg::forward::uid), GETINT(cfg::forward::net), msg, MT_MESSAGE, ext, true);
    }
  }

  void CntForwarder::onNewMsg(int cnt, cMessage *msg) {
    // TODO: wykrywanie zapêtlania
    Forwarder::onNewMsg(cnt, msg);
  }

  void CntForwarder::onISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::tpl::forward, DT_CT_STR, 
      "Wiadomoœæ od kontaktu [<b>{msgFrom}</b>] wys³ana o <b>{msgTime}</b>{, msgDate}:"
      "\r\n\r\n{msgBody}", "kAway2/tpl/forward");

    Ctrl->SetColumn(DTCFG, cfg::tpl::forwardSummary, DT_CT_STR, 
      "Od czasu w³¹czenia away'a, dosta³eœ <b>{msgCount}</b> wiadomoœci od <b>{userCount}</b> osób ({userList}).\r\n"
      "Ostatnia otrzymana wiadomoœæ jest od <b>{lastMsgFrom}</b>.", "kAway2/tpl/forwardSummary");

    Ctrl->SetColumn(DTCFG, cfg::forward::cnt, DT_CT_INT, 0, "kAway2/forward/cnt");
    Ctrl->SetColumn(DTCFG, cfg::forward::type, DT_CT_INT, toCnt, "kAway2/forward/type");
    Ctrl->SetColumn(DTCFG, cfg::forward::uid, DT_CT_STR, "", "kAway2/forward/uid");
    Ctrl->SetColumn(DTCFG, cfg::forward::net, DT_CT_INT, 0, "kAway2/forward/net");

    Forwarder::onISetCols();
  }

  void CntForwarder::onIPrepare() {
    IconRegister(IML_16, ico::forward, Ctrl->hDll(), IDI_FORWARD);

    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_GROUP, "Wiadomoœci przesy³aj ...");
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_RADIO | ACTSRADIO_BYPOS, "... do podanego kontaktu", cfg::forward::type);
    UIActionCfgAdd(ui::forward::cfgGroup, ui::forward::userCombo, ACTT_COMBO | ACTSCOMBO_LIST | ACTR_INIT, 0, 
      cfg::forward::cnt);

    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_RADIO | ACTSRADIO_BYPOS | ACTSRADIO_LAST, "... na podany ni¿ej adres", 
      cfg::forward::type);
    UIActionCfgAdd(ui::forward::cfgGroup, ui::forward::netsCombo, ACTT_COMBO | ACTSCOMBO_LIST | ACTR_INIT | ACTSC_INLINE, 0, 
      cfg::forward::net);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_EDIT | ACTSC_FULLWIDTH, 0, cfg::forward::uid);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_GROUPEND);

    Forwarder::onIPrepare();
  }

  void CntForwarder::onAction(int id, int code) {
    if (id == ui::forward::userCombo && code == ACTN_CREATE) {
      int count = Ctrl->IMessage(IMC_CNT_COUNT);
      String combo;

      for (int i = 0; i < count; i++) {
        if (i) {
          combo += strlen(GETCNTC(i, CNT_DISPLAY)) ? GETCNTC(i, CNT_DISPLAY) : GETCNTC(i, CNT_UID);
          combo += AP_ICO + itos(UIIcon(IT_LOGO, GETCNTI(i, CNT_NET), 0, 0));
          combo += AP_VALUE + itos(i) + "\n";
        } else {
          combo += "Wy³¹czone" AP_ICO "#2E" AP_VALUE "0\n";
        }
      }
      UIActionSetText(ui::forward::cfgGroup, ui::forward::userCombo, Helpers::trim(combo).a_str());
    } else if (id == ui::forward::netsCombo && code == ACTN_CREATE) {
      String combo, name;

      int count = Ctrl->IMessage(IMC_PLUG_COUNT);
      int id, type, net;

      for (int i = 0; i < count; i++) {
        if (i) {
          id = Ctrl->IMessage(IMC_PLUG_ID, 0, 0, i);
          type = Ctrl->IMessageDirect(IM_PLUG_TYPE, id);

          if ((type & IMT_NET) == IMT_NET) {
            net = (int) Ctrl->IMessageDirect(IM_PLUG_NET, id);
            name = (char*) Ctrl->IMessageDirect(IM_PLUG_NETNAME, id);

            if (name.length()) {
              combo += name + AP_ICO + itos(UIIcon(IT_LOGO, net, 0, 0));
              combo += AP_VALUE + itos(net) + "\n";
            }
          }
        } else {
          combo += "Brak" AP_ICO "#2E" AP_VALUE "0\n";
        }
      }
      UIActionSetText(ui::forward::cfgGroup, ui::forward::netsCombo, Helpers::trim(combo).a_str());
    }
  }
}