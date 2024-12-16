#include "semantic_analyser.hpp"
#include "lexical_analyser/token_type.hpp"
#include "semantic_analyser/ast_and_result.hpp"
#include "semantic_analyser/return_value.hpp"
#include "semantic_analyser/symbol_table.hpp"
#include "semantic_analyser/symbol_type.hpp"
#include "syntactic_analyser/ast_node.hpp"
#include "syntactic_analyser/non_terminal.hpp"
#include <string>

using syntactic_analyser::ASTNode;
using syntactic_analyser::NonTerminal;

namespace semantic_analyser {

SemanticAnalyser::SemanticAnalyser()
    : temp_index{0},
      log{"log/semantic_analyser.log"} {}

SemanticAnalyser::~SemanticAnalyser() = default;

const std::list<BinaryTree<Token *> *> &SemanticAnalyser::expression_tree_list() const {
    return this->expression_tree_list_;
}

ReturnValue SemanticAnalyser::generateCode(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code) {
    if (node->isTerminal()) {
        if (node->getTerminal().type() == lexical_analyser::TokenType::IDENT) {
            return generateCodeIdent(node, table, code);
        }
        if (node->getTerminal().type() == lexical_analyser::TokenType::INT_CONSTANT) {
            return generateCodeIntLiteral(node, table, code);
        }
        if (node->getTerminal().type() == lexical_analyser::TokenType::STRING_CONSTANT) {
            return generateCodeStringLiteral(node, table, code);
        }
        return ReturnValue();
    }

    switch (node->getNonTerminal()) {
    case syntactic_analyser::NonTerminal::ALLOCEXPRESSION:
    case syntactic_analyser::NonTerminal::ALLOCEXPRESSION_:
    case syntactic_analyser::NonTerminal::ATRIBSTAT:
    case syntactic_analyser::NonTerminal::ATRIBSTAT_:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::EXPRESSION:
        return generateCodeExpression(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::FORSTAT:
    case syntactic_analyser::NonTerminal::FUNCCALL:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::FUNCDEF:
        return generateCodeFunctionDef(node, table, code);
    case syntactic_analyser::NonTerminal::FUNCLIST:
    case syntactic_analyser::NonTerminal::FUNCLIST_:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::IFSTAT:
        return generateCodeIf(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::IFSTAT_:
    case syntactic_analyser::NonTerminal::NUMEXPR1:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::NUMEXPRESSION:
        return generateCodeNumExpression(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::TERM:
    case syntactic_analyser::NonTerminal::UNARYEXPR:
    case syntactic_analyser::NonTerminal::FACTOR:
    case syntactic_analyser::NonTerminal::EXPRESSION_:
    case syntactic_analyser::NonTerminal::EXPRESSION__:
    case syntactic_analyser::NonTerminal::LVALUE:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::NUMEXPRLIST:
    case syntactic_analyser::NonTerminal::OPTERM:
    case syntactic_analyser::NonTerminal::OPVARDECL:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::PARAMLIST:
        return generateCodeParamList(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::PARAMLIST_:
    case syntactic_analyser::NonTerminal::PARAMLISTCALL:
    case syntactic_analyser::NonTerminal::PARAMLISTCALL_:
    case syntactic_analyser::NonTerminal::PRINTSTAT:
    case syntactic_analyser::NonTerminal::PROGRAM:
    case syntactic_analyser::NonTerminal::READSTAT:
    case syntactic_analyser::NonTerminal::RETURNSTAT:
    case syntactic_analyser::NonTerminal::STATELIST:
    case syntactic_analyser::NonTerminal::STATELIST_:
    case syntactic_analyser::NonTerminal::STATEMENT:
    case syntactic_analyser::NonTerminal::UNARYTERM:
        return generateCodeDefault(node, table, code);
        break;
    case syntactic_analyser::NonTerminal::VARDECL:
        return generateCodeVardecl(node, table, code);
        break;
    }
    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeDefault(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                  std::string &code) {
    if (node->size() == 1) {
        return this->generateCode(*node->begin(), table, code);
    }

    for (auto child : *node) {
        this->generateCode(child, table, code);
    }
    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeNumExpression(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                        std::string &code) {
    std::string log_buffer = "Gerando código para expressão numérica.";
    log.write(log_buffer);
    node->printTree(&log, 0);
    std::list<ASTNodeAndResultValue> nodes;
    std::list<ASTNode *> stack;
    stack.push_front(node);
    while (!stack.empty()) {
        auto current = stack.front();
        stack.pop_front();
        for (auto child : *current) {
            if (child->isNonTerminal() && child->getNonTerminal() == NonTerminal::OPTERM) {
                stack.push_front(child);
            } else {
                nodes.push_back(ASTNodeAndResultValue(child));
                nodes.push_back(ASTNodeAndResultValue(child));
            }
        }
    }

    //print
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        auto current = *it;
        std::string log_buffer = "Current: -------------";
        log.write(log_buffer);
        if (current.isASTNode()) {
            auto current_node = current.getASTNode();
            current_node->printTree(&log, 0);
        } else {
            std::string log_buffer = "Return Value:" + current.getResultValue()->getVariableName();
            log.write(log_buffer);
        }
        log.write("-------------");
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            auto current = *it;
            if (current.isResultValue()) {
                continue;
            }

            if (current.getASTNode()->isTerminal() &&
                current.getASTNode()->getTokenType() == lexical_analyser::TokenType::LOW_PRIORITY_OPERATOR) {
                if (it != nodes.begin() && std::next(it) != nodes.end()) {

                    ReturnValue left;
                    if (std::prev(it)->isResultValue()) {
                        left = *std::prev(it)->getResultValue();
                    } else {
                        left = ReturnValue(temp_index++);
                    }
                    ReturnValue right;
                    if (std::next(it)->isResultValue()) {
                        right = *std::next(it)->getResultValue();
                    } else {
                        right = this->generateCode(std::next(it)->getASTNode(), table, code);
                    }

                    ReturnValue result(temp_index++);
                    code += result.getVariableName() + " = " + left.getVariableName() + " " +
                            current.getASTNode()->getTerminal().value() + " " + right.getVariableName() + "\n";
                    nodes.insert(it, ASTNodeAndResultValue(new ReturnValue(result)));
                    nodes.erase(std::prev(it));
                    nodes.erase(std::next(it));

                    changed = true;
                }
            }
        }
    }
    if (nodes.size() >= 1) {
        if (nodes.front().isResultValue()) {
            auto result = nodes.front().getResultValue();
            return *result;
        } else {
            log.write("Erro:x3  Comparador sem operando.");
            for (auto it = nodes.begin(); it != nodes.end(); it++) {
                auto current = *it;
                std::string log_buffer = "x: -------------";
                log.write(log_buffer);
                if (current.isASTNode()) {
                    auto current_node = current.getASTNode();
                    current_node->printTree(&log, 0);
                } else {
                    std::string log_buffer = "Return Value:" + current.getResultValue()->getVariableName();
                    log.write(log_buffer);
                }
                log.write("-------------");
            }
        }
    } else {
        if (nodes.size() == 0) {
            log.write("Erro:3  Comparador sem operando.");
        }
        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            auto current = *it;
            std::string log_buffer = "Current: -------------";
            log.write(log_buffer);
            if (current.isASTNode()) {
                auto current_node = current.getASTNode();
                current_node->printTree(&log, 0);
            }
            log.write("-------------");
        }
    }
    log.write("Erro:4  Comparador sem operando.");
    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeFunctionDef(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                      std::string &code) {
    std::string log_buffer = "Gerando código para definição de função.";
    log.write(log_buffer);
    node->printTree(&log, 0);
    ASTNode *ident_node = nullptr;
    ASTNode *paramlist = nullptr;
    ASTNode *statlist = nullptr;

    log.write(log_buffer);

    for (auto child : *node) {
        if (child->isNonTerminal()) {
            NonTerminal nt = child->getNonTerminal();
            switch (nt) {
            case NonTerminal::PARAMLIST:
                paramlist = child;
                break;
            case NonTerminal::STATELIST:
                statlist = child;
                break;
            default:
                break;
            }
        } else if (child->isTerminal() && child->getTokenType() == lexical_analyser::TokenType::IDENT) {
            ident_node = child;
        }
    }

    if (ident_node == nullptr || paramlist == nullptr || statlist == nullptr) {
        log.write("Erro: Identificador, lista de parâmetros ou lista de instruções não encontrados.");
    }

    SymbolTable *new_table = table->getNewSymbolTable();
    this->generateCode(paramlist, new_table, code);

    auto label = "F_" + ident_node->getTerminal().value();
    code += label + ":\n";
    Symbol *symbol = new Symbol(SymbolType::FUNCTION, ident_node->getTerminal().value());
    symbol->setInternalRepresentation(label);
    table->addSymbol(symbol);
    this->generateCode(statlist, new_table, code);

    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeIntLiteral(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                     std::string &code) {
    std::string log_buffer = "Gerando código para literal inteiro.";

    log.write(log_buffer);

    ReturnValue result(temp_index++);
    code += result.getVariableName() + " = " + node->getTerminal().value() + "\n";

    return result;
}

ReturnValue SemanticAnalyser::generateCodeStringLiteral(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                        std::string &code) {
    std::string log_buffer = "Gerando código para literal string.";

    log.write(log_buffer);

    ReturnValue result(temp_index++);
    code += result.getVariableName() + " = \"" + node->getTerminal().value() + "\"\n";

    return result;
}

ReturnValue SemanticAnalyser::generateCodeParamList(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                    std::string &code) {
    std::list<ASTNode *> nodes;
    std::list<ASTNode *> stack;
    stack.push_front(node);
    while (!stack.empty()) {
        auto current = stack.front();
        stack.pop_front();
        for (auto child : *current) {
            if (child->isNonTerminal() && child->getNonTerminal() == NonTerminal::PARAMLIST_) {
                stack.push_front(child);
            } else {
                nodes.push_back(child);
                nodes.push_back(child);
            }
        }
    }

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        auto current = *it;
        if (current->isTerminal() && current->getTokenType() == lexical_analyser::TokenType::IDENT) {
            table->addSymbol(new Symbol(SymbolType::VARIABLE, current->getTerminal().value()));
        }
    }

    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeVardecl(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                  std::string &code) {
    ASTNode *ident_node = nullptr;
    for (auto child : *node) {
        if (child->isTerminal() && child->getTokenType() == lexical_analyser::TokenType::IDENT) {
            ident_node = child;
            break;
        }
    }

    std::string log_buffer = "Declarando variável " + ident_node->getTerminal().value();
    log.write(log_buffer);

    Symbol *symbol = new Symbol(SymbolType::VARIABLE, ident_node->getTerminal().value());
    table->addSymbol(symbol);

    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeIf(syntactic_analyser::ASTNode *node, SymbolTable *table, std::string &code) {
    ASTNode *cond_node = nullptr;
    ASTNode *then_node = nullptr;
    ASTNode *else_node = nullptr;
    std::string log_buffer = "Gerando código para if.";
    log.write(log_buffer);

    for (auto child : *node) {
        if (child->isNonTerminal()) {
            NonTerminal nt = child->getNonTerminal();
            switch (nt) {
            case NonTerminal::EXPRESSION:
                cond_node = child;
                break;
            case NonTerminal::STATEMENT:
                then_node = child;
                break;
            case NonTerminal::IFSTAT_:
                else_node = child;
                break;
            default:
                break;
            }
        }
    }
    if (cond_node == nullptr || then_node == nullptr) {
        log.write("Erro: Condição ou corpo do if não encontrado.");
        return ReturnValue();
    }

    auto return_value = this->generateCode(cond_node, table, code);

    auto end_then_label = "L" + std::to_string(temp_index++);

    code += "if False " + return_value.getVariableName() + " == 1 goto " + end_then_label + "\n";

    this->generateCode(then_node, table, code);
    code += end_then_label + ":\n";

    if (else_node != nullptr) {
        this->generateCode(else_node, table, code);
    }

    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeIdent(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                std::string &code) {

    if (node->isTerminal() && node->getTokenType() == lexical_analyser::TokenType::IDENT) {
        auto symbol = table->resolveSymbol(node->getTerminal().value());
        if (symbol == nullptr) {
            std::string log_buffer = "Variável " + node->getTerminal().value() + " não declarada.";
            log.write(log_buffer);
            return ReturnValue();
        } else {
            return ReturnValue(symbol->getInternalRepresentation());
        }
    }
    return ReturnValue();
}

ReturnValue SemanticAnalyser::generateCodeExpression(syntactic_analyser::ASTNode *node, SymbolTable *table,
                                                     std::string &code) {
    std::string log_buffer = "Gerando código para expressão.";
    log.write(log_buffer);
    // node->printTree(&log, 0);
    std::string log_buffer2 = "-----------------------";
    log.write(log_buffer2);
    std::list<ASTNodeAndResultValue> nodes;
    std::list<ASTNode *> stack;
    stack.push_front(node);
    while (!stack.empty()) {
        auto current = stack.front();
        stack.pop_front();
        for (auto child : *current) {
            if (child->isNonTerminal() && child->getNonTerminal() == NonTerminal::EXPRESSION_) {
                stack.push_front(child);
            } else if (child->isNonTerminal() && child->getNonTerminal() != NonTerminal::EXPRESSION__) {
                nodes.push_back(ASTNodeAndResultValue(child));
            } else if (child->isTerminal()) {
                nodes.push_back(ASTNodeAndResultValue(child));
            }
        }
    }
    bool change = true;

    while (nodes.size() != 1 && change) {
        change = false;
        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            auto current = *it;
            if (current.isASTNode()) {
                auto current_node = current.getASTNode();
                if (current_node->isTerminal()) {
                    if (current_node->getTerminal().type() == lexical_analyser::TokenType::COMPARATOR) {
                        if (it != nodes.begin() && std::next(it) != nodes.end()) {
                            auto prev = *std::prev(it);
                            auto next = *std::next(it);

                            ReturnValue left;
                            if (prev.isASTNode()) {
                                prev.getASTNode()->printTree(&log, 0);
                                left = this->generateCode(prev.getASTNode(), table, code);
                                std::string log_buffer = "Left: " + left.getVariableName();
                                log.write(log_buffer);
                            } else if (prev.isResultValue()) {
                                left = *prev.getResultValue();
                                std::string log_buffer = "xLeft: " + left.getVariableName();
                                log.write(log_buffer);
                            }

                            ReturnValue right;
                            if (next.isASTNode()) {
                                next.getASTNode()->printTree(&log, 0);

                                right = this->generateCode(next.getASTNode(), table, code);
                                std::string log_buffer = "Right: " + right.getVariableName();
                                log.write(log_buffer);

                            } else if (next.isResultValue()) {
                                right = *next.getResultValue();
                                std::string log_buffer = "xRight: " + right.getVariableName();
                                log.write(log_buffer);
                            }

                            if (left.getVariableName() != "INVALID" && right.getVariableName() != "INVALID") {
                                ReturnValue result(temp_index++);
                                code += result.getVariableName() + " = 0\n";
                                std::string temp_label = "L" + std::to_string(temp_index++);
                                code += "if False " + left.getVariableName() + " " +
                                        current_node->getTerminal().value() + " " + right.getVariableName() + " goto " +
                                        temp_label + "\n";
                                code += result.getVariableName() + " = 1\n";
                                code += temp_label + ":\n";

                                // remove left and right and replace comparator to result
                                nodes.erase(std::prev(it));
                                nodes.erase(std::next(it));

                                nodes.insert(it, ASTNodeAndResultValue(new ReturnValue(result)));
                                change = true;
                            } else {
                                log.write("Erro:1  Comparador sem operando.");
                            }
                        } else {
                            log.write("Erro:2 Comparador sem operando.");
                        }
                    }
                }
            }
        }
    }
    if (nodes.size() >= 1) {
        if (nodes.front().isResultValue()) {
            auto result = nodes.front().getResultValue();
            return *result;
        } else {
            log.write("Erro:x3  Comparador sem operando.");
            for (auto it = nodes.begin(); it != nodes.end(); it++) {
                auto current = *it;
                std::string log_buffer = "x: -------------";
                log.write(log_buffer);
                if (current.isASTNode()) {
                    auto current_node = current.getASTNode();
                    current_node->printTree(&log, 0);
                } else {
                    std::string log_buffer = "Return Value:" + current.getResultValue()->getVariableName();
                    log.write(log_buffer);
                }
                log.write("-------------");
            }
        }
    } else {
        if (nodes.size() == 0) {
            log.write("Erro:3  Comparador sem operando.");
        }
        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            auto current = *it;
            std::string log_buffer = "Current: -------------";
            log.write(log_buffer);
            if (current.isASTNode()) {
                auto current_node = current.getASTNode();
                current_node->printTree(&log, 0);
            }
            log.write("-------------");
        }
    }
    log.write("Erro:4  Comparador sem operando.");
    return ReturnValue();
}

void SemanticAnalyser::analyse(ASTNode *root) {
    SymbolTable global_table;

    std::string code = "";

    this->generateCode(root, &global_table, code);

    this->log.write("--------------------");
    this->log.write(code);
    this->log.write("--------------------");
    this->log.write("Análise semântica concluída.");

    log.write("Tabela de símbolos:");

    std::string symbol_log_msg = "\t";
    for (auto symbol : global_table) {
        symbol_log_msg += symbol.first + ": " + toString(symbol.second->type()) + " " + symbol.second->name();
        log.write(symbol_log_msg);
        symbol_log_msg.clear();
        symbol_log_msg += "\t";
    }
}

} // namespace semantic_analyser
