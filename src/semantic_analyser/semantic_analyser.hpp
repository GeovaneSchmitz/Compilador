#pragma once

#include "../binary_tree/binary_tree.hpp"
#include "log/log.hpp"
#include "semantic_analyser/return_value.hpp"
#include "semantic_analyser/symbol_table.hpp"
#include "syntactic_analyser/ast_node.hpp"
#include <list>

using binary_tree::BinaryTree;
using lexical_analyser::Token;

namespace semantic_analyser {

class SemanticAnalyser {
    public:
    SemanticAnalyser();

    ~SemanticAnalyser();

    void analyse(syntactic_analyser::ASTNode *root);
    ReturnValue generateCode(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeVardecl(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeIf(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeExpression(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeDefault(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeFunctionDef(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeIdent(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeIntLiteral(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeStringLiteral(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeParamList(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    ReturnValue generateCodeNumExpression(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code);
    const std::list<BinaryTree<Token *> *> &expression_tree_list() const;

    private:
    int temp_index;
    std::list<BinaryTree<Token *> *> expression_tree_list_;
    cmp_log::Log log;
};

} // namespace semantic_analyser