/*
 *  kAway2 [shared functions]
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: $
 */

#pragma once

namespace shared {
  std::string intToStr(int i, int radix = 10) {
    char buff[64]; _itoa(i, buff, radix);
    return((std::string)buff);
  }

  std::string Icon32(int ico) {
    std::string buff = AP_IMGURL + std::string("reg://IML32/") + shared::intToStr(ico) + ".ICON";
    return(buff);
  }

  std::string Icon16(int ico) {
    std::string buff = "reg://IML16/" + shared::intToStr(ico) + ".ICON";
    return(buff);
  }
}