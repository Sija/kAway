/**
  *  Configuration actions
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision: 77 $
  *  @modifiedby   $LastChangedBy: sija $
  *  @lastmodified $Date: 2006-11-20 17:42:35 +0100 (Pn, 20 lis 2006) $
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __CFGACTIONS_H__
#define __CFGACTIONS_H__

#include "Helpers.h"

using namespace Konnekt;
using namespace Stamina;
using namespace boost;

#define CfgActionFlagMethod(name, flag) \
  inline tInstance& name(bool set) { \
    return setFlag(flag, set); \
  } \
  inline bool name() { \
    return haveFlag(flag); \
  }

namespace Konnekt {
  class CfgDummyAction;
  class CfgGroup;
  class CfgPage;

  typedef std::list<CfgDummyAction> tCfgActions;
  typedef std::list<CfgGroup> tCfgGroups;
  typedef std::list<CfgPage> tCfgPages;

  /*
   * Configuration infoBox class
   */
  class CfgInfoBox {
  public:
    CfgInfoBox(): height(0), ico(0), set(false), frame(false) { }
    CfgInfoBox(char* _header, char* _desc, int _ico, int _height = 3, bool _frame = true):
      header(_header), desc(_desc), height(_height), ico(_ico), frame(_frame), set(true) { }

    inline void draw(int parent) {
      if (!set) return;

      UIActionCfgAddPluginInfoBox2(parent, desc.c_str(), header.c_str(), 
        Helpers::icon16(ico).a_str(), - height, 0, frame);
    }
  public:
    String header;
    String desc;

    int height;
    int ico;

    bool frame;
    bool set;
  };

  /*
   * Base configuration action class
   * \todo notifications
   */
  template <class Action>
  class CfgActionBase : public iObject {
  public:
    typedef Action tInstance;

  public:
    CfgActionBase(): id(0), parent(0), status(0), neededNet(0), 
      bitsLevel(ShowBits::levelBeginner) { }
    virtual ~CfgActionBase() { }

    inline tInstance& instance() {
      return *this->castObject<tInstance>();
    }

    inline tInstance& needPlugin(int net) {
      this->neededNet = net;
      return instance();
    }

    inline tInstance& setID(int id) {
      this->id = id;
      return instance();
    }

    inline tInstance& setParent(int parent) {
      this->parent = parent;
      return instance();
    }

  public:
    inline tInstance& setFlag(int flag, bool active = true) {
      if (!active) {
        return clearFlag(flag);
      }
      if (!haveFlag(flag)) {
        status |= flag;
      }
      return instance();
    }
    inline tInstance& clearFlag(int flag) {
      if (haveFlag(flag)) {
        status &= ~flag;
      }
      return instance();
    }
    inline bool haveFlag(int flag) {
      return status & flag;
    }

  public:
    inline tInstance& setBits(ShowBits::enLevel level) {
      this->bitsLevel = level;
      return instance();
    }
    inline bool checkBits() {
      return ShowBits::checkLevel(bitsLevel);
    }

  public:
    ShowBits::enLevel bitsLevel;
    int neededNet;
    int status;

    int parent;
    int id;
  };

  /*
   * Configuration action class
   */
  template <class ChildAction = CfgDummyAction>
  class CfgAction : public CfgActionBase<ChildAction> {
  public:
    CfgAction(): col(0), type(0), x(0), y(0), width(0), height(0), 
      tipHtml(false), tipWidth(0) { };
    
    CfgAction(ChildAction& action) {
      this = *action;
    }
    virtual ~CfgAction() { }

  public:
    virtual void draw(int parent);

  public:
    inline tInstance& setX(int x) {
      this->x = x;
      return instance();
    }
    inline tInstance& setY(int y) {
      this->y = y;
      return instance();
    }
    inline tInstance& setWidth(int width) {
      this->width = width;
      return instance();
    }
    inline tInstance& setHeight(int height) {
      this->height = height;
      return instance();
    }

  public:
    virtual int getID();
    virtual String getText();

  public:
    inline ChildAction& setTip(char* txt, bool html = false, int width = 0) {
      this->tipWidth = width;
      this->tipHtml = html;
      this->tip = txt;
      return instance();
    }
    inline ChildAction& setTipHtml(bool html) {
      this->tipHtml = html;
      return instance();
    }
    inline ChildAction& setTipWidth(int width) {
      this->tipWidth = width;
      return instance();
    }

  public:
    CfgActionFlagMethod(isInline, ACTSC_INLINE);
    CfgActionFlagMethod(isFullWidth, ACTSC_FULLWIDTH);
    CfgActionFlagMethod(isBold, ACTSC_BOLD);
    CfgActionFlagMethod(isInt, ACTSC_INT);
    CfgActionFlagMethod(needRestart, ACTSC_NEEDRESTART);
    CfgActionFlagMethod(isEnabled, ACTS_DISABLED);

  public:
	  String text;
	  String tip;

    int type;
    int col;

	  int x;
	  int y;
	  int width;
	  int height;

	  int tipWidth;
	  bool tipHtml;
  };

  /*
   * Configuration actions collection class
   */
  template <class Collection, class CollectionOf>
  class CfgActionsCollection : public CfgActionBase<Collection> {
  public:
    typedef std::list<CollectionOf*> tActions;

  public:
    CfgActionsCollection() { }

    virtual ~CfgActionsCollection() {
      for (tActions::iterator it = _actions.begin(); it != _actions.end(); it++) {
        delete *it;
      }
    }

    template <class ChildAction>
    inline Collection& operator += (ChildAction& action) {
      return addAction(action);
    }

    template <class ChildAction>
    inline Collection& operator << (ChildAction& action) {
      return addAction(action);
    }

    template <class ChildAction>
    inline Collection& addAction(ChildAction& action) {
      ChildAction* _action = new ChildAction(action);

      this->_actions.push_back((CollectionOf*) _action);
      return instance();
    }

    inline const tActions& getActions() const {
      return _actions;
    }

    inline bool checkBits() {
      if (!CfgActionBase<Collection>::checkBits()) {
        return false;
      }
      bool anyAlive = false;

      for (tActions::iterator it = this->_actions.begin(); it != this->_actions.end(); it++) {
        if ((*it)->checkBits()) {
          anyAlive = true; break;
        }
      }
      return anyAlive;
    }

  protected:
    tActions _actions;
  };

  /*
   * Configuration group class
   */
  class CfgGroup : public CfgActionsCollection<CfgGroup, CfgDummyAction> {
  public:
    CfgGroup(char* _txt = 0): txt(_txt) { }
    CfgGroup(): txt(0) { };

    void draw(int parent);

  protected:
    char* txt;
  };

  /*
   * Configuration page class
   */
  class CfgPage : public CfgActionsCollection<CfgPage, CfgGroup> {
  public:
    CfgPage(int _id, int _parent, char* _txt, int _ico = 0): txt(_txt), ico(_ico) {
      parent = _parent;
      id = _id;
    }
    CfgPage(): txt(0), ico(0) { };

    void draw();

    inline CfgPage& addInfoBox(CfgInfoBox& infoBox) {
      _infoBox = infoBox;
      return *this;
    }

    inline CfgPage& operator += (CfgInfoBox& infoBox) {
      return addInfoBox(infoBox);
    }

    inline CfgPage& operator += (CfgGroup& group) {
      return CfgActionsCollection<CfgPage, CfgGroup>::operator +=(group);
    }

  protected:
    CfgInfoBox _infoBox;

    char* txt;
    int ico;
  };

  /*
   * Custom actions
   */
  class CfgDummyAction: public CfgAction<CfgDummyAction> {
  };

  class CfgLabel: public CfgAction<CfgLabel> {
  public:
    CfgLabel(char* _text = 0, bool wordwrap = false) {
      type = wordwrap ? ACTT_INFO : ACTT_COMMENT;
      text = _text;
    }
  };

  class CfgEdit: public CfgAction<CfgEdit> {
  public:
    CfgEdit(int _col = 0) {
      type = ACTT_EDIT;
      col = _col;
    }

    CfgActionFlagMethod(readOnly, ACTSEDIT_READONLY);
  };

  class CfgText: public CfgAction<CfgText> {
  public:
    CfgText(int _col = 0) {
      type = ACTT_TEXT;
      col = _col;
    }
  };

  class CfgImage: public CfgAction<CfgImage> {
  public:
    CfgImage(char* _text = 0) {
      type = ACTT_IMAGE;
      text = _text;
    }
  };

  class CfgComboBox: public CfgAction<CfgComboBox> {
  public:
    CfgComboBox(int _col = 0) {
      type = ACTT_COMBO;
      col = _col;
    }

    CfgActionFlagMethod(byPos, ACTSCOMBO_BYPOS);
    CfgActionFlagMethod(noIcon, ACTSCOMBO_NOICON);
    CfgActionFlagMethod(isSort, ACTSCOMBO_SORT);
    CfgActionFlagMethod(isList, ACTSCOMBO_LIST);
  };

  class CfgCheckBox: public CfgAction<CfgCheckBox> {
  public:
    CfgCheckBox(char* _text = 0, int _col = 0) {
      type = ACTT_CHECK;
      text = _text;
      col = _col;
    }
  };

  class CfgRadio: public CfgAction<CfgRadio> {
  public:
    explicit CfgRadio(char* _text = 0, int _col = 0, int _id = 0) {
      type = ACTT_RADIO;
      text = _text;
      col = _col;
      id = _id;
    }

  public:
    CfgActionFlagMethod(isLast, ACTSRADIO_LAST);
    CfgActionFlagMethod(byPos, ACTSRADIO_BYPOS);

    inline tInstance& setValue(int value) {
      return setValue((char*) inttostr(value).c_str());
    }

    inline tInstance& setValue(char* value) {
      this->value = value;
      return instance();
    }

  public:
    int getID() {
      IMLOG("id = %i", id);
      return id;
    }

    String getText() {
      String result = CfgAction<CfgRadio>::getText();

      if (value.length()) {
        result += AP_VALUE;
        result += value;
      }
      return result;
    }

  public:
    String value;
  };

  class CfgFile: public CfgAction<CfgFile> {
  public:
    CfgFile(int _col = 0) {
      type = ACTT_FILE;
      col = _col;
    }
  };

  class CfgFolder: public CfgAction<CfgFolder> {
  public:
    CfgFolder(int _col = 0) {
      type = ACTT_DIR;
      col = _col;
    }
  };

  class CfgSeparator: public CfgAction<CfgSeparator> {
  public:
    CfgSeparator(char* _text = 0) {
      type = ACTT_SEPARATOR;
      text = _text;
    }
  };

  class CfgSep: public CfgSeparator {
  };

  /*
   * Configuration action class
   */
  template <class ChildAction>
  void CfgAction<ChildAction>::draw(int parent) {
    if (!this->checkBits() || (neededNet && !Helpers::pluginExists(neededNet))) {
      return;
    }

    IMLOG("[CfgAction<%p>::draw()]: parent = %i, id = %i, type = %i, text = %s", 
      this, parent, getID(), type, getText().a_str());
    UIActionCfgAdd(parent, getID(), type | status, getText().a_str(), col, 
      x, y, width, height);
  }

  template <class ChildAction>
  String CfgAction<ChildAction>::getText() {
    String result = text;

    if (tip.length()) {
      result += tipHtml ? AP_TIPRICH : AP_TIP;
      result += tip;
    }
    if (tipWidth) {
      result += AP_TIP_WIDTH;
      result += inttostr(tipWidth);
    }
    return result;
  }

  template <class ChildAction>
  int CfgAction<ChildAction>::getID() {
    return id ? id : col;
  }
}

#endif // __CFGACTIONS_H__