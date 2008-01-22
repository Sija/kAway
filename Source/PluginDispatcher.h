/**
  *  Plugin Dispatcher class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2008 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy: sija $
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __PLUGINDISPATCHER_H__
#define __PLUGINDISPATCHER_H__

#include "EventDispatcher.h"
#include "Event.h"

#include "ActionDispatcher.h"
#include "ActionEvent.h"

#include "IMessageDispatcher.h"
#include "IMEvent.h"

namespace Konnekt {
  class PluginDispatcher : public SharedObject<iSharedObject>, public signals::trackable {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(PluginDispatcher, iSharedObject, Version(0,3,0,0));

  public:
    /**
     * Constructs a new PluginDispatcher.
     */
    inline PluginDispatcher() {
      // setting/unsetting Ctrl global pointer
      _global_dispatcher.connect(IM_PLUG_INIT, bind(&PluginDispatcher::_onPlugInit, this, _1));
      _global_dispatcher.connect(IM_PLUG_DEINIT, bind(&PluginDispatcher::_onPlugDeInit, this, _1));

      // actions subclassing
      _global_dispatcher.connect(IM_UI_PREPARE, bind(&ActionDispatcher::doSubclass, &_action_dispatcher, _1));
    }

  public:
    /**
     * Dispatch incoming IMessage
     *
     * @param sIMessage_base*
     */
    inline int dispatch(sIMessage_base* msgBase) {
      // dispatch IMessage
      oEvent ev = _global_dispatcher.dispatch(msgBase);

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
      return _global_dispatcher;
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
    IMessageDispatcher _global_dispatcher;
    ActionDispatcher _action_dispatcher;
  };
}

#endif // __PLUGINDISPATCHER_H__