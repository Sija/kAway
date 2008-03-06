/**
  *  Context class
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
  *  @modifiedby    $LastChangedBy: sija $
  */

#pragma once

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Events/IMessageDispatcher.h"
#include "Events/ActionDispatcher.h"

#include "Config.h"

namespace Konnekt {
  class Context : public SharedObject<iSharedObject> {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(Context, iSharedObject, Version(0,1,0,0));

  public:
    typedef SharedPtr<Context> oInstance;

  protected:
    inline Context() {
      _config.attachListeners(getIMessageDispatcher());
    }

  public:
    /**
     * Returns instance of Context object.
     */
    inline static Context* getInstance() {
      if (!_instance.isValid()) {
        _instance = new Context;
      }
      return _instance;
    }

    /**
     * Returns Config class instance.
     */
    inline Config& getConfig() {
      return _config;
    }

    /**
     * Returns reference to IMessageDispatcher.
     */
    inline IMessageDispatcher& getIMessageDispatcher() {
      return _imessage_dispatcher;
    }

    /**
     * Returns reference to IMActionDispatcher.
     */
    inline ActionDispatcher& getActionDispatcher() {
      return _action_dispatcher;
    }

  private:
    static oInstance _instance;

  private:
    IMessageDispatcher _imessage_dispatcher;
    ActionDispatcher _action_dispatcher;
    Config _config;
  };

  __declspec(selectany)
  Context::oInstance Context::_instance = 0;
}

#endif // __CONTEXT_H__