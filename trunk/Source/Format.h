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
    // chcia�em tutaj wrzucic union ale p�niej co� si� burczy
    // o niezgodnosc typow przy podstawianiu
    FUNC function; // funkcja, kt�ra zwraca string
    std::string value; // lub mo�e by� to jaki� tekst

    // defaultowy konstruktor, �eby nie trzeba by�o si� bawi� w deklarowanie zmiennych
    sVar(const char * name = NULL, TYPE type = STRING, FUNC function = NULL, const char * value = NULL) {
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
      Format();
      ~Format();

    public:
      std::string Parse(std::string txt);

      bool GetVar(std::string name, std::string &val);
      void AddVar(const char * name, FUNC function);
      void AddVar(const char * name, const char * value);
      void RemoveVar(const char * name);
      bool VarExists(const char * name);
      void ClearVars();

    protected:
      tVars vars;
  };
}