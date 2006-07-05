/**
 *  Contact Forwarder class
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @link         svn://kplugins.net/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once
#include "CntForwarder.h"

namespace kAway2 {
  CntForwarder::CntForwarder() : Forwarder("forward", "Lustereczko", ico::forward, true, true) {
    fCtrl->setEvtOnISetCols(boost::bind(&CntForwarder::onISetCols, this));
    fCtrl->setEvtOnIPrepare(boost::bind(&CntForwarder::onIPrepare, this));
    fCtrl->setEvtOnAction(boost::bind(&CntForwarder::onAction, this, _1, _2));
    fCtrl->setEvtOnNewMsg(boost::bind(&CntForwarder::onNewMsg, this, _1, _2));
    fCtrl->setEvtOnEnable(boost::bind(&CntForwarder::onEnable, this));
    fCtrl->setEvtOnDisable(boost::bind(&CntForwarder::onDisable, this));

    this->setCfgCols();
  }

  void CntForwarder::send(std::string msg) {
    std::string ext;
    ext = SetExtParam(ext, cfg::extParamName, itos(cfg::tpl::forward));
    ext = SetExtParam(ext, MEX_ADDINFO, "kAway2");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    Message::send(GETINT(cfg::forward::cnt), msg, MT_MESSAGE, ext, true);
  }

  void CntForwarder::onISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::tpl::forward, DT_CT_STR, 
      "Wiadomoœæ od kontaktu [<b>{msgFrom}</b>] wys³ana o <b>{msgTime}</b>{, msgDate}:"
      "\r\n\r\n{msgBody}", "kAway2/tpl/forward");

    Ctrl->SetColumn(DTCFG, cfg::tpl::forwardSummary, DT_CT_STR, 
      "Od czasu w³¹czenia away'a, dosta³eœ <b>{msgCount}</b> wiadomoœci od <b>{userCount}</b> osób ({userList}).\r\n"
      "Ostatnia otrzymana wiadomoœæ jest od <b>{lastMsgFrom}</b>.", "kAway2/tpl/forwardSummary");

    Ctrl->SetColumn(DTCFG, cfg::forward::cnt, DT_CT_INT, 0, "kAway2/forward/cnt");

    Forwarder::onISetCols();
  }

  void CntForwarder::onIPrepare() {
    IconRegister(IML_16, ico::forward, Ctrl->hDll(), IDI_FORWARD);

    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::forward::cfgGroup, ui::forward::userCombo, ACTT_COMBO | ACTSCOMBO_LIST | ACTR_INIT | ACTSC_INLINE, 0, 
      cfg::forward::cnt);
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_COMMENT, "Kontakt do którego wysy³ane bêd¹ wiadomoœci");
    UIActionCfgAdd(ui::forward::cfgGroup, 0, ACTT_GROUPEND);

    Forwarder::onIPrepare();
  }

  void CntForwarder::onAction(int id, int code) {
    if (id == ui::forward::userCombo && code == ACTN_CREATE) {
      int count = Ctrl->IMessage(IMC_CNT_COUNT);
      std::string combo;

      for (int i = 0; i < count; i++) {
        if (i) {
          combo += strlen(GETCNTC(i, CNT_DISPLAY)) ? GETCNTC(i, CNT_DISPLAY) : GETCNTC(i, CNT_UID);
          combo += AP_ICO + itos(UIIcon(IT_LOGO, GETCNTI(i, CNT_NET), 0, 0));
          combo += AP_VALUE + itos(i) + "\n";
        } else {
          combo += "Wy³¹czone" AP_ICO "#2E" AP_VALUE "0\n";
        }
      }
      UIActionSetText(ui::forward::cfgGroup, ui::forward::userCombo, Helpers::trim(combo).c_str());
    }
  }
}