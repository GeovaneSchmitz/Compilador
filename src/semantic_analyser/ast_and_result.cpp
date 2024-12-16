#include "ast_and_result.hpp"

namespace semantic_analyser {
ASTNodeAndResultValue::ASTNodeAndResultValue(syntactic_analyser::ASTNode *nt)
    : is_result_value_{false},
      result_value_{nullptr},
      ast_node_{nt} {}
ASTNodeAndResultValue::ASTNodeAndResultValue(ReturnValue *rv)
    : is_result_value_{true},
      result_value_{rv},
      ast_node_{nullptr} {}
ASTNodeAndResultValue::~ASTNodeAndResultValue() {}

bool ASTNodeAndResultValue::isResultValue() const { return is_result_value_; }
bool ASTNodeAndResultValue::isASTNode() const { return !is_result_value_; }

ReturnValue *ASTNodeAndResultValue::getResultValue() const { return result_value_; }
syntactic_analyser::ASTNode *ASTNodeAndResultValue::getASTNode() const { return ast_node_; }

} // namespace semantic_analyser
