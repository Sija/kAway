/**
  *  MRU class
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

#ifndef __MRU_H__
#define __MRU_H__

#include "Helpers.h"

class MRU : public SharedObject<iSharedObject> {
public:
  typedef std::deque<String> tItems;

  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(MRU, iSharedObject, Version(0,1,0,0));

public:
  MRU(const string& name, int count = 100) : 
    _name(name), _count(count) { }

public:
  tItems get(bool rev = true, const char * buff = 0, int buffSize = 1024);

  void append(const StringRef& current);
  void append(const tItems& list);

  void set(const StringRef& current);
  void set(const tItems& list);

  void clear();

  inline unsigned int getCount() {
    return _count;
  }
  inline void setCount(int count) {
    _count = count;
  }

protected:
  string _name;
  unsigned int _count;
};

class MRUConfigurable : public MRU {
public:
  MRUConfigurable(const string& name, int cfg_column) : MRU(name),
    _cfg_column(cfg_column) { }

public:
  inline unsigned int getCount() {
    return GETINT(_cfg_column);
  }
  inline void setCount(int count) {
    SETINT(_cfg_column, count);
  }

protected:
  unsigned int _cfg_column;
};

typedef SharedPtr<MRU> oMRU;

#endif // __MRU_H__