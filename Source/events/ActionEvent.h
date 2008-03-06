/**
  *  Action Event class
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
     * @param msgBase     IMessage sent from Core
     * @param dispatcher  ActionDispatcher object
     */
    ActionEvent(sIMessage_base* msgBase, ActionDispatcher& dispatcher): IMEvent(msgBase), _dispatcher(dispatcher) {
      // get pointer to sUIActionNotify structure
      sUIActionNotify* an = static_cast<sUIActionNotify*>((sUIActionNotify_base*) getP1());

      // set pointer to sUIActionNotify (for performance reasons)
      setActionNotify(an);
      // set action id
      setID(an->act.id);
    }

    /**
     * Returns string represantation of the event.
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
     */
    inline sUIActionNotify* getActionNotify() const {
      return _an;
    }

    /**
     * Returns attached action.
     */
    inline const sUIAction& getAction() const {
      return getActionNotify()->act;
    }

    /**
     * Returns contact id.
     */
    inline int getCnt() const {
      return getAction().cnt;
    }

    /**
     * Returns parent action id.
     */
    inline int getParent() const {
      return getAction().parent;
    }

    /**
     * Returns action code.
     */
    inline int getCode() const {
      return getActionNotify()->code;
    }

    /**
     * Returns true if given code is the same as action's.
     */
    inline bool isCode(int code) const {
      return getCode() == code;
    }

    /**
     * Returns reference to ActionDispatcher class.
     */
    inline ActionDispatcher& getDispatcher() {
      return _dispatcher;
    }

    /**
     * Returns true if action is subclassed
     */
    inline bool isSubclassed() const {
      return _dispatcher.isSublassed(getID(), getParent());
    }

    /**
     * Returns SubclassInfo action info.
     */
    inline ActionDispatcher::SubclassInfo& getSubclassInfo() {
      return getDispatcher().getSublassInfo(getID(), getParent());
    }

    /**
     * Forwards action to it's previous owner.
     */
    inline void forward() {
      tPluginId previous_owner = getSubclassInfo().getPrevOwner();
      if (previous_owner == pluginNotFound) {
        throw new ExceptionString("Previous owner plugin not found");
      }
      setReturnValue(Ctrl->IMessageDirect(getIMessage()->id, previous_owner, (int) getActionNotify()));
    }

  protected:
    /**
     * Sets action dispatcher for this event.
     */
    inline void setDispatcher(ActionDispatcher& dispatcher) {
      _dispatcher = dispatcher;
    }

    /**
     * Sets pointer to sUIActionNotify structure.
     */
    inline void setActionNotify(sUIActionNotify* an) {
      _an = an;
    }

  protected:
    ActionDispatcher& _dispatcher;
    sUIActionNotify* _an;
  };

  /// smart pointer type
  typedef SharedPtr<ActionEvent> oActionEvent;
}

#endif // __ACTIONEVENT_H__