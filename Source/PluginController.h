/**
  *  Plugin Controller class
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

#ifndef __PLUGINCONTROLLER_H__
#define __PLUGINCONTROLLER_H__

#include "EventDispatcher.h"
#include "Event.h"

#include "ActionDispatcher.h"
#include "ActionEvent.h"

#include "IMessageDispatcher.h"
#include "IMEvent.h"

#include "Config.h"

using namespace Stamina;
using namespace boost;

namespace Konnekt {
  template <class T>
  class PluginController : public SharedObject<iSharedObject>, public signals::trackable {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(PluginController<T>, iSharedObject, Version(0,3,0,0));

  public:
    typedef SharedPtr<T> oInstance;

  protected:
    /**
     * Creates new PluginController object.
     */
    inline PluginController() {
      // setting/unsetting Ctrl global pointer
      _dispatcher.connect(IM_PLUG_INIT, bind(&PluginController::_onPlugInit, this, _1));
      _dispatcher.connect(IM_PLUG_DEINIT, bind(&PluginController::_onPlugDeInit, this, _1));

      // actions subclassing
      _dispatcher.connect(IM_UI_PREPARE, bind(&ActionDispatcher::doSubclass, &_action_dispatcher, _1));

      // initialize configuration
      config = new Config(getIMessageDispatcher());
    }

  public:
    /**
     * Returns instance of object T.
     *
     * @return T*
     */
    inline static T* getInstance() {
      if (!instance.isValid()) {
        instance = new T;
      }
      return instance;
    }

    /**
     * Returns Config class instance.
     *
     * @return Config*
     */
    inline static Config* getConfig() {
      return getInstance()->config;
    }

  public:
    /**
     * Dispatch incoming IMessage
     *
     * @param sIMessage_base*
     */
    inline int dispatch(sIMessage_base* msgBase) {
      // dispatch IMessage
      oEvent ev = _dispatcher.dispatch(msgBase);

      // dispatch action
      if (ev->getID() == IM_UIACTION) {
        ev = _action_dispatcher.dispatch(msgBase);
      }
      return ev->getReturnValue();
    }

    /**
     * Returns reference to IMessageDispatcher
     *
     * @return IMessageDispatcher&
     */
    inline IMessageDispatcher& getIMessageDispatcher() {
      return _dispatcher;
    }

    /**
     * Returns reference to IMActionDispatcher
     *
     * @return IMActionDispatcher&
     */
    inline ActionDispatcher& getActionDispatcher() {
      return _action_dispatcher;
    }

  protected:
    /**
     * Plugin initialization
     *
     * @param IMEvent&
     */
    inline void _onPlugInit(IMEvent& ev) {
      sIMessage* im = ev.getIMessage();
      Plug_Init(im->p1, im->p2);

      ev.setSuccess();
    }

    /**
     * Plugin deinitialization
     *
     * @param IMEvent&
     */
    inline void _onPlugDeInit(IMEvent& ev) {
      sIMessage* im = ev.getIMessage();
      Plug_Deinit(im->p1, im->p2);

      ev.setSuccess();
    }

  protected:
    IMessageDispatcher _dispatcher;
    ActionDispatcher _action_dispatcher;

    static oInstance instance;
    oConfig config;
  };

  template <class T>
  SharedPtr<T> PluginController<T>::instance = 0;
}

#endif // __PLUGINCONTROLLER_H__