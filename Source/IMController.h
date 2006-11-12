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
    inline IMController() : SharedObject<iSharedObject>(), returnCodeSet(false), returnCode(0) { 
      if (!instance.isValid()) {
        // instance = this->castObject<CC>();
      }

      this->addStaticValue(IM_PLUG_SDKVERSION, KONNEKT_SDK_V);

      this->registerObserver(IM_PLUG_INIT, boost::bind(&IMController::onInit, this, _1));
      this->registerObserver(IM_PLUG_DEINIT, boost::bind(&IMController::onDeInit, this, _1));
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
      SharedObject<iSharedObject>::~SharedObject();
    }

  public:
    inline static CC* getInstance() {
      // static SharedPtr<CC> instance;
      if (!instance.isValid()) {
        instance = new CC;
      }
      return instance;
    }

    void inline onInit(CC* self) {
      Plug_Init(this->getIM()->p1, this->getIM()->p2);
      return this->setSuccess();
    }

    void inline onDeInit(CC* self) {
      Plug_Deinit(this->getIM()->p1, this->getIM()->p2);
      return this->setSuccess();
    }

    inline bool handle(sIMessage_base* msgBase) {
      sIMessage_2params* msg = static_cast<sIMessage_2params*>(msgBase);
      IMLOG("[IMController<%i>::handle()]: id = %i", this, msg->id);

      if (this->staticValues.find(msg->id) != this->staticValues.end()) {
        this->setReturnCode(this->staticValues[msg->id]);
      }
      this->notifyObservers(msg);

      if (msg->id == IM_UIACTION) {
        this->notifyActionObservers(msg);
      }
      return this->isReturnCodeSet();
    }

    inline bool registerObserver(int id, fOnIMessage f, int priority = 0) {
      if (!f.empty()) {
        if (this->observers.find(id) == this->observers.end()) {
          this->observers[id] = new sigOnIMessage;
        }
        return (*this->observers[id]).connect(priority, f).connected();
      }
      return false;
    }

    inline void notifyObservers(sIMessage_2params* msg) {
      if (!this->isObserved(msg->id)) return;

      this->setIM(msg);
      (*this->observers[msg->id])(CC::getInstance());
    }

    inline bool registerActionObserver(int id, fOnIMessage f, int priority = 0) {
      if (!f.empty()) {
        if (this->actionObservers.find(id) == this->actionObservers.end()) {
          this->actionObservers[id] = new sigOnIMessage;
        }
        return (*this->actionObservers[id]).connect(priority, f).connected();
      }
      return false;
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