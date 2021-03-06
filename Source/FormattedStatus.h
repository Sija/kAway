/**
  *  Formatted Status class
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

#ifndef __FORMATTEDSTATUS_H__
#define __FORMATTEDSTATUS_H__

#include "Status.h"
#include "Format.h"

class FormattedStatus : public Status {
public:
  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(FormattedStatus, Status, Version(0,1,0,0));

  FormattedStatus(NetList *_lCtrl, int _onHiddenCfgCol = 0, int _dotsCfgCol = 0, const StringRef& _infoVar = "") : 
    Status(_lCtrl, _onHiddenCfgCol, _dotsCfgCol), infoVar(_infoVar) 
  {
    this->stringFormatter = new Format;
  }

  inline String parseInfo(StringRef info, int net, int st = -1) {
    bool dynSt = false;
    
    if (this->infoVar.length()) {
      dynSt = stringFormatter->addVar(infoVar, isRemembered(net) ? getInfo(net) : getActualInfo(net), false);
    }

    info = stringFormatter->parse(info);
    info = Status::parseInfo(info, net, st);

    if (dynSt) {
      stringFormatter->removeVar(this->infoVar);
    }
    return PassStringRef(info);
  }

public:
  // nazwa zmiennej do kt�rej wrzucany b�dzie aktualny opis statusu
  String infoVar;
  // formatowanie statusu
  oFormat stringFormatter;
};

typedef SharedPtr<FormattedStatus> oFormattedStatus;

#endif // __FORMATTEDSTATUS_H__