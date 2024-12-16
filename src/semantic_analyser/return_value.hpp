#pragma once
#include <string>

namespace semantic_analyser {

class ReturnValue {
    public:
    // copy constructor
    ReturnValue(const ReturnValue &rv);
    // copy assignment
    ReturnValue &operator=(const ReturnValue &rv);
    // move constructor
    ReturnValue(ReturnValue &&rv);
    // move assignment
    ReturnValue &operator=(ReturnValue &&rv);

    ReturnValue(int temp_index);
    ReturnValue();
    ReturnValue(std::string variable);
    ~ReturnValue();

    std::string getVariableName() const;

    private:
    std::string name;
};

} // namespace semantic_analyser