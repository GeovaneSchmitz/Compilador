#pragma once
#include "token_property.hpp"
#include "token_property_base.hpp"
#include "token_type.hpp"
#include <string>
#include <vector>

namespace lexical_analyser {

class Token {
    public:
    Token(TokenType type, std::string::iterator start, std::string::iterator end);
    Token(TokenType type);
    Token(TokenType type, std::string& value);
    ~Token();

    const std::string &value() const;

    TokenType type();

    template <typename T> void add_property(std::string &id, T &value);

    /**
     * NOTE: Lança std::out_of_range se não encontrar nenhuma propriedade com id
     * dado.
     */
    template <typename T> void set_property_value(std::string &id, T &value);

    private:
    TokenType type_;
    std::string value_;
    std::vector<TokenPropertyBase *> properties_;

    /**
     * NOTE: Retorna nullptr se não encontrada a propriedade.
     */
    template <typename T> const TokenProperty<T> *get_property(std::string &id) const;
};
} // namespace lexical_analyser