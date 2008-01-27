/**
  *  Status class
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

#ifndef __STATUS_H__
#define __STATUS_H__

#include "Helpers.h"
#include "NetList.h"

class Status : public SharedObject<iSharedObject> {
public:
  /**
   * Structure holding IMessage id constants
   */
  static const struct IM {
    static const unsigned int key = IM_USER + (900 * 1000);
    static const unsigned int infoCharLimit = key + 1;
  };

public:
  /*
   * Class version macro
   */
	STAMINA_OBJECT_CLASS_VERSION(Status, iSharedObject, Version(0,2,0,0));

  class Item {
  public:
    friend class Status;

    /**
     * Constructs a new Item.
     *
     * @param int net id
     * @param int status code
     * @param String status info message
     */
    Item(tNet net, tStatus status, const StringRef& info = "") : 
      _net(net), _status(status), _info(info) { }

    /**
     * Constructs an empty Item.
     */
    Item() { }

  public:
    /**
     * Returns net id
     *
     * @return int
     */
    inline tNet getNet() const {
      return _net;
    }

    /**
     * Returns status code
     *
     * @return int
     */
    inline tStatus getStatus() const {
      return _status;
    }

    /**
     * Returns status info message
     *
     * @return String
     */
    inline const StringRef& getInfo() const {
      return _info;
    }

  protected:
    tNet _net;
    tStatus _status;
    String _info;
  };

  /**
   * Structure holding status replacements for certain nets
   */
  struct sStReplacement {
    tNet net;
    tStatus before;
    tStatus after;

    sStReplacement(tNet net, tStatus before, tStatus after) : 
      net(net), before(before), after(after) { }
  };

  typedef std::deque<Item> tItems;
  typedef std::deque<sStReplacement> tStReplacements;
  typedef stdext::hash_map<tNet, tItems> tOmittedInfos;
  typedef stdext::hash_map<tNet, int> tNetInfoLimits;

public:
  Status(NetList* netList, int onHiddenCfgCol = 0, int dotsCfgCol = 0);

  // Obcina status do maks. d�ugo�ci
  String limitChars(StringRef status, int net);

  // Sprawdza czy sie� 'nadaje si� do u�ytku'
  bool isNetValid(int net);
  // Czy zmienia� przy statusie 'ukryty' ?
  bool changeOnHidden();
  // obs�uga akcji
  void actionHandle(IMEvent& ev);

  // tekstowa nazwa statusu
  String getStatusLabel(int status);
  // "..." przy obcinanym opisie
  String getDots();
  // zwraca limit znak�w opisu dla danej sieci
  int getInfoCharLimit(int net);

  // Formatuje status
  virtual String parseInfo(StringRef info, int net, int st = -1);

  // Zmienia status, txt - opis, st - id statusu
  void changeStatusInfo(const StringRef& info, int st = -1);
  void changeStatus(int st);
  // Zmienia status na wybranej sieci
  void changeStatusInfo(int net, const StringRef& info, int st = -1);
  void changeStatus(int net, int st);

  bool isRemembered(int net = 0);
  // Zapami�tuje aktualny opis na ka�dej sieci
  void rememberInfo();
  // Zapami�tuje aktualny opis na wybranej sieci
  void rememberInfo(int net);

  // Przywraca opis na ka�dej sieci
  void restoreInfo();
  // Przywraca opis na wybranej sieci
  void restoreInfo(int net);

  // Zwraca aktualny opis sieci net
  String getActualInfo(int net);
  // Zwraca opis sieci net
  String getInfo(int net);

  // Zwraca aktualny status sieci net
  tStatus getActualStatus(int net);
  // Zwraca status sieci net
  int getStatus(int net);

  tStatus applyReplacementSt(int net, int st);
  void addReplacementSt(int net, int before, int after);
  void removeReplacementSt(int net, int before);

  // lista z sieciami
  NetList* netList;

protected:
  // kolumna konfiguracji ktora odpowiada za opcje 'zmieniaj status przy statusie ukryty'
  unsigned int onHiddenCfgCol;
  unsigned int dotsCfgCol;
  bool remember;

  tOmittedInfos omittedSt;
  // zapamietane statusy i ich opisy
  tItems rememberedSt;
  // zamienniki nieobs�ugiwanych status�w
  tStReplacements stReplacements;
  // maksymalne d�ugo�ci status�w opisowych
  tNetInfoLimits infoCharLimits;
};

typedef SharedPtr<Status> oStatus;

#endif // __STATUS_H__