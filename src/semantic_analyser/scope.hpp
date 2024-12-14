#pragma once
#include "symbol.hpp"

#include <map>
#include <string>

namespace semantic_analyser {

class Scope {
    public:
    Scope();
    Scope(Scope *global_scope, Scope *parent_scope);

    Symbol *resolve_symbol(const std::string symbol_name) const;

    Scope *get_new_scope();

    private:
    Scope *parent_scope_;
    Scope *global_scope_;
    std::map<const std::string, Symbol *> symbol_map_;
};

} // namespace semantic_analyser