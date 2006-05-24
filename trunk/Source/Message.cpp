/**
 *  Message class
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
#include "Message.h"

void Message::reply(cMessage *msg, std::string body, std::string ext, bool useHtml) {
  Message::send(msg->fromUid, "", msg->net, body, msg->type, ext, useHtml);
}

cMessage Message::prepare(const std::string& to, const std::string& from, int net, const std::string& body, 
  int type, const std::string& ext, int flag) {
  cMessage msg;

  msg.flag = flag;
  msg.fromUid = (char*) from.c_str();
  msg.toUid = (char*) to.c_str();
  msg.net = net;
  msg.time = _time64(0);
  msg.type = type;
  msg.body = (char*) body.c_str();
  msg.ext = (char*) ext.c_str();

  return(msg);
}

void Message::send(cMessage *msg) {
  sMESSAGESELECT ms;
  if ((ms.id = Ctrl->ICMessage(IMC_NEWMESSAGE, (int)msg)) > 0) {
    Ctrl->ICMessage(IMC_MESSAGEQUEUE, (int)&ms);
  }
  logDebug("[Message::send().msg]: id = %i, net = %i, fromUid = %s, toUid = %s, body = %s", 
    ms.id, msg->net, nullChk(msg->fromUid), nullChk(msg->toUid), msg->body);
}

void Message::send(int cnt, std::string from, std::string body, int type, std::string ext, bool useHtml, bool insInMsgWnd) {
  std::string to = GETCNTC(cnt, CNT_UID);
  cMessage msg = Message::prepare(to, from, GETCNTI(cnt, CNT_NET), body, type, ext, MF_SEND | (useHtml ? MF_HTML : 0));

  Message::send(&msg);
  if (insInMsgWnd) Message::insInMsgWnd(&msg, cnt);
}

void Message::send(std::string to, std::string from, int net, std::string body, int type, std::string ext, bool useHtml) {
  cMessage msg = Message::prepare(to, from, net, body, type, ext, MF_SEND | (useHtml ? MF_HTML : 0));
  Message::send(&msg);
}

void Message::sms(std::string to, std::string body, std::string gate, std::string from, std::string ext) {
  ext = SetExtParam(ext, Sms::extFrom, from); // nie zawsze wymagany
  ext = SetExtParam(ext, Sms::extGate, gate);  // wymagany!
  // Je¿eli NIE chcemy, ¿eby wtyczka SMS dzieli³a wiadomoœæ, ustawiamy parametr extPart...
  // ext = SetExtParam(ext, Sms::extPart, "0");

  cMessage msg = Message::prepare(to, "", Sms::net, body, MT_SMS, ext, MF_SEND);
  Message::send(&msg);
}

void Message::insInMsgWnd(cMessage *msg, int cntID, const char * display, bool scroll) {
  Konnekt::UI::Notify::_insertMsg ni(msg, display, scroll);
  ni.act = sUIAction(IMIG_MSGWND, Konnekt::UI::ACT::msg_ctrlview, cntID);

  UIActionCall(&ni);
}