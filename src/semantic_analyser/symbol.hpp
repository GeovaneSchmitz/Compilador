#pragma once
#include "symbol_type.hpp"
#include <string>

namespace semantic_analyser {

class Symbol {
    public:
    Symbol(SymbolType type, std::string name);
    ~Symbol();

    SymbolType type() const;
    std::string name() const;

    private:
    SymbolType type_;
    std::string name_;
};

} // namespace semantic_analyser