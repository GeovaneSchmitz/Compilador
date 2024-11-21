#ifndef CONTEXT_H
#define CONTEXT_H

#include "symbol.h"
#include <map>
#include <string>
class Context
{
public:
    Context();
    Context(Context* global_context,  Context *parent_context);

    Symbol * resolve_symbol(const std::string symbol_name) const;
    void add_symbol(const Symbol symbol);
    Context *get_new_context();


private:
    bool m_is_global;
    Context* m_parent_context;
    Context* m_global_context;
    std::map<const std::string, Symbol*> m_symbol_map;
};

#endif // CONTEXT_H
