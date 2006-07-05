/**
 *  Forwarders Control class
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
#include "Forwarder.h"

#include <boost/signal.hpp>
#include <boost/function.hpp>

namespace kAway2 {
  class FwdControl {
  public:
    typedef std::deque<Forwarder *> tForwarders;

    typedef boost::function<void (unsigned)> fUnsigned;
    typedef boost::function<void ()> fVoid;
    typedef boost::function<void (int, int)> fAction;
    typedef boost::function<void (int, cMessage *)> fNewMsg;

    typedef boost::signal<void ()> sOnIStart;
    typedef boost::signal<void ()> sOnIEnd;
    typedef boost::signal<void ()> sOnISetCols;
    typedef boost::signal<void ()> sOnIPrepare;
    typedef boost::signal<void (int, int)> sOnAction;

    typedef boost::signal<void ()> sOnEnable;
    typedef boost::signal<void ()> sOnDisable;
    typedef boost::signal<void (int, cMessage *)> sOnNewMsg;

  protected:
    int summarizableCount;
    int forwardableCount;

  public:
    FwdControl();
    ~FwdControl();

    void UIDraw(int colCount = 3);
    void registerCfgGroups();
    Forwarder* getById(std::string id);
    void fwdRegister(Forwarder *f);

  public:
		sOnIStart onIStart;
		sOnIEnd onIEnd;
		sOnISetCols onISetCols;
		sOnIPrepare onIPrepare;
		sOnAction onAction;

		sOnEnable onEnable;
		sOnDisable onDisable;
		sOnNewMsg onNewMsg;

  public:
    tForwarders forwarders;

  public:
		const sOnIStart& getEvtOnIStart() const {
			return(this->onIStart);
		}

    inline bool setEvtOnIStart(fVoid f) {
      if (!f.empty()) {
		    this->onIStart.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnIEnd& getEvtOnIEnd() const {
			return(this->onIEnd);
		}

    inline bool setEvtOnIEnd(fVoid f) {
      if (!f.empty()) {
		    this->onIEnd.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnISetCols& getEvtOnISetCols() const {
			return(this->onISetCols);
		}

    inline bool setEvtOnISetCols(fVoid f) {
      if (!f.empty()) {
		    this->onISetCols.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnIPrepare& getEvtOnIPrepare() const {
			return(this->onIPrepare);
		}

    inline bool setEvtOnIPrepare(fVoid f) {
      if (!f.empty()) {
		    this->onIPrepare.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnAction& getEvtOnAction() const {
			return(this->onAction);
		}

    inline bool setEvtOnAction(fAction f) {
      if (!f.empty()) {
		    this->onAction.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnEnable& getEvtOnEnable() const {
			return(this->onEnable);
		}

    inline bool setEvtOnEnable(fVoid f) {
      if (!f.empty()) {
		    this->onEnable.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnDisable& getEvtOnDisable() const {
			return(this->onDisable);
		}

    inline bool setEvtOnDisable(fVoid f) {
      if (!f.empty()) {
		    this->onDisable.connect(f);
		    return(true);
      }
			return(false);
    }

		const sOnNewMsg& getEvtOnNewMsg() const {
			return(this->onNewMsg);
		}

    inline bool setEvtOnNewMsg(fNewMsg f) {
      if (!f.empty()) {
		    this->onNewMsg.connect(f);
		    return(true);
      }
			return(false);
    }
  };
}