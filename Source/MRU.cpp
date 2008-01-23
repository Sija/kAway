/**
  *  MRU class
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

#include "stdafx.h"
#include "MRU.h"

MRU::tItems MRU::get(bool rev, const char * buff, int buffSize) {
  tItems list;
  sMRU mruList;

  mruList.name = _name.c_str();
  mruList.count = getCount();
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
      list.push_back((String) mru.MRU->values[i]) :
      list.push_front((String) mru.MRU->values[i]);
  }
  return list;
}

void MRU::append(const StringRef& current) {
  tItems list;
  list.push_back(current);

  append(list);
}

void MRU::append(const tItems& list) {
  int count = getCount();
  for (tItems::const_iterator it = list.begin(); it != list.end(); it++) {
    sMRU mru;

    mru.name = _name.c_str();
    mru.flags = MRU_SET_LOADFIRST | MRU_GET_USETEMP;
    mru.current = it->a_str();
    mru.count = count;

    Ctrl->IMessage(&sIMessage_MRU(IMC_MRU_SET, &mru));
  }
}

void MRU::clear() {
  sMRU mru;

  mru.name = _name.c_str();
  mru.count = 0;

  Ctrl->IMessage(&sIMessage_MRU(IMC_MRU_SET, &mru));
}

void MRU::set(const StringRef& current) {
  clear();
  append(current);
}

void MRU::set(const tItems& list) {
  clear();
  append(list);
}