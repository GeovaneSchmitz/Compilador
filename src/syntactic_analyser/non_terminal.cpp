#include "non_terminal.hpp"
#include <string>

namespace syntactic_analyser {

std::string to_string(NonTerminal nt) {
    switch (nt) {
    case NonTerminal::ALLOCEXPRESSION:
        return "ALLOCEXPRESSION";
    case NonTerminal::ALLOCEXPRESSION_:
        return "ALLOCEXPRESSION_";
    case NonTerminal::ATRIBSTAT:
        return "ATRIBSTAT";
    case NonTerminal::ATRIBSTAT_:
        return "ATRIBSTAT_";
    case NonTerminal::EXPRESSION:
        return "EXPRESSION";
    case NonTerminal::EXPRESSION_:
        return "EXPRESSION_";
    case NonTerminal::EXPRESSION__:
        return "EXPRESSION__";
    case NonTerminal::FACTOR:
        return "FACTOR";
    case NonTerminal::FORSTAT:
        return "FORSTAT";
    case NonTerminal::FUNCCALL:
        return "FUNCCALL";
    case NonTerminal::FUNCDEF:
        return "FUNCDEF";
    case NonTerminal::FUNCLIST:
        return "FUNCLIST";
    case NonTerminal::FUNCLIST_:
        return "FUNCLIST_";
    case NonTerminal::IFSTAT:
        return "IFSTAT";
    case NonTerminal::IFSTAT_:
        return "IFSTAT_";
    case NonTerminal::LVALUE:
        return "LVALUE";
    case NonTerminal::NUMEXPR1:
        return "NUMEXPR1";
    case NonTerminal::NUMEXPRESSION:
        return "NUMEXPRESSION";
    case NonTerminal::NUMEXPRLIST:
        return "NUMEXPRLIST";
    case NonTerminal::OPTERM:
        return "OPTERM";
    case NonTerminal::OPVARDECL:
        return "OPVARDECL";
    case NonTerminal::PARAMLIST:
        return "PARAMLIST";
    case NonTerminal::PARAMLIST_:
        return "PARAMLIST_";
    case NonTerminal::PARAMLISTCALL:
        return "PARAMLISTCALL";
    case NonTerminal::PARAMLISTCALL_:
        return "PARAMLISTCALL_";
    case NonTerminal::PRINTSTAT:
        return "PRINTSTAT";
    case NonTerminal::PROGRAM:
        return "PROGRAM";
    case NonTerminal::READSTAT:
        return "READSTAT";
    case NonTerminal::RETURNSTAT:
        return "RETURNSTAT";
    case NonTerminal::STATELIST:
        return "STATELIST";
    case NonTerminal::STATELIST_:
        return "STATELIST_";
    case NonTerminal::STATEMENT:
        return "STATEMENT";
    case NonTerminal::TERM:
        return "TERM";
    case NonTerminal::UNARYEXPR:
        return "UNARYEXPR";
    case NonTerminal::UNARYTERM:
        return "UNARYTERM";
    case NonTerminal::VARDECL:
        return "VARDECL";
    }
    return "UNKNOWN";
}

} // namespace syntactic_analyser
