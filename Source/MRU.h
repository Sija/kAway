/**
 *  MRU class
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

#include "stdafx.h"
#include "Helpers.h"

typedef std::deque<std::string> tMRUlist;

class MRU {
  public:
    MRU(std::string name, int count = 100, bool dtbCount = false);
    ~MRU();

  public:
    tMRUlist get(bool rev = true, const char * buff = 0, int buffSize = 1024);
    void append(std::string current);
    void append(tMRUlist list);

    inline void set(std::string current) {
      this->clear();
      this->append(current);
    }

    inline void set(tMRUlist list) {
      this->clear();
      this->append(list);
    }

    inline void clear() {
      sMRU mru;

      mru.name = this->name.c_str();
      mru.count = 0;

      Ctrl->IMessage(&sIMessage_MRU(IMC_MRU_SET, &mru));
    }

    inline int getCount() {
      return((this->dtbCount) ? GETINT(this->count) : this->count);
    }

    /*
     *  Static methods
     */
    inline static tMRUlist get(const char * name, int count = 100, bool rev = true, 
      const char * buff = 0, int buffSize = 1024) {
      MRU mru(name, count);
      return(mru.get(rev, buff, buffSize));
    }

    inline static void append(const char * name, std::string current, int count = 100) {
      MRU mru(name, count);
      mru.append(current);
    }

    inline static void append(const char * name, tMRUlist list, int count = 100) {
      MRU mru(name, count);
      mru.append(list);
    }

    inline static void set(const char * name, std::string current, int count = 100) {
      MRU mru(name, count);
      mru.set(current);
    }

    inline static void set(const char * name, tMRUlist list, int count = 100) {
      MRU mru(name, count);
      mru.set(list);
    }

    inline static void clear(const char * name) {
      MRU mru(name);
      mru.clear();
    }

  public:
    std::string name;
    unsigned int count;
    bool dtbCount;
};