/*
 *  Message class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  class Message {
    public:
      static void ReplyMsg(cMessage * msg, std::string body, std::string ext = "", bool useHtml = false);
      static void SendMsg(std::string to, int net, std::string body, int type = MT_MESSAGE, std::string ext = "", bool useHtml = false);
  };
}