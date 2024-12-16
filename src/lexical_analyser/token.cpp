#include "token.hpp"
#include <stdexcept>

namespace lexical_analyser {

Token::Token(TokenType type, std::string::iterator start, std::string::iterator end)
    : type_{type},
      value_{start, end} {}

Token::Token(TokenType type, std::string& value) : type_(type), value_(value) {}

Token::~Token() {
    for (auto prop : this->properties_) {
        delete prop;
    }
}

const std::string &Token::value() const { return value_; }

TokenType Token::type() { return type_; }

} // namespace lexical_analyser