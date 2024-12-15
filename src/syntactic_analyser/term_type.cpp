#include "term_type.hpp"
#include "lexical_analyser/token_type.hpp"
#include <stdexcept>
#include <string>

using lexical_analyser::TokenType;

namespace syntactic_analyser {

TermType::TermType(NonTerminal nt)
    : is_terminal_{false},
      terminal_{TokenType::INVALID},
      non_terminal_{nt} {}

TermType::TermType(TokenType tt)
    : is_terminal_{true},
      terminal_{tt},
      non_terminal_{NonTerminal::PROGRAM} {}

TermType::~TermType() {}

bool TermType::isTerminal() const { return is_terminal_; }

bool TermType::isNonTerminal() const { return !is_terminal_; }

TokenType TermType::getTerminal() const {
    if (!is_terminal_) {
        throw std::runtime_error("Term is non-terminal");
    }
    return terminal_;
}

NonTerminal TermType::getNonTerminal() const {
    if (is_terminal_) {
        throw std::runtime_error("Term is terminal");
    }

    return non_terminal_;
}

std::string TermType::toString() const {
    if (this->isTerminal()) {
        return lexical_analyser::toString(this->getTerminal());
    } else {
        return to_string(this->getNonTerminal());
    }
}

std::ostream &operator<<(std::ostream &os, TermType const &m) {
    if (m.isTerminal()) {
        os << lexical_analyser::toString(m.getTerminal());
    } else {
        os << to_string(m.getNonTerminal());
    }
    return os;
}
} // namespace syntactic_analyser
