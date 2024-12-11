#ifndef __XPP_SEMANTIC_ANALYSER__
#define __XPP_SEMANTIC_ANALYSER__

#include "lexical_analyser.hpp"
#include <map>
#include <list>

using lexical_analyser::Symbol;

namespace semantic_analyser {

    class Context {
        public:
            
            Context() : global_scope_(this), parent_scope_(nullptr) {}

            Context(Context *global_scope, Context *parent_scope) : global_scope_(global_scope), parent_scope_(parent_scope) {}

            Symbol* resolve_symbol(const std::string symbol_name) const {
                auto it = symbol_map_.find(symbol_name);
                if (it != symbol_map_.end()) {
                    return it->second;
                }
                if (parent_scope_ != nullptr) {
                    return parent_scope_->resolve_symbol(std::move(symbol_name));
                }
                return nullptr;
            }


            Context* get_new_scope(){
                return new Context(global_scope_, this);
            }


        private:
            bool is_global_;
            Context* parent_scope_;
            Context* global_scope_;
            std::map<const std::string, Symbol*> symbol_map_;
        };

    
    class ASTNode {
        public:
            ASTNode();
            virtual ~ASTNode() = 0;

            virtual Symbol* gen_code(std::list<const std::string>*code, Context*scope);
            virtual Symbol* eval(Context*scope);

    };


} // semantic_analyser

#endif