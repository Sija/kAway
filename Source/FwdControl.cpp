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
  FwdControl::FwdControl() {
    //
  }

  FwdControl::~FwdControl() {
    for (tForwarders::iterator it = this->forwarders.begin(); it != this->forwarders.end(); it++) {
      delete *it; *it = NULL;
    }
    this->forwarders.clear();
  }
}