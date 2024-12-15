#include "semantic_analyser.hpp"
#include "semantic_analyser/return_value.hpp"
#include "semantic_analyser/symbol_table.hpp"
#include "semantic_analyser/symbol_type.hpp"

using syntactic_analyser::ASTNode;

namespace semantic_analyser {

SemanticAnalyser::SemanticAnalyser()
    : log{"log/semantic_analyser.log"} {}

SemanticAnalyser::~SemanticAnalyser() = default;

const std::list<BinaryTree<Token *> *> &SemanticAnalyser::expression_tree_list() const {
    return this->expression_tree_list_;
}

ReturnValue SemanticAnalyser::generateCode(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code) {
    if (node->isTerminal()) {
        return ReturnValue();
    }

    switch (node->getNonTerminal()) {
    case syntactic_analyser::NonTerminal::ALLOCEXPRESSION:
    case syntactic_analyser::NonTerminal::ALLOCEXPRESSION_:
    case syntactic_analyser::NonTerminal::ATRIBSTAT:
    case syntactic_analyser::NonTerminal::ATRIBSTAT_:
    case syntactic_analyser::NonTerminal::EXPRESSION:
    case syntactic_analyser::NonTerminal::EXPRESSION_:
    case syntactic_analyser::NonTerminal::EXPRESSION__:
    case syntactic_analyser::NonTerminal::FACTOR:
    case syntactic_analyser::NonTerminal::FORSTAT:
    case syntactic_analyser::NonTerminal::FUNCCALL:
    case syntactic_analyser::NonTerminal::FUNCDEF:
    case syntactic_analyser::NonTerminal::FUNCLIST:
    case syntactic_analyser::NonTerminal::FUNCLIST_:
    case syntactic_analyser::NonTerminal::IFSTAT:
    case syntactic_analyser::NonTerminal::IFSTAT_:
    case syntactic_analyser::NonTerminal::LVALUE:
    case syntactic_analyser::NonTerminal::NUMEXPR1:
    case syntactic_analyser::NonTerminal::NUMEXPRESSION:
    case syntactic_analyser::NonTerminal::NUMEXPRLIST:
    case syntactic_analyser::NonTerminal::OPTERM:
    case syntactic_analyser::NonTerminal::OPVARDECL:
    case syntactic_analyser::NonTerminal::PARAMLIST:
    case syntactic_analyser::NonTerminal::PARAMLIST_:
    case syntactic_analyser::NonTerminal::PARAMLISTCALL:
    case syntactic_analyser::NonTerminal::PARAMLISTCALL_:
    case syntactic_analyser::NonTerminal::PRINTSTAT:
    case syntactic_analyser::NonTerminal::PROGRAM:
    case syntactic_analyser::NonTerminal::READSTAT:
    case syntactic_analyser::NonTerminal::RETURNSTAT:
    case syntactic_analyser::NonTerminal::STATELIST:
    case syntactic_analyser::NonTerminal::STATELIST_:
    case syntactic_analyser::NonTerminal::STATEMENT:
    case syntactic_analyser::NonTerminal::TERM:
    case syntactic_analyser::NonTerminal::UNARYEXPR:
    case syntactic_analyser::NonTerminal::UNARYTERM:
    case syntactic_analyser::NonTerminal::VARDECL:
        break;
    }

    return ReturnValue();
}

void SemanticAnalyser::analyse(ASTNode *root) {
    SymbolTable global_table;

    std::string code = "";

    this->generateCode(root, &global_table, code);

    this->log.write("--------------------");
    this->log.write(code);
    this->log.write("--------------------");
    this->log.write("Análise semântica concluída.");

    log.write("Tabela de símbolos:");

    std::string symbol_log_msg = "\t";
    for (auto symbol : global_table) {
        symbol_log_msg += symbol.first + ": " + toString(symbol.second->type()) + " " + symbol.second->name();
        log.write(symbol_log_msg);
        symbol_log_msg.clear();
        symbol_log_msg += "\t";
    }
}

} // namespace semantic_analyser
