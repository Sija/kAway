#include "stdafx.h"

#include "ActionDispatcher.h"
#include "ActionEvent.h"

namespace Konnekt {
  /**
   * Dispatches incoming Action IMessage
   *
   * @param sIMessage_base*
   */
  Event* ActionDispatcher::dispatch(sIMessage_base* msgBase) {
    // create event object
    ActionEvent* ev = new ActionEvent(msgBase, *this);

    try {
      // notify action listeners
      notify(*ev);

      try {
        if (ev->getSubclassedInfo().isAutoForwardable()) {
          ev->forward();
        }
      } catch (...) { }

      // log IMessage
      string msg = stringf("[ActionDispatcher::dispatch()]: id = %i, code = %i, cnt = %i", 
        ev->getID(), ev->getCode(), ev->getParent());

      if (ev->isProcessed()) {
        msg += stringf(", returnValue = %i", ev->getReturnValue());
      }
      IMLOG(msg.c_str());

      // return event
      return ev;

    } catch (...) {
      delete ev;
      throw;
    }
  }
}