
#include "grammar.hpp"

#include <map>
#include <string>
#include <vector>

namespace syntactic_analyser {

Grammar::Grammar()
    : terminals{{"def",
                 "ident",
                 "(",
                 ")",
                 "{",
                 "}",
                 "int",
                 "float",
                 "string",
                 ",",
                 ";",
                 "[",
                 "]",
                 "=",
                 "print",
                 "read",
                 "return",
                 "if",
                 "else",
                 "for",
                 "new",
                 "int_constant",
                 "float_constant",
                 "string_constant",
                 "null",
                 "break",
                 "<",
                 ">",
                 "+",
                 "-",
                 "*",
                 "/",
                 "%"}},
      non_terminals{
          {"PROGRAM",       "FUNCLIST",       "FUNCLIST'",  "FUNCDEF",     "PARAMLIST",       "PARAMLIST'",
           "STATEMENT",     "VARDECL",        "OPVARDECL",  "ATRIBSTAT",   "ATRIBSTAT'",      "FUNCCALL",
           "PARAMLISTCALL", "PARAMLISTCALL'", "PRINTSTAT",  "READSTAT",    "RETURNSTAT",      "IFSTAT",
           "IFSTAT'",       "FORSTAT",        "STATELIST",  "STATELIST'",  "ALLOCEXPRESSION", "ALLOCEXPRESSION'",
           "NUMEXPR1",      "NUMEXPRLIST",    "EXPRESSION", "EXPRESSION'", "EXPRESSION''",    "NUMEXPRESSION",
           "OPTERM",        "TERM",           "UNARYTERM",  "UNARYEXPR",   "FACTOR",          "LVALUE"}},
      productions() {}

Grammar::~Grammar() {}

void Grammar::add_production(const std::string &head, const std::string &body) { productions[head].push_back(body); }

} // namespace syntactic_analyser
