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
      // get pointer to sUIActionNotify structure
      sUIActionNotify* an = static_cast<sUIActionNotify*>((sUIActionNotify_base*) getIMessage()->p1);

      // set pointer to sUIActionNotify (for performance reasons)
      setActionNotify(an);
      // set action id
      setID(an->act.id);
    }

    /**
     * Returns string represantation of the event
     *
     * @return string 
     */
    inline string toString() const {
      string str = stringf("id = %i, code = %i", getID(), getCode());

      // add contact id if exist
      if (getCnt()) {
        str += stringf(", cnt = %i", getCnt());
      }
      // add return value if event was processed
      if (isProcessed()) {
        str += stringf(", returnValue = %i", getReturnValue());
      }
      return str;
    }

  public:
    /**
     * Returns pointer to sUIActionNotify structure.
     *
     * @return sUIActionNotify*
     */
    inline sUIActionNotify* getActionNotify() const {
      return _an;
    }

    /**
     * Returns contact id
     *
     * @return int
     */
    inline int getCnt() const {
      return _an->act.cnt;
    }

    /**
     * Returns parent action id
     *
     * @return int
     */
    inline int getParent() const {
      return _an->act.parent;
    }

    /**
     * Returns action code
     *
     * @return int
     */
    inline int getCode() const {
      return _an->code;
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
    inline bool isSubclassed() const {
      return _dispatcher.isSublassed(getID(), getParent());
    }

    /**
     * Returns SubclassInfo action info
     *
     * @return ActionDispatcher::SubclassInfo&
     */
    inline ActionDispatcher::SubclassInfo& getSubclassInfo() {
      return _dispatcher.getSublassInfo(getID(), getParent());
    }

    /**
     * Forwards action to it's previous owner.
     */
    inline void forward() {
      setReturnValue(Ctrl->IMessageDirect(getIMessage()->id, getSubclassInfo().getPrevOwner(), (int) getActionNotify()));
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

    /**
     * Sets pointer to sUIActionNotify structure.
     *
     * @param sUIActionNotify*
     */
    inline void setActionNotify(sUIActionNotify* an) {
      _an = an;
    }

  protected:
    ActionDispatcher& _dispatcher;
    sUIActionNotify* _an;
  };
}

#endif // __ACTIONEVENT_H__