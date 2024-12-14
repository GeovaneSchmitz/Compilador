#pragma once
#include "scope.hpp"

#include <list>
#include <string>

namespace semantic_analyser {

class ASTNode {
    public:
    ASTNode();
    virtual ~ASTNode() = 0;

    virtual Symbol *gen_code(std::list<const std::string> *code, Scope *scope);
    virtual Symbol *eval(Scope *scope);
};
} // namespace semantic_analyser