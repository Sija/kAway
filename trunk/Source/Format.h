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

#include "stdafx.h"
#include "Helpers.h"

class Format {
  public:
    typedef void (__stdcall *tModifierFunc)
      (std::string &value, std::string &prefix, std::string &suffix, 
      Stamina::RegEx pRegEx, Stamina::RegEx sRegEx);
    /*
    typedef void (__stdcall *tModifierByPrefixFunc)
      (std::string &value, std::string &prefix, Stamina::RegEx pRegEx);
    typedef void (__stdcall *tModifierBySuffixFunc)
      (std::string &value, std::string &suffix, Stamina::RegEx sRegEx);
    */

    typedef std::string (__stdcall *tFunc)(class Format *fCtrl);

    enum enOperator {
      opAnd,
      opOr
    };

    enum enType {
      typeFunction,
      typeString
    };

    struct sModifier {
      tModifierFunc function;

      std::string prefix;
      std::string suffix;
      enOperator op;

      sModifier(tModifierFunc _function, std::string _prefix, std::string _suffix, enOperator _op = opOr) : 
        function(_function), prefix(_prefix), suffix(_suffix), op(_op) { }
    };

    struct sHelpVar {
      const std::string name;
      const std::string desc;

      sHelpVar(std::string _name, std::string _desc) : name(_name), desc(_desc) { }
    };

    struct sVar {
      std::string name;
      enType type;
      std::string value;
      tFunc function;

      sVar(std::string _name, enType _type, tFunc function, std::string value) : 
        name(_name), type(_type) {
        switch(_type) {
          case typeFunction:
            this->function = function;
            break;
          case typeString:
            this->value = value;
            break;
        }
      }
    };

    typedef std::list<sModifier> tModifiers;
    typedef std::list<Format::sHelpVar> tHelpVars;
    typedef std::list<sVar> tVars;
    typedef std::map<std::string, int> tPartsPos;

  public:
    Format(std::string ldelim = "{", std::string rdelim = "}");
    ~Format();

    inline std::string operator [] (std::string &var) {
      return(this->getVar(var));
    }

  public:
    std::string parse(std::string txt);

    inline void setPattern(std::string pattern) {
      this->pattern = pattern;
    }

    std::string buildVarsList();
    std::string buildVarsList(Format::tHelpVars vars);

    void UIDrawHelpBtn(int cfgGroup, int ico = 0);
    void UIDrawHelpBtn(Format::tHelpVars vars, int cfgGroup, int ico = 0);

    inline void clearModifiers() {
      this->modifiers.clear();
    }

    inline void clearVars() {
      this->vars.clear();
    }

    int applyModifiers(std::string &value, std::string &prefix, std::string &suffix);
    bool addModifier(tModifierFunc function, std::string prefix,
      std::string suffix = "", enOperator op = opOr);
    bool removeModifier(tModifierFunc function);

    std::string getVar(std::string name);
    bool getVar(std::string name, std::string &val);

    bool addVar(std::string name, tFunc function, bool overwrite = true);
    bool addVar(std::string name, std::string value, bool overwrite = true);

    bool removeVar(std::string name);
    bool varExists(std::string name);

    tPartsPos partsPos;

  protected:
    std::string pattern;
    tModifiers modifiers;
    tVars vars;
};