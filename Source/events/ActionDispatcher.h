/**
  *  Action Dispatcher class
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

#ifndef __ACTIONDISPATCHER_H__
#define __ACTIONDISPATCHER_H__

#include "EventDispatcher.h"
#include "IMEvent.h"

namespace Konnekt {
  class ActionDispatcher : public EventDispatcher<class ActionEvent> {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(ActionDispatcher, EventDispatcher<ActionEvent>, Version(0,1,0,0));

  public:
    class SubclassInfo {
    public:
      /**
       * Constructs a new SubclassInfo.
       *
       * @param int action id
       * @param int parent action id
       * @param bool automatically forwards action to it's parent if set to true
       */
      SubclassInfo(int id, int parent, bool auto_forward):
        _id(id), _parent(parent), _prev_owner(-1), _auto_forward(auto_forward) { }

      /**
       * Constructs an empty instance of SubclassInfo.
       */
      SubclassInfo(): _id(0), _parent(0), _prev_owner(-1), _auto_forward(false) { }

    /**
     * Returns action ID
     *
     * @return int
     */
    inline int getID() {
      return _id;
    }

    /**
     * Returns parent action ID
     *
     * @return int
     */
    inline int getParent() {
      return _parent;
    }

    /**
     * Returns previous owner plugin ID
     *
     * @return int
     */
    inline int getPrevOwner() {
      return _prev_owner;
    }

    /**
     * Sets previous owner plugin ID
     *
     * @param int
     */
    inline void setPrevOwner(int owner) {
      _prev_owner = owner;
    }

    /**
     * Returns true if action should be forwarded to it's previous owner
     *
     * @return bool
     */
    inline bool autoForward() {
      return _auto_forward;
    }

    protected:
      unsigned int _parent;
      unsigned int _id;
      int _prev_owner;
      bool _auto_forward;
    };

  public:
    typedef deque<SubclassInfo> tSubclassed;

  public:
    /**
     * Returns true if the given action is subclassed
     *
     * @param int action id
     * @param int parent action id
     *
     * @return bool
     */
    inline bool isSublassed(int id, int parent) {
      for (tSubclassed::iterator it = _subclassed.begin(); it != _subclassed.end(); it++) {
        if (it->getID() == id && it->getParent() == parent) return true;
      }
      return false;
    }

    /**
     * Returns reference of SubclassInfo
     *
     * @param int action id
     * @param int parent action id
     *
     * @return SubclassInfo&
     */
    inline SubclassInfo& getSublassInfo(int id, int parent) {
      for (tSubclassed::iterator it = _subclassed.begin(); it != _subclassed.end(); it++) {
        if (it->getID() == id && it->getParent() == parent) return *it;
      }
      throw new ExceptionString("Action is not subclassed");
    }

    /**
     * Subclasses the given action
     *
     * @param int action id
     * @param int parent action id
     * @param bool true if should forward to parent
     */
    inline void subclass(int id, int parent, bool auto_forward = false) {
      if (isSublassed(id, parent)) {
        return;
      }
      _subclassed.push_back(SubclassInfo(id, parent, auto_forward));
    }

    /**
     * Dispatches incoming Action IMessage
     *
     * @param sIMessage_base*
     * @return oEvent
     */
    oEvent dispatch(sIMessage_base* msgBase);

    /**
     * Subclasses all registered actions
     *
     * @param IMEvent&
     */
    inline void doSubclass(IMEvent& ev) {
      for (tSubclassed::iterator it = _subclassed.begin(); it != _subclassed.end(); it++) {
        sUIActionInfo nfo(it->getParent(), it->getID());
        nfo.mask = UIAIM_ALL;
        nfo.txt = new char[100];
        nfo.txtSize = 99;

        UIActionGet(nfo);

        int prev_owner = 0;
        if (!(prev_owner = Ctrl->ICMessage(IMI_ACTION_GETOWNER, (int) &nfo.act))) {
          prev_owner = Ctrl->ICMessage(IMC_PLUG_ID, 0);
        }
        it->setPrevOwner(prev_owner);

        IMLOG("[ActionDispatcher::subclass()]: name = %s, id = %i, parent = %i, prevOwner = %i",
          nfo.txt, it->getID(), it->getParent(), it->getPrevOwner());

        Ctrl->ICMessage(IMI_ACTION_REMOVE, (int) &nfo.act);
        Ctrl->ICMessage(IMI_ACTION, (int) &nfo);

        delete [] nfo.txt;
      }
    }

  protected:
    tSubclassed _subclassed;
  };
}

#endif // __ACTIONDISPATCHER_H__