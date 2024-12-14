#pragma once
#include "grammar.hpp"
#include <map>
#include <string>
#include <vector>

#include "log/log.hpp"

namespace syntactic_analyser {

class SyntacticAnalyser {
    public:
    SyntacticAnalyser(const Grammar &grammar);

    bool analyse(const std::string &entrada);

    private:
    log::Log log_;
    Grammar grammar_;
    std::vector<std::string> stack_;
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> table_;

    void push(std::string &str);

    const std::string pop();

    const std::vector<std::string> &stack() const;

    const std::map<std::pair<std::string, std::string>, std::vector<std::string>> &table() const;
};

} // namespace syntactic_analyser
