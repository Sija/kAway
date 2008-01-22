/**
  *  Configuration class
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "IMessageDispatcher.h"
#include "Helpers.h"

using namespace Konnekt::Tables;
using namespace Stamina;
using namespace boost;

namespace Konnekt {
  class Config : public SharedObject<iSharedObject>, public signals::trackable {
  public:
    /* Class version */
    STAMINA_OBJECT_CLASS_VERSION(Config, iSharedObject, Version(0,2,0,0));

  public:
    typedef std::deque<sIMessage_setColumn*> tColumns;

  public:
    inline Config(IMessageDispatcher& dispatcher) {
      this->attachObservers(dispatcher);
    }
    inline Config() { }

    inline virtual ~Config() { 
      for (tColumns::iterator it = _cols.begin(); it != _cols.end(); it++) {
        delete *it;
      }
    }

  public:
    /**
     * automagical registration of configuration columns (set via setColumn())
     */
    inline void attachObservers(IMessageDispatcher& dispatcher) {
      dispatcher.connect(IM_SETCOLS, bind(&Config::_setColumns, this, _1));
    }

    inline void setColumn(tTable table, tColId id, int type, const char* def, const char* name) {
      _cols.push_back(new sIMessage_setColumn(table, id, type, def, name));
    }
    inline void setColumn(tTable table, tColId id, int type, int def, const char* name) {
      _cols.push_back(new sIMessage_setColumn(table, id, type, def, name));
    }

    inline void resetColumns(tTable table = tableNotFound) {
      if (!_cols.size()) return;

      bool resetCnt = table == tableContacts;
      bool resetCfg = table == tableConfig;

      if (table == tableNotFound) {
        resetCfg = resetCnt = true;
      }

      if (!resetCnt && !resetCfg) {
        return;
      }

      tColumns dtCnt;
      for (tColumns::iterator it = _cols.begin(); it != _cols.end(); it++) {
        if ((*it)->_table == tableConfig && resetCfg) {
          _resetColumn(*it);
        }
        if ((*it)->_table == tableContacts && resetCnt) {
          dtCnt.push_back(*it);
        }
      }

      if (dtCnt.size()) {
        int count = Ctrl->IMessage(IMC_CNT_COUNT);
        for (int i = 1; i < count; i++) {
          for (tColumns::iterator it = dtCnt.begin(); it != dtCnt.end(); it++) {
            _resetColumn(*it, i);
          }
        }
      }
    }

    inline void resetColumn(tColId id, tCntId cnt = 0, sUIAction* an = 0) {
      if (!_cols.size()) return;

      tTable table = !cnt ? tableConfig : tableContacts;
      for (tColumns::iterator it = _cols.begin(); it != _cols.end(); it++) {
        if ((*it)->_id == id && (*it)->_table == table) {
          _resetColumn(*it, cnt, an); break;
        }
      }
    }

    /* Helpers accessors */
    inline int getInt(tColId col) const {
      return GETINT(col);
    }
    inline int getInt(tColId col, tCntId cnt) const {
      return GETCNTI(cnt, col);
    }
    inline __int64 getInt64(tColId col, tCntId cnt) const {
      return GETCNTI64(cnt, col);
    }

    inline const char* getChar(tColId col) const {
      return GETSTR(col);
    }
    inline const char* getChar(tColId col, tCntId cnt) const {
      return GETCNTC(cnt, col);
    }

    inline String getString(tColId col) const {
      return getChar(col);
    }
    inline String getString(tColId col, tCntId cnt) const {
      return getChar(col, cnt);
    }

    inline void set(tColId col, int val) {
      SETINT(col, val);
    }
    inline void set(tColId col, tCntId cnt, int val) {
      SETCNTI(cnt, col, val);
    }
    inline void set(tColId col, tCntId cnt, __int64 val) {
      SETCNTI64(cnt, col, val);
    }

    inline void set(tColId col, const StringRef& val) {
      SETSTR(col, val.a_str());
    }
    inline void set(tColId col, tCntId cnt, const StringRef& val) {
      SETCNTC(cnt, col, val.a_str());
    }

    /*
     * TODO: find some better way to handle it
     */
    inline int getInheritedIValue(tColId col, tCntId cnt) const {
      return getInt(col, cnt) >= 0 ? getInt(col, cnt) : getInt(col);
    }

    inline bool getInheritedBValue(tColId col, tCntId cnt) const {
      return (getInt(col) && (getInt(col, cnt) < 2)) || (!getInt(col) && (getInt(col, cnt) == 1));
    }

    inline const char* getInheritedCValue(tColId col, tCntId cnt) const {
      const char* val = getChar(col, cnt);

      return strlen(val) ? val : getChar(col);
    }

  protected:
    inline void _resetColumn(sIMessage_setColumn* it, tCntId cnt = 0, sUIAction* an = 0) {
      bool isCnt = it->_table == tableContacts && cnt;
      bool isConfig = it->_table == tableConfig;

      if (!isCnt && !isConfig) {
        return;
      }

      bool convert = true;
      String val;

      switch (it->_type) {
        case ctypeInt: {
          if (isConfig) {
            set(it->_id, it->_def);
          }
          if (isCnt) {
            set(it->_id, cnt, it->_def);
          }
          val = inttostr(it->_def);
          break;
        }
        case ctypeInt64: {
          if (isConfig) {
            // set(it->_id, *it->_def_p64);
          }
          if (isCnt) {
            set(it->_id, cnt, *it->_def_p64);
          }
          val = i64tostr(*it->_def_p64);
          break;
        }
        case ctypeString: {
          if (isConfig) {
            set(it->_id, it->_def_ch);
          }
          if (isCnt) {
            set(it->_id, cnt, it->_def_ch);
          }
          val = it->_def_ch;
          convert = false;
          break;
        }
      }
      if (an) {
        UIActionCfgSetValue(*an, val.c_str(), convert);
      }
    }

    inline void _setColumns(IMEvent& ev) {
      for (tColumns::iterator it = _cols.begin(); it != _cols.end(); it++) {
        Ctrl->IMessage(*it);
      }
      ev.setSuccess();
    }

  protected:
    tColumns _cols;
  };

  // smart pointer type
  typedef SharedPtr<Config> oConfig;
}

#endif // __CONFIG_H__