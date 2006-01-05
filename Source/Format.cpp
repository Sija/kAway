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

namespace kAway2 {
  std::string __stdcall fCallback(Stamina::RegEx *reg, void * param) {
    Format * fCtrl = static_cast<Format*>(param);

    // Stamina::String value, prefix, suffix;
    std::string value, prefix, suffix;
    std::string result, buff;

    if (fCtrl->getVar(reg->getSub(2), buff)) {
      if (!buff.length() && reg->getSub(3).length()) {
        buff = reg->getSub(3);
        buff = (buff.substr(0, 1) == "(") ? buff.substr(2, buff.length() - 4) : fCtrl->getVar(buff);
      }

      if (buff.length()) {
        prefix = reg->getSub(1);
        suffix = reg->getSub(4);
        value = buff;

        /*
        if (fCtrl->format && prefix.length()) {
          if (prefix.substr(0, 1) == "+") {
            value.makeUpper();
            prefix.erase(0, 1);
          } else if (prefix.substr(0, 1) == "-") {
            value.makeLower();
            prefix.erase(0, 1);
          }
        }
        */
        Control::Debug("[fCallback()]: prefix = %s, value = %s, suffix = %s",
          nullChk(prefix), value.c_str(), nullChk(suffix));

        result = prefix + value + suffix;
      }
    } else {
      result = reg->getSub(0);
    }
    return(result);
  }

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
    std::string val = this->buildVarsList();
    val += (ico) ? AP_ICO + itos(ico) : "";
    UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.c_str());
  }

  void Format::UIDrawHelpBtn(tHelpVars vars, int cfgGroup, int ico) {
    std::string val = this->buildVarsList(vars);
    val += (ico) ? AP_ICO + itos(ico) : "";
    UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.c_str());
  }

  std::string Format::buildVarsList(tHelpVars vars) {
    tHelpVars::iterator end = vars.end(); end--;
    std::string result = AP_TIPRICH;
    result += "<b>Dostêpne zmienne</b>:<br/>";

    for (tHelpVars::iterator it = vars.begin(); it != vars.end(); it++) {
      result += "  [<b>+</b>] <b>" + it->name + "</b> - ";
      result += (it->desc.length()) ? it->desc : "<span class='note'>(brak opisu)</span>";
      if (it != end) result += "<br/>\n";
    }
    result += AP_TIP_WIDTH "250";
    return(result);
  }

  std::string Format::buildVarsList() {
    tVars::iterator end = this->vars.end(); end--;
    std::string result = AP_TIPRICH;
    result += "<b>Dostêpne zmienne</b>:<br/><br/>";

    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      result += "<b>" + it->name + "</b>";
      if (it != end) result += ", ";
    }
    result += AP_TIP_WIDTH "250";
    return(result);
  }

  void Format::addVar(std::string name, tFunc function) {
    Control::Debug("[Format::addVar()]: name = %s, type = function", name.c_str());

    this->removeVar(name);
    this->vars.push_back(sVar(name, FUNCTION, function, ""));
  }

  void Format::addVar(std::string name, std::string value) {
    Control::Debug("[Format::addVar()]: name = %s, type = string, value = %s", 
      name.c_str(), nullChk(value));

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
            val = (*it->function)(this);
            break;
          case STRING:
            val = it->value;
            break;
        }
        Control::Debug("[Format::getVar()]: name = %s, value = %s",
          name.c_str(), nullChk(val));
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