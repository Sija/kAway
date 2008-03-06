/**
  *  Action Dispatcher class
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

#include "stdafx.h"

#include "ActionDispatcher.h"
#include "ActionEvent.h"

namespace Konnekt {
  oEvent ActionDispatcher::dispatch(sIMessage_base* msgBase) {
    // create event object
    oActionEvent ev = new ActionEvent(msgBase, *this);

    // notify action listeners
    notify(*ev);

    try {
      if (ev->getSubclassInfo().autoForward()) {
        ev->forward(); // forward to previous owner
      }
    } catch (NotSubclassedException&) { }

    // log IMessage
    if (Ctrl) {
      Ctrl->logMsg(logEvent, "ActionDispatcher", "dispatch", ev->toString().c_str());
    }
    // return event
    return ev;
  }
}