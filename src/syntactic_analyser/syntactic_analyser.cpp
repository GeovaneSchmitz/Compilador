#include "syntactic_analyser.hpp"
#include "../lexical_analyser/token_type.hpp"
#include "../syntactic_analyser/non_terminal.hpp"
#include "../syntactic_analyser/term.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using lexical_analyser::LexicalAnalyser;
using lexical_analyser::Token;
using lexical_analyser::TokenType;

namespace syntactic_analyser {

const std::map<std::pair<NonTerminal, TokenType>, std::vector<Term>> &SyntacticAnalyser::table() const {
    return table_;
}

SyntacticAnalyser::SyntacticAnalyser() : log_("log/syntactic_analyser.log") {
    this->initialize_table();
}

bool SyntacticAnalyser::analyse(LexicalAnalyser &lex) {
    std::vector<Term> stack_;
    stack_.push_back(TokenType::END_OF_FILE);
    stack_.push_back(NonTerminal::PROGRAM);

    std::vector<Token*> token_list;

    std::string buffer_log;
    std::string stack_log;
    std::string prod_log;

    auto current_item = stack_.back();
    Token *token = lex.getNextToken();
    lex.append_token_list(token);

    while (token != nullptr) {
        buffer_log = "Buffer: " + to_string(token->type()) + ":" + token->value();
        this->log_.write(buffer_log);
        stack_log = "Stack: ";

        for (const auto &elem : stack_) {
            stack_log += elem.get_string() + " ";
        }

        this->log_.write(stack_log);

        if (current_item.isTerminal()) {
            if (current_item.getTerminal() == token->type()) {
                stack_.pop_back();
                token = lex.getNextToken();
                lex.append_token_list(token);
            } else {
                std::string item_token_error_str("Erro: Token lido (");
                item_token_error_str += token->value() + ") não corresponde ao topo da pilha (" + current_item.get_string() + ")";
                this->log_.write(item_token_error_str);
                return false;
            }
        } else {
            prod_log = "Procurando produção para " + current_item.get_string() + " e " + to_string(token->type());
            this->log_.write(prod_log);
            const auto production = table_.find({current_item.getNonTerminal(), token->type()});
            if (production == table_.end()) {
                this->log_.write("Erro: Produção não encontrada na tabela.");
                return false;
            }

            std::string prods;

            prods += current_item.get_string() + " -> ";
            for (const auto &item : production->second) {
                prods += item.get_string() + " ";
            }
            this->log_.write(prods);

            stack_.pop_back();
            if (!production->second.empty()) {
                stack_.insert(stack_.end(), production->second.rbegin(), production->second.rend());
            }
        }

        // FIXME: Esse if aqui não deveria existir, temos que repensar a condição do while(token != nullptr) da linha 39. 
        if (token == nullptr) {
            if (stack_.back().isTerminal()) {
                std::string error("Erro: token é nullptr mas há um terminal no topo da pilha: ");
                error += stack_.back().get_string() + "!";
                this->log_.write(error);
            } else {
                bool quick_fix = stack_.back().getNonTerminal() == NonTerminal::FUNCLIST_ || stack_.back().getNonTerminal() == NonTerminal::IFSTAT_;
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

    if (current_item.isTerminal() && current_item.getTerminal() == TokenType::END_OF_FILE) {
        this->log_.write("Análise sintática concluída com sucesso!");
        return true;
    }

    this->log_.write("Erro: Análise sintática falhou.");
    return false;
}

void SyntacticAnalyser::initialize_table() {

    // PROGRAM
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_DEF)] = {Term(NonTerminal::FUNCLIST)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::IDENT)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::OPEN_CURLY_BRACE)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_INT)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_FLOAT)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_STRING)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::SEMICOLON)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_BREAK)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_PRINT)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_READ)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_RETURN)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_IF)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::RESERVED_WORD_FOR)] = {Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::PROGRAM, TokenType::END_OF_FILE)] = {}; // Se END_OF_FILE = $, tudo certo

    // FUNCLIST
    table_[std::make_pair(NonTerminal::FUNCLIST, TokenType::RESERVED_WORD_DEF)] = {
        Term(NonTerminal::FUNCDEF), Term(NonTerminal::FUNCLIST_)
    };
    
    // FUNCLIST'
    table_[std::make_pair(NonTerminal::FUNCLIST_, TokenType::RESERVED_WORD_DEF)] = {Term(NonTerminal::FUNCLIST)};
    table_[std::make_pair(NonTerminal::FUNCLIST_, TokenType::END_OF_FILE)] = {};

    // FUNCDEF
    table_[std::make_pair(NonTerminal::FUNCDEF, TokenType::RESERVED_WORD_DEF)] = {
        Term(TokenType::RESERVED_WORD_DEF), Term(TokenType::IDENT),
        Term(TokenType::OPEN_PARENTHESIS),  Term(NonTerminal::PARAMLIST),
        Term(TokenType::CLOSE_PARENTHESIS), Term(TokenType::OPEN_CURLY_BRACE),
        Term(NonTerminal::STATELIST),       Term(TokenType::CLOSE_CURLY_BRACE)
    };

    // PARAMLIST
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::RESERVED_WORD_INT)] = {
        Term(TokenType::RESERVED_WORD_INT), Term(TokenType::IDENT), Term(NonTerminal::PARAMLIST_)};
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::RESERVED_WORD_FLOAT)] = {
        Term(TokenType::RESERVED_WORD_FLOAT), Term(TokenType::IDENT), Term(NonTerminal::PARAMLIST_)};
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::RESERVED_WORD_STRING)] = {
        Term(TokenType::RESERVED_WORD_STRING), Term(TokenType::IDENT), Term(NonTerminal::PARAMLIST_)};
    table_[std::make_pair(NonTerminal::PARAMLIST, TokenType::CLOSE_PARENTHESIS)] = {
        Term(NonTerminal::PARAMLIST_)};


    // PARAMLIST'
    table_[std::make_pair(NonTerminal::PARAMLIST_, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::PARAMLIST_, TokenType::COMMA)] = {
        Term(TokenType::COMMA), Term(NonTerminal::PARAMLIST)
    };

    // STATEMENT
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::IDENT)] = {
        Term(NonTerminal::ATRIBSTAT), Term(TokenType::SEMICOLON)
    };
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::OPEN_CURLY_BRACE)] = {
        Term(TokenType::OPEN_CURLY_BRACE), Term(NonTerminal::STATELIST), Term(TokenType::CLOSE_CURLY_BRACE)
    };
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_INT)] = {
        Term(NonTerminal::VARDECL), Term(TokenType::SEMICOLON)
    };
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_FLOAT)] = {Term(NonTerminal::VARDECL),
                                                                                      Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_STRING)] = {Term(NonTerminal::VARDECL),
                                                                                       Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::SEMICOLON)] = {Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_BREAK)] = {
        Term(TokenType::RESERVED_WORD_BREAK), Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_PRINT)] = {Term(NonTerminal::PRINTSTAT),
                                                                                      Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_READ)] = {Term(NonTerminal::READSTAT),
                                                                                     Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_RETURN)] = {Term(NonTerminal::RETURNSTAT),
                                                                                       Term(TokenType::SEMICOLON)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_IF)] = {Term(NonTerminal::IFSTAT)};
    table_[std::make_pair(NonTerminal::STATEMENT, TokenType::RESERVED_WORD_FOR)] = {Term(NonTerminal::FORSTAT)};

    // VARDECL
    table_[std::make_pair(NonTerminal::VARDECL, TokenType::RESERVED_WORD_INT)] = {
        Term(TokenType::RESERVED_WORD_INT), Term(TokenType::IDENT), Term(NonTerminal::OPVARDECL)};
    table_[std::make_pair(NonTerminal::VARDECL, TokenType::RESERVED_WORD_FLOAT)] = {
        Term(TokenType::RESERVED_WORD_FLOAT), Term(TokenType::IDENT), Term(NonTerminal::OPVARDECL)};
    table_[std::make_pair(NonTerminal::VARDECL, TokenType::RESERVED_WORD_STRING)] = {
        Term(TokenType::RESERVED_WORD_STRING), Term(TokenType::IDENT), Term(NonTerminal::OPVARDECL)};

    // OPVARDECL
    table_[std::make_pair(NonTerminal::OPVARDECL, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::OPVARDECL, TokenType::OPEN_SQUARE_BRACKET)] = {
        Term(TokenType::OPEN_SQUARE_BRACKET), Term(TokenType::INT_CONSTANT), Term(TokenType::CLOSE_SQUARE_BRACKET),
        Term(NonTerminal::OPVARDECL)};

    // ATRIBSTAT
    table_[std::make_pair(NonTerminal::ATRIBSTAT, TokenType::IDENT)] = {
        Term(NonTerminal::LVALUE), Term(TokenType::ASSIGNMENT), Term(NonTerminal::ATRIBSTAT_)};

    // ATRIBSTAT'
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::IDENT)] = {Term(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::OPEN_PARENTHESIS)] = {Term(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::INT_CONSTANT)] = {Term(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::RESERVED_WORD_CALL)] = {Term(NonTerminal::FUNCCALL)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::RESERVED_WORD_NEW)] = {
        Term(NonTerminal::ALLOCEXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::LOW_PRIORITY_OPERATOR)] = {Term(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::FLOAT_CONSTANT)] = {Term(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::STRING_CONSTANT)] = {Term(NonTerminal::EXPRESSION)};
    table_[std::make_pair(NonTerminal::ATRIBSTAT_, TokenType::RESERVED_WORD_NULL)] = {Term(NonTerminal::EXPRESSION)};
    
    // FUNCCALL
    table_[std::make_pair(NonTerminal::FUNCCALL, TokenType::RESERVED_WORD_CALL)] = {
        Term(TokenType::RESERVED_WORD_CALL), Term(TokenType::IDENT), Term(TokenType::OPEN_PARENTHESIS),
        Term(NonTerminal::PARAMLISTCALL), Term(TokenType::CLOSE_PARENTHESIS)};

    // PARAMLISTCALL
    table_[std::make_pair(NonTerminal::PARAMLISTCALL, TokenType::IDENT)] = {Term(TokenType::IDENT),
                                                                            Term(NonTerminal::PARAMLISTCALL_)};
    table_[std::make_pair(NonTerminal::PARAMLISTCALL, TokenType::CLOSE_PARENTHESIS)] = {};

    // PARAMLISTCALL'
    table_[std::make_pair(NonTerminal::PARAMLISTCALL_, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::PARAMLISTCALL_, TokenType::COMMA)] = {Term(TokenType::COMMA),
                                                                             Term(NonTerminal::PARAMLISTCALL)};

    // PRINTSTAT
    table_[std::make_pair(NonTerminal::PRINTSTAT, TokenType::RESERVED_WORD_PRINT)] = {
        Term(TokenType::RESERVED_WORD_PRINT), Term(NonTerminal::EXPRESSION)};

    // READSTAT
    table_[std::make_pair(NonTerminal::READSTAT, TokenType::RESERVED_WORD_READ)] = {Term(TokenType::RESERVED_WORD_READ),
                                                                                    Term(NonTerminal::LVALUE)};

    // RETURNSTAT
    table_[std::make_pair(NonTerminal::RETURNSTAT, TokenType::RESERVED_WORD_RETURN)] = {
        Term(TokenType::RESERVED_WORD_RETURN)};

    // IFSTAT
    table_[std::make_pair(NonTerminal::IFSTAT, TokenType::RESERVED_WORD_IF)] = {
        Term(TokenType::RESERVED_WORD_IF),  Term(TokenType::OPEN_PARENTHESIS), Term(NonTerminal::EXPRESSION),
        Term(TokenType::CLOSE_PARENTHESIS), Term(NonTerminal::STATEMENT),      Term(NonTerminal::IFSTAT_)};

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
        Term(TokenType::RESERVED_WORD_ELSE), Term(NonTerminal::STATEMENT)};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::RESERVED_WORD_FOR)] = {};
    table_[std::make_pair(NonTerminal::IFSTAT_, TokenType::END_OF_FILE)] = {};

    // FORSTAT
    table_[std::make_pair(NonTerminal::FORSTAT, TokenType::RESERVED_WORD_FOR)] = {
        Term(TokenType::RESERVED_WORD_FOR), Term(TokenType::OPEN_PARENTHESIS),  Term(NonTerminal::ATRIBSTAT),
        Term(TokenType::SEMICOLON),         Term(NonTerminal::EXPRESSION),      Term(TokenType::SEMICOLON),
        Term(NonTerminal::ATRIBSTAT),       Term(TokenType::CLOSE_PARENTHESIS), Term(NonTerminal::STATEMENT)};

    table_[std::make_pair(NonTerminal::STATELIST, TokenType::IDENT)] = {Term(NonTerminal::STATEMENT),
                                                                        Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::OPEN_CURLY_BRACE)] = {Term(NonTerminal::STATEMENT),
                                                                                   Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_INT)] = {Term(NonTerminal::STATEMENT),
                                                                                    Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_FLOAT)] = {Term(NonTerminal::STATEMENT),
                                                                                      Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_STRING)] = {Term(NonTerminal::STATEMENT),
                                                                                       Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::SEMICOLON)] = {Term(NonTerminal::STATEMENT),
                                                                            Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_BREAK)] = {Term(NonTerminal::STATEMENT),
                                                                                      Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_PRINT)] = {Term(NonTerminal::STATEMENT),
                                                                                      Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_READ)] = {Term(NonTerminal::STATEMENT),
                                                                                     Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_RETURN)] = {Term(NonTerminal::STATEMENT),
                                                                                       Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_IF)] = {Term(NonTerminal::STATEMENT),
                                                                                   Term(NonTerminal::STATELIST_)};
    table_[std::make_pair(NonTerminal::STATELIST, TokenType::RESERVED_WORD_FOR)] = {Term(NonTerminal::STATEMENT),
                                                                                    Term(NonTerminal::STATELIST_)};

    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::IDENT)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::OPEN_CURLY_BRACE)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::CLOSE_CURLY_BRACE)] = {};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_INT)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_FLOAT)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_STRING)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::SEMICOLON)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_BREAK)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_PRINT)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_READ)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_RETURN)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_IF)] = {Term(NonTerminal::STATELIST)};
    table_[std::make_pair(NonTerminal::STATELIST_, TokenType::RESERVED_WORD_FOR)] = {Term(NonTerminal::STATELIST)};

    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION, TokenType::RESERVED_WORD_NEW)] = {
        Term(TokenType::RESERVED_WORD_NEW), Term(NonTerminal::ALLOCEXPRESSION_)};

    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION_, TokenType::RESERVED_WORD_INT)] = {
        Term(TokenType::RESERVED_WORD_INT), Term(NonTerminal::NUMEXPR1)};
    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION_, TokenType::RESERVED_WORD_FLOAT)] = {
        Term(TokenType::RESERVED_WORD_FLOAT), Term(NonTerminal::NUMEXPR1)};
    table_[std::make_pair(NonTerminal::ALLOCEXPRESSION_, TokenType::RESERVED_WORD_STRING)] = {
        Term(TokenType::RESERVED_WORD_STRING), Term(NonTerminal::NUMEXPR1)};

    table_[std::make_pair(NonTerminal::NUMEXPR1, TokenType::OPEN_SQUARE_BRACKET)] = {
        Term(TokenType::OPEN_SQUARE_BRACKET), Term(NonTerminal::NUMEXPRESSION),
        Term(TokenType::CLOSE_SQUARE_BRACKET), Term(NonTerminal::NUMEXPRLIST)

    };
    table_[std::make_pair(NonTerminal::NUMEXPR1, TokenType::SEMICOLON)] = {};


    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::OPEN_SQUARE_BRACKET)] = {
        Term(TokenType::OPEN_SQUARE_BRACKET), Term(NonTerminal::NUMEXPRESSION), Term(TokenType::CLOSE_SQUARE_BRACKET),
        Term(NonTerminal::NUMEXPRLIST)};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::CLOSE_SQUARE_BRACKET)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::COMPARATOR)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::LOW_PRIORITY_OPERATOR)] = {};
    table_[std::make_pair(NonTerminal::NUMEXPRLIST, TokenType::HIGH_PRIORITY_OPERATOR)] = {};

    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::IDENT)] = {Term(NonTerminal::NUMEXPRESSION),
                                                                         Term(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::OPEN_PARENTHESIS)] = {Term(NonTerminal::NUMEXPRESSION),
                                                                                    Term(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::INT_CONSTANT)] = {Term(NonTerminal::NUMEXPRESSION),
                                                                                Term(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::LOW_PRIORITY_OPERATOR)] = {
        Term(NonTerminal::NUMEXPRESSION), Term(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::FLOAT_CONSTANT)] = {Term(NonTerminal::NUMEXPRESSION),
                                                                                  Term(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::STRING_CONSTANT)] = {Term(NonTerminal::NUMEXPRESSION),
                                                                                   Term(NonTerminal::EXPRESSION_)};
    table_[std::make_pair(NonTerminal::EXPRESSION, TokenType::RESERVED_WORD_NULL)] = {Term(NonTerminal::NUMEXPRESSION),
                                                                                      Term(NonTerminal::EXPRESSION_)};

    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::ASSIGNMENT)] = {
        Term(TokenType::ASSIGNMENT), Term(NonTerminal::EXPRESSION__), Term(NonTerminal::NUMEXPRESSION)};
    table_[std::make_pair(NonTerminal::EXPRESSION_, TokenType::COMPARATOR)] = {
        Term(TokenType::COMPARATOR), Term(NonTerminal::EXPRESSION__), Term(NonTerminal::NUMEXPRESSION)};

    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::IDENT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::OPEN_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::INT_CONSTANT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::LOW_PRIORITY_OPERATOR)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::FLOAT_CONSTANT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::STRING_CONSTANT)] = {};
    table_[std::make_pair(NonTerminal::EXPRESSION__, TokenType::RESERVED_WORD_NULL)] = {};

    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::IDENT)] = {Term(NonTerminal::TERM),
                                                                            Term(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::OPEN_PARENTHESIS)] = {Term(NonTerminal::TERM),
                                                                                       Term(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::INT_CONSTANT)] = {Term(NonTerminal::TERM),
                                                                                   Term(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::LOW_PRIORITY_OPERATOR)] = {Term(NonTerminal::TERM),
                                                                                            Term(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::FLOAT_CONSTANT)] = {Term(NonTerminal::TERM),
                                                                                     Term(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::STRING_CONSTANT)] = {Term(NonTerminal::TERM),
                                                                                      Term(NonTerminal::OPTERM)};
    table_[std::make_pair(NonTerminal::NUMEXPRESSION, TokenType::RESERVED_WORD_NULL)] = {Term(NonTerminal::TERM),
                                                                                         Term(NonTerminal::OPTERM)};

    table_[std::make_pair(NonTerminal::OPTERM, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::CLOSE_SQUARE_BRACKET)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::COMPARATOR)] = {};
    table_[std::make_pair(NonTerminal::OPTERM, TokenType::LOW_PRIORITY_OPERATOR)] = {
        Term(TokenType::LOW_PRIORITY_OPERATOR), Term(NonTerminal::TERM), Term(NonTerminal::OPTERM)};

    table_[std::make_pair(NonTerminal::TERM, TokenType::IDENT)] = {Term(NonTerminal::UNARYEXPR),
                                                                   Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::OPEN_PARENTHESIS)] = {Term(NonTerminal::UNARYEXPR),
                                                                              Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::INT_CONSTANT)] = {Term(NonTerminal::UNARYEXPR),
                                                                          Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::LOW_PRIORITY_OPERATOR)] = {Term(NonTerminal::UNARYEXPR),
                                                                                   Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::LOW_PRIORITY_OPERATOR)] = {Term(NonTerminal::UNARYEXPR),
                                                                                   Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::FLOAT_CONSTANT)] = {Term(NonTerminal::UNARYEXPR),
                                                                            Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::STRING_CONSTANT)] = {Term(NonTerminal::UNARYEXPR),
                                                                             Term(NonTerminal::UNARYTERM)};
    table_[std::make_pair(NonTerminal::TERM, TokenType::RESERVED_WORD_NULL)] = {Term(NonTerminal::UNARYEXPR),
                                                                                Term(NonTerminal::UNARYTERM)};

    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::CLOSE_PARENTHESIS)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::SEMICOLON)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::CLOSE_SQUARE_BRACKET)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::ASSIGNMENT)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::COMPARATOR)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::LOW_PRIORITY_OPERATOR)] = {};
    table_[std::make_pair(NonTerminal::UNARYTERM, TokenType::HIGH_PRIORITY_OPERATOR)] = {
        Term(TokenType::HIGH_PRIORITY_OPERATOR), Term(NonTerminal::UNARYEXPR), Term(NonTerminal::UNARYTERM)};

    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::IDENT)] = {Term(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::OPEN_PARENTHESIS)] = {Term(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::INT_CONSTANT)] = {Term(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::LOW_PRIORITY_OPERATOR)] = {
        Term(TokenType::LOW_PRIORITY_OPERATOR), Term(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::FLOAT_CONSTANT)] = {Term(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::STRING_CONSTANT)] = {Term(NonTerminal::FACTOR)};
    table_[std::make_pair(NonTerminal::UNARYEXPR, TokenType::RESERVED_WORD_NULL)] = {Term(NonTerminal::FACTOR)};

    table_[std::make_pair(NonTerminal::FACTOR, TokenType::IDENT)] = {Term(NonTerminal::LVALUE)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::OPEN_PARENTHESIS)] = {
        Term(TokenType::OPEN_PARENTHESIS), Term(NonTerminal::NUMEXPRESSION), Term(TokenType::CLOSE_PARENTHESIS)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::INT_CONSTANT)] = {Term(TokenType::INT_CONSTANT)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::FLOAT_CONSTANT)] = {Term(TokenType::FLOAT_CONSTANT)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::STRING_CONSTANT)] = {Term(TokenType::STRING_CONSTANT)};
    table_[std::make_pair(NonTerminal::FACTOR, TokenType::RESERVED_WORD_NULL)] = {Term(TokenType::RESERVED_WORD_NULL)};

    table_[std::make_pair(NonTerminal::LVALUE, TokenType::IDENT)] = {Term(TokenType::IDENT),
                                                                     Term(NonTerminal::NUMEXPRLIST)};
}

} // namespace syntactic_analyser
