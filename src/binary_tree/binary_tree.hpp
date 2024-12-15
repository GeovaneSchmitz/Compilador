#pragma once

#include <sys/types.h>

namespace binary_tree {

    template <typename T>
    class BTNode {
        public:
            BTNode();
            BTNode(T* root);
            BTNode(T& value);
            BTNode(T* root, T& value);
            BTNode(T* left, T* right, T& value);
            BTNode(T* left, T* right, T* root, T& value);
            ~BTNode();

            // Consulta usual de árvore binária
            const T* left() const;
            const T* right() const;
            const T& value() const;
            bool contains(T& value) const;

            // Operações usuais de árvore binária
            void set_left(T* left);
            void set_right(T* right);
            void set_root(T* root);
            void set_value(T& value);


        private:
            T* left_;
            T* right_;
            T* root_;
            T value_;
    };

    template <typename T>
    class BinaryTree {
        public:
            BinaryTree();
            BinaryTree(BTNode<T>* root);
            ~BinaryTree();

            // FIXME: Implementar busca em profundidade (se necessário)
            
            bool contains(T& value) const;

        private:
            BTNode<T>* root_;
            uint size_;
            uint height_;
    };

}