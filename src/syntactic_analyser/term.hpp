#pragma once

#include "lexical_analyser/token.hpp"
#include "non_terminal.hpp"

namespace syntactic_analyser {
/**
 *  @brief  Term class serve para fazer polimorfismo com terminais e não terminais
 */
class Term {
    public:
    Term(NonTerminal nt);
    Term(lexical_analyser::Token tt);
    ~Term();

    bool isTerminal() const;
    bool isNonTerminal() const;

    std::string toString() const;

    lexical_analyser::Token getTerminal() const;
    NonTerminal getNonTerminal() const;
    friend std::ostream &operator<<(std::ostream &os, Term const &m);

    private:
    bool is_terminal_;
    lexical_analyser::Token terminal_;
    NonTerminal non_terminal_;
};

} // namespace syntactic_analyser
