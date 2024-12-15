#include "symbol_table.hpp"

#include <map>
#include <string>

namespace semantic_analyser {

SymbolTable::SymbolTable()
    : parent_scope_(nullptr),
      global_scope_(this) {}

SymbolTable::SymbolTable(SymbolTable *global_scope, SymbolTable *parent_scope)
    : parent_scope_(parent_scope),
      global_scope_(global_scope) {}

Symbol *SymbolTable::resolveSymbol(const std::string symbol_name) const {
    auto it = symbol_map_.find(symbol_name);
    if (it != symbol_map_.end()) {
        return it->second;
    }
    if (parent_scope_ != nullptr) {
        return this->parent_scope_->resolveSymbol(std::move(symbol_name));
    }
    return nullptr;
}

void SymbolTable::addSymbol(Symbol *symbol) { symbol_map_[symbol->name()] = symbol; }

std::map<const std::string, Symbol *>::const_iterator SymbolTable::begin() const { return symbol_map_.begin(); }
std::map<const std::string, Symbol *>::const_iterator SymbolTable::end() const { return symbol_map_.end(); }

SymbolTable *SymbolTable::getNewSymbolTable() { return new SymbolTable(global_scope_, this); }

} // namespace semantic_analyser