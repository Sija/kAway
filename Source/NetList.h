/**
  *  NetList class
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

#ifndef __NETLIST_H__
#define __NETLIST_H__

#include <konnekt/contrib/Events/ActionDispatcher.h>
#include <konnekt/contrib/Events/ActionEvent.h>
#include <konnekt/contrib/Config.h>

#include "Helpers.h"

class NetList : public SharedObject<iSharedObject> {
public:
  /**
   * Structure holding IMessage id constants
   */
  static const struct IM {
    static const unsigned int key = IM_USER + (900 * 1000) + 50;
    static const unsigned int hidePresence = key + 1;
  };

public:
  /*
   * Class version macro
   */
	STAMINA_OBJECT_CLASS_VERSION(NetList, iSharedObject, Version(0,2,0,0));

  class Item {
  public:
    friend class NetList;

    /**
     * Constructs a new Item.
     *
     * @param string net name
     * @param int action id
     * @param int parent action id
     * @param bool true if net is checked as active
     */
    Item(unsigned int net, int action_id, int parent_id, const string& name, bool active = true) :
      _action_id(action_id), _parent_id(parent_id), _net(net), _name(name), _active(active) { }

    /**
     * Constructs an empty Item.
     */
    Item(): _action_id(0), _parent_id(0), _net(Net::none), _active(false) { }

  public:
    /**
     * Returns true if net is connected
     *
     * @return bool
     */
    inline bool isConnected() const {
      return Ctrl->IMessage(IM_ISCONNECTED, getNet(), imtProtocol) == 1;
    }

    /**
     * Returns icon id
     *
     * @return int
     */
    inline int getIconID() const {
      return UIIcon(IT_LOGO, (int) getNet(), 0, 0);
    }

    /**
     * Returns action id
     *
     * @return int
     */
    inline int getActionID() const {
      return _action_id;
    }

    /**
     * Returns parent action id
     *
     * @return int
     */
    inline int getParentID() const {
      return _parent_id;
    }

    /**
     * Returns net id
     *
     * @return int
     */
    inline tNet getNet() const {
      return (tNet) _net;
    }

    /**
     * Returns net name
     *
     * @return string
     */
    inline string getName() const {
      return _name;
    }

    /**
     * Returns true if item is active
     *
     * @return bool
     */
    inline bool isActive() const {
      return _active;
    }

    /**
     * Sets the item status
     *
     * @param bool
     */
    inline void setActive(bool active) {
      _active = active;
    }

  protected:
    /**
     * Draws the ui control
     *
     * @param bool true if control should be drawn inline
     * @param int left margin (in px)
     */
    inline void draw(bool draw_inline, int left_margin = 0) {
      Ctrl->log(logFunc, "NetList::Item", "draw", "name = %s, active = %s",
        getName().c_str(), btoa(isActive()));

      UIActionCfgAdd(getParentID(), 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(getIconID()).a_str(), 0, left_margin);
      UIActionCfgAdd(getParentID(), getActionID(), ACTT_CHECK | (draw_inline ? ACTSC_INLINE : 0), getName().c_str());
    }

  protected:
    unsigned int _action_id;
    unsigned int _parent_id;
    unsigned int _net;
    string _name;
    bool _active;
  };

  typedef deque<unsigned int> tIgnored;
  typedef deque<Item> tItems;

public:
  /**
   * Constructs a new NetList object.
   */
  NetList(ActionDispatcher& dispatcher, int _cfgCol, int _cfgGroup, int _dynActGroup, int _actCreate) :
    cfgCol(_cfgCol), cfgGroup(_cfgGroup), dynActGroup(_dynActGroup), actCreate(_actCreate), defaultUse(true), _opened(false)
  {
    dispatcher.connect(actCreate, boost::bind(&NetList::_groupListener, this, _1));
    dispatcher.connect(cfgGroup, boost::bind(&NetList::_onAction, this, _1));
  }

  /**
   * Loads NetList state from configuration.
   */
  void load();
  /**
   * Save NetList state to configuration.
   */
  void save();

  /**
   * Draws items group in configuration
   *
   * @param int number of columns
   * @param string& group title
   */
  void drawGroup(int columns = 3, const string& title = "");
  /**
   * Draws items in configuration
   *
   * @param int number of columns
   */
  void drawItems(int columns = 3);

  /**
   * Updates loaded items based on ui controls selection
   */
  void refreshFromUI();
  /**
   * Sets controls statuses based on loaded items
   */
  void refreshUI();

  /**
   * Returns true if given net is loaded
   *
   * @param int net id
   *
   * @return bool
   */
  bool hasItem(unsigned int net) const;

  /**
   * Returns reference to Item class for given net id
   *
   * @param int net id
   *
   * @return Item&
   */
  Item& getItem(unsigned int net);

  /**
   * Returns reference Items list
   *
   * @return tItems&
   */
  inline tItems& getItems() {
    return _items;
  }

  /**
   * Returns true if given net is ignored
   *
   * @param int
   *
   * @return bool
   */
  bool isIgnored(unsigned int net) const;

  /**
   * Adds fiven net to the ignored list
   *
   * @param int
   */
  void addIgnored(unsigned int net);

  /**
   * Removes given net from the ignored list
   *
   * @param int
   */
  void removeIgnored(unsigned int net);

protected:
  /**
   * Events handling methods
   */
  void _groupListener(ActionEvent& ev);
  void _onAction(ActionEvent& ev);

protected:
  tIgnored _ignored;
  tItems _items;
  bool _opened;

  // TODO: zrobic cos z tym syfem
  unsigned int cfgCol;
  unsigned int cfgGroup;
  unsigned int dynActGroup;
  unsigned int actCreate;
  bool defaultUse;
};

// smart pointer type
typedef SharedPtr<NetList> oNetList;

#endif // __NETLIST_H__