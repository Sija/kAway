/**
  *  Forwarder Base class
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

#ifndef __FORWARDER_H__
#define __FORWARDER_H__

#include "kAway2.h"
#include "Helpers.h"
#include "Format.h"

#include <konnekt/contrib/Events/IMEvent.h>

namespace kAway2 {
  class Forwarder : public signals::trackable {
  public:
    class Summary;
    class Forward;

  public:
    Forwarder(const StringRef& id, const StringRef& title, int ico,
      bool isSummarizable = false, 
      bool isForwardable = false);
    virtual ~Forwarder();

  public:
    virtual void setCfgCols() = 0;
    virtual void send(const StringRef& msg) = 0;

    virtual bool preForward(int cnt, Message *msg) = 0;
    virtual bool preSummary() = 0;

    inline void onISetCols() {
      this->setCfgCols();

      if (this->isSummarizable) {
        Ctrl->SetColumn(tableConfig, this->cfgCols["isSummaryActive"], ctypeInt, 0, ("kAway2/" + this->id + "/isSummaryActive").a_str());
      }
      if (this->isForwardable) {
        Ctrl->SetColumn(tableConfig, this->cfgCols["isForwardActive"], ctypeInt, 0, ("kAway2/" + this->id + "/isForwardActive").a_str());
      }
    }
    inline void onIPrepare() {
      if (this->isSummarizable) {
        this->oSummary->cfgTplDraw();
      }
      if (this->isForwardable) {
        this->oForward->cfgTplDraw();
      }
    }
    virtual void onNewMsg(IMEvent& ev) {
      Message* msg = (Message*) ev.getP1();
      int cnt = 0; // baaad

      if (this->isSummarizable) {
        this->oSummary->onNewMsg(cnt, msg);
      }
      if (this->isForwardable) {
        this->oForward->onNewMsg(cnt, msg);
      }
    }
    inline void onEnable() {
      if (this->isSummarizable) {
        this->oSummary->onEnable();
      }
    }
    inline void onDisable() {
      if (this->isSummarizable) {
        this->oSummary->onDisable();
      }
    }

  public:
    std::map<std::string, int> cfgCols;
    unsigned int ico;
    String title;
    String id;

    bool isSummarizable;
    bool isForwardable;

  public:
    Summary *oSummary;
    Forward *oForward;

  /**
    * Summary
    */
  public:
    class Summary {
    public:
      typedef std::deque<String> tMsgSenders;

    public:
      Summary(Forwarder *fwd);

    public:
      inline void clear() {
        this->receivedMsgCount = 0;
        this->msgSenders.clear();
      }
      void send();

      void cfgTplDraw();

      inline bool isActive() {
        return GETINT(this->parent->cfgCols["isSummaryActive"]);
      }

      void onEnable();
      void onDisable();
      void onNewMsg(int cnt, Message *msg);

      String getMsgSendersList();
      String getBody();

    protected:
      shared_ptr<TimerDynamic> timer;
      unsigned int receivedMsgCount;
      String lastMsgFrom;
      tMsgSenders msgSenders;
      Forwarder *parent;
    };

  /**
    * Forward
    */
  public:
    class Forward {
    public:
      Forward(Forwarder *fwd);

    public:
      void cfgTplDraw();
      void onNewMsg(int cnt, Message *msg);
      String getBody(int cnt, Message *msg);

      inline bool isActive() {
        return GETINT(this->parent->cfgCols["isForwardActive"]);
      }

    protected:
      Forwarder *parent;
    };
  };
}

#endif // __FORWARDER_H__