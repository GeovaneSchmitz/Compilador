#pragma once

#include "../syntactic_analyser/syntactic_analyser.hpp"
#include "../binary_tree/binary_tree.hpp"

using lexical_analyser::Token;
using binary_tree::BinaryTree;

namespace semantic_analyser {

    class SemanticAnalyser {
        public:
            SemanticAnalyser();
            ~SemanticAnalyser();

            const std::list<BinaryTree<Token*>*>& expression_tree_list() const;

        private:
            std::list<BinaryTree<Token*>*> expression_tree_list_;
    };

} // namespace semantic_analyser