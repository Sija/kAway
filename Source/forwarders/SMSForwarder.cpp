/**
  *  SMS Forwarder class
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
#include "SMSForwarder.h"

namespace kAway2 {
  SMSForwarder::SMSForwarder() : Forwarder("sms", "SMS", 501, true, true) {
    Controller* pCtrl = Controller::getInstance();

    pCtrl->registerObserver(IM_SETCOLS, bind(resolve_cast0(&SMSForwarder::onISetCols), this));
    pCtrl->registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&SMSForwarder::onIPrepare), this));
    pCtrl->registerObserver(IM_MSG_RCV, bind(&SMSForwarder::onNewMsg, this, _1)); // baaad
    pCtrl->registerObserver(im::away, bind(resolve_cast0(&SMSForwarder::onEnable), this));
    pCtrl->registerObserver(im::back, bind(resolve_cast0(&SMSForwarder::onDisable), this));
    pCtrl->registerActionObserver(ui::sms::number, bind(&SMSForwarder::refreshCombo, this, _1));
  }

  void SMSForwarder::send(const StringRef& msg) {
    Message::sms(itos(GETINT(cfg::sms::number)), msg, GETSTRA(cfg::sms::gate), GETSTRA(cfg::sms::sig));
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
    Ctrl->SetColumn(DTCFG, cfg::sms::sig, DT_CT_STR, "kAway2", "kAway2/sms/sig");

    Forwarder::onISetCols();
  }

  void SMSForwarder::onIPrepare() {
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::sms::cfgGroup, ui::sms::number, ACTT_EDIT | ACTSC_INLINE | ACTSC_INT | ACTR_CHECK | ACTR_INIT, 0, 
      cfg::sms::number, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Numer na który bêd¹ wysy³ane SMSy");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, AP_TIPRICH 
      "<b>Uwaga</b>: niektóre bramki SMS wymagaj¹ podpisu do poprawnego wys³ania wiadomoœci!", cfg::sms::sig, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Podpis dodawany do SMSa");
    UIActionCfgAdd(ui::sms::cfgGroup, ui::sms::gates, ACTT_COMBO | ACTSC_INLINE | ACTSCOMBO_LIST | ACTSCOMBO_NOICON | ACTSCOMBO_SORT, 0,
      cfg::sms::gate, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Bramka która zostanie u¿yta do wys³ania wiadomoœci");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUPEND);

    Forwarder::onIPrepare();
  }

  void SMSForwarder::refreshCombo(Controller* pCtrl) {
    int code = pCtrl->getAN()->code;
    if (code == ACTN_CHECK || code == ACTN_CREATE) {
      UIActionSetText(ui::sms::cfgGroup, ui::sms::gates, (char*) Ctrl->IMessage(Sms::IM::getGatewaysComboText, NET_SMS, IMT_ALL, (int) UIActionCfgGetValue(sUIAction(ui::sms::cfgGroup, ui::sms::number), 0, 0)));
    }
  }
}