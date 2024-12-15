#include "ast_node.hpp"
#include "semantic_analyser/scope.hpp"

namespace semantic_analyser {

ASTNode::ASTNode(Term t)
    : term{t} {}

ASTNode::~ASTNode() {}

void ASTNode::addChild(ASTNode *node) { children.push_back(node); }

Symbol *ASTNode::gen_code(std::list<const std::string> *code, Scope *scope) { return nullptr; }

Symbol *ASTNode::eval(Scope *scope) { return nullptr; }

} // namespace semantic_analyser