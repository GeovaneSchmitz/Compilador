#pragma once

#include "semantic_analyser/return_value.hpp"
#include "syntactic_analyser/ast_node.hpp"

namespace semantic_analyser {
/**
 *  @brief  Term class serve para fazer polimorfismo com terminais e não terminais
 */
class ASTNodeAndResultValue {
    public:
    ASTNodeAndResultValue(syntactic_analyser::ASTNode *nt);
    ASTNodeAndResultValue(ReturnValue *rv);
    ~ASTNodeAndResultValue();

    bool isResultValue() const;
    bool isASTNode() const;

    ReturnValue *getResultValue() const;
    syntactic_analyser::ASTNode *getASTNode() const;

    private:
    bool is_result_value_;
    ReturnValue *result_value_;
    syntactic_analyser::ASTNode *ast_node_;
};

} // namespace semantic_analyser
