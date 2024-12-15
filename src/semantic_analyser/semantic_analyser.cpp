#include "semantic_analyser.hpp"

namespace semantic_analyser {

    SemanticAnalyser::SemanticAnalyser() : expression_tree_list_() {}

    SemanticAnalyser::~SemanticAnalyser() = default;

    const std::list<BinaryTree<Token*>*>& SemanticAnalyser::expression_tree_list() const {
        return this->expression_tree_list_;
    }


} // namespace semantic_analyser
