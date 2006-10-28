/**
 *  kAway2 global pointers
 *
 *  TODO: remove it in near future
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

#pragma once

#ifndef __KAWAY2_GLOBALS_H__
#define __KAWAY2_GLOBALS_H__

class FormattedStatus;
class NetList;
class MRU;

namespace kAway2 {
  class Control;
  class FwdControl;
  class AwayWnd;

  namespace lCtrl {
    extern __declspec(selectany) NetList* status = NULL;

    extern __declspec(selectany) NetList* reply = NULL;
    extern __declspec(selectany) NetList* sms = NULL;
    extern __declspec(selectany) NetList* email = NULL;
    extern __declspec(selectany) NetList* forward = NULL;
  }

  extern __declspec(selectany) Control* pCtrl = NULL;
  extern __declspec(selectany) FwdControl* fCtrl = NULL;
  extern __declspec(selectany) FormattedStatus* sCtrl = NULL;
  extern __declspec(selectany) AwayWnd* wCtrl = NULL;
  extern __declspec(selectany) MRU* MRUlist = NULL;
}

#endif // __KAWAY2_GLOBALS_H__