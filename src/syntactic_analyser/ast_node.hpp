#pragma once
#include "lexical_analyser/token.hpp"
#include "syntactic_analyser/non_terminal.hpp"
#include <string>
#include <vector>

namespace syntactic_analyser {

class ASTNode {
    public:
    ASTNode(NonTerminal nt);
    ASTNode(lexical_analyser::Token tt);
    ASTNode(lexical_analyser::TokenType tt);

    virtual ~ASTNode();

    virtual void addChild(ASTNode *);

    virtual std::string toString() const;

    bool isTerminal() const;
    bool isNonTerminal() const;
    void setToken(lexical_analyser::Token token);

    lexical_analyser::Token getTerminal() const;
    lexical_analyser::TokenType getTokenType() const;

    void printTree(int level) const;

    NonTerminal getNonTerminal() const;
    friend std::ostream &operator<<(std::ostream &os, ASTNode const &m);

    private:
    bool is_terminal_;
    bool is_token_type_;

    lexical_analyser::Token terminal_;
    lexical_analyser::TokenType token_type_;

    NonTerminal non_terminal_;

    std::vector<ASTNode *> children;
};
} // namespace syntactic_analyser