/**
 *  Formatted Status class
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
    this->oFormat = new Format;
  }

  ~FormattedStatus() {
    delete this->oFormat;
    this->oFormat = NULL;
  }

  inline String parseInfo(StringRef info, int net, int st = -1) {
    bool dynSt = this->infoVar.length() && 
      this->oFormat->addVar(this->infoVar, this->getInfo(net), false);

    info = this->oFormat->parse(info);
    info = Status::parseInfo(info, net, st);

    if (dynSt) {
      this->oFormat->removeVar(this->infoVar);
    }
    return PassStringRef(info);
  }

public:
  // nazwa zmiennej do której wrzucany bêdzie aktualny opis statusu
  String infoVar;
  // formatowanie statusu
  Format *oFormat;
};

#endif // __FORMATTEDSTATUS_H__