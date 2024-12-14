#pragma once

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

namespace lexical_analyser {

enum class TokenType {
    IDENT,
    INT_CONSTANT,
    FLOAT_CONSTANT,
    STRING_CONSTANT,
    COMPARATOR,
    HIGHEST_PRIORITY_OPERATOR,
    HIGH_PRIORITY_OPERATOR,
    LOW_PRIORITY_OPERATOR,
    SEMICOLON,
    ASSIGNMENT,
    COMMA,
    OPEN_CURLY_BRACE,
    CLOSE_CURLY_BRACE,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS
};

class TokenPropertyBase {
    public:
    TokenPropertyBase() = default;
    virtual ~TokenPropertyBase() = default;
};

template <typename T> class TokenProperty : public TokenPropertyBase {
    public:
    TokenProperty(std::string id)
        : id_(std::move(id)) {}
    TokenProperty(std::string id, const T &value)
        : id_(std::move(id)),
          value_(value) {}

    ~TokenProperty() = default;

    const std::string &id() const { return this->id_; }

    const T &value() const { return this->value_; }

    void set_value(const T &new_value) { this->value_ = new_value; }

    private:
    std::string id_;
    T value_;
};

class Token {
    public:
    Token(TokenType type, char *start, char *end) {
        std::size_t len = end - start;
        this->value_ = new char[len + 1];
        std::memcpy(this->value_, start, len);
        this->value_[len] = '\0';
        this->type_ = type;
    }

    ~Token() {
        delete[] value_;
        for (auto prop : this->properties_) {
            delete prop;
        }
    }

    const char *value() const { return value_; }

    TokenType type() { return type_; }

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

    private:
    TokenType type_;
    char *value_;
    std::vector<TokenPropertyBase *> properties_;

    /**
     * NOTE: Retorna nullptr se não encontrada a propriedade.
     */
    template <typename T> const TokenProperty<T> *get_property(std::string &id) const {
        TokenProperty<T> *prop = nullptr;
        for (auto p : this->properties_) {
            prop = (TokenProperty<T> *)p;
            if (prop->id() == id) {
                prop = p;
            }
        }
        return prop;
    }
};
} // namespace lexical_analyser