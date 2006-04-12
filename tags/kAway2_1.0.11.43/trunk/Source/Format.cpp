/**
 *  Format class
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
#include "Format.h"

std::string __stdcall fCallback(Stamina::RegEx *reg, void * param) {
  Format * fCtrl = static_cast<Format*>(param);

  std::string value, prefix, suffix;
  std::string result, buff;

  if (fCtrl->getVar(reg->getSub(fCtrl->partsPos["var"]), buff)) {
    if (!buff.length() && reg->getSub(fCtrl->partsPos["altVar"]).length()) {
      buff = reg->getSub(fCtrl->partsPos["altVar"]);
      buff = (buff.substr(0, 1) == "(") ? buff.substr(2, buff.length() - 4) : fCtrl->getVar(buff);
    }

    if (buff.length()) {
      prefix = reg->getSub(fCtrl->partsPos["prefixString"]);
      prefix = (prefix.length()) ? prefix.substr(2, prefix.length() - 4) : reg->getSub(fCtrl->partsPos["prefix"]);

      suffix = reg->getSub(fCtrl->partsPos["suffixString"]);
      suffix = (suffix.length()) ? suffix.substr(2, suffix.length() - 4) : reg->getSub(fCtrl->partsPos["suffix"]);

      value = buff;
      int modsApplied = fCtrl->applyModifiers(value, prefix, suffix);

      logDebug("[Format<%i>::fCallback()]: prefix = %s, value = %s, suffix = %s, modifiers applied = %i",
        fCtrl, nullChk(prefix), nullChk(value), nullChk(suffix), modsApplied);

      result = prefix + value + suffix;
    }
  } else {
    result = reg->getSub(0);
  }
  return(result);
}

Format::Format(std::string ldelim, std::string rdelim) {
  this->pattern = 
    "/\\" + ldelim + "([^a-z0-9]*|(\\((?:\"[^\"]+\"|'[^']+')\\)))"
    "([a-z0-9]+)(?:\\|([a-z0-9]+|(?:\\((?:\"[^\"]+\"|'[^']+')\\))))?"
    "([^a-z0-9]*|(\\((?:\"[^\"]+\"|'[^']+')\\)))\\" + rdelim + "/i";

  this->partsPos["var"] = 3;
  this->partsPos["altVar"] = 4;
  this->partsPos["prefix"] = 1;
  this->partsPos["prefixString"] = 2;
  this->partsPos["suffix"] = 5;
  this->partsPos["suffixString"] = 6;
}

Format::~Format() {
  this->vars.clear();
}

std::string Format::parse(std::string txt) {
  if (!this->vars.size() || !txt.length()) return(txt);

  Stamina::RegEx reg;

  reg.setPattern(this->pattern);
  reg.setSubject(txt);

  std::string result = reg.replace(fCallback, NULL, this);

  logDebug("[Format<%i>::parse()]: before = %s, after = %s",
    this, txt.c_str(), nullChk(result));

  return(result);
}

void Format::UIDrawHelpBtn(int cfgGroup, int ico) {
  std::string val = this->buildVarsList();
  if (ico) val += AP_ICO + itos(ico);
  UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.c_str());
}

void Format::UIDrawHelpBtn(tHelpVars vars, int cfgGroup, int ico) {
  std::string val = this->buildVarsList(vars);
  if (ico) val += AP_ICO + itos(ico);
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

int Format::applyModifiers(std::string &value, std::string &prefix, std::string &suffix) {
  int howMany = 0;

  for (tModifiers::iterator it = this->modifiers.begin(); it != this->modifiers.end(); it++) {
    Stamina::RegEx pReg, sReg;

    if (it->prefix.length())
      pReg.match(it->prefix.c_str(), prefix.c_str());
    if (it->suffix.length())
      sReg.match(it->suffix.c_str(), suffix.c_str());

    if (((it->op == opAnd) && (pReg.isMatched() && sReg.isMatched())) ||
        ((it->op == opOr) && (pReg.isMatched() || sReg.isMatched()))) {

      (*it->function)(value, prefix, suffix, pReg, sReg);
      howMany++;
    }
  }
  return(howMany);
}

bool Format::addModifier(tModifierFunc function, std::string prefix, std::string suffix, enOperator op) {
  if (!prefix.length() && !suffix.length())
    return(false);

  logDebug("[Format<%i>::addModifier()]: prefix = %s, suffix = %s, operator = %s",
    this, nullChk(prefix), nullChk(suffix), (op == opAnd) ? "AND" : "OR");

  this->modifiers.push_back(sModifier(function, prefix, suffix, op));
  return(true);
}

bool Format::removeModifier(tModifierFunc function) {
  for (tModifiers::iterator it = this->modifiers.begin(); it != this->modifiers.end(); it++) {
    if (it->function == function) {
      it = this->modifiers.erase(it); return(true);
    }
  }
  return(false);
}

bool Format::addVar(std::string name, tFunc function, bool overwrite) {
  if (overwrite)
    this->removeVar(name);
  else if (this->varExists(name))
    return(false);

  logDebug("[Format<%i>::addVar()]: name = %s, type = function", 
    this, name.c_str());

  this->vars.push_back(sVar(name, typeFunction, function, ""));
  return(true);
}

bool Format::addVar(std::string name, std::string value, bool overwrite) {
  if (overwrite)
    this->removeVar(name);
  else if (this->varExists(name))
    return(false);

  logDebug("[Format<%i>::addVar()]: name = %s, type = string, value = %s", 
    this, name.c_str(), nullChk(value));

  this->vars.push_back(sVar(name, typeString, NULL, value));
  return(true);
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
        case typeFunction:
          val = (*it->function)(this);
          break;
        case typeString:
          val = it->value;
          break;
      }
      logDebug("[Format<%i>::getVar()]: name = %s, value = %s",
        this, name.c_str(), nullChk(val));
      return(true);
    }
  }
  return(false);
}

bool Format::removeVar(std::string name) {
  for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
    if (it->name == name) {
      it = this->vars.erase(it); return(true);
    }
  }
  return(false);
}

bool Format::varExists(std::string name) {
  for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
    if (it->name == name) return(true);
  }
  return(false);
}