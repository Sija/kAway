/*
 *  Status Window class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2004 Kuba Niegowski
 *  Copyright (C)2003-2004 Olórin
 *
 *  $Id$
 */

#pragma once

namespace kAway2 {
  struct wndData {
    unsigned int net;
    HIMAGELIST hImlOnline;
    HIMAGELIST hImlChat;
    HIMAGELIST hImlAway;
    HIMAGELIST hImlNa;
    HIMAGELIST hImlDnd;
    HIMAGELIST hImlInv;
    HIMAGELIST hImlOffline;

    wndData(int _net = 0): net(_net), hImlOnline(0), hImlChat(0), hImlAway(0), hImlNa(0), hImlDnd(0), hImlInv(0), hImlOffline(0) { }
    ~wndData() {
      if (hImlOnline)  ImageList_Destroy(hImlOnline);
      if (hImlChat)    ImageList_Destroy(hImlChat);
      if (hImlAway)    ImageList_Destroy(hImlAway);
      if (hImlNa)      ImageList_Destroy(hImlNa);
      if (hImlDnd)     ImageList_Destroy(hImlDnd);
      if (hImlInv)     ImageList_Destroy(hImlInv);
      if (hImlOffline) ImageList_Destroy(hImlOffline);
    }
  };

  struct tStat {
    unsigned int id;
    std::string name;
    HIMAGELIST img;

    tStat(int _id, std::string _name, HIMAGELIST _img) : id(_id), name(_name), img(_img) { }
  };

  typedef std::list<tStat> tStats;

  class AwayWnd {
    public:
      AwayWnd(std::string className, std::string mruName);
      ~AwayWnd();

      LRESULT CALLBACK actionProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
      void prepareButtonImage(HIMAGELIST &hIml, HWND hWnd, int net, int status);

      inline bool haveInstance(int key) {
        return((this->instances.find(key) != this->instances.end()) ? true : false);
      }

      inline HWND getInstance(int key) {
        return(this->instances[key]);
      }

      inline void addInstance(int key, HWND hWnd) {
        this->instances[key] = hWnd;
      }

      inline void removeInstance(int key) {
        this->instances.erase(key);
      }

      inline std::string getMruName() {
        return(this->mruName);
      }

      inline std::string getWndDesc() {
        return(this->wndDesc);
      }

      void classRegister();
      void classUnRegister();
      void show(std::string title, std::string desc = "");

    protected:
      std::string wndDesc;
      std::string className;
      std::string mruName;
      std::map<int, HWND> instances;
  };
}