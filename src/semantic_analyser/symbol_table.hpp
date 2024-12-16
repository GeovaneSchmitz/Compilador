#pragma once
#include "symbol.hpp"

#include <map>
#include <string>

namespace semantic_analyser {

class SymbolTable {
    public:
    SymbolTable();
    SymbolTable(SymbolTable *global_scope, SymbolTable *parent_scope);

    Symbol *resolveSymbol(const std::string symbol_name) const;
    void addSymbol(Symbol *symbol);
    int generateTempIndex();

    SymbolTable *getNewSymbolTable();

    std::map<const std::string, Symbol *>::const_iterator begin() const;
    std::map<const std::string, Symbol *>::const_iterator end() const;

    private:
    int temp_index;
    SymbolTable *parent_scope_;
    SymbolTable *global_scope_;
    std::map<const std::string, Symbol *> symbol_map_;
};

} // namespace semantic_analyser