/*
 *  Message class
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005-2006 Sijawusz Pur Rahnama
 *
 *  $Id$
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

void Message::insInMsgWnd(cMessage *msg, int cntID, const char * display, bool scroll) {
  Konnekt::UI::Notify::_insertMsg ni(msg, display, scroll);
  ni.act = sUIAction(IMIG_MSGWND, Konnekt::UI::ACT::msg_ctrlview, cntID);

  UIActionCall(&ni);
}