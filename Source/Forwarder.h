/**
 *  Forwarder Base class
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
#include "main.h"

#include "Helpers.h"
#include "Format.h"

#include <boost/bind.hpp>
#include <stamina/timer.h>

namespace kAway2 {
  class Forwarder {
  public:
    class Summary;
    class Forward;

  public:
    Forwarder(std::string id, std::string title, int ico,
      bool isSummarizable = false, 
      bool isForwardable = false);
    virtual ~Forwarder();

  public:
    virtual void setCfgCols() = 0;
    virtual void send(std::string msg) = 0;

    virtual bool preForward(int cnt, cMessage *msg) = 0;
    virtual bool preSummary() = 0;

    inline void onISetCols() {
      this->setCfgCols();

      if (this->isSummarizable) {
        Ctrl->SetColumn(DTCFG, this->cfgCols["isSummaryActive"], DT_CT_INT, 0, ("kAway2/" + this->id + "/isSummaryActive").c_str());
      }
      if (this->isForwardable) {
        Ctrl->SetColumn(DTCFG, this->cfgCols["isForwardActive"], DT_CT_INT, 0, ("kAway2/" + this->id + "/isForwardActive").c_str());
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
    inline void onNewMsg(int cnt, cMessage *msg) {
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
    std::string title;
    std::string id;

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
      typedef std::deque<std::string> tMsgSenders;

    public:
      Summary(Forwarder *fwd);
      ~Summary();

    public:
      inline void clear() {
        this->receivedMsgCount = 0;
        this->msgSenders.clear();
      }
      void send();

      void cfgTplDraw();

      inline bool isActive() {
        return(GETINT(this->parent->cfgCols["isSummaryActive"]));
      }

      void onEnable();
      void onDisable();
      void onNewMsg(int cnt, cMessage *msg);

      std::string getMsgSendersList();
      std::string getBody();

    protected:
      boost::shared_ptr<Stamina::TimerDynamic> timer;
      unsigned int receivedMsgCount;
      std::string lastMsgFrom;
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
      void onNewMsg(int cnt, cMessage *msg);
      std::string getBody(int cnt, cMessage *msg);

      inline bool isActive() {
        return(GETINT(this->parent->cfgCols["isForwardActive"]));
      }

    protected:
      Forwarder *parent;
    };
  };
}