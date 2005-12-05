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

namespace kAway2 {
  Format::Format() {
    //
  }

  Format::~Format() {
    this->vars.clear();
  }

  std::string Format::Parse(std::string txt) {
    /*
    cPreg *preg = new cPreg();

    preg->setPattern("/\{([^a-z0-9]*)([a-z0-9]+)([^a-z0-9]*)\}/i");
    preg->setSubject(txt);
    preg->reset();

    std::string result, buff;
    int ret;
    while ((ret = preg->match_global()) > 0) {
      if (this->GetVar(preg->subs[2], buff)) {
        result += preg->subs[1];
        result += buff.length() ? buff : preg->subs[2];
        result += preg->subs[3];
      }
    }
    preg->reset();

    delete preg; preg = NULL;
    */
    return(txt);
  }

  void Format::ClearVars() {
    this->vars.clear();
  }

  bool Format::VarExists(const char * name) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name.c_str() == name) return(true);
    }
    return(false);
  }

  void Format::AddVar(const char * name, FUNC function) {
    Control::Debug("[Format::AddVar()]: name = %s, function = ?", name);

    sVar item(name, FUNCTION, function, NULL);
    this->vars.push_back(item);
  }

  void Format::AddVar(const char * name, const char * value) {
    Control::Debug("[Format::AddVar()]: name = %s, value = %s", 
      name, (value ? value : "(none)"));

    sVar item(name, STRING, NULL, value);
    this->vars.push_back(item);
  }

  void Format::RemoveVar(const char * name) {
    for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
      if (it->name.c_str() == name) {
        it = this->vars.erase(it);
      }
    }
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
        Control::Debug("[Format::GetVar()]: name = %s, value = %s", name.c_str(), val.c_str());
        return(true);
      }
    }
    return(false);
  }
}