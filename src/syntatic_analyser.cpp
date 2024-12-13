#include "syntactic_analyser.hpp"

namespace syntactic_analyser {
    
    // Grammar class implementation

    void Grammar::add_production(const std::string& head, const std::string& body) {
        productions[head].push_back(body);
    }

    // SyntacticAnalyzer class implementation

    SyntacticAnalyser::SyntacticAnalyser(const Grammar& grammar) : grammar_(grammar) {
        stack_.push_back("$");
        stack_.push_back(grammar.non_terminals[0]);

        table_[std::make_pair("PROGRAM", "def")] = {"FUNCLIST"};
        table_[std::make_pair("PROGRAM", "ident")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "}")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "int")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "float")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "string")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", ";")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "break")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "print")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "read")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "return")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "for")] = {"STATEMENT"};
        table_[std::make_pair("PROGRAM", "$")] = {""};

        table_[std::make_pair("FUNCLIST", "def")] = {"FUNCDEF", "FUNCLIST'"};

        table_[std::make_pair("FUNCLIST'", "def")] = {"FUNCDEF", "FUNCLIST'"};
        table_[std::make_pair("FUNCLIST'", "$")] = {""};

        table_[std::make_pair("FUNCDEF", "def")] = {"def", "ident", "(", "PARAMLIST", ")", "{", "STATELIST", "}"};

        table_[std::make_pair("PARAMLIST", "int")] = {"int", "ident", "PARAMLIST'"};
        table_[std::make_pair("PARAMLIST", "float")] = {"float", "ident", "PARAMLIST'"};
        table_[std::make_pair("PARAMLIST", "string")] = {"string", "ident", "PARAMLIST'"};

        table_[std::make_pair("PARAMLIST'", ")")] = {""};
        table_[std::make_pair("PARAMLIST'", "int")] = {"PARAMLIST"};
        table_[std::make_pair("PARAMLIST'", "float")] = {"PARAMLIST"};
        table_[std::make_pair("PARAMLIST'", "string")] = {"PARAMLIST"};
        table_[std::make_pair("PARAMLIST'", ",")] = {",", "PARAMLIST"};

        table_[std::make_pair("STATEMENT", "ident")] = {"ATRIBSTAT", ";"};
        table_[std::make_pair("STATEMENT", "{")] = {"{", "STATELIST", "}"};
        table_[std::make_pair("STATEMENT", "int")] = {"VARDECL", ";"};
        table_[std::make_pair("STATEMENT", "float")] = {"VARDECL", ";"};
        table_[std::make_pair("STATEMENT", "string")] = {"VARDECL", ";"};
        table_[std::make_pair("STATEMENT", ";")] = {";"};
        table_[std::make_pair("STATEMENT", "break")] = {"break", ";"};
        table_[std::make_pair("STATEMENT", "print")] = {"PRINTSTAT", ";"};
        table_[std::make_pair("STATEMENT", "read")] = {"READSTAT", ";"};
        table_[std::make_pair("STATEMENT", "return")] = {"RETURNSTAT", ";"};
        table_[std::make_pair("STATEMENT", "if")] = {"IFSTAT"};
        table_[std::make_pair("STATEMENT", "for")] = {"FORSTAT"};    

        table_[std::make_pair("VARDECL", "int")] = {"int", "ident", "OPVARDECL"};
        table_[std::make_pair("VARDECL", "float")] = {"float", "ident", "OPVARDECL"};
        table_[std::make_pair("VARDECL", "string")] = {"string", "ident", "OPVARDECL"};

        table_[std::make_pair("OPVARDECL", ";")] = {""};
        table_[std::make_pair("OPVARDECL", "[")] = {"[", "int_constant", "]", ";"};

        table_[std::make_pair("ATRIBSTAT", "ident")] = {"LVALUE", "=", "ATRIBSTAT'"};

        table_[std::make_pair("ATRIBSTAT'", "ident")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "(")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "int_constant")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "call")] = {"FUNCCALL"};
        table_[std::make_pair("ATRIBSTAT'", "new")] = {"ALLOCEXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "-")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "+")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "float_constant")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "string_constant")] = {"EXPRESSION"};
        table_[std::make_pair("ATRIBSTAT'", "null")] = {"EXPRESSION"};

        table_[std::make_pair("FUNCCALL", "call")] = {"call", "ident", "(", "PARAMLISTCALL", ")"};

        table_[std::make_pair("PARAMLISTCALL", "ident")] = {"ident", "PARAMLISTCALL'"};
        table_[std::make_pair("PARAMLISTCALL", ")")] = {""};

        table_[std::make_pair("PARAMLISTCALL'", ")")] = {""};
        table_[std::make_pair("PARAMLISTCALL'", ",")] = {",", "PARAMLISTCALL"};

        table_[std::make_pair("PRINTSTAT", "print")] = {"print", "EXPRESSION"};

        table_[std::make_pair("READSTAT", "read")] = {"read", "LVALUE"};

        table_[std::make_pair("RETURNSTAT", "return")] = {"return"};

        table_[std::make_pair("IFSTAT", "if")] = {"if", "(", "EXPRESSION", ")", "STATEMENT", "IFSTAT'"};

        table_[std::make_pair("IFSTAT'", "ident")] = {""};
        table_[std::make_pair("IFSTAT'", "{")] = {""};
        table_[std::make_pair("IFSTAT'", "}")] = {""};
        table_[std::make_pair("IFSTAT'", "int")] = {""};
        table_[std::make_pair("IFSTAT'", "float")] = {""};
        table_[std::make_pair("IFSTAT'", "string")] = {""};
        table_[std::make_pair("IFSTAT'", ";")] = {""};
        table_[std::make_pair("IFSTAT'", "break")] = {""};
        table_[std::make_pair("IFSTAT'", "print")] = {""};
        table_[std::make_pair("IFSTAT'", "read")] = {""};
        table_[std::make_pair("IFSTAT'", "return")] = {""};
        table_[std::make_pair("IFSTAT'", "if")] = {""};
        table_[std::make_pair("IFSTAT'", "else")] = {"else", "STATEMENT"};
        table_[std::make_pair("IFSTAT'", "$")] = {""};

        table_[std::make_pair("FORSTAT", "for")] = {"for", "(", "ATRIBSTAT", ";", "EXPRESSION", ";", "ATRIBSTAT", ")", "STATEMENT"};

        table_[std::make_pair("STATELIST", "ident")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "{")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "int")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "float")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "string")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", ";")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "break")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "print")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "read")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "return")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "if")] = {"STATEMENT", "STATELIST'"};
        table_[std::make_pair("STATELIST", "for")] = {"STATEMENT", "STATELIST'"};

        table_[std::make_pair("STATELIST'", "ident")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "{")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "}")] = {""};
        table_[std::make_pair("STATELIST'", "int")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "float")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "string")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", ";")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "break")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "print")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "read")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "return")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "if")] = {"STATELIST"};
        table_[std::make_pair("STATELIST'", "for")] = {"STATELIST"};

        table_[std::make_pair("ALLOCEXPRESSION", "new")] = {"new", "ALLOCEXPRESSION'"};

        table_[std::make_pair("ALLOCEXPRESSION'", "int")] = {"int", "NUMEXPR1"};
        table_[std::make_pair("ALLOCEXPRESSION'", "float")] = {"float", "NUMEXPR1"};
        table_[std::make_pair("ALLOCEXPRESSION'", "string")] = {"string", "NUMEXPR1"};

        table_[std::make_pair("NUMEXPR1", "int")] = {"[", "NUMEXPRESSION ", "]", "NUMEXPRLIST"};

        table_[std::make_pair("NUMEXPRLIST", ")")] = {""};
        table_[std::make_pair("NUMEXPRLIST", ";")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "[")] = {"[", "NUMEXPRESSION ", "]", "NUMEXPRLIST"};
        table_[std::make_pair("NUMEXPRLIST", "]")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "=")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "<")] = {""};
        table_[std::make_pair("NUMEXPRLIST", ">")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "-")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "+")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "*")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "/")] = {""};
        table_[std::make_pair("NUMEXPRLIST", "%")] = {""};

        table_[std::make_pair("EXPRESSION", "ident")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "(")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "int_constant")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "-")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "+")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "float_constant")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "string_constant")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table_[std::make_pair("EXPRESSION", "null")] = {"NUMEXPRESSION", "EXPRESSION'"};

        table_[std::make_pair("EXPRESSION'", ")")] = {""};
        table_[std::make_pair("EXPRESSION'", ";")] = {""};
        table_[std::make_pair("EXPRESSION'", "=")] = {"=", "EXPRESSION''", "NUMEXPRESSION"};
        table_[std::make_pair("EXPRESSION'", "<")] = {"<", "EXPRESSION''", "NUMEXPRESSION"};
        table_[std::make_pair("EXPRESSION'", ">")] = {">", "EXPRESSION''", "NUMEXPRESSION"};

        table_[std::make_pair("EXPRESSION''", "ident")] = {""};
        table_[std::make_pair("EXPRESSION''", "(")] = {""};
        table_[std::make_pair("EXPRESSION''", "int_constant")] = {""};
        table_[std::make_pair("EXPRESSION''", "=")] = {""};
        table_[std::make_pair("EXPRESSION''", "-")] = {""};
        table_[std::make_pair("EXPRESSION''", "+")] = {""};
        table_[std::make_pair("EXPRESSION''", "float_constant")] = {""};
        table_[std::make_pair("EXPRESSION''", "string_constant")] = {""};
        table_[std::make_pair("EXPRESSION''", "null")] = {""};

        table_[std::make_pair("NUMEXPRESSION", "ident")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "(")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "int_constant")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "-")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "+")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "float_constant")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "string_constant")] = {"TERM", "OPTERM"};
        table_[std::make_pair("NUMEXPRESSION", "null")] = {"TERM", "OPTERM"};

        table_[std::make_pair("OPTERM", ")")] = {""};
        table_[std::make_pair("OPTERM", ";")] = {""};
        table_[std::make_pair("OPTERM", "]")] = {""};
        table_[std::make_pair("OPTERM", "=")] = {""};
        table_[std::make_pair("OPTERM", "<")] = {""};
        table_[std::make_pair("OPTERM", ">")] = {""};
        table_[std::make_pair("OPTERM", "-")] = {"-", "TERM", "OPTERM"};
        table_[std::make_pair("OPTERM", "+")] = {"+", "TERM", "OPTERM"};

        table_[std::make_pair("TERM", "ident")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "(")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "int_constant")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "-")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "+")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "float_constant")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "string_constant")] = {"UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("TERM", "null")] = {"UNARYEXPR", "UNARYTERM"};

        table_[std::make_pair("UNARYTERM", ")")] = {""};
        table_[std::make_pair("UNARYTERM", ";")] = {""};
        table_[std::make_pair("UNARYTERM", "]")] = {""};
        table_[std::make_pair("UNARYTERM", "=")] = {""};
        table_[std::make_pair("UNARYTERM", "<")] = {""};
        table_[std::make_pair("UNARYTERM", ">")] = {""};
        table_[std::make_pair("UNARYTERM", "-")] = {""};
        table_[std::make_pair("UNARYTERM", "+")] = {""};
        table_[std::make_pair("UNARYTERM", "*")] = {"*", "UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("UNARYTERM", "/")] = {"/", "UNARYEXPR", "UNARYTERM"};
        table_[std::make_pair("UNARYTERM", "%")] = {"%", "UNARYEXPR", "UNARYTERM"};

        table_[std::make_pair("UNARYEXPR", "ident")] = {"FACTOR"};
        table_[std::make_pair("UNARYEXPR", "(")] = {"FACTOR"};
        table_[std::make_pair("UNARYEXPR", "int_constant")] = {"FACTOR"};
        table_[std::make_pair("UNARYEXPR", "-")] = {"-", "FACTOR"};
        table_[std::make_pair("UNARYEXPR", "+")] = {"+", "FACTOR"};
        table_[std::make_pair("UNARYEXPR", "float_constant")] = {"FACTOR"};
        table_[std::make_pair("UNARYEXPR", "string_constant")] = {"FACTOR"};
        table_[std::make_pair("UNARYEXPR", "null")] = {"FACTOR"};

        table_[std::make_pair("FACTOR", "ident")] = {"LVALUE"};
        table_[std::make_pair("FACTOR", "(")] = {"(", "NUMEXPRESSION", ")"};
        table_[std::make_pair("FACTOR", "int_constant")] = {"int_constant"};
        table_[std::make_pair("FACTOR", "float_constant")] = {"float_constant"};
        table_[std::make_pair("FACTOR", "string_constant")] = {"string_constant"};
        table_[std::make_pair("FACTOR", "null")] = {"null"};

        table_[std::make_pair("LVALUE", "ident")] = {"ident", "NUMEXPRLIST"};
    }

    bool SyntacticAnalyser::analyse(const std::string& input) {
        size_t i = 0;
        std::string x = stack_.back();
        std::string input_with_end = input + " $";

        while (x != "$") {
            while (i < input_with_end.size() && input_with_end[i] == ' ') {
                i++;
            }
            if (i >= input_with_end.size()) break;

            std::string buffer;
            size_t j = i;
            bool found = false;

            while (j < input_with_end.size()) {
                buffer += input_with_end[j++];
                auto it = std::find_if(grammar_.terminals.begin(), grammar_.terminals.end(), 
                    [&buffer](const std::string& terminal) {
                        return terminal.find(buffer) != std::string::npos;
                    });
                if (it != grammar_.terminals.end()) {
                    found = true;
                } else if (found) {
                    buffer.pop_back();
                    break;
                }
            }

            std::cout << "Buffer: " << buffer << std::endl;
            std::cout << "Stack: ";
            for (const auto& elem : stack_) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;

            if (x == buffer) {
                stack_.pop_back();
                i += buffer.size();
            } else if (std::find(grammar_.terminals.begin(), grammar_.terminals.end(), x) != grammar_.terminals.end()) {
                std::cerr << "Erro: Terminal não corresponde ao buffer\n";
                return false;
            } else if (table_.find({x, buffer}) == table_.end()) {
                std::cerr << "Erro: Produção não encontrada na tabela\n";
                return false;
            } else {
                const auto& production = table_.at({x, buffer});
                std::cout << x << " -> ";
                for (const auto& item : production) {
                    std::cout << item << " ";
                }
                std::cout << "\n";

                stack_.pop_back();
                if (!production.empty() && production[0] != "") {
                    stack_.insert(stack_.end(), production.rbegin(), production.rend());
                }
            }

            if (!stack_.empty()) {
                x = stack_.back();
            } else {
                break;
            }
        }

        if (x == "$" && i == input_with_end.size() - 1) {
            std::cout << "Análise sintática concluída com sucesso!\n";
            stack_.clear();
            stack_.push_back("$");
            stack_.push_back(grammar_.non_terminals[0]);
            return true;
        }

        std::cerr << "Erro: Análise sintática falhou\n";
        return false;
    }

} // namespace syntactic analyser


