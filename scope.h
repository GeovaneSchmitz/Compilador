#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include <map>
#include <string>
class Context
{
public:
    Context();
    Context(Context* global_scope,  Context *parent_scope);

    Symbol * resolve_symbol(const std::string symbol_name) const;
    void add_symbol(const Symbol symbol);
    Context *get_new_scope();


private:
    bool m_is_global;
    Context* m_parent_scope;
    Context* m_global_scope;
    std::map<const std::string, Symbol*> m_symbol_map;
};

#endif // SCOPE_H
