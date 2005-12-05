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
  typedef std::string (*FUNC)();

  enum TYPE {
    FUNCTION,
    STRING
  };

  struct sVar {
    std::string name; // nazwa zmiennej
    TYPE type; // typ zmiennej
    std::string value; // wartosc zmiennej (TYPE == STRING)
    FUNC function; // funkcja, która zwraca string

    // defaultowy konstruktor, ¿eby nie trzeba by³o siê bawiæ w deklarowanie zmiennych
    sVar(std::string name, TYPE type, FUNC function, std::string value) {
      this->name = name;
      this->type = type;

      switch(type) {
        case FUNCTION:
          this->function = function;
          break;
        case STRING:
          this->value = value;
          break;
      }
    }
  };

  typedef std::list<sVar> tVars;

  class Format {
    public:
      Format(std::string pattern = "/\{([^a-z0-9]*)([a-z0-9]+)([^a-z0-9]*)\}/i");
      ~Format();

    public:
      std::string Parse(std::string txt);
      void ClearVars();

      std::string GetVar(std::string name);
      bool GetVar(std::string name, std::string &val);

      void AddVar(std::string name, FUNC function);
      void AddVar(std::string name, std::string value);

      void RemoveVar(std::string name);
      bool VarExists(std::string name);

    protected:
      std::string pattern;
      tVars vars;
  };
}