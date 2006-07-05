/**
 *  SMS Forwarder class
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
#include "SMSForwarder.h"

namespace kAway2 {
  SMSForwarder::SMSForwarder() : Forwarder("sms", "SMS", 501, true, true) {
    fCtrl->setEvtOnISetCols(boost::bind(&SMSForwarder::onISetCols, this));
    fCtrl->setEvtOnIPrepare(boost::bind(&SMSForwarder::onIPrepare, this));
    fCtrl->setEvtOnAction(boost::bind(&SMSForwarder::onAction, this, _1, _2));
    fCtrl->setEvtOnNewMsg(boost::bind(&SMSForwarder::onNewMsg, this, _1, _2));
    fCtrl->setEvtOnEnable(boost::bind(&SMSForwarder::onEnable, this));
    fCtrl->setEvtOnDisable(boost::bind(&SMSForwarder::onDisable, this));

    this->setCfgCols();
  }

  void SMSForwarder::send(std::string msg) {
    Message::sms(itos(GETINT(cfg::sms::number)), msg, GETSTRA(cfg::sms::gate), "kAway2");
  }

  void SMSForwarder::onISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::tpl::smsForward, DT_CT_STR, 
      "Wiadomoœæ od kontaktu [{msgFrom}] wys³ana o {msgTime}{, msgDate}:"
      "\r\n\r\n{msgBody}", "kAway2/tpl/smsForward");

    Ctrl->SetColumn(DTCFG, cfg::tpl::smsSummary, DT_CT_STR, 
      "Od czasu w³¹czenia away'a, dosta³eœ {msgCount} wiadomoœci od {userCount} osób ({userList}).\r\n"
      "Ostatnia otrzymana wiadomoœæ jest od {lastMsgFrom}.", "kAway2/tpl/smsSummary");

    Ctrl->SetColumn(DTCFG, cfg::sms::gate, DT_CT_STR, "", "kAway2/sms/gate");
    Ctrl->SetColumn(DTCFG, cfg::sms::number, DT_CT_INT, 0, "kAway2/sms/number");

    Forwarder::onISetCols();
  }

  void SMSForwarder::onIPrepare() {
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::sms::cfgGroup, ui::sms::number, ACTT_EDIT | ACTSC_INLINE | ACTSC_INT | ACTR_CHECK | ACTR_INIT, 0, 
      cfg::sms::number, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Numer na który bêd¹ wysy³ane SMSy");
    UIActionCfgAdd(ui::sms::cfgGroup, ui::sms::gates, ACTT_COMBO | ACTSC_INLINE | ACTSCOMBO_LIST | ACTSCOMBO_NOICON | ACTSCOMBO_SORT, 0,
      cfg::sms::gate, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Bramka która zostanie u¿yta do wys³ania wiadomoœci");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUPEND);

    Forwarder::onIPrepare();
  }

  void SMSForwarder::onAction(int id, int code) {
    if (id == ui::sms::number && (code == ACTN_CHECK || code == ACTN_CREATE)) {
      UIActionSetText(ui::sms::cfgGroup, ui::sms::gates, (char*) Ctrl->IMessage(Sms::IM::getGatewaysComboText, NET_SMS, IMT_ALL, (int) UIActionCfgGetValue(sUIAction(ui::sms::cfgGroup, ui::sms::number), 0, 0)));
    }
  }
}