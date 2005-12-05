/*
 *  Format class
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
  std::string result, buff;

  if (fCtrl->GetVar(reg->getSub(2), buff)) {
    if (buff.length()) {
      result += reg->getSub(1);
      result += buff;
      result += reg->getSub(3);
    }
  } else {
    result = reg->getSub(0);
  }
  return(result);
}

namespace kAway2 {
  Format::Format(std::string pattern) {
    this->pattern = pattern;
  }

  Format::~Format() {
    this->vars.clear();
  }

  std::string Format::Parse(std::string txt) {
    Stamina::RegEx *reg = new Stamina::RegEx;

    reg->setLocale("pl_PL");
    reg->setPattern(this->pattern);
    reg->setSubject(txt);

    std::string result = reg->replace(fCallback, NULL, this);

    Control::Debug("[Format::Parse()]: before = %s, after = %s",
      txt.c_str(), result.c_str());

    delete reg; reg = NULL;
    return(result);
  }

  void Format::ClearVars() {
    this->vars.clear();
  }

  bool Format::VarExists(std::string name) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name == name) return(true);
    }
    return(false);
  }

  void Format::AddVar(std::string name, FUNC function) {
    Control::Debug("[Format::AddVar()]: name = %s, function = ?", name.c_str());

    sVar item(name, FUNCTION, function, NULL);
    this->vars.push_back(item);
  }

  void Format::AddVar(std::string name, std::string value) {
    Control::Debug("[Format::AddVar()]: name = %s, value = %s", 
      name.c_str(), (value.length() ? value.c_str() : "(none)"));

    sVar item(name, STRING, NULL, value);
    this->vars.push_back(item);
  }

  void Format::RemoveVar(std::string name) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name == name) {
        it = this->vars.erase(it);
        break;
      }
    }
  }

  std::string Format::GetVar(std::string name) {
    std::string result;
    this->GetVar(name, result);
    return(result);
  }

  bool Format::GetVar(std::string name, std::string &val) {
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
        Control::Debug("[Format::GetVar()]: name = %s, value = %s",
          name.c_str(), (val.length() ? val.c_str() : "(none)"));
        return(true);
      }
    }
    return(false);
  }
}