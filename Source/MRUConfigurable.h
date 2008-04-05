/**
  *  MRU class using dtb for storing count value
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2008 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy: sija $
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __MRU_CONFIGURABLE_H__
#define __MRU_CONFIGURABLE_H__

#include "MRU.h"
#include <konnekt/contrib/Config.h>

class MRUConfigurable : public MRU {
public:
  MRUConfigurable(const string& name, const Config::Item& cfg_column) : MRU(name), _cfg_column(cfg_column) { }

public:
  inline int getCount() {
    return _cfg_column.to_i();
  }
  inline void setCount(int count) {
    _cfg_column.set(count);
  }

protected:
  Config::Item _cfg_column;
};

#endif // __MRU_CONFIGURABLE_H__