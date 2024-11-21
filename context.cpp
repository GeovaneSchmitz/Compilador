#include "context.h"

Context::Context() {
    m_global_context = this;
    m_parent_context = nullptr;
}


Context::Context(Context*global_context, Context*parent_context){
    m_global_context = global_context;
    m_parent_context = parent_context;
}


Symbol * Context::resolve_symbol(const std::string symbol_name) const {
    auto it = m_symbol_map.find(symbol_name);
    if (it != m_symbol_map.end()) {
        return it->second;
    }
    if (m_parent_context != nullptr) {
        return m_parent_context->resolve_symbol(std::move(symbol_name));
    }
    return nullptr;
}


Context *Context::get_new_context(){
    return new Context(m_global_context, this);
}
