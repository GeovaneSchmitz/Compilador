#pragma once

#include <map>
#include <string>
#include <vector>

namespace syntactic_analyser {

class Grammar {
    public:
    Grammar();
    ~Grammar();

    std::vector<std::string> terminals;

    std::vector<std::string> non_terminals;

    std::map<std::string, std::vector<std::string>> productions;

    void add_production(const std::string &head, const std::string &body);
};

} // namespace syntactic_analyser
