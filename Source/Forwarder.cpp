/**
  *  Forwarder Base class
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
#include "Forwarder.h"

namespace kAway2 {
  Forwarder::Forwarder(const StringRef& _id, const StringRef& _title, int _ico, bool isSummarizable, bool isForwardable) :
    title(_title), ico(_ico), id(_id) {

    this->isSummarizable = isSummarizable;
    this->isForwardable = isForwardable;

    if (this->isSummarizable) {
      this->oSummary = new Summary(this);
    }
    if (this->isForwardable) {
      this->oForward = new Forward(this);
    }
  }

  Forwarder::~Forwarder() {
    if (this->isSummarizable) {
      delete this->oSummary;
      this->oSummary = NULL;
    }
    if (this->isForwardable) {
      delete this->oForward;
      this->oForward = NULL;
    }
  }

  /**
    * Forwarder::Summary
    */
  Forwarder::Summary::Summary(Forwarder *fwd) : parent(fwd) {
    this->timer.reset(timerTmplCreate(bind(&Forwarder::Summary::send, this)));
    this->clear();
  }

  void Forwarder::Summary::send() {
    int minMsgCount = GETINT(cfg::summary::minMsgCount);
    int interval = GETINT(cfg::summary::interval);

    if (!this->isActive() || !this->receivedMsgCount || !Ctrl->ICMessage(IMC_CONNECTED) || 
      !interval || !this->parent->preSummary() || (minMsgCount && this->receivedMsgCount < minMsgCount)) 
      return;

    this->parent->send(this->getBody());
    this->clear();
  }

  void Forwarder::Summary::onEnable() {
    int interval = GETINT(cfg::summary::interval);

    if (!this->isActive() || !interval || (!GETINT(cfg::summary::inAutoAway) && 
      Ctrl->IMessageDirect(api::isAutoAway, Ctrl->ID()))) return;

    this->timer->repeat(interval * 60 * 1000);
  }

  void Forwarder::Summary::onDisable() {
    this->timer->stop();
    this->clear();
  }

  void Forwarder::Summary::onNewMsg(int cnt, cMessage *msg) {
    if (!strlen(msg->fromUid)) return;

    this->lastMsgFrom = GETCNTC(cnt, CNT_DISPLAY);
    if (!this->lastMsgFrom.length()) {
      this->lastMsgFrom = msg->fromUid;
    }
    this->receivedMsgCount++;

    for (tMsgSenders::iterator it = this->msgSenders.begin(); it != this->msgSenders.end(); it++) {
      if ((*it) == this->lastMsgFrom) return;
    }
    this->msgSenders.push_back(this->lastMsgFrom);
  }

  String Forwarder::Summary::getMsgSendersList() {
    tMsgSenders::iterator begin = this->msgSenders.begin();
    String userList;

    for (tMsgSenders::iterator it = begin; it != this->msgSenders.end(); it++) {
      userList += (it != begin ? ", " : "") + (*it);
    }
    return userList;
  }

  String Forwarder::Summary::getBody() {
    Format format;
    format.addVar("msgCount", itos(this->receivedMsgCount));
    format.addVar("userCount", itos(this->msgSenders.size()));
    format.addVar("lastMsgFrom", this->lastMsgFrom);
    format.addVar("userList", this->getMsgSendersList());

    return format.parse(GETSTRA(this->parent->cfgCols["tplSummary"]));
  }

  void Forwarder::Summary::cfgTplDraw() {
    Format::tHelpVars tplVars;
    Format format;

    /* tplVars.push_back(Format::sHelpVar("date", "Data w³¹czenia trybu away"));
    tplVars.push_back(Format::sHelpVar("time", "Czas w³¹czenia trybu away"));
    tplVars.push_back(Format::sHelpVar("msg", "Przyczyna nieobecnoœci")); */

    tplVars.push_back(Format::sHelpVar("msgCount", "Liczba otrzymanych wiad."));
    tplVars.push_back(Format::sHelpVar("userCount", "Liczba ró¿nych nadawców wiad."));
    tplVars.push_back(Format::sHelpVar("userList", "Lista nadawców wiad. po przecinku"));
    tplVars.push_back(Format::sHelpVar("lastMsgFrom", "Nadawca ostatniej wiadomoœci"));

    UIActionCfgAdd(this->parent->cfgCols["cfgGroup"], 0, ACTT_GROUP, "Szablon raportu");
    format.UIDrawHelpBtn(tplVars, this->parent->cfgCols["cfgGroup"]);
    UIActionCfgAdd(this->parent->cfgCols["cfgGroup"], 0, ACTT_TEXT, 0, this->parent->cfgCols["tplSummary"]);
    UIActionCfgAdd(this->parent->cfgCols["cfgGroup"], 0, ACTT_GROUPEND);
  }

  /**
    * Forwarder::Forward
    */
  Forwarder::Forward::Forward(Forwarder *fwd) : parent(fwd) {
    //
  }

  String Forwarder::Forward::getBody(int cnt, cMessage *msg) {
    Date64 date(msg->time);
    Format format;

    format.addVar("msgFrom", !strlen(GETCNTC(cnt, CNT_DISPLAY)) ? msg->fromUid : GETCNTC(cnt, CNT_DISPLAY));
    format.addVar("msgTime", date.strftime(GETSTRA(cfg::timeFormat)));
    format.addVar("msgDate", Helpers::isToday(date) ? "" : date.strftime(GETSTRA(cfg::dateFormat)));
    format.addVar("msgBody", msg->body);

    return format.parse(GETSTRA(this->parent->cfgCols["tplForward"]));
  }

  void Forwarder::Forward::cfgTplDraw() {
    Format::tHelpVars tplVars;
    Format format;

    /* tplVars.push_back(Format::sHelpVar("date", "Data w³¹czenia trybu away"));
    tplVars.push_back(Format::sHelpVar("time", "Czas w³¹czenia trybu away"));
    tplVars.push_back(Format::sHelpVar("msg", "Przyczyna nieobecnoœci")); */

    tplVars.push_back(Format::sHelpVar("msgFrom", "Nadawca wiadomoœci"));
    tplVars.push_back(Format::sHelpVar("msgTime", "Czas nadejœcia wiadomoœci"));
    tplVars.push_back(Format::sHelpVar("msgDate", "Data nadejœcia wiadomoœci"));
    tplVars.push_back(Format::sHelpVar("msgBody", "Treœæ wiadomoœci"));

    UIActionCfgAdd(this->parent->cfgCols["cfgGroup"], 0, ACTT_GROUP, "Szablon przekierowania");
    format.UIDrawHelpBtn(tplVars, this->parent->cfgCols["cfgGroup"]);
    UIActionCfgAdd(this->parent->cfgCols["cfgGroup"], 0, ACTT_TEXT, 0, this->parent->cfgCols["tplForward"]);
    UIActionCfgAdd(this->parent->cfgCols["cfgGroup"], 0, ACTT_GROUPEND);
  }

  void Forwarder::Forward::onNewMsg(int cnt, cMessage *msg) {
    if (!this->isActive() || !strlen(msg->fromUid) || (!GETINT(cfg::fwd::inAutoAway) && 
      Ctrl->IMessageDirect(api::isAutoAway, Ctrl->ID())) || !this->parent->preForward(cnt, msg)) {
      return;
    }
    this->parent->send(this->getBody(cnt, msg));
  }
}