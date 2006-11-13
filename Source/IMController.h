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
  *  @lastmodified $Date: 2006-10-28 21:39:17 +0200 (So, 28 pa� 2006) $
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
    typedef boost::function<void(CC*)> fOnIMessage;
    typedef boost::signal<void(CC*)> sigOnIMessage;

  public:
    typedef std::map<int, sigOnIMessage*> tObservers;
    typedef std::map<int, int> tStaticValues;

		CC* operator -> () {
			return this->getInstance();
		}

  protected:
    inline IMController() : returnCodeSet(false), returnCode(0) { 
      this->registerObserver(IM_PLUG_INIT, boost::bind(resolve_cast0(&IMController::onInit), this));
      this->registerObserver(IM_PLUG_DEINIT, boost::bind(resolve_cast0(&IMController::onDeInit), this));
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

    inline bool registerObserver(int id, fOnIMessage f, int priority = 0, boost::signals::connect_position pos = boost::signals::at_back) {
      return this->_registerObserver<fOnIMessage, observers>(id, f, priority, pos);
    }

    inline bool registerActionObserver(int id, fOnIMessage f, int priority = 0, boost::signals::connect_position pos = boost::signals::at_back) {
      return this->_registerObserver<fOnIMessage, actionObservers>(id, f, priority, pos);
    }

    inline void notifyObservers(sIMessage_2params* msg) {
      if (!this->isObserved(msg->id)) return;

      this->setIM(msg);
      (*this->observers[msg->id])(CC::getInstance());
    }

    inline void notifyActionObservers(sIMessage_2params* msg) {
      this->setIM(msg);
      sUIActionNotify_2params* an = this->getAN();

      if (!this->isActionObserved(an->act.id)) return;
      (*this->actionObservers[an->act.id])(CC::getInstance());
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
        return !(*this->observers[id]).empty();
      }
      return false;
    }

    inline bool isActionObserved(int id) {
      if (this->actionObservers.find(id) != this->actionObservers.end()) {
        return !(*this->actionObservers[id]).empty();
      }
      return false;
    }

  protected:
    template <typename F, tObservers& O>
    inline bool _registerObserver(int id, F f, int priority, boost::signals::connect_position pos, tObservers& list = O) {
      if (!f.empty()) {
        if (list.find(id) == list.end()) {
          list[id] = new sigOnIMessage;
        }
        return (*list[id]).connect(priority, f, pos).connected();
      }
      return false;
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