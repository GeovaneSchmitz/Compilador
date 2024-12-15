#include "binary_tree.hpp"

namespace binary_tree {

    template <typename T>
    BTNode<T>::BTNode() : left_(nullptr), right_(nullptr), root_(nullptr), value_() {}

    template <typename T>
    BTNode<T>::BTNode(T* root) : left_(nullptr), right_(nullptr), root_(root), value_() {}

    template <typename T>
    BTNode<T>::BTNode(T& value) : left_(nullptr), right_(nullptr), root_(nullptr), value_(value) {}

    template <typename T>
    BTNode<T>::BTNode(T* root, T& value) : left_(nullptr), right_(nullptr), root_(root), value_(value) {}

    template <typename T>
    BTNode<T>::BTNode(T* left, T* right, T& value) : left_(left), right_(right), root_(nullptr), value_(value) {}

    template <typename T>
    BTNode<T>::BTNode(T* left, T* right, T* root, T& value) : 
        left_(left), right_(right), root_(root), value_(value) {}

    template <typename T>
    BTNode<T>::~BTNode() = default;

    template <typename T>
    const T* BTNode<T>::left() const { return this->left_; }
    
    template <typename T>
    const T* BTNode<T>::right() const { return this->right_; }

    template <typename T>
    const T& BTNode<T>::value() const { return this->value_; };

    template <typename T>
    bool BTNode<T>::contains(T& value) const {
        if (this->value_ == value) { return true; }

        if (this->left() != nullptr) { 
            if (this->left()->contains(value)) { return true; } 
        }

        if (this->right() != nullptr) {
            if (this->right()->contains(value))  { return true; }
        }

        return false;
    }

    template <typename T>
    void BTNode<T>::set_left(T* left) { this->left_ = left; };

    template <typename T>
    void BTNode<T>::set_right(T* right) { this->right_ = right; };

    template <typename T>
    void BTNode<T>::set_root(T* root) { this->root_ = root; };

    template <typename T>
    void BTNode<T>::set_value(T& value) { this->value_ = value; };

    // ***************
    // * BINARY TREE *
    // ***************

    template <typename T>
    BinaryTree<T>::BinaryTree() : root_(nullptr), size_(0), height_(0) {}

    template <typename T>
    BinaryTree<T>::BinaryTree(BTNode<T>* root) : root_(root), size_(1), height_(1) {}
    
    template <typename T>
    BinaryTree<T>::~BinaryTree() = default;

    // FIXME: Implementar busca em profundidade (se necessário)
    
    template <typename T>
    bool BinaryTree<T>::contains(T& value) const {
        if (this->root_ == nullptr) { 
            return this->root_->contains(value); 
        }
    }


}