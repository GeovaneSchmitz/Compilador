#include "term.hpp"
#include <stdexcept>

namespace syntactic_analyser {

Term::Term(NonTerminal nt)
    : is_terminal_{false},
      terminal_{lexical_analyser::TokenType::END_OF_FILE},
      non_terminal_{nt} {}

Term::Term(lexical_analyser::TokenType tt)
    : is_terminal_{true},
      terminal_{tt},
      non_terminal_{NonTerminal::PROGRAM} {}

Term::~Term() {}

bool Term::isTerminal() const { return is_terminal_; }

bool Term::isNonTerminal() const { return !is_terminal_; }

lexical_analyser::TokenType Term::getTerminal() const {
    if (!is_terminal_) {
        throw std::runtime_error("Term is non-terminal");
    }
    return terminal_;
}

NonTerminal Term::getNonTerminal() const {
    if (is_terminal_) {
        throw std::runtime_error("Term is terminal");
    }

    return non_terminal_;
}

std::string Term::get_string() const {
    if (this->isTerminal()) {
        return lexical_analyser::to_string(this->getTerminal());
    } else {
        return to_string(this->getNonTerminal());
    }
}

std::ostream &operator<<(std::ostream &os, Term const &m) {
    if (m.isTerminal()) {
        os << lexical_analyser::to_string(m.getTerminal());
    } else {
        os << to_string(m.getNonTerminal());
    }
    return os;
}

} // namespace syntactic_analyser
