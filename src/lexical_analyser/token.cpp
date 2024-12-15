#include "token.hpp"
#include <stdexcept>

namespace lexical_analyser {

Token::Token(TokenType type, std::string::iterator start, std::string::iterator end)
    : type_{type},
      value_{start, end} {}

Token::Token(TokenType type, std::string &value)
    : type_(type),
      value_(value) {}

Token::Token(TokenType type)
    : type_{type},
      value_{""} {}

Token::~Token() {
    for (auto prop : this->properties_) {
        delete prop;
    }
}

const std::string &Token::value() const { return value_; }

TokenType Token::type()  const { return type_; }

template <typename T> void Token::add_property(std::string &id, T &value) {
    if (this->get_property<T>(id) != nullptr) {

        return;
    }
    TokenProperty<T> new_property = new TokenProperty<T>(id, value);
    this->properties_.push_back(new_property);
}

/**
 * NOTE: Lança std::out_of_range se não encontrar nenhuma propriedade com id
 * dado.
 */
template <typename T> void Token::set_property_value(std::string &id, T &value) {
    TokenProperty<T> *prop = this->get_property<T>(id);
    if (prop == nullptr) {
        throw std::out_of_range("Propriedade \"" + id + "\" não encontrada.");
    }

    prop->set_value(value);
}

/**
 * NOTE: Retorna nullptr se não encontrada a propriedade.
 */
template <typename T> const TokenProperty<T> *Token::get_property(std::string &id) const {
    TokenProperty<T> *prop = nullptr;
    for (auto p : this->properties_) {
        prop = (TokenProperty<T> *)p;
        if (prop->id() == id) {
            prop = p;
        }
    }
    return prop;
}

} // namespace lexical_analyser