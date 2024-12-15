#pragma once
#include "../lexical_analyser/lexical_analyser.hpp"
#include "../lexical_analyser/token_type.hpp"
#include "../log/log.hpp"
#include "non_terminal.hpp"
#include "term_type.hpp"
#include <map>
#include <vector>

namespace syntactic_analyser {

class SyntacticAnalyser {
    public:
    SyntacticAnalyser();

    bool analyse(lexical_analyser::LexicalAnalyser &lex);
    void initialize_table();

    private:
    cmp_log::Log log;
    std::map<std::pair<NonTerminal, lexical_analyser::TokenType>, std::vector<TermType>> table_;

    const std::map<std::pair<NonTerminal, lexical_analyser::TokenType>, std::vector<TermType>> &table() const;
};

} // namespace syntactic_analyser
