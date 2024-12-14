#include "token_property.hpp"
#include <cstdlib>
#include <cstring>
#include <sys/types.h>

namespace lexical_analyser {

template <typename T>
TokenProperty<T>::TokenProperty(std::string id)
    : id_(std::move(id)) {}

template <typename T>
TokenProperty<T>::TokenProperty(std::string id, const T &value)
    : id_(std::move(id)),
      value_(value) {}

template <typename T> TokenProperty<T>::~TokenProperty() {}

template <typename T> const std::string &TokenProperty<T>::id() const { return this->id_; }

template <typename T> const T &TokenProperty<T>::value() const { return this->value_; }

template <typename T> void TokenProperty<T>::set_value(const T &new_value) { this->value_ = new_value; }

} // namespace lexical_analyser