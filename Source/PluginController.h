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

#include "PluginDispatcher.h"
#include "Config.h"

using namespace Stamina;
using namespace boost;

namespace Konnekt {
  template <class T>
  class PluginController : public PluginDispatcher {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(PluginController<T>, PluginDispatcher, Version(0,1,0,0));

  public:
    typedef SharedPtr<T> oInstance;

  protected:
    /**
     * Creates new PluginController object.
     */
    inline PluginController() {
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

  protected:
    static oInstance instance;
    oConfig config;
  };

  template <class T>
  SharedPtr<T> PluginController<T>::instance = 0;
}

#endif // __PLUGINCONTROLLER_H__