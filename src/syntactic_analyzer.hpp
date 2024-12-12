#ifndef __XPP_SYNTACTIC_ANALYSER__
#define __XPP_SYNTACTIC_ANALYSER__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

class Grammar {
public:
    std::vector<std::string> terminals = {"def", "ident", "(", ")", "{", "}", "int", "float", "string", ",", 
    ";", "[", "]", "=", "print","read", "return", "if", "else", "for", "new", "int_constant",
     "float_constant", "string_constant", "null", "break", "<", ">", "+", "-", "*", "/", "%"};

    std::vector<std::string> non_terminals = {"PROGRAM", "FUNCLIST", "FUNCLIST'", "FUNCDEF", "PARAMLIST", "PARAMLIST'", "STATEMENT", "VARDECL", 
    "OPVARDECL", "ATRIBSTAT", "ATRIBSTAT'", "FUNCCALL", "PARAMLISTCALL", "PARAMLISTCALL'", "PRINTSTAT", "READSTAT", "RETURNSTAT", 
    "IFSTAT", "IFSTAT'", "FORSTAT", "STATELIST", "STATELIST'", "ALLOCEXPRESSION", "ALLOCEXPRESSION'", "NUMEXPR1", 
    "NUMEXPRLIST", "EXPRESSION", "EXPRESSION'", "EXPRESSION''", "NUMEXPRESSION", "OPTERM", "TERM", "UNARYTERM", "UNARYEXPR", "FACTOR", "LVALUE"};

    std::map<std::string, std::vector<std::string>> productions;

    Grammar() = default;

    void add_production(const std::string& head, const std::string& body);
};

class SyntacticAnalyzer {
public:
    Grammar grammar;
    std::vector<std::string> stack;
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> table;

    SyntacticAnalyzer(const Grammar& grammar);

    bool analyze(const std::string& entrada);
};

#endif 