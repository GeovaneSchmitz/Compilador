#include "return_value.hpp"
#include <string>

namespace semantic_analyser {

ReturnValue::ReturnValue(int ti)
    : name{"T" + std::to_string(ti)} {}

ReturnValue::ReturnValue(std::string variable)
    : name{variable} {}

ReturnValue::ReturnValue()
    : name{"INVALID"} {}

ReturnValue::~ReturnValue() {};

std::string ReturnValue::getVariableName() const { return name; }


// copy constructor
ReturnValue::ReturnValue(const ReturnValue &rv)
    : name{rv.name} {}

// copy assignment
ReturnValue &ReturnValue::operator=(const ReturnValue &rv) {
    name = rv.name;
    return *this;
}

// move constructor
ReturnValue::ReturnValue(ReturnValue &&rv)
    : name{std::move(rv.name)} {}

// move assignment
ReturnValue &ReturnValue::operator=(ReturnValue &&rv) {
    name = std::move(rv.name);
    return *this;
}

} // namespace semantic_analyser