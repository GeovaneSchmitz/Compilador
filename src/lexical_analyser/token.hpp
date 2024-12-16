#pragma once
#include "token_property.hpp"
#include "token_property_base.hpp"
#include "token_type.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace lexical_analyser {

class Token {
    public:
    Token(TokenType type, std::string::iterator start, std::string::iterator end);
    Token(TokenType type, std::string& value);
    ~Token();

    const std::string &value() const;

    TokenType type();

    template <typename T> void add_property(std::string &id, T &value) {
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
    template <typename T> void set_property_value(std::string &id, T &value) {
        TokenProperty<T> *prop = this->get_property<T>(id);
        if (prop == nullptr) {
            throw std::out_of_range("Propriedade \"" + id + "\" não encontrada.");
        }

        prop->set_value(value);
    }

    /**
    * NOTE: Retorna nullptr se não encontrada a propriedade.
    */
    template <typename T> const TokenProperty<T>* get_property(std::string &id) const {
        TokenProperty<T> *prop = nullptr;
        for (auto p : this->properties_) {
            prop = (TokenProperty<T> *)p;
            if (prop->id() == id) {
                prop = p;
            }
        }
        return prop;
    }

    private:
    TokenType type_;
    std::string value_;
    std::vector<TokenPropertyBase *> properties_;

};
} // namespace lexical_analyser