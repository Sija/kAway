/**
  *  IMessage Controller class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision: 51 $
  *  @modifiedby   $LastChangedBy: sija $
  *  @lastmodified $Date: 2006-10-28 21:39:17 +0200 (So, 28 paü 2006) $
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __IMESSAGECTRL_H__
#define __IMESSAGECTRL_H__

#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace Konnekt {
  template<typename TC, typename TR>
  TR (TC::* resolve_cast0(TR (TC::* pFunc)(void)))(void) {
    return pFunc;
  }

  template<typename TP, typename TC, typename TR>
  TR (TC::* resolve_cast1(TR (TC::* pFunc)(TP)))(TP) {
    return pFunc;
  }

  template <class CC /* Controller Class */> 
  class IMController : public SharedObject<iSharedObject> {
  public:
    /*
     * Class version
     */
	  STAMINA_OBJECT_CLASS_VERSION(IMController<CC>, iSharedObject, Version(0,1,0,0));

  public:
    typedef function<void(CC*)> fOnIMessage;
    typedef signal<void(CC*)> sigOnIMessage;

  public:
    typedef std::map<String, signals::connection> tConnections;

    struct sObserver {
      tConnections connections;
      sigOnIMessage signal;
    };

    typedef std::map<int, sObserver*> tObservers;
    typedef std::map<int, int> tStaticValues;

  protected:
    inline IMController() : returnCodeSet(false), returnCode(0) { 
      this->registerObserver(IM_PLUG_INIT, bind(resolve_cast0(&IMController::onInit), this));
      this->registerObserver(IM_PLUG_DEINIT, bind(resolve_cast0(&IMController::onDeInit), this));
      this->addStaticValue(IM_PLUG_SDKVERSION, KONNEKT_SDK_V);
    }

    inline IMController(CC const&) { }
    inline IMController& operator=(CC const&) { 
      return *this; 
    }

    inline ~IMController() { 
      for (tObservers::iterator it = this->observers.begin(); it != this->observers.end(); it++) {
        delete it->second;
      }
      for (tObservers::iterator it = this->actionObservers.begin(); it != this->actionObservers.end(); it++) {
        delete it->second;
      }
    }

  public:
    inline static CC* getInstance() {
      if (!instance.isValid()) {
        instance = new CC;
      }
      return instance;
    }

    void inline onInit() {
      Plug_Init(this->getIM()->p1, this->getIM()->p2);
      return this->setSuccess();
    }

    void inline onDeInit() {
      Plug_Deinit(this->getIM()->p1, this->getIM()->p2);
      return this->setSuccess();
    }

    inline bool handle(sIMessage_base* msgBase) {
      sIMessage_2params* msg = static_cast<sIMessage_2params*>(msgBase);

      IMLOG("[IMController<%i>::handle()]: id = %i", this, msg->id);
      this->clear();

      if (this->staticValues.find(msg->id) != this->staticValues.end()) {
        this->setReturnCode(this->staticValues[msg->id]);
      }
      this->notifyObservers(msg);

      if (msg->id == IM_UIACTION) {
        this->notifyActionObservers(msg);
      }
      return this->isReturnCodeSet();
    }

    /*
      for (tObservers::iterator it = this->observers.begin(); it != this->observers.end(); it++) {
        for (tConnections::iterator it2 = it->second->connections.begin(); it2 != it->second->connections.end(); it2++) {
          IMLOG("Observer[%i].connection: %s", it->first, it2->first.c_str());
        }
      }
    */

    inline bool registerObserver(int id, fOnIMessage f, int priority = 0, signals::connect_position pos = signals::at_back, 
      const StringRef& name = "", bool overwrite = true) 
    {
      return this->_registerObserver(id, f, priority, pos, name, overwrite, this->observers);
    }

    inline bool registerActionObserver(int id, fOnIMessage f, int priority = 0, signals::connect_position pos = signals::at_back, 
      const StringRef& name = "", bool overwrite = true) 
    {
      return this->_registerObserver(id, f, priority, pos, name, overwrite, this->actionObservers);
    }

    inline void notifyObservers(sIMessage_2params* msg) {
      if (!this->isObserved(msg->id)) return;

      this->setIM(msg);
      this->observers[msg->id]->signal(CC::getInstance());
    }

    inline void notifyActionObservers(sIMessage_2params* msg) {
      this->setIM(msg);
      sUIActionNotify_2params* an = this->getAN();

      if (!this->isActionObserved(an->act.id)) return;
      this->actionObservers[an->act.id]->signal(CC::getInstance());
    }

    inline void clear() {
      this->returnCodeSet = false;
      this->returnCode = NULL;
      this->im = NULL;
    }

    inline int getReturnCode() {
      this->returnCodeSet = false;
      return this->returnCode;
    }

    inline void setReturnCode(int code) {
      this->returnCodeSet = true;
      this->returnCode = code;
    }

    inline void setSuccess() {
      return this->setReturnCode(1);
    }

    inline void setFailure() {
      return this->setReturnCode(-1);
    }

    inline bool isReturnCodeSet() {
      return this->returnCodeSet;
    }

    inline sIMessage_2params* getIM() {
      return this->im;
    }

    inline sUIActionNotify_2params* getAN() {
      return static_cast<sUIActionNotify_2params*>((sUIActionNotify_base*) this->getIM()->p1);
    }

    inline void setIM(sIMessage_2params* im) { 
      this->im = im;
    }

    inline void addStaticValue(int id, int value) {
      this->staticValues[id] = value;
    }

    inline bool isObserved(int id) {
      if (this->observers.find(id) != this->observers.end()) {
        return !this->observers[id]->signal.empty();
      }
      return false;
    }

    inline bool isActionObserved(int id) {
      if (this->actionObservers.find(id) != this->actionObservers.end()) {
        return !this->actionObservers[id]->signal.empty();
      }
      return false;
    }

  protected:
    inline bool _registerObserver(
      int id, fOnIMessage f, int priority, signals::connect_position pos, 
      StringRef name, bool overwrite, tObservers& list) 
    {
      if (f.empty()) {
        return false;
      }
      if (list.find(id) == list.end()) {
        list[id] = new sObserver;
      }
      if (!name.length()) {
        name = "unnamed[" + itos(list[id]->connections.size()) + "]";
      }
      if (list[id]->connections.find(name) != list[id]->connections.end()) {
        if (overwrite) {
          list[id]->connections[name].disconnect();
        } else {
          return false;
        }
      }
      return (list[id]->connections[name] = list[id]->signal.connect(priority, f, pos)).connected();
    }

    static SharedPtr<CC> instance;
    tStaticValues staticValues;
    tObservers actionObservers;
    tObservers observers;
    sIMessage_2params* im;

    bool returnCodeSet;
    int returnCode;
  };

  template <typename CC> 
  SharedPtr<CC> IMController<CC>::instance = 0;
}

#endif // __IMESSAGECTRL_H__