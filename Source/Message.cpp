/*
 *  Message class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

namespace kAway2 {
  void Message::ReplyMsg(cMessage * msg, std::string body, std::string ext, bool useHtml) {
    Message::SendMsg(msg->fromUid, msg->net, body, msg->type, ext, useHtml);
  }

  void Message::SendMsg(std::string to, int net, std::string body, int type, std::string ext, bool useHtml) {
    cMessage msg;
    msg.flag = MF_SEND | (useHtml ? MF_HTML : 0);
    msg.fromUid = "";
    msg.toUid = (char*) to.c_str();
    msg.net = net;
    msg.time = 0;
    msg.type = type;
    msg.body = (char*) body.c_str();
    msg.ext = (char*) ext.c_str();
    sMESSAGESELECT msc;

    msc.id = ICMessage(IMC_NEWMESSAGE, (int) &msg);
    if (msc.id) ICMessage(IMC_MESSAGEQUEUE, (int) &msc);
  }
}