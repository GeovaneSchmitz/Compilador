#include "scope.hpp"

#include <map>
#include <string>

namespace semantic_analyser {

Scope::Scope()
    : parent_scope_(nullptr),
      global_scope_(this) {}

Scope::Scope(Scope *global_scope, Scope *parent_scope)
    : parent_scope_(parent_scope),
      global_scope_(global_scope) {}

Symbol *Scope::resolve_symbol(const std::string symbol_name) const {
    auto it = symbol_map_.find(symbol_name);
    if (it != symbol_map_.end()) {
        return it->second;
    }
    if (parent_scope_ != nullptr) {
        return parent_scope_->resolve_symbol(std::move(symbol_name));
    }
    return nullptr;
}

Scope *Scope::get_new_scope() { return new Scope(global_scope_, this); }

} // namespace semantic_analyser