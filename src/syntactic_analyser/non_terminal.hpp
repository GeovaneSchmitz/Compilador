#pragma once
#include <string>

namespace syntactic_analyser {

enum class NonTerminal {
    ALLOCEXPRESSION,
    ALLOCEXPRESSION_,
    ATRIBSTAT,
    ATRIBSTAT_,
    EXPRESSION,
    EXPRESSION_,
    EXPRESSION__,
    FACTOR,
    FORSTAT,
    FUNCCALL,
    FUNCDEF,
    FUNCLIST,
    FUNCLIST_,
    IFSTAT,
    IFSTAT_,
    LVALUE,
    NUMEXPR1,
    NUMEXPRESSION,
    NUMEXPRLIST,
    OPTERM,
    OPVARDECL,
    PARAMLIST,
    PARAMLIST_,
    PARAMLISTCALL,
    PARAMLISTCALL_,
    PRINTSTAT,
    PROGRAM,
    READSTAT,
    RETURNSTAT,
    STATELIST,
    STATELIST_,
    STATEMENT,
    TERM,
    UNARYEXPR,
    UNARYTERM,
    VARDECL,
};

std::string to_string(NonTerminal nt);

} // namespace syntactic_analyser
