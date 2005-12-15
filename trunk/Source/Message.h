/*
 *  kAway2 Message class
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
      static cMessage prepare(const std::string& to, int net, const std::string& body, int type, 
        const std::string& ext, int flag);
      static void reply(cMessage *msg, std::string body, std::string ext = "", bool useHtml = false);

      static void send(cMessage *msg);
      static void send(int cnt, std::string body, int type = MT_MESSAGE, 
        std::string ext = "", bool useHtml = false, bool insInMsgWnd = false);
      static void send(std::string to, int net, std::string body, int type = MT_MESSAGE, 
        std::string ext = "", bool useHtml = false);

      static void insInMsgWnd(cMessage *msg, int cntID, const char * display = 0, bool scroll = true);
  };
}