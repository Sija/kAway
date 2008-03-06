/**
  *  MessageEx class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2008 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#include "stdafx.h"
#include "Message.h"

#include <konnekt/sms.h>

void MessageEx::reply(Message& msg, const StringRef& body, const StringRef& ext, bool html) {
  MessageEx::send(msg.getFromUid(), "", msg.getNet(), body, msg.getType(), ext, html);
}

Message MessageEx::prepare(const StringRef& to, const StringRef& from, tNet net, const StringRef& body, Message::enType type, const StringRef& ext, Message::enFlags flags) {
  Message msg;

  msg.setFlags(flags);
  msg.setFromUid(from);
  msg.setToUid(to);
  msg.setNet(net);
  msg.setTime(_time64(0));
  msg.setType(type);
  msg.setBody(body);
  msg.setExt(ext);

  return msg;
}

void MessageEx::send(Message& msg) {
  msg.addToQueue(true);

  Ctrl->log(logMisc, "Message", "send", "id = %i, net = %i, fromUid = %s, toUid = %s, body = %s", 
    msg.getId(), msg.getNet(), msg.getFromUid().a_str(), msg.getToUid().a_str(), msg.getBody().a_str());
}

void MessageEx::send(int cnt, const StringRef& from, const StringRef& body, Message::enType type, const StringRef& ext, bool html, bool inject) {
  String to = GETCNTC(cnt, CNT_UID);
  Message msg = MessageEx::prepare(to, from, (tNet) GETCNTI(cnt, CNT_NET), body, type, ext, (tMsgFlags) (Message::flagSend | (html ? Message::flagHTML : 0)));

  if (inject) {
    MessageEx::inject(msg, cnt);
  }
  MessageEx::send(msg);
}

void MessageEx::send(const StringRef& to, const StringRef& from, tNet net, const StringRef& body, Message::enType type, const StringRef& ext, bool html) {
  Message msg = MessageEx::prepare(to, from, net, body, type, ext, (tMsgFlags) (Message::flagSend | (Message::flagSend ? Message::flagHTML : 0)));
  MessageEx::send(msg);
}

void MessageEx::sms(const StringRef& to, const StringRef& body, const StringRef& gate, const StringRef& from, StringRef ext) {
  ext = SetExtParam(ext, Sms::extFrom, from); // nie zawsze wymagany
  ext = SetExtParam(ext, Sms::extGate, gate);  // wymagany!
  // Je¿eli NIE chcemy, ¿eby wtyczka SMS dzieli³a wiadomoœæ, ustawiamy parametr extPart...
  // ext = SetExtParam(ext, Sms::extPart, "0");

  Message msg = MessageEx::prepare(to, "", Net::sms, body, Message::typeSMS, ext, Message::flagSend);
  MessageEx::send(msg);
}

void MessageEx::inject(Message& msg, int cntID, const char * display, bool scroll) {
  /*
  Konnekt::UI::Notify::_insertMsg ni(&msg, display, scroll);
  ni.act = sUIAction(IMIG_MSGWND, Konnekt::UI::ACT::msg_ctrlview, cntID);

  UIActionCall(&ni);
  */
}