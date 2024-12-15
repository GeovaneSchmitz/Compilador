#pragma once

#include "lexical_analyser/token_type.hpp"
#include "non_terminal.hpp"

namespace syntactic_analyser {
/**
 *  @brief  Term class serve para fazer polimorfismo com terminais e não terminais
 */
class TermType {
    public:
    TermType(NonTerminal nt);
    TermType(lexical_analyser::TokenType tt);
    ~TermType();

    bool isTerminal() const;
    bool isNonTerminal() const;

    std::string toString() const;

    lexical_analyser::TokenType getTerminal() const;
    NonTerminal getNonTerminal() const;
    friend std::ostream &operator<<(std::ostream &os, TermType const &m);

    private:
    bool is_terminal_;
    lexical_analyser::TokenType terminal_;
    NonTerminal non_terminal_;
};

} // namespace syntactic_analyser
