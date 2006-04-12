/**
 *  MRU class
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @link         svn://kplugins.net/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once
#include "MRU.h"

MRU::MRU(std::string name, int count, bool dtbCount) {
  this->dtbCount = dtbCount;
  this->count = count;
  this->name = name;
}

MRU::~MRU() {
  //
}

tMRUlist MRU::get(bool rev, const char * buff, int buffSize) {
  tMRUlist list;
  sMRU mruList;

  mruList.name = this->name.c_str();
  mruList.count = this->getCount();
  mruList.flags = MRU_SET_LOADFIRST;

  if (buff) {
    mruList.buffer = (char*) buff;
    mruList.flags |= MRU_GET_ONEBUFF;
    mruList.buffSize = buffSize;
  } else {
    mruList.flags |= MRU_GET_USETEMP;
    mruList.buffSize = 1024;
  }

  sIMessage_MRU mru(IMC_MRU_GET, &mruList);
  Ctrl->IMessage(&mru);

  for (int i = 0; i < mru.MRU->count; i++) {
    if (!mru.MRU->values[i]) break;
    (rev) ?
      list.push_back((char*)mru.MRU->values[i]) :
      list.push_front((char*)mru.MRU->values[i]);
  }
  return(list);
}

void MRU::append(std::string current) {
  tMRUlist list;
  list.push_back(current);
  this->append(list);
}

void MRU::append(tMRUlist list) {
  int count = this->getCount();
  for (tMRUlist::iterator it = list.begin(); it != list.end(); it++) {
    sMRU mru;

    mru.name = this->name.c_str();
    mru.flags = MRU_SET_LOADFIRST | MRU_GET_USETEMP;
    mru.current = (*it).c_str();
    mru.count = count;

    Ctrl->IMessage(&sIMessage_MRU(IMC_MRU_SET, &mru));
  }
}