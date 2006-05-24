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
  SMSForward::SMSForward() : Forwarder() {
    fCtrl->setEvtOnISetCols(boost::bind(&SMSForward::onISetCols, this));
    fCtrl->setEvtOnIPrepare(boost::bind(&SMSForward::onIPrepare, this));
    fCtrl->setEvtOnEnable(boost::bind(&SMSForward::onEnable, this));
    fCtrl->setEvtOnDisable(boost::bind(&SMSForward::onDisable, this));
    fCtrl->setEvtOnAction(boost::bind(&SMSForward::onAction, this, _1, _2));
    fCtrl->setEvtOnNewMsg(boost::bind(&SMSForward::onNewMsg, this, _1, _2));

    this->timer.reset(Stamina::timerTmplCreate(boost::bind(&SMSForward::send, this)));
    this->receivedMsgCount = 0;
  }

  SMSForward::~SMSForward() {
    this->msgSenders.clear();
    this->timer.reset();
  }

  void SMSForward::send() {
    if (!this->receivedMsgCount || !Ctrl->ICMessage(IMC_CONNECTED) || !GETINT(cfg::sms::interval) || 
      !strlen(GETSTRA(cfg::sms::number)) || !strlen(GETSTRA(cfg::sms::gate)) ||
      (GETINT(cfg::sms::minMsgCount) && this->receivedMsgCount < GETINT(cfg::sms::minMsgCount))) return;

    tMsgSenders::iterator begin = this->msgSenders.begin();
    std::string userList;

    for (tMsgSenders::iterator it = begin; it != this->msgSenders.end(); it++) {
      if (it != begin) userList += ", ";
      userList += (*it);
    }

    Format format;
    format.addVar("msgCount", itos(this->receivedMsgCount));
    format.addVar("userCount", itos(this->msgSenders.size()));
    format.addVar("userList", userList);
    format.addVar("lastMsgFrom", this->lastMsgFrom);

    Message::sms(itos(GETINT(cfg::sms::number)), format.parse(GETSTRA(cfg::tpl::sms)), 
      GETSTRA(cfg::sms::gate), "kAway2");

    this->receivedMsgCount = 0;
    this->msgSenders.clear();
  }

  void SMSForward::onISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::tpl::sms, DT_CT_STR, 
      "Od czasu w³¹czenia away'a, dosta³eœ {msgCount} wiadomoœci od {userCount} osób ({userList}).\r\n"
      "Ostatnia otrzymana wiadomoœæ jest od {lastMsgFrom}.", "kAway2/tpl/sms");

    Ctrl->SetColumn(DTCFG, cfg::sms::interval, DT_CT_INT, 60, "kAway2/sms/interval");
    Ctrl->SetColumn(DTCFG, cfg::sms::gate, DT_CT_STR, "", "kAway2/sms/gate");
    Ctrl->SetColumn(DTCFG, cfg::sms::number, DT_CT_INT, 0, "kAway2/sms/number");
    Ctrl->SetColumn(DTCFG, cfg::sms::minMsgCount, DT_CT_INT, 0, "kAway2/sms/minMsgCount");
  }

  void SMSForward::onIPrepare() {
    tHelpVars tplVars;

    /* tplVars.push_back(sHelpVar("date", "Data w³¹czenia trybu away"));
    tplVars.push_back(sHelpVar("time", "Czas w³¹czenia trybu away"));
    tplVars.push_back(sHelpVar("msg", "Przyczyna nieobecnoœci")); */

    tplVars.push_back(sHelpVar("msgCount", "Liczba otrzymanych wiad."));
    tplVars.push_back(sHelpVar("userCount", "Liczba ró¿nych nadawców wiad."));
    tplVars.push_back(sHelpVar("userList", "Lista nadawców wiad. po przecinku"));
    tplVars.push_back(sHelpVar("lastMsgFrom", "Nadawca ostatniej wiadomoœci"));

    UIGroupAdd(ui::cfgGroup, ui::sms::cfgGroup, ACTR_SAVE, "Raporty SMS", 501);

    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_SEPARATOR, "Odstêp czasu pomiêdzy wysy³anymi raportami [min]:");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_SLIDER, "Nie wysy³aj\n1 dzieñ" AP_MINIMUM "0" AP_MAXIMUM "1440" AP_STEP "60", 
      cfg::sms::interval);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::sms::cfgGroup, ui::sms::number, ACTT_EDIT | ACTSC_INLINE | ACTSC_INT | ACTR_CHECK | ACTR_INIT, 0, 
      cfg::sms::number, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Numer na który bêd¹ wysy³ane SMSy");
    UIActionCfgAdd(ui::sms::cfgGroup, ui::sms::gates, ACTT_COMBO | ACTSC_INLINE | ACTSCOMBO_LIST | ACTSCOMBO_NOICON | ACTSCOMBO_SORT, 0,
      cfg::sms::gate, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Bramka z której zostanie wys³any raport");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_SPINNER | ACTSC_INLINE, AP_MINIMUM "0" AP_MAXIMUM "1000" AP_TIP "0 = bez limitu", 
      cfg::sms::minMsgCount, 0, 0, 120);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_COMMENT, "Minimalna iloœæ wiadomoœci");
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUP, "Szablon powiadomienia");
    sCtrl->fCtrl->UIDrawHelpBtn(tplVars, ui::sms::cfgGroup);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::sms);
    UIActionCfgAdd(ui::sms::cfgGroup, 0, ACTT_GROUPEND);
  }

  void SMSForward::onAction(int id, int code) {
    if (id == ui::sms::number) {
      if (code == ACTN_CHECK || code == ACTN_CREATE) {
        UIActionSetText(ui::sms::cfgGroup, ui::sms::gates, (char*) Ctrl->IMessage(Sms::IM::getGatewaysComboText, NET_SMS, IMT_ALL, (int) UIActionCfgGetValue(sUIAction(ui::sms::cfgGroup, ui::sms::number), 0, 0)));
      }
    }
  }

  void SMSForward::onEnable() {
    this->receivedMsgCount = 0;
    this->msgSenders.clear();
    this->timer->repeat(GETINT(cfg::sms::interval) * 60 * 1000);
  }

  void SMSForward::onDisable() {
    this->timer->stop();
  }

  void SMSForward::onNewMsg(int cnt, cMessage *msg) {
    if (!strlen(msg->fromUid)) return;

    this->lastMsgFrom = GETCNTC(cnt, CNT_DISPLAY);
    this->receivedMsgCount++;

    if (!this->lastMsgFrom.length()) {
      this->lastMsgFrom = msg->fromUid;
    }

    for (tMsgSenders::iterator it = this->msgSenders.begin(); it != this->msgSenders.end(); it++) {
      if ((*it) == this->lastMsgFrom) return;
    }
    this->msgSenders.push_back(this->lastMsgFrom);
  }
}