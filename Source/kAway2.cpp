/**
  *  kAway2 main file
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#include "stdafx.h"
#include "Controller.h"

using namespace kAway2;

int __stdcall DllMain(void* hinstDLL, unsigned long fdwReason, void* lpvReserved) {
  return true;
}

int __stdcall IMessageProc(sIMessage_base* msgBase) {
  if (Controller::getInstance()->process(msgBase)) {
    return Controller::getInstance()->getReturnCode();
  } else {
    if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
  }
  return 0;
}