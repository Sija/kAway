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

#pragma once

#ifndef __MESSAGE_EX_H__
#define __MESSAGE_EX_H__

#include <konnekt/core_message.h>

#include "Helpers.h"

class MessageEx : public iObject {
public:
  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(MessageEx, iObject, Version(0,1,0,0));

  static Message prepare(const StringRef& to, const StringRef& from, tNet net, const StringRef& body, Message::enType type, const StringRef& ext, Message::enFlags flags);

  static void reply(Message& msg, const StringRef& body, const StringRef& ext = "", bool html = false);

  static void send(Message& msg);

  inline static void send(tCntId cnt, const StringRef& body, Message::enType type = Message::typeMessage, const StringRef& ext = "", bool html = false, bool inject = false) {
    MessageEx::send(cnt, "", body, type, ext, html, inject);
  }

  static void send(int cnt, const StringRef& from, const StringRef& body, Message::enType type = Message::typeMessage, const StringRef& ext = "", bool html = false, bool inject = false);

  inline static void send(const StringRef& to, tNet net, const StringRef& body, Message::enType type = Message::typeMessage, const StringRef& ext = "", bool html = false) {
    MessageEx::send(to, "", net, body, type, ext, html);
  }

  static void send(const StringRef& to, const StringRef& from, tNet net, const StringRef& body, Message::enType type = Message::typeMessage, const StringRef& ext = "", bool html = false);

  static void sms(const StringRef& to, const StringRef& body, const StringRef& gate, const StringRef& from = "", StringRef ext = "");

  static void inject(Message& msg, int cntID, const char * display = 0, bool scroll = true);
};

#endif // __MESSAGE_EX_H__