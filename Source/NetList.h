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

#include "Events/ActionDispatcher.h"
#include "Events/ActionEvent.h"

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
	STAMINA_OBJECT_CLASS_VERSION(NetList, iSharedObject, Version(0,1,0,0));

  class Item {
  public:
    friend class NetList;

    /**
     * Constructs a new Item.
     *
     * @param string net name
     * @param int action id
     * @param bool true if net is checked as active
     * @param bool true if net should be ignored
     */
    Item(int net, int action_id, const string& name, bool active = true) :
      _action_id(action_id), _net(net), _name(name), _active(active) { }

    /**
     * Constructs an empty Item.
     */
    Item(): _action_id(0), _net(0), _active(false) { }

  public:
    /**
     * Returns true if net is connected
     *
     * @return bool
     */
    inline bool isConnected() {
      return (bool) Ctrl->IMessage(IM_ISCONNECTED, getNet(), IMT_PROTOCOL);
    }

    /**
     * Returns icon id
     *
     * @return int
     */
    inline int getIconID() {
      return UIIcon(IT_LOGO, (int) getNet(), 0, 0);
    }

    /**
     * Returns action id
     *
     * @return int
     */
    inline int getActionID() {
      return _action_id;
    }

    /**
     * Returns net id
     *
     * @return int
     */
    inline int getNet() {
      return _net;
    }

    /**
     * Returns net name
     *
     * @return string
     */
    inline string getName() {
      return _name;
    }

    /**
     * Returns true if item is active
     *
     * @return bool
     */
    inline bool isActive() {
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
    unsigned int _action_id;
    unsigned int _net;
    string _name;
    bool _active;
  };

  typedef deque<int> tIgnored;
  typedef deque<Item> tItems;

public:
  /**
   * Constructs a new NetList object.
   */
  NetList(ActionDispatcher& dispatcher, int _cfgCol, int _cfgGroup, int _dynActGroup, int _actCreate, int _actDestroy) :
    cfgCol(_cfgCol), cfgGroup(_cfgGroup), dynActGroup(_dynActGroup), actCreate(_actCreate), actDestroy(_actDestroy), 
    defaultUse(true), _opened(false)
  {
    dispatcher.connect(actCreate, boost::bind(&NetList::_onCreate, this, _1));
    dispatcher.connect(actDestroy, boost::bind(&NetList::_onDestroy, this, _1));
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
  bool hasItem(int net);

  /**
   * Returns reference to Item class for given net id
   *
   * @param int net id
   *
   * @return Item&
   */
  Item& getItem(int net);

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
   * @return bool
   */
  bool isIgnored(int net);

  /**
   * Adds fiven net to the ignored list
   *
   * @param int
   */
  void addIgnored(int net);

  /**
   * Removes given net from the ignored list
   *
   * @param int
   */
  void removeIgnored(int net);

protected:
  /**
   * Draws given Item object
   *
   * @param Item&
   * @param bool true if control should be drawn as inline 
   * @param int x position
   */
  void _drawItem(Item& item, bool draw_inline, int x);

  /**
   * Events handling methods
   */
  void _onCreate(ActionEvent& ev);
  void _onDestroy(ActionEvent& ev);
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
  unsigned int actDestroy;
  bool defaultUse;
};

// smart pointer type
typedef SharedPtr<NetList> oNetList;

#endif // __NETLIST_H__