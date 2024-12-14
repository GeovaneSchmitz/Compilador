#pragma once
#include "../lexical_analyser/lexical_analyser.hpp"
#include "../lexical_analyser/token_type.hpp"
#include "../log/log.hpp"
#include "non_terminal.hpp"
#include "term.hpp"
#include <vector>
#include <map>
#include <string>

namespace syntactic_analyser {

class SyntacticAnalyser {
    public:
    SyntacticAnalyser();

    bool analyse(lexical_analyser::LexicalAnalyser &lex);

    private:
    cmp_log::Log log_;
    std::map<std::pair<NonTerminal, lexical_analyser::TokenType>, std::vector<Term>> table_;

    Term pop();

    void delete_tokens(std::vector<lexical_analyser::Token*> &list);

    const std::vector<Term> &stack() const;
    void initialize_table();

    const std::map<std::pair<NonTerminal, lexical_analyser::TokenType>, std::vector<Term>> &table() const;
};

} // namespace syntactic_analyser
