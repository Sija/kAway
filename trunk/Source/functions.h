/*
 *  kAway2 [shared functions]
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

std::string itos(int i, int radix = 10) {
  char buff[64]; _itoa(i, buff, radix);
  return((std::string) buff);
}

char * btoa(bool value) {
  return((value ? "true" : "false"));
}

std::string btos(bool value) {
  return(std::string(btoa(value)));
}

namespace shared {
  std::string Icon32(int ico) {
    std::string buff = AP_IMGURL + std::string("reg://IML32/") + itos(ico) + ".ico";
    return(buff);
  }

  std::string Icon16(int ico) {
    std::string buff = "reg://IML16/" + itos(ico) + ".ico";
    return(buff);
  }
}