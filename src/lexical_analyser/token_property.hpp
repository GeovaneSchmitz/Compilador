#pragma once
#include "token_property_base.hpp"
#include <string>

namespace lexical_analyser {

template <typename T> class TokenProperty : public TokenPropertyBase {
    public:
    TokenProperty(std::string id);
    TokenProperty(std::string id, const T &value);

    ~TokenProperty();

    const std::string &id() const;
    const T &value() const;

    void set_value(const T &new_value);

    private:
    std::string id_;
    T value_;
};

} // namespace lexical_analyser