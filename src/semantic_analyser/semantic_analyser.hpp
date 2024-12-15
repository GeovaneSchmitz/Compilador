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

    const std::list<BinaryTree<Token *> *> &expression_tree_list() const;

    private:
    std::list<BinaryTree<Token *> *> expression_tree_list_;
    cmp_log::Log log;
};

} // namespace semantic_analyser