#include "stdafx.h"

#include "IMessageDispatcher.h"
#include "IMEvent.h"

namespace Konnekt {
  /**
   * Dispatch incoming IMessage
   *
   * @param sIMessage_base*
   */
  Event* IMessageDispatcher::dispatch(sIMessage_base* msgBase) {
    // get imessage id
    int id = msgBase->id;

    // create event object
    IMEvent* ev = new IMEvent(msgBase);

    try {
      // look for static values
      if (_static_values.find(id) != _static_values.end()) {
        ev->setReturnValue(_static_values[id]);
      } else {
        // notify global listeners
        notify(*ev);
      }

      // log IMessage
      string msg = stringf("[IMessageDispatcher::dispatch()]: id = %i", id);

      if (ev->isProcessed()) {
        msg += stringf(", returnValue = %i", ev->getReturnValue());
      } else {
        // set error if no result
        if (Ctrl) {
          Ctrl->setError(IMERROR_NORESULT);
        }
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