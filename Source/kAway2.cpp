/**
 *  kAway2 Core
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

#include "kAway2.h"
#include "Helpers.h"
#include "MRU.h"
#include "NetList.h"
#include "FormattedStatus.h"
#include "AwayWnd.h"
#include "Control.h"
#include "IMControl.h"

//#include "FwdControl.h"

//#include "Forwarders/CntForwarder.h"
//#include "Forwarders/SMSForwarder.h"

using namespace kAway2;

int __stdcall DllMain(void* hinstDLL, unsigned long fdwReason, void* lpvReserved) {
  return true;
}

int __stdcall IMessageProc(sIMessage_base* msgBase) {
  if (Controller::getInstance()->handle(msgBase)) {
    return Controller::getInstance()->getReturnCode();
  } else {
    if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
  }
  return 0;
}