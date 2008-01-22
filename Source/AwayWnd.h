/**
  *  kAway2 Away Window class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2008 Sijawusz Pur Rahnama
  *  @copyright    Copyright (c) 2004 Kuba 'nix' Niegowski
  *  @copyright    Copyright (c) 2003-2004 Kamil 'Olórin' Figiela
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __AWAYWND_H__
#define __AWAYWND_H__

#include "kAway2.h"

#include "Helpers.h"
#include "MRU.h"
#include "EditFix.h"

namespace kAway2 {
  class AwayWnd : public SharedObject<iSharedObject> {
  public:
    /*
     * Class version
     */
	  STAMINA_OBJECT_CLASS_VERSION(AwayWnd, iSharedObject, Version(0,1,0,0));

  public:
    struct sWndData {
      unsigned int net;
      HIMAGELIST hImlOnline;
      HIMAGELIST hImlChat;
      HIMAGELIST hImlAway;
      HIMAGELIST hImlNa;
      HIMAGELIST hImlDnd;
      HIMAGELIST hImlInv;
      HIMAGELIST hImlOffline;

      sWndData(int _net = 0): net(_net), hImlOnline(0), hImlChat(0), hImlAway(0), hImlNa(0), hImlDnd(0), hImlInv(0), hImlOffline(0) { }
      ~sWndData() {
        if (hImlOnline)  ImageList_Destroy(hImlOnline);
        if (hImlChat)    ImageList_Destroy(hImlChat);
        if (hImlAway)    ImageList_Destroy(hImlAway);
        if (hImlNa)      ImageList_Destroy(hImlNa);
        if (hImlDnd)     ImageList_Destroy(hImlDnd);
        if (hImlInv)     ImageList_Destroy(hImlInv);
        if (hImlOffline) ImageList_Destroy(hImlOffline);
      }
    };

    struct sStatus {
      unsigned int id;
      const char* name;
      HIMAGELIST img;

      sStatus(int _id, const char* _name, HIMAGELIST _img) : id(_id), name(_name), img(_img) { }
    };

    typedef std::list<sStatus> tStats;

  public:
    AwayWnd();
    ~AwayWnd();

    static LRESULT CALLBACK wndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

    void prepareButtonImage(HIMAGELIST &hIml, HWND hWnd, int net, int status);

    inline bool haveInstance(int key) {
      return this->instances.find(key) != this->instances.end();
    }

    inline HWND getInstance(int key) {
      return this->instances[key];
    }

    inline void addInstance(int key, HWND hWnd) {
      this->instances[key] = hWnd;
    }

    inline void removeInstance(int key) {
      this->instances.erase(key);
    }

    void show();

  protected:
    std::map<int, HWND> instances;
  };

  typedef SharedPtr<AwayWnd> oAwayWnd;
}

#endif // __AWAYWND_H__