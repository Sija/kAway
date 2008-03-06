/**
  *  Plugin Controller class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @file
  *  @link          svn://konnekt.info/kaway2/ "SVN Repository"
  *  @author        Sijawusz Pur Rahnama <sija@gibbon.pl>
  *  @license       http://creativecommons.org/licenses/LGPL/2.1/
  *  @version       $Revision$
  *  @date          $Date$
  *  @modifiedby    $LastChangedBy$
  */

#pragma once

#ifndef __PLUGINCONTROLLER_H__
#define __PLUGINCONTROLLER_H__

#include "Context.h"

#include "Events/IMEvent.h"
#include "Events/ActionEvent.h"

using namespace Stamina;
using namespace boost;

namespace Konnekt {
  template <class T>
  class PluginController : public SharedObject<iSharedObject> {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(PluginController<T>, iSharedObject, Version(0,3,0,0));

  public:
    typedef SharedPtr<T> oInstance;

  protected:
    inline PluginController() {
      IMessageDispatcher& dispatcher = getIMessageDispatcher();
      ActionDispatcher& action_dispatcher = getActionDispatcher();

      // setting/unsetting Ctrl global pointer
      dispatcher.connect(IM_PLUG_INIT, bind(&PluginController::_onPlugInit, this, _1));
      dispatcher.connect(IM_PLUG_DEINIT, bind(&PluginController::_onPlugDeInit, this, _1));

      // actions subclassing
      dispatcher.connect(IM_UI_PREPARE, bind(&ActionDispatcher::doSubclass, &action_dispatcher, _1));
    }

  public:
    inline static T* getInstance() {
      if (!_instance.isValid()) {
        _instance = new T;
      }
      return _instance;
    }

    /**
     * Returns Config class instance.
     */
    inline Config& getConfig() {
      return Context::getInstance()->getConfig();
    }

    /**
     * Returns reference to IMessageDispatcher.
     */
    inline IMessageDispatcher& getIMessageDispatcher() {
      return Context::getInstance()->getIMessageDispatcher();
    }

    /**
     * Returns reference to IMActionDispatcher.
     */
    inline ActionDispatcher& getActionDispatcher() {
      return Context::getInstance()->getActionDispatcher();
    }

  public:
    /**
     * Dispatch incoming IMessage.
     */
    inline int dispatch(sIMessage_base* msgBase) {
      // dispatch IMessage
      oEvent ev = getIMessageDispatcher().dispatch(msgBase);

      // dispatch action
      if (ev->getID() == IM_UIACTION) {
        ev = getActionDispatcher().dispatch(msgBase);
      }
      return ev->getReturnValue();
    }

  protected:
    /**
     * Plugin initialization callback
     */
    inline void _onPlugInit(IMEvent& ev) {
      Plug_Init(ev.getP1(), ev.getP2());
      ev.setSuccess();
    }

    /**
     * Plugin deinitialization callback
     */
    inline void _onPlugDeInit(IMEvent& ev) {
      Plug_Deinit(ev.getP1(), ev.getP2());
      ev.setSuccess();
    }

  protected:
    static oInstance _instance;
  };

  template <class T>
  SharedPtr<T> PluginController<T>::_instance = 0;
}

#endif // __PLUGINCONTROLLER_H__