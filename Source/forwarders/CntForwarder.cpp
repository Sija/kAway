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
    Controller* pCtrl = Controller::getInstance();

    pCtrl->registerObserver(IM_SETCOLS, bind(resolve_cast0(&CntForwarder::onISetCols), this));
    pCtrl->registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&CntForwarder::onIPrepare), this));
    pCtrl->registerObserver(IM_UIACTION, bind(&CntForwarder::onAction, this, _1));
    pCtrl->registerObserver(IM_MSG_RCV, bind(&CntForwarder::onNewMsg, this, _1)); // baaad
    pCtrl->registerObserver(im::away, bind(resolve_cast0(&CntForwarder::onEnable), this));
    pCtrl->registerObserver(im::back, bind(resolve_cast0(&CntForwarder::onDisable), this));
  }

  void CntForwarder::send(const StringRef& msg) {
    String ext;
    ext = SetExtParam(ext, cfg::extParamName, inttostr(cfg::tpl::forward));
    ext = SetExtParam(ext, MEX_ADDINFO, "kAway2");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    if (GETINT(cfg::forward::type) == toCnt) {
      Message::send(GETINT(cfg::forward::cnt), msg, MT_MESSAGE, ext, true);
    } else {
      Message::send(GETSTR(cfg::forward::uid), GETINT(cfg::forward::net), msg, MT_MESSAGE, ext, true);
    }
  }

  void CntForwarder::onNewMsg(Controller* pCtrl) {
    // TODO: wykrywanie zap�tlania
    Forwarder::onNewMsg(pCtrl);
  }

  void CntForwarder::onISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::tpl::forward, DT_CT_STR, 
      "Wiadomo�� od kontaktu [<b>{msgFrom}</b>] wys�ana o <b>{msgTime}</b>{, msgDate}:"
      "\r\n\r\n{msgBody}", "kAway2/tpl/forward");

    Ctrl->SetColumn(DTCFG, cfg::tpl::forwardSummary, DT_CT_STR, 
      "Od czasu w��czenia away'a, dosta�e� <b>{msgCount}</b> wiadomo�ci od <b>{userCount}</b> os�b ({userList}).\r\n"
      "Ostatnia otrzymana wiadomo�� jest od <b>{lastMsgFrom}</b>.", "kAway2/tpl/forwardSummary");

    Ctrl->SetColumn(DTCFG, cfg::forward::cnt, DT_CT_INT, 0, "kAway2/forward/cnt");
    Ctrl->SetColumn(DTCFG, cfg::forward::type, DT_CT_INT, toCnt, "kAway2/forward/type");
    Ctrl->SetColumn(DTCFG, cfg::forward::uid, DT_CT_STR, "", "kAway2/forward/uid");
    Ctrl->SetColumn(DTCFG, cfg::forward::net, DT_CT_INT, 0, "kAway2/forward/net");

    Forwarder::onISetCols();
  }

  void CntForwarder::onIPrepare() {
    IconRegister(IML_16, ico::forward, Ctrl->hDll(), IDI_FORWARD);

    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_GROUP, "Wiadomo�ci przesy�aj ...");
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_RADIO | ACTSRADIO_BYPOS, "... do podanego kontaktu", cfg::forward::type);
    UIActionCfgAdd(ui::forward::cfgGroup, ui::forward::userCombo, ACTT_COMBO | ACTSCOMBO_LIST | ACTR_INIT, 0, 
      cfg::forward::cnt);

    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_RADIO | ACTSRADIO_BYPOS | ACTSRADIO_LAST, "... na podany ni�ej adres", 
      cfg::forward::type);
    UIActionCfgAdd(ui::forward::cfgGroup, ui::forward::netsCombo, ACTT_COMBO | ACTSCOMBO_LIST | ACTR_INIT | ACTSC_INLINE, 0, 
      cfg::forward::net);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_EDIT | ACTSC_FULLWIDTH, 0, cfg::forward::uid);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_GROUPEND);

    Forwarder::onIPrepare();
  }

  void CntForwarder::onAction(Controller* pCtrl) {
    int id = pCtrl->getAN()->act.id, code = pCtrl->getAN()->code;

    if (id == ui::forward::userCombo && code == ACTN_CREATE) {
      int count = Ctrl->IMessage(IMC_CNT_COUNT);
      String combo;

      for (int i = 0; i < count; i++) {
        if (i) {
          combo += strlen(GETCNTC(i, CNT_DISPLAY)) ? GETCNTC(i, CNT_DISPLAY) : GETCNTC(i, CNT_UID);
          combo += AP_ICO + inttostr(UIIcon(IT_LOGO, GETCNTI(i, CNT_NET), 0, 0));
          combo += AP_VALUE + inttostr(i) + "\n";
        } else {
          combo += "Wy��czone" AP_ICO "#2E" AP_VALUE "0\n";
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
              combo += name + AP_ICO + inttostr(UIIcon(IT_LOGO, net, 0, 0));
              combo += AP_VALUE + inttostr(net) + "\n";
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