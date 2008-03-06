/**
  *  Generic Event Dispatcher class
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

#ifndef __EVENTDISPATCHER_H__
#define __EVENTDISPATCHER_H__

#include <hash_map>
#include <deque>

#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace Stamina;
using namespace boost;

using namespace std;
using namespace stdext;

namespace Konnekt {
  /**
   * Exception which stops chain execution
   */
  class StopEventNotifyException : public ExceptionString {
  public:
    StopEventNotifyException() : ExceptionString("Event notification was interrupted") { }
  };

  template <class E>
  class EventDispatcher : public SharedObject<iSharedObject> {
  public:
    /**
     * Class version macro
     */
    STAMINA_OBJECT_CLASS_VERSION(EventDispatcher<E>, iSharedObject, Version(0,1,0,0));

  public:
    typedef deque<signals::connection> tConnections;

  public:
    typedef function<void(E&)> fListener;
    typedef signal<void(E&)> sigListener;

  public:
    /**
     * Structure holding listener and it's connections
     */
    struct sListener {
      tConnections connections;
      sigListener signal;
    };

  public:
    typedef hash_map<int, sListener*> tListeners;

  public:
    inline ~EventDispatcher() {
      for (tListeners::iterator it = _listeners.begin(); it != _listeners.end(); it++) {
        delete it->second;
      }
    }

  public:
    /**
     * Connects a listener to a given event id.
     *
     * @param id        An event id
     * @param f         Listener callback
     * @param priority  Listener priority
     * @param pos       Position in list
     *
     * @return connection object
     */
    inline signals::connection connect(int id, const fListener& f, int priority = 0, signals::connect_position pos = signals::at_back) {
      if (f.empty()) {
        throw ExceptionString("Empty functor was given.");
      }

      if (_listeners.find(id) == _listeners.end()) {
        _listeners[id] = new sListener;
      }

      signals::connection c = _listeners[id]->signal.connect(priority, f, pos);
      if (!c.connected()) {
        throw ExceptionString("Listener was not connected.");
      }

      _listeners[id]->connections.push_back(c);
      return c;
    }

    /**
     * Disconnects a listener for a given event id.
     * TODO: jak to obsluzyc ?
     *
     * @param id   An event id
     * @param f    Listener callback
     *
     * @return  true if listener was successfuly diconnected
     */
    inline bool disconnect(int id, const fListener& f) {
      if (!hasListeners(id)) {
        return false;
      }
      return true;
    }

    /**
     * Notifies all listeners of a given event.
     *
     * @param ev  An Event object
     * @return    Modified @a ev object
     */
    inline E& notify(E& ev) {
      int id = ev.getID();

      if (hasListeners(id)) {
        try {
          _listeners[id]->signal(ev);
        } catch (StopEventNotifyException&) { }
      }
      return ev;
    }

    /**
     * Returns true if the given event id has some listeners.
     * @param  id  The event id
     */
    inline bool hasListeners(int id) {
      if (_listeners.find(id) == _listeners.end()) {
        return false;
      }
      return !_listeners[id]->signal.empty();
    }

  protected:
    tListeners _listeners;
  };
}

#endif // __EVENTDISPATCHER_H__