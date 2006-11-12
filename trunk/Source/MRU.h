/**
 *  MRU class
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

#ifndef __MRU_H__
#define __MRU_H__

#include "Helpers.h"

class MRU : public SharedObject<iSharedObject> {
public:
  typedef std::deque<String> tMRUlist;

  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(MRU, iSharedObject, Version(0,1,0,0));

public:
  MRU(const StringRef& _name, int _count = 100, bool _dtbCount = false) : 
    SharedObject<iSharedObject>(), name(_name), count(_count), dtbCount(_dtbCount) { }

public:
  tMRUlist get(bool rev = true, const char * buff = 0, int buffSize = 1024);
  void append(const StringRef& current);
  void append(tMRUlist list);

  void set(const StringRef& current);
  void set(tMRUlist list);

  void clear();

  inline int getCount() {
    return this->dtbCount ? GETINT(this->count) : this->count;
  }

  /*
   *  Static methods
   */
  inline static tMRUlist get(const StringRef& name, int count = 100, bool rev = true, 
    const char * buff = 0, int buffSize = 1024) {
    MRU mru(name, count);
    return mru.get(rev, buff, buffSize);
  }

  inline static void append(const StringRef& name, const StringRef& current, int count = 100) {
    MRU mru(name, count);
    mru.append(current);
  }

  inline static void append(const StringRef& name, tMRUlist list, int count = 100) {
    MRU mru(name, count);
    mru.append(list);
  }

  inline static void set(const StringRef& name, const StringRef& current, int count = 100) {
    MRU mru(name, count);
    mru.set(current);
  }

  inline static void set(const StringRef& name, tMRUlist list, int count = 100) {
    MRU mru(name, count);
    mru.set(list);
  }

  inline static void clear(const StringRef& name) {
    MRU mru(name);
    mru.clear();
  }

protected:
  String name;
  unsigned int count;
  bool dtbCount;
};

typedef Stamina::SharedPtr<MRU> oMRU;

#endif // __MRU_H__