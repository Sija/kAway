/**
  *  Action Event class
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

#ifndef __ACTIONEVENT_H__
#define __ACTIONEVENT_H__

#include "IMEvent.h"
#include "ActionDispatcher.h"

namespace Konnekt {
  class ActionEvent : public IMEvent {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(ActionEvent, IMEvent, Version(0,1,0,0));

  public:
    /**
     * Constructs a new ActionEvent.
     *
     * @param sIMessage_base* sUIActionNotify_base structure
     * @param ActionDispatcher& ActionDispatcher object reference
     */
    ActionEvent(sIMessage_base* msgBase, ActionDispatcher& dispatcher): IMEvent(msgBase), _dispatcher(dispatcher) {
      setID(getActionNotify()->act.id);
    }

  public:
    /**
     * Returns pointer to sUIActionNotify structure.
     *
     * @return sUIActionNotify*
     */
    inline sUIActionNotify* getActionNotify() {
      return static_cast<sUIActionNotify*>((sUIActionNotify_base*) getIMessage()->p1);
    }

    /**
     * Returns contact id
     *
     * @return int
     */
    inline int getCnt() {
      return getActionNotify()->act.cnt;
    }

    /**
     * Returns parent action id
     *
     * @return int
     */
    inline int getParent() {
      return getActionNotify()->act.parent;
    }

    /**
     * Returns action code
     *
     * @return int
     */
    inline int getCode() {
      return getActionNotify()->code;
    }

    /**
     * Returns reference to ActionDispatcher class.
     *
     * @return ActionDispatcher&
     */
    inline ActionDispatcher& getDispatcher() {
      return _dispatcher;
    }

    /**
     * Returns true if action is subclassed
     *
     * @return bool
     */
    inline bool isSubclassed() {
      sUIActionNotify* an = getActionNotify();
      return _dispatcher.isSublassed(an->act.id, an->act.parent);
    }

    /**
     * Returns subclassed action info
     *
     * @return ActionDispatcher::Subclassed&
     */
    inline ActionDispatcher::Subclassed& getSubclassedInfo() {
      sUIActionNotify* an = getActionNotify();
      return _dispatcher.getSublassed(an->act.id, an->act.parent);
    }

    /**
     * Forwards action to it's parent.
     */
    inline void forward() {
      setReturnValue(Ctrl->IMessageDirect(getIMessage()->id, getSubclassedInfo().getPrevOwner(), (int) getActionNotify()));
    }

  protected:
    /**
     * Sets action dispatcher for this event
     *
     * @param ActionDispatcher&
     */
    inline void setDispatcher(ActionDispatcher& dispatcher) {
      _dispatcher = dispatcher;
    }

  protected:
    ActionDispatcher& _dispatcher;
  };
}

#endif // __ACTIONEVENT_H__