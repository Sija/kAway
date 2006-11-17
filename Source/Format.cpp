/**
  *  Format class
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
  *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#include "stdafx.h"
#include "Format.h"

std::string __stdcall fCallback(RegEx* reg, void* param) {
  Format* fCtrl = static_cast<Format*>(param);

  String value, prefix, suffix;
  String result, buff;

  if (fCtrl->getVar(reg->getSub(fCtrl->partsPos["var"]), buff)) {
    if (!buff.length() && reg->getSub(fCtrl->partsPos["altVar"]).length()) {
      buff = reg->getSub(fCtrl->partsPos["altVar"]);
      buff = (buff.substr(0, 1) == "(") ? buff.substr(2, buff.length() - 4) : fCtrl->getVar(buff);
    }

    if (buff.length()) {
      prefix = reg->getSub(fCtrl->partsPos["prefixString"]);
      prefix = prefix.length() ? prefix.substr(2, prefix.length() - 4) : reg->getSub(fCtrl->partsPos["prefix"]);

      suffix = reg->getSub(fCtrl->partsPos["suffixString"]);
      suffix = suffix.length() ? suffix.substr(2, suffix.length() - 4) : reg->getSub(fCtrl->partsPos["suffix"]);

      value = buff;
      int modsApplied = fCtrl->applyModifiers(value, prefix, suffix);

      logDebug("[Format<%i>::fCallback()]: prefix = %s, value = %s, suffix = %s, modifiers applied = %i",
        fCtrl, nullChk(prefix), nullChk(value), nullChk(suffix), modsApplied);

      result = prefix + value + suffix;
    }
  } else {
    result = reg->getSub(0);
  }
  return result;
}

Format::Format(const StringRef& ldelim, const StringRef& rdelim) {
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
  this->clearModifiers();
  this->clearVars();
}

void Format::clearModifiers() {
  tModifiers mods = this->modifiers;

  for (tModifiers::iterator it = mods.begin(); it != mods.end(); it++) {
    this->removeModifier((*it)->id);
  }
}

void Format::clearVars() {
  tVars vars = this->vars;

  for (tVars::iterator it = vars.begin(); it != vars.end(); it++) {
    this->removeVar((*it)->name);
  }
}

String Format::parse(const StringRef& txt) {
  if (!this->vars.size() || !txt.length()) return txt;

  RegEx reg;

  reg.setPattern(this->pattern);
  reg.setSubject(txt);

  String result = reg.replace(fCallback, NULL, this);

  logDebug("[Format<%i>::parse()]: before = %s, after = %s",
    this, txt.c_str(), nullChk(result));

  return result;
}

void Format::UIDrawHelpBtn(int cfgGroup, int ico) {
  String val = this->buildVarsList();
  if (ico) val += AP_ICO + itos(ico);
  UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.a_str());
}

void Format::UIDrawHelpBtn(const tHelpVars& vars, int cfgGroup, int ico) {
  String val = Format::buildVarsList(vars);
  if (ico) val += AP_ICO + itos(ico);
  UIActionCfgAdd(cfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, val.a_str());
}

String Format::buildVarsList(const tHelpVars& vars) {
  tHelpVars::const_iterator end = vars.end(); end--;

  String result = AP_TIPTITLE;
  result += "Dostêpne zmienne";
  result += AP_TIPRICH;

  for (tHelpVars::const_iterator it = vars.begin(); it != vars.end(); it++) {
    result += "<b>" + it->name + "</b> - ";
    result += it->desc.length() ? it->desc : (String) "<span class='note'>(brak opisu)</span>";
    if (it != end) result += "<br/>\n";
  }
  result += AP_TIP_WIDTH "250";
  return result;
}

String Format::buildVarsList() {
  tVars::iterator end = this->vars.end(); end--;

  String result = AP_TIPTITLE;
  result += "Dostêpne zmienne";
  result += AP_TIPRICH;

  for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
    result += "<b>" + (*it)->name + "</b>";
    if (it != end) result += ", ";
  }
  result += AP_TIP_WIDTH "250";
  return result;
}

int Format::applyModifiers(StringRef& value, StringRef& prefix, StringRef& suffix) {
  int howMany = 0;

  for (tModifiers::iterator it = this->modifiers.begin(); it != this->modifiers.end(); it++) {
    sModifier* mod = *it;
    RegEx pReg, sReg;

    if (mod->prefix.length())
      pReg.match(mod->prefix.a_str(), prefix.a_str());
    if (mod->suffix.length())
      sReg.match(mod->suffix.a_str(), suffix.a_str());

    if (((mod->op == opAnd) && (pReg.isMatched() && sReg.isMatched())) ||
        ((mod->op == opOr) && (pReg.isMatched() || sReg.isMatched()))) {

      mod->callback(value, prefix, suffix, pReg, sReg);
      howMany++;
    }
  }
  return howMany;
}

bool Format::addModifier(const StringRef& id, ModifierCallback callback, const StringRef& prefix, const StringRef& suffix, enOperator op) {
  if (!prefix.length() && !suffix.length()) return false;

  logDebug("[Format<%i>::addModifier()]: id = %s, prefix = %s, suffix = %s, operator = %s",
    this, id.c_str(), nullChk(prefix), nullChk(suffix), (op == opAnd) ? "AND" : "OR");

  this->modifiers.push_back(new sModifier(id, callback, prefix, suffix, op));
  return true;
}

bool Format::removeModifier(const StringRef& id) {
  for (tModifiers::iterator it = this->modifiers.begin(); it != this->modifiers.end(); it++) {
    if ((*it)->id == id) {
      delete *it; it = this->modifiers.erase(it); return true;
    }
  }
  return false;
}

bool Format::addVarCallback(const StringRef& name, VarProcessCallback callback, bool overwrite) {
  if (overwrite) {
    this->removeVar(name);
  } else if (this->varExists(name)) {
    return false;
  }

  logDebug("[Format<%i>::addVarCallback()]: name = %s", this, name.c_str());
  this->vars.push_back(new sVar(name, callback));

  return true;
}

bool Format::addVar(const StringRef& name, const StringRef& value, bool overwrite) {
  if (overwrite) {
    this->removeVar(name);
  } else if (this->varExists(name)) {
    return false;
  }

  logDebug("[Format<%i>::addVar()]: name = %s, value = %s", this, name.c_str(), nullChk(value));
  this->vars.push_back(new sVar(name, value));

  return true;
}

String Format::getVar(const StringRef& name) {
  String result;
  this->getVar(name, result);

  return result;
}

bool Format::getVar(const StringRef& name, StringRef &val) {
  for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
    sVar* var = *it;

    if (var->name == name) {
      switch (var->type) {
        case typeCallback:
          val = var->callback(this);
          break;
        case typeString:
          val = var->value;
          break;
      }
      logDebug("[Format<%i>::getVar()]: name = %s, value = %s",
        this, name.c_str(), nullChk(val));
      return true;
    }
  }
  return false;
}

bool Format::removeVar(const StringRef& name) {
  for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
    if ((*it)->name == name) {
      delete *it; it = this->vars.erase(it); return true;
    }
  }
  return false;
}

bool Format::varExists(const StringRef& name) {
  for (tVars::iterator it = this->vars.begin(); it != this->vars.end(); it++) {
    if ((*it)->name == name) return true;
  }
  return false;
}