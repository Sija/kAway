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
#include "FwdControl.h"

namespace kAway2 {
  FwdControl::FwdControl() : summarizableCount(0), forwardableCount(0) {
    this->setEvtOnIPrepare(boost::bind(&FwdControl::registerCfgGroups, this));
  }

  FwdControl::~FwdControl() {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      delete *it; *it = NULL;
    }
    this->forwarders.clear();
  }

  void FwdControl::registerCfgGroups() {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      UIGroupAdd(ui::cfgGroup, (*it)->cfgCols["cfgGroup"], ACTR_SAVE, (*it)->title.c_str(), (*it)->ico);
    }
  }

  void FwdControl::UIDraw(int colCount) {
    int i = 0, col = 0, fwdCount = this->summarizableCount;

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Aktywne raporty");
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      if (!(*it)->isSummarizable) continue;

      col %= colCount;
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16((*it)->ico).c_str(), 0, (col > 0) ? 10 : 0);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ((col != (colCount - 1) && i != (fwdCount - 1)) ? ACTSC_INLINE : 0), 
        (*it)->title.c_str(), (*it)->cfgCols["isSummaryActive"]);
      i++; col++;
    }
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    i = col = 0; fwdCount = this->forwardableCount;

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Aktywne przekierowania");
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      if (!(*it)->isForwardable) continue;

      col %= colCount;
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16((*it)->ico).c_str(), 0, (col > 0) ? 10 : 0);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ((col != (colCount - 1) && i != (fwdCount - 1)) ? ACTSC_INLINE : 0), 
        (*it)->title.c_str(), (*it)->cfgCols["isForwardActive"]);
      i++; col++;
    }
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
  }

  void FwdControl::fwdRegister(Forwarder *f) {
    this->forwarders.push_back(f);

    if (f->isSummarizable) {
      this->summarizableCount++;
    }
    if (f->isForwardable) {
      this->forwardableCount++;
    }
  }

  Forwarder* FwdControl::getById(std::string id) {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      if ((*it)->id == id) return(*it);
    }
  }
}