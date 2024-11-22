#include "scope.h"

Context::Context() {
    m_global_scope = this;
    m_parent_scope = nullptr;
}


Context::Context(Context *global_scope, Context *parent_scope){
    m_global_scope = global_scope;
    m_parent_scope = parent_scope;
}


Symbol * Context::resolve_symbol(const std::string symbol_name) const {
    auto it = m_symbol_map.find(symbol_name);
    if (it != m_symbol_map.end()) {
        return it->second;
    }
    if (m_parent_scope != nullptr) {
        return m_parent_scope->resolve_symbol(std::move(symbol_name));
    }
    return nullptr;
}


Context *Context::get_new_scope(){
    return new Context(m_global_scope, this);
}
