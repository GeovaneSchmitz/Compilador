#ifndef ASTNODE_H
#define ASTNODE_H
#include <list>
#include <string>
#include "symbol.h"
#include "scope.h"
class ASTNode
{
public:
    ASTNode();
    virtual ~ASTNode() = 0;


    virtual Symbol* gen_code(std::list<const std::string>*code, Context*scope);
    virtual Symbol* eval(Context*scope);

};

#endif // ASTNODE_H
