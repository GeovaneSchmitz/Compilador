#include "ast_node.hpp"
#include "lexical_analyser/token_type.hpp"
#include <iostream>
#include <stdexcept>

using lexical_analyser::Token;
using lexical_analyser::TokenType;

namespace syntactic_analyser {

ASTNode::ASTNode(NonTerminal nt)
    : is_terminal_{false},
      is_token_type_{false},
      terminal_{Token(TokenType::INVALID)},
      token_type_{TokenType::INVALID},
      non_terminal_{nt} {}

ASTNode::ASTNode(Token tt)
    : is_terminal_{true},
      is_token_type_{false},
      terminal_{tt},
      token_type_{TokenType::INVALID},
      non_terminal_{NonTerminal::PROGRAM} {}

ASTNode::ASTNode(TokenType tt)
    : is_terminal_{true},
      is_token_type_{true},
      terminal_{Token(TokenType::INVALID)},
      token_type_{tt},
      non_terminal_{NonTerminal::PROGRAM} {}

ASTNode::~ASTNode() {}

bool ASTNode::isTerminal() const { return is_terminal_; }

bool ASTNode::isNonTerminal() const { return !is_terminal_; }

Token ASTNode::getTerminal() const {
    if (!is_terminal_) {
        throw std::runtime_error("ASTNode is non-terminal");
    }
    if (is_token_type_) {
        throw std::runtime_error("ASTNode is token type");
    }
    return terminal_;
}

void ASTNode::setToken(Token token) {
    if (!is_terminal_) {
        throw std::runtime_error("ASTNode is non-terminal");
    }
    if (!is_token_type_) {
        throw std::runtime_error("ASTNode is non-terminal");
    }

    terminal_ = token;
    token_type_ = TokenType::INVALID;
    is_token_type_ = false;
}

void ASTNode::addChild(ASTNode *node) { children.push_back(node); }

TokenType ASTNode::getTokenType() const {
    if (!is_terminal_) {
        throw std::runtime_error("ASTNode is non-terminal");
    }
    if (!is_token_type_) {
        return this->terminal_.type();
    }
    return token_type_;
}

NonTerminal ASTNode::getNonTerminal() const {
    if (is_terminal_) {
        throw std::runtime_error("ASTNode is terminal");
    }

    return non_terminal_;
}

std::string ASTNode::toString() const {
    if (this->is_token_type_) {
        return lexical_analyser::to_string(this->token_type_);
    } else if (this->isTerminal()) {
        return lexical_analyser::to_string(this->getTerminal().type()) + std::string(":") + this->getTerminal().value();
    } else {
        return to_string(this->getNonTerminal());
    }
}

void ASTNode::printTree(int level) const {
    std::string indent(static_cast<unsigned long>(level), ' ');
    if (this->isTerminal()) {
        std::cout << indent << this->getTerminal().value() << std::endl;
    } else {
        std::cout << indent << to_string(this->getNonTerminal()) << std::endl;
        for (const auto &child : this->children) {
            child->printTree(level + 1);
        }
    }
}

std::ostream &operator<<(std::ostream &os, ASTNode const &m) {
    if (m.isTerminal()) {
        os << lexical_analyser::to_string(m.getTerminal().type()) << std::string(":") << m.getTerminal().value();
    } else {
        os << to_string(m.getNonTerminal());
    }
    return os;
}
} // namespace syntactic_analyser