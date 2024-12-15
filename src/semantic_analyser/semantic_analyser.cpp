#include "semantic_analyser.hpp"

namespace semantic_analyser {

    SemanticAnalyser::SemanticAnalyser() : expression_tree_list_() {}

    SemanticAnalyser::~SemanticAnalyser() {
        if (this->expression_tree_list().size() > 0) {
            for (auto tree : this->expression_tree_list()) {
                delete tree;
            }
        }
    }

    const std::list<BinaryTree<Token*>*>& SemanticAnalyser::expression_tree_list() const {
        return this->expression_tree_list_;
    }


} // namespace semantic_analyser
