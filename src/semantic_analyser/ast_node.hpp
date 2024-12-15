#pragma once
#include "scope.hpp"

#include "syntactic_analyser/term.hpp"
#include <list>
#include <string>
#include <vector>

using syntactic_analyser::Term;

namespace semantic_analyser {

class ASTNode {
    public:
    ASTNode(Term t);

    virtual ~ASTNode();
    virtual void addChild(ASTNode *);

    virtual Symbol *gen_code(std::list<const std::string> *code, Scope *scope);
    virtual Symbol *eval(Scope *scope);

    private:
    Term term;
    std::vector<ASTNode *> children;
};
} // namespace semantic_analyser