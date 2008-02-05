/**
  *  Forwarders Controller class
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

#include "FwdController.h"
#include "Controller.h"

namespace kAway2 {
  FwdController::FwdController() : summarizableCount(0), forwardableCount(0) {
    Context::getInstance()->getIMessageDispatcher().connect(IM_UI_PREPARE, bind(&FwdController::registerCfgGroups, this, _1));
  }

  FwdController::~FwdController() {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      delete *it; *it = NULL;
    }
    this->forwarders.clear();
  }

  void FwdController::registerCfgGroups(IMEvent &ev) {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      UIGroupAdd(ui::cfgGroup, (*it)->cfgCols["cfgGroup"], ACTR_SAVE, (*it)->title.a_str(), (*it)->ico);
    }
  }

  void FwdController::UIDrawActiveSum() {
    int i = 0, col = 0, colCount = 3, fwdCount = this->summarizableCount;

    // UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Aktywne raporty");
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      if (!(*it)->isSummarizable) continue;

      col %= colCount;
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16((*it)->ico).a_str(), 0, (col > 0) ? 10 : 0);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ((col != (colCount - 1) && i != (fwdCount - 1)) ? ACTSC_INLINE : 0), 
        (*it)->title.a_str(), (*it)->cfgCols["isSummaryActive"]);
      i++; col++;
    }
    // UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
  }

  void FwdController::UIDrawActiveFwd() {
    int i = 0, col = 0, colCount = 3, fwdCount = this->forwardableCount;

    // UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Aktywne przekierowania");
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      if (!(*it)->isForwardable) continue;

      col %= colCount;
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16((*it)->ico).a_str(), 0, (col > 0) ? 10 : 0);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ((col != (colCount - 1) && i != (fwdCount - 1)) ? ACTSC_INLINE : 0), 
        (*it)->title.a_str(), (*it)->cfgCols["isForwardActive"]);
      i++; col++;
    }
    // UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
  }

  void FwdController::fwdRegister(Forwarder *f) {
    this->forwarders.push_back(f);

    if (f->isSummarizable) {
      this->summarizableCount++;
    }
    if (f->isForwardable) {
      this->forwardableCount++;
    }
  }

  Forwarder* FwdController::getById(const StringRef& id) {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      if ((*it)->id == id) return *it;
    }
  }
}