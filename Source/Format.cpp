/*
 *  kAway2 Format class
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2005 Winthux
 *
 *  $Id$
 */

#pragma once

std::string __stdcall fCallback(Stamina::RegEx *reg, void * param) {
  kAway2::Format * fCtrl = static_cast<kAway2::Format*>(param);

  std::string result, buff, prefix, suffix;
  Stamina::String value;

  if (fCtrl->getVar(reg->getSub(2), buff)) {
    if (buff.length()) {
      prefix = reg->getSub(1);
      suffix = reg->getSub(3);
      value = buff;

      if (fCtrl->format && prefix.length()) {
        if (prefix.substr(0, 1) == "+") {
          value = value.toUpper();
          prefix = prefix.substr(1, prefix.length());
        } else if (prefix.substr(0, 1) == "-") {
          value = value.toLower();
          prefix = prefix.substr(1, prefix.length());
        }
      }

      result += prefix;
      result += value;
      result += suffix;
    }
  } else {
    result = reg->getSub(0);
  }
  return(result);
}

namespace kAway2 {
  Format::Format(bool format, std::string pattern) {
    this->format = format;
    this->pattern = pattern;
  }

  Format::~Format() {
    this->vars.clear();
  }

  std::string Format::parse(std::string txt) {
    if (!this->vars.size()) return(txt);

    Stamina::RegEx *reg = new Stamina::RegEx;

    reg->setLocale("pl_PL");
    reg->setPattern(this->pattern);
    reg->setSubject(txt);

    std::string result = reg->replace(fCallback, NULL, this);

    Control::Debug("[Format::parse()]: before = %s, after = %s",
      txt.c_str(), result.c_str());

    delete reg; reg = NULL;
    return(result);
  }

  void Format::UIDrawHelpBtn(int cfgGroup, int ico) {
    std::string val = this->buildHtmlHelp();
    val += (ico) ? AP_ICO + itos(ico) : "";
    UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.c_str());
  }

  void Format::UIDrawHelpBtn(tHelpVars vars, int cfgGroup, int ico) {
    std::string val = this->buildHtmlHelp(vars);
    val += (ico) ? AP_ICO + itos(ico) : "";
    UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.c_str());
  }

  std::string Format::buildHtmlHelp(tHelpVars vars) {
    tHelpVars::iterator end = vars.end(); end--;
    std::string result = AP_TIPRICH;

    if (this->format) {
      result += "Je¿eli dodasz '+' przed prefixem, wynik zostanie zwrócony du¿ymi literami, a jeœli '-' - ma³ymi<br/><br/>";
    }

    for (tHelpVars::iterator it = vars.begin(); it != vars.end(); it++) {
      result += "<b>" + it->name + "</b> - ";
      result += (it->desc.length()) ? it->desc : "<span class='note'>(brak opisu)</span>";
      if (it != end) result += "<br/>\n";
    }
    result += AP_TIP_WIDTH "240";
    return(result);
  }

  std::string Format::buildHtmlHelp() {
    tVars::iterator end = this->vars.end(); end--;
    std::string result = AP_TIPRICH;

    if (this->format) {
      result += "Je¿eli dodasz '+' przed prefixem, wynik zostanie zwrócony du¿ymi literami, a jeœli '-' - ma³ymi<br/><br/>";
    }

    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      result += "<b>" + it->name + "</b>";
      if (it != end) result += ", ";
    }
    result += AP_TIP_WIDTH "240";
    return(result);
  }

  void Format::addVar(std::string name, tFunc function) {
    Control::Debug("[Format::addVar()]: name = %s, type = function", name.c_str());

    this->removeVar(name);
    this->vars.push_back(sVar(name, FUNCTION, function, NULL));
  }

  void Format::addVar(std::string name, std::string value) {
    Control::Debug("[Format::addVar()]: name = %s, value = %s", 
      name.c_str(), (value.length() ? value.c_str() : "(none)"));

    this->removeVar(name);
    this->vars.push_back(sVar(name, STRING, NULL, value));
  }

  std::string Format::getVar(std::string name) {
    std::string result;

    this->getVar(name, result);
    return(result);
  }

  bool Format::getVar(std::string name, std::string &val) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name == name) {
        switch (it->type) {
          case FUNCTION:
            val = (*it->function)();
            break;
          case STRING:
            val = it->value;
            break;
        }
        Control::Debug("[Format::getVar()]: name = %s, value = %s",
          name.c_str(), (val.length() ? val.c_str() : "(none)"));
        return(true);
      }
    }
    return(false);
  }

  void Format::removeVar(std::string name) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name == name) {
        it = this->vars.erase(it); break;
      }
    }
  }

  bool Format::varExists(std::string name) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name == name) return(true);
    }
    return(false);
  }
}