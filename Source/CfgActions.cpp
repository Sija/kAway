/**
  *  Configuration actions
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision: 77 $
  *  @modifiedby   $LastChangedBy: sija $
  *  @lastmodified $Date: 2006-11-20 17:42:35 +0100 (Pn, 20 lis 2006) $
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#include "stdafx.h"
#include "CfgActions.h"

/*
 * Configuration group class
 */
void CfgGroup::draw(int parent) {
  if (!this->checkBits()) return;

  UIActionCfgAdd(parent, 0, ACTT_GROUP | status, txt);
  for (tActions::iterator it = _actions.begin(); it != _actions.end(); it++) {
    (*it)->castObject<CfgLabel>()->draw(parent);
  }
	UIActionAdd(parent, 0, ACTT_GROUPEND);
}

/*
 * Configuration page class
 */
void CfgPage::draw() {
  if (!this->checkBits()) return;

  UIGroupAdd(parent, id, status, txt, ico);
  _infoBox.draw(id);

  for (tActions::iterator it = _actions.begin(); it != _actions.end(); it++) {
    (*it)->castObject<CfgGroup>()->draw(parent);
  }
}