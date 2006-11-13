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

#pragma once

#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <stamina/exception.h>

#include "Helpers.h"

class FormatException : public ExceptionString { 
public:
	STAMINA_OBJECT_CLASS(FormatException, ExceptionString);
  FormatException(const StringRef& reason) : ExceptionString(reason) { }
};

class Format : public SharedObject<iSharedObject> {
public:
  /*
   * Class version
   */
	STAMINA_OBJECT_CLASS_VERSION(Format, iSharedObject, Version(0,1,0,0));

  typedef function<void(StringRef& value, StringRef& prefix, StringRef& suffix, 
    RegEx& pRegEx, RegEx& sRegEx)> ModifierCallback;
  typedef signal<void(StringRef& value, StringRef& prefix, StringRef& suffix, 
    RegEx& pRegEx, RegEx& sRegEx)> ModifierSig;

  typedef function<String(Format* fCtrl)> VarProcessCallback;
  typedef signal<String(Format* fCtrl)> VarProcessSig;

  enum enOperator {
    opAnd,
    opOr
  };

  enum enType {
    typeCallback,
    typeString
  };

  struct sModifier {
    ModifierSig callback;
    enOperator op;

    String id;
    String prefix;
    String suffix;

    sModifier(const StringRef& _id, ModifierCallback f, const StringRef& _prefix, const StringRef& _suffix, enOperator _op) : 
      id(_id), prefix(_prefix), suffix(_suffix), op(_op) { 

      if (f.empty()) {
        throw new FormatException("Error! Callback function passed for Format variable modifier '" + _id + "' is empty.");
      } else {
        callback.connect(f);
      }
    }
  };

  struct sHelpVar {
    String name;
    String desc;

    sHelpVar(const StringRef& _name, const StringRef& _desc) : name(_name), desc(_desc) { }
  };

  struct sVar {
    VarProcessSig callback;
    enType type;

    String name;
    String value;

    sVar(const StringRef& _name, const StringRef& _value) : name(_name), value(_value), type(typeString) { }
    sVar(const StringRef& _name, VarProcessCallback f) : name(_name), type(typeCallback) { 
      if (f.empty()) {
        throw new FormatException("Error! Callback function passed for Format variable '" + _name + "' is empty.");
      } else {
        callback.connect(f);
      }
    }
  };

  typedef std::list<sModifier*> tModifiers;
  typedef std::list<Format::sHelpVar> tHelpVars;
  typedef std::list<sVar*> tVars;
  typedef std::map<std::string, int> tPartsPos;

public:
  Format(const StringRef& ldelim = "{", const StringRef& rdelim = "}");
  ~Format();

  inline String operator [] (const StringRef& var) {
    return this->getVar(var);
  }

public:
  String parse(const StringRef& txt);

  inline void setPattern(const StringRef& pattern) {
    this->pattern = pattern;
  }

  String buildVarsList();
  String buildVarsList(const tHelpVars& vars);

  void UIDrawHelpBtn(int cfgGroup, int ico = 0);
  void UIDrawHelpBtn(const tHelpVars& vars, int cfgGroup, int ico = 0);

  void clearModifiers();
  void clearVars();

  int applyModifiers(StringRef& value, StringRef& prefix, StringRef& suffix);
  bool addModifier(const StringRef& id, ModifierCallback callback, const StringRef& prefix,
    const StringRef& suffix = "", enOperator op = opOr);
  bool removeModifier(const StringRef& id);

  String getVar(const StringRef& name);
  bool getVar(const StringRef& name, StringRef& val);

  bool addVarCallback(const StringRef& name, VarProcessCallback callback, bool overwrite = true);
  bool addVar(const StringRef& name, const StringRef& value, bool overwrite = true);

  bool removeVar(const StringRef& name);
  bool varExists(const StringRef& name);

  tPartsPos partsPos;

protected:
  String pattern;
  tModifiers modifiers;
  tVars vars;
};

typedef SharedPtr<Format> oFormat;

#endif // __FORMAT_H__