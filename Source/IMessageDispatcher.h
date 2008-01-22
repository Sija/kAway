/**
  *  IMessage Dispatcher class
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

#ifndef __IMESSAGEDISPATCHER_H__
#define __IMESSAGEDISPATCHER_H__

#include "EventDispatcher.h"
#include "Event.h"

namespace Konnekt {
  class IMessageDispatcher : public EventDispatcher<class IMEvent> {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(IMessageDispatcher, EventDispatcher<IMEvent>, Version(0,1,0,0));

  public:
    typedef hash_map<int, int> tStaticValues;

  public:
    inline IMessageDispatcher() {
      // default values for common messages, may be overriden
      setStaticValue(IM_PLUG_NETNAME, 0);
      setStaticValue(IM_PLUG_PRIORITY, 0);
      setStaticValue(IM_PLUG_SDKVERSION, KONNEKT_SDK_V);
      setStaticValue(IM_PLUG_VERSION, 0);
      setStaticValue(IM_PLUG_CORE_V, (int) "W98");
      setStaticValue(IM_PLUG_UI_V, 0);
    }

  public:
    /**
     * Sets the static value for a given IMessage id
     *
     * @param int IMessage id
     * @param int value
     */
    inline void setStaticValue(int id, int value) {
      _static_values[id] = value;
    }

    /**
     * Dispatch incoming IMessage
     *
     * @param sIMessage_base*
     */
    Event* dispatch(sIMessage_base* msgBase);

  protected:
    tStaticValues _static_values;
  };
}

#endif // __IMESSAGEDISPATCHER_H__