#include "syntactic_analyser.hpp"
#include "ast_node.hpp"
#include "lexical_analyser/token_type.hpp"
#include "non_terminal.hpp"
#include "term_type.hpp"
#include <list>
#include <map>
#include <string>
#include <vector>

using lexical_analyser::LexicalAnalyser;
using lexical_analyser::Token;
using lexical_analyser::TokenType;

namespace syntactic_analyser {

const std::map<std::pair<NonTerminal, TokenType>, std::vector<TermType>> &SyntacticAnalyser::table() const {
    return table_;
}

SyntacticAnalyser::SyntacticAnalyser()
    : log("log/syntactic_analyser.log") {}
bool SyntacticAnalyser::analyse(LexicalAnalyser &lex, ASTNode *root) {
    std::list<ASTNode *> stack_;

    stack_.push_back(new ASTNode(TokenType::END_OF_FILE));
    stack_.push_back(root);

    std::string buffer_log;
    std::string stack_log;
    std::string prod_log;

    auto current_item = stack_.back();
    Token token = lex.nextToken();

    while (token.type() != TokenType::END_OF_FILE) {
        buffer_log = "Buffer: " + toString(token.type());
        this->log.write(buffer_log);
        stack_log = "Stack: ";

        for (const auto &elem : stack_) {
            stack_log += elem->toString() + " ";
        }

        this->log.write(stack_log);

        if (current_item->isTerminal() && current_item->getTokenType() == TokenType::END_OF_FILE) {
            if (stack_.size() == 1) {
                break;
            }
        }

        if (current_item->isTerminal()) {
            if (current_item->getTokenType() == token.type()) {
                current_item->setToken(token);
                stack_.pop_back();
                token = lex.nextToken();
            } else {
                std::string item_token_error_str("Erro: Token lido (");
                item_token_error_str += lexical_analyser::toString(token.type()) +
                                        ") não corresponde ao topo da pilha (" + current_item->toString() + ")";
                this->log.write(item_token_error_str);
                return false;
            }
        } else {
            prod_log = "Procurando produção para " + current_item->toString() + " e " +
                       lexical_analyser::toString(token.type());
            this->log.write(prod_log);
            const auto production = table_.find({current_item->getNonTerminal(), token.type()});
            if (production == table_.end()) {
                this->log.write("Erro: Produção não encontrada na tabela.");
                return false;
            }

            std::string prods;

            prods += current_item->toString() + " -> ";
            for (const auto &item : production->second) {
                prods += item.toString() + " ";
            }
            this->log.write(prods);

            stack_.pop_back();
            for (auto it = production->second.rbegin(); it != production->second.rend(); ++it) {
                if (it->isNonTerminal()) {
                    auto *node = new ASTNode(it->getNonTerminal());
                    current_item->addChild(node);
                    stack_.push_back(node);
                } else {
                    auto *node = new ASTNode(it->getTerminal());
                    current_item->addChild(node);
                    stack_.push_back(node);
                }
            }
        }

        // FIXME: Esse if aqui não deveria existir, temos que repensar a condição do while(token != TokenTi) da
        // linha 36.
        if (token.type() == TokenType::END_OF_FILE) {
            if (stack_.back()->isTerminal()) {
                std::string error("Erro: token é END_OF_FILE mas há um terminal no topo da pilha: ");
                error += stack_.back()->toString() + "!";
                this->log.write(error);
            } else {
                bool quick_fix = stack_.back()->getNonTerminal() == NonTerminal::FUNCLIST_ ||
                                 stack_.back()->getNonTerminal() == NonTerminal::IFSTAT_;
                if (quick_fix) {
                    stack_.pop_back();
                }
            }
        }

        if (!stack_.empty()) {
            current_item = stack_.back();
        } else {
            break;
        }
    }

    if (current_item->isTerminal() && current_item->getTokenType() == TokenType::END_OF_FILE) {
        root->printTree(&this->log, 0);
        this->log.write("Análise sintática concluída com sucesso!");
        return true;
    }

    this->log.write("Erro: Análise sintática falhou.");
    return false;
}

void SyntacticAnalyser::initialize_table() {

    // PROGRAM
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_DEF)] = {TermType(NonTerminal::FUNCLIST)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::IDENT)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::OPEN_CURLY_BRACE)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_INT)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_FLOAT)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_STRING)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::SEMICOLON)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_BREAK)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_PRINT)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_READ)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_RETURN)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_IF)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_FOR)] = {TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::END_OF_FILE)] = {}; // Se END_OF_FILE = $, tudo certo

    // FUNCLIST
    table_[std::make_pair(NonTerminal::FUNCLIST, TokenType::RESERVED_WORD_DEF)] = {TermType(NonTerminal::FUNCDEF),
                                                                                   TermType(NonTerminal::FUNCLIST_)};

    // FUNCLIST'
    table_[std::make_pair(NonTerminal::FUNCLIST_, TokenType::RESERVED_WORD_DEF)] = {TermType(NonTerminal::FUNCLIST)};
    table_[std::make_pair(NonTerminal::FUNCLIST_, TokenType::END_OF_FILE)] = {};

    // FUNCDEF
    table_[std::make_pair(NonTerminal::FUNCDEF, TokenType::RESERVED_WORD_DEF)] = {
        TermType(TokenType::RESERVED_WORD_DEF), TermType(TokenType::IDENT),
        TermType(TokenType::OPEN_PARENTHESIS),  TermType(NonTerminal::PARAMLIST),
        TermType(TokenType::CLOSE_PARENTHESIS), TermType(TokenType::OPEN_CURLY_BRACE),
        TermType(NonTerminal::STATELIST),       TermType(TokenType::CLOSE_CURLY_BRACE)};

    // PARAMLIST
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::RESERVED_WORD_INT)] = {
        TermType(TokenType::RESERVED_WORD_INT), TermType(TokenType::IDENT), TermType(NonTerminal::PARAMLIST_)};
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::RESERVED_WORD_FLOAT)] = {
        TermType(TokenType::RESERVED_WORD_FLOAT), TermType(TokenType::IDENT), TermType(NonTerminal::PARAMLIST_)};
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::RESERVED_WORD_STRING)] = {
        TermType(TokenType::RESERVED_WORD_STRING), TermType(TokenType::IDENT), TermType(NonTerminal::PARAMLIST_)};

    // PARAMLIST'
    table_[std::make_pair(NonTerminal::PARAMLIST_, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::PARAMLIST_, TokenType::COMMA)] = {TermType(TokenType::COMMA),
                                                                         TermType(NonTerminal::PARAMLIST)};

    // STATEMENT
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::IDENT)] = {TermType(NonTerminal::ATRIBSTAT),
                                                                        TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::OPEN_CURLY_BRACE)] = {
        TermType(TokenType::OPEN_CURLY_BRACE), TermType(NonTerminal::STATELIST),
        TermType(TokenType::CLOSE_CURLY_BRACE)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_INT)] = {TermType(NonTerminal::VARDECL),
                                                                                    TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_FLOAT)] = {TermType(NonTerminal::VARDECL),
                                                                                      TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_STRING)] = {TermType(NonTerminal::VARDECL),
                                                                                       TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::SEMICOLON)] = {TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_BREAK)] = {
        TermType(TokenType::RESERVED_WORD_BREAK), TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_PRINT)] = {TermType(NonTerminal::PRINTSTAT),
                                                                                      TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_READ)] = {TermType(NonTerminal::READSTAT),
                                                                                     TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_RETURN)] = {
        TermType(NonTerminal::RETURNSTAT), TermType(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_IF)] = {TermType(NonTerminal::IFSTAT)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_FOR)] = {TermType(NonTerminal::FORSTAT)};

    // VARDECL
    table_[std::make_pair(NonTerminal::VARDECL, TokenType::RESERVED_WORD_INT)] = {
        TermType(TokenType::RESERVED_WORD_INT), TermType(TokenType::IDENT), TermType(NonTerminal::OPVARDECL)};
    table_[std::make_pair(NonTerminal::VARDECL, TokenType::RESERVED_WORD_FLOAT)] = {
        TermType(TokenType::RESERVED_WORD_FLOAT), TermType(TokenType::IDENT), TermType(NonTerminal::OPVARDECL)};
    table_[std::make_pair(NonTerminal::VARDECL, TokenType::RESERVED_WORD_STRING)] = {
        TermType(TokenType::RESERVED_WORD_STRING), TermType(TokenType::IDENT), TermType(NonTerminal::OPVARDECL)};

    // OPVARDECL
    table_[std::make_pair(NonTerminal::OPVARDECL, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::OPVARDECL, TokenType::OPEN_SQUARE_BRACKET)] = {
        TermType(TokenType::OPEN_SQUARE_BRACKET), TermType(TokenType::INT_CONSTANT),
        TermType(TokenType::CLOSE_SQUARE_BRACKET), TermType(NonTerminal::OPVARDECL)};

    // ATRIBSTAT
    table_[std::make_pair(NonTerminal::ATRIBSTAT, TokenType::IDENT)] = {
        TermType(NonTerminal::LVALUE), TermType(TokenType::ASSIGNMENT), TermType(NonTerminal::ATRIBSTAT_)};

    // ATRIBSTAT'
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::IDENT)] = {TermType(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::OPEN_PARENTHESIS)] = {TermType(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::INT_CONSTANT)] = {TermType(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::RESERVED_WORD_CALL)] = {TermType(NonTerminal::FUNCCALL)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::RESERVED_WORD_NEW)] = {
        TermType(NonTerminal::ALLOCEXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::LOW_PRIORITY_OPERATOR)] = {
        TermType(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::FLOAT_CONSTANT)] = {TermType(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::STRING_CONSTANT)] = {TermType(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::RESERVED_WORD_NULL)] = {
        TermType(NonTerminal::EXPRESSION)};

    // FUNCCALL
    table_[std::make_pair(NonTerminal::FUNCCALL, TokenType::RESERVED_WORD_CALL)] = {
        TermType(TokenType::RESERVED_WORD_CALL), TermType(TokenType::IDENT), TermType(TokenType::OPEN_PARENTHESIS),
        TermType(NonTerminal::PARAMLISTCALL), TermType(TokenType::CLOSE_PARENTHESIS)};

    // PARAMLISTCALL
    table_[std::make_pair(NonTerminal::PARAMLISTCALL, TokenType::IDENT)] = {TermType(TokenType::IDENT),
                                                                            TermType(NonTerminal::PARAMLISTCALL_)};
    table_[std::make_pair(NonTerminal::PARAMLISTCALL, TokenType::CLOSE_PARENTHESIS)] = {};

    // PARAMLISTCALL'
    table_[std::make_pair(NonTerminal::PARAMLISTCALL_, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::PARAMLISTCALL_, TokenType::COMMA)] = {TermType(TokenType::COMMA),
                                                                             TermType(NonTerminal::PARAMLISTCALL)};

    // PRINTSTAT
    table_[std::make_pair(NonTerminal::PRINTSTAT, TokenType::RESERVED_WORD_PRINT)] = {
        TermType(TokenType::RESERVED_WORD_PRINT), TermType(NonTerminal::EXPRESSION)};

    // READSTAT
    table_[std::make_pair(NonTerminal::READSTAT, TokenType::RESERVED_WORD_READ)] = {
        TermType(TokenType::RESERVED_WORD_READ), TermType(NonTerminal::LVALUE)};

    // RETURNSTAT
    table_[std::make_pair(NonTerminal::RETURNSTAT, TokenType::RESERVED_WORD_RETURN)] = {
        TermType(TokenType::RESERVED_WORD_RETURN)};

    // IFSTAT
    table_[std::make_pair(NonTerminal::IFSTAT, TokenType::RESERVED_WORD_IF)] = {
        TermType(TokenType::RESERVED_WORD_IF), TermType(TokenType::OPEN_PARENTHESIS),
        TermType(NonTerminal::EXPRESSION),     TermType(TokenType::CLOSE_PARENTHESIS),
        TermType(NonTerminal::STATEMENT),      TermType(NonTerminal::IFSTAT_)};

    // IFSTAT'
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::IDENT)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::OPEN_CURLY_BRACE)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::CLOSE_CURLY_BRACE)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_INT)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_FLOAT)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_STRING)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_BREAK)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_PRINT)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_READ)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_RETURN)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_IF)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_ELSE)] = {
        TermType(TokenType::RESERVED_WORD_ELSE), TermType(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_FOR)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::END_OF_FILE)] = {};

    // FORSTAT
    table_[std::make_pair(NonTerminal::FORSTAT, TokenType::RESERVED_WORD_FOR)] = {
        TermType(TokenType::RESERVED_WORD_FOR), TermType(TokenType::OPEN_PARENTHESIS),
        TermType(NonTerminal::ATRIBSTAT),       TermType(TokenType::SEMICOLON),
        TermType(NonTerminal::EXPRESSION),      TermType(TokenType::SEMICOLON),
        TermType(NonTerminal::ATRIBSTAT),       TermType(TokenType::CLOSE_PARENTHESIS),
        TermType(NonTerminal::STATEMENT)};

    table_[std::make_pair(NonTerminal::STATELIST, TokenType::IDENT)] = {TermType(NonTerminal::STATEMENT),
                                                                        TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::OPEN_CURLY_BRACE)] = {TermType(NonTerminal::STATEMENT),
                                                                                   TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_INT)] = {TermType(NonTerminal::STATEMENT),
                                                                                    TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_FLOAT)] = {
        TermType(NonTerminal::STATEMENT), TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_STRING)] = {
        TermType(NonTerminal::STATEMENT), TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::SEMICOLON)] = {TermType(NonTerminal::STATEMENT),
                                                                            TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_BREAK)] = {
        TermType(NonTerminal::STATEMENT), TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_PRINT)] = {
        TermType(NonTerminal::STATEMENT), TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_READ)] = {TermType(NonTerminal::STATEMENT),
                                                                                     TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_RETURN)] = {
        TermType(NonTerminal::STATEMENT), TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_IF)] = {TermType(NonTerminal::STATEMENT),
                                                                                   TermType(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_FOR)] = {TermType(NonTerminal::STATEMENT),
                                                                                    TermType(NonTerminal::STATELIST_)};

    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::IDENT)] = {TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::OPEN_CURLY_BRACE)] = {TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::CLOSE_CURLY_BRACE)] = {};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_INT)] = {TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_FLOAT)] = {
        TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_STRING)] = {
        TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::SEMICOLON)] = {TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_BREAK)] = {
        TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_PRINT)] = {
        TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_READ)] = {TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_RETURN)] = {
        TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_IF)] = {TermType(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_FOR)] = {TermType(NonTerminal::STATELIST)};

    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION, TokenType::RESERVED_WORD_NEW)] = {
        TermType(TokenType::RESERVED_WORD_NEW), TermType(NonTerminal::ALLOCEXPRESSION_)};

    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION_, TokenType::RESERVED_WORD_INT)] = {
        TermType(TokenType::RESERVED_WORD_INT), TermType(NonTerminal::NUMEXPR1)};
    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION_, TokenType::RESERVED_WORD_FLOAT)] = {
        TermType(TokenType::RESERVED_WORD_FLOAT), TermType(NonTerminal::NUMEXPR1)};
    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION_, TokenType::RESERVED_WORD_STRING)] = {
        TermType(TokenType::RESERVED_WORD_STRING), TermType(NonTerminal::NUMEXPR1)};

    table_[std::make_pair(NonTerminal::NUMEXPR1, TokenType::OPEN_SQUARE_BRACKET)] = {
        TermType(TokenType::OPEN_SQUARE_BRACKET), TermType(NonTerminal::NUMEXPRESSION),
        TermType(TokenType::CLOSE_SQUARE_BRACKET), TermType(NonTerminal::NUMEXPRLIST)

    };
    table_[std::make_pair(NonTerminal::NUMEXPR1, TokenType::SEMICOLON)] = {};

    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::OPEN_SQUARE_BRACKET)] = {
        TermType(TokenType::OPEN_SQUARE_BRACKET), TermType(NonTerminal::NUMEXPRESSION),
        TermType(TokenType::CLOSE_SQUARE_BRACKET), TermType(NonTerminal::NUMEXPRLIST)};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::CLOSE_SQUARE_BRACKET)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::COMPARATOR)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::LOW_PRIORITY_OPERATOR)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::HIGH_PRIORITY_OPERATOR)] = {};

    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::IDENT)] = {TermType(NonTerminal::NUMEXPRESSION),
                                                                         TermType(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::OPEN_PARENTHESIS)] = {
        TermType(NonTerminal::NUMEXPRESSION), TermType(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::INT_CONSTANT)] = {TermType(NonTerminal::NUMEXPRESSION),
                                                                                TermType(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::LOW_PRIORITY_OPERATOR)] = {
        TermType(NonTerminal::NUMEXPRESSION), TermType(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::FLOAT_CONSTANT)] = {TermType(NonTerminal::NUMEXPRESSION),
                                                                                  TermType(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::STRING_CONSTANT)] = {TermType(NonTerminal::NUMEXPRESSION),
                                                                                   TermType(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::RESERVED_WORD_NULL)] = {
        TermType(NonTerminal::NUMEXPRESSION), TermType(NonTerminal::EXPRESSION_)};

    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::ASSIGNMENT)] = {
        TermType(TokenType::ASSIGNMENT), TermType(NonTerminal::EXPRESSION__), TermType(NonTerminal::NUMEXPRESSION)};
    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::COMPARATOR)] = {
        TermType(TokenType::COMPARATOR), TermType(NonTerminal::EXPRESSION__), TermType(NonTerminal::NUMEXPRESSION)};

    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::IDENT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::OPEN_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::INT_CONSTANT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::LOW_PRIORITY_OPERATOR)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::FLOAT_CONSTANT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::STRING_CONSTANT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::RESERVED_WORD_NULL)] = {};

    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::IDENT)] = {TermType(NonTerminal::TERM),
                                                                            TermType(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::OPEN_PARENTHESIS)] = {TermType(NonTerminal::TERM),
                                                                                       TermType(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::INT_CONSTANT)] = {TermType(NonTerminal::TERM),
                                                                                   TermType(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::LOW_PRIORITY_OPERATOR)] = {
        TermType(NonTerminal::TERM), TermType(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::FLOAT_CONSTANT)] = {TermType(NonTerminal::TERM),
                                                                                     TermType(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::STRING_CONSTANT)] = {TermType(NonTerminal::TERM),
                                                                                      TermType(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::RESERVED_WORD_NULL)] = {TermType(NonTerminal::TERM),
                                                                                         TermType(NonTerminal::OPTERM)};

    table_[std::make_pair(NonTerminal::OPTERM, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::CLOSE_SQUARE_BRACKET)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::COMPARATOR)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::LOW_PRIORITY_OPERATOR)] = {
        TermType(TokenType::LOW_PRIORITY_OPERATOR), TermType(NonTerminal::TERM), TermType(NonTerminal::OPTERM)};

    table_[std::make_pair(NonTerminal::TERM, TokenType::IDENT)] = {TermType(NonTerminal::UNARYEXPR),
                                                                   TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::OPEN_PARENTHESIS)] = {TermType(NonTerminal::UNARYEXPR),
                                                                              TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::INT_CONSTANT)] = {TermType(NonTerminal::UNARYEXPR),
                                                                          TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::LOW_PRIORITY_OPERATOR)] = {TermType(NonTerminal::UNARYEXPR),
                                                                                   TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::LOW_PRIORITY_OPERATOR)] = {TermType(NonTerminal::UNARYEXPR),
                                                                                   TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::FLOAT_CONSTANT)] = {TermType(NonTerminal::UNARYEXPR),
                                                                            TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::STRING_CONSTANT)] = {TermType(NonTerminal::UNARYEXPR),
                                                                             TermType(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::RESERVED_WORD_NULL)] = {TermType(NonTerminal::UNARYEXPR),
                                                                                TermType(NonTerminal::UNARYTERM)};

    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::CLOSE_SQUARE_BRACKET)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::COMPARATOR)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::LOW_PRIORITY_OPERATOR)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::HIGH_PRIORITY_OPERATOR)] = {
        TermType(TokenType::HIGH_PRIORITY_OPERATOR), TermType(NonTerminal::UNARYEXPR),
        TermType(NonTerminal::UNARYTERM)};

    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::IDENT)] = {TermType(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::OPEN_PARENTHESIS)] = {TermType(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::INT_CONSTANT)] = {TermType(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::LOW_PRIORITY_OPERATOR)] = {
        TermType(TokenType::LOW_PRIORITY_OPERATOR), TermType(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::FLOAT_CONSTANT)] = {TermType(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::STRING_CONSTANT)] = {TermType(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::RESERVED_WORD_NULL)] = {TermType(NonTerminal::FACTOR)};

    table_[std::make_pair(NonTerminal::FACTOR, TokenType::IDENT)] = {TermType(NonTerminal::LVALUE)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::OPEN_PARENTHESIS)] = {TermType(TokenType::OPEN_PARENTHESIS),
                                                                                TermType(NonTerminal::NUMEXPRESSION),
                                                                                TermType(TokenType::CLOSE_PARENTHESIS)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::INT_CONSTANT)] = {TermType(TokenType::INT_CONSTANT)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::FLOAT_CONSTANT)] = {TermType(TokenType::FLOAT_CONSTANT)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::STRING_CONSTANT)] = {TermType(TokenType::STRING_CONSTANT)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::RESERVED_WORD_NULL)] = {
        TermType(TokenType::RESERVED_WORD_NULL)};

    table_[std::make_pair(NonTerminal::LVALUE, TokenType::IDENT)] = {TermType(TokenType::IDENT),
                                                                     TermType(NonTerminal::NUMEXPRLIST)};
}

} // namespace syntactic_analyser
