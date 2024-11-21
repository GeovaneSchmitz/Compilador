#ifndef ASTNODE_H
#define ASTNODE_H
#include <list>
#include <string>
#include "symbol.h"
#include "context.h"
class ASTNode
{
public:
    ASTNode();
    virtual ~ASTNode() = 0;


    virtual Symbol* gen_code(std::list<const std::string>*code, Context*context);
    virtual Symbol* eval(Context*context);

};

#endif // ASTNODE_H
