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

#include "stdafx.h"
#include "Helpers.h"

class Message {
  public:
    static cMessage prepare(const std::string& to, const std::string& from, int net, 
      const std::string& body, int type, const std::string& ext, int flag);
    static void reply(cMessage *msg, std::string body, std::string ext = "", 
      bool useHtml = false);

    static void send(cMessage *msg);

    inline static void send(int cnt, std::string body, int type = MT_MESSAGE, 
      std::string ext = "", bool useHtml = false, bool insInMsgWnd = false) {
      Message::send(cnt, "", body, type, ext, useHtml, insInMsgWnd);
    }
    static void send(int cnt, std::string from, std::string body, int type = MT_MESSAGE, 
      std::string ext = "", bool useHtml = false, bool insInMsgWnd = false);

    inline static void send(std::string to, int net, std::string body, int type = MT_MESSAGE, 
      std::string ext = "", bool useHtml = false) {
      Message::send(to, "", net, body, type, ext, useHtml);
    }
    static void send(std::string to, std::string from, int net, std::string body, 
      int type = MT_MESSAGE, std::string ext = "", bool useHtml = false);

    static void insInMsgWnd(cMessage *msg, int cntID, const char * display = 0, 
      bool scroll = true);
};