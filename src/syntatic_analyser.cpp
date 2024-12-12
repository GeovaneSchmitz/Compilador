#include "syntactic_analyser.hpp"

namespace syntactic_analyser {
    
    // Grammar class implementation

    void Grammar::add_production(const std::string& head, const std::string& body) {
        productions[head].push_back(body);
    }

    // SyntacticAnalyzer class implementation

    SyntacticAnalyser::SyntacticAnalyser(const Grammar& grammar) : grammar(grammar) {
        stack.push_back("$");
        stack.push_back(grammar.non_terminals[0]);

        table[std::make_pair("PROGRAM", "def")] = {"FUNCLIST"};
        table[std::make_pair("PROGRAM", "ident")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "}")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "int")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "float")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "string")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", ";")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "break")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "print")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "read")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "return")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "for")] = {"STATEMENT"};
        table[std::make_pair("PROGRAM", "$")] = {""};

        table[std::make_pair("FUNCLIST", "def")] = {"FUNCDEF", "FUNCLIST'"};

        table[std::make_pair("FUNCLIST'", "def")] = {"FUNCDEF", "FUNCLIST'"};
        table[std::make_pair("FUNCLIST'", "$")] = {""};

        table[std::make_pair("FUNCDEF", "def")] = {"def", "ident", "(", "PARAMLIST", ")", "{", "STATELIST", "}"};

        table[std::make_pair("PARAMLIST", "int")] = {"int", "ident", "PARAMLIST'"};
        table[std::make_pair("PARAMLIST", "float")] = {"float", "ident", "PARAMLIST'"};
        table[std::make_pair("PARAMLIST", "string")] = {"string", "ident", "PARAMLIST'"};

        table[std::make_pair("PARAMLIST'", ")")] = {""};
        table[std::make_pair("PARAMLIST'", "int")] = {"PARAMLIST"};
        table[std::make_pair("PARAMLIST'", "float")] = {"PARAMLIST"};
        table[std::make_pair("PARAMLIST'", "string")] = {"PARAMLIST"};
        table[std::make_pair("PARAMLIST'", ",")] = {",", "PARAMLIST"};

        table[std::make_pair("STATEMENT", "ident")] = {"ATRIBSTAT", ";"};
        table[std::make_pair("STATEMENT", "{")] = {"{", "STATELIST", "}"};
        table[std::make_pair("STATEMENT", "int")] = {"VARDECL", ";"};
        table[std::make_pair("STATEMENT", "float")] = {"VARDECL", ";"};
        table[std::make_pair("STATEMENT", "string")] = {"VARDECL", ";"};
        table[std::make_pair("STATEMENT", ";")] = {";"};
        table[std::make_pair("STATEMENT", "break")] = {"break", ";"};
        table[std::make_pair("STATEMENT", "print")] = {"PRINTSTAT", ";"};
        table[std::make_pair("STATEMENT", "read")] = {"READSTAT", ";"};
        table[std::make_pair("STATEMENT", "return")] = {"RETURNSTAT", ";"};
        table[std::make_pair("STATEMENT", "if")] = {"IFSTAT"};
        table[std::make_pair("STATEMENT", "for")] = {"FORSTAT"};    

        table[std::make_pair("VARDECL", "int")] = {"int", "ident", "OPVARDECL"};
        table[std::make_pair("VARDECL", "float")] = {"float", "ident", "OPVARDECL"};
        table[std::make_pair("VARDECL", "string")] = {"string", "ident", "OPVARDECL"};

        table[std::make_pair("OPVARDECL", ";")] = {""};
        table[std::make_pair("OPVARDECL", "[")] = {"[", "int_constant", "]", ";"};

        table[std::make_pair("ATRIBSTAT", "ident")] = {"LVALUE", "=", "ATRIBSTAT'"};

        table[std::make_pair("ATRIBSTAT'", "ident")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "(")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "int_constant")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "call")] = {"FUNCCALL"};
        table[std::make_pair("ATRIBSTAT'", "new")] = {"ALLOCEXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "-")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "+")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "float_constant")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "string_constant")] = {"EXPRESSION"};
        table[std::make_pair("ATRIBSTAT'", "null")] = {"EXPRESSION"};

        table[std::make_pair("FUNCCALL", "call")] = {"call", "ident", "(", "PARAMLISTCALL", ")"};

        table[std::make_pair("PARAMLISTCALL", "ident")] = {"ident", "PARAMLISTCALL'"};
        table[std::make_pair("PARAMLISTCALL", ")")] = {""};

        table[std::make_pair("PARAMLISTCALL'", ")")] = {""};
        table[std::make_pair("PARAMLISTCALL'", ",")] = {",", "PARAMLISTCALL"};

        table[std::make_pair("PRINTSTAT", "print")] = {"print", "EXPRESSION"};

        table[std::make_pair("READSTAT", "read")] = {"read", "LVALUE"};

        table[std::make_pair("RETURNSTAT", "return")] = {"return"};

        table[std::make_pair("IFSTAT", "if")] = {"if", "(", "EXPRESSION", ")", "STATEMENT", "IFSTAT'"};

        table[std::make_pair("IFSTAT'", "ident")] = {""};
        table[std::make_pair("IFSTAT'", "{")] = {""};
        table[std::make_pair("IFSTAT'", "}")] = {""};
        table[std::make_pair("IFSTAT'", "int")] = {""};
        table[std::make_pair("IFSTAT'", "float")] = {""};
        table[std::make_pair("IFSTAT'", "string")] = {""};
        table[std::make_pair("IFSTAT'", ";")] = {""};
        table[std::make_pair("IFSTAT'", "break")] = {""};
        table[std::make_pair("IFSTAT'", "print")] = {""};
        table[std::make_pair("IFSTAT'", "read")] = {""};
        table[std::make_pair("IFSTAT'", "return")] = {""};
        table[std::make_pair("IFSTAT'", "if")] = {""};
        table[std::make_pair("IFSTAT'", "else")] = {"else", "STATEMENT"};
        table[std::make_pair("IFSTAT'", "$")] = {""};

        table[std::make_pair("FORSTAT", "for")] = {"for", "(", "ATRIBSTAT", ";", "EXPRESSION", ";", "ATRIBSTAT", ")", "STATEMENT"};

        table[std::make_pair("STATELIST", "ident")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "{")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "int")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "float")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "string")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", ";")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "break")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "print")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "read")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "return")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "if")] = {"STATEMENT", "STATELIST'"};
        table[std::make_pair("STATELIST", "for")] = {"STATEMENT", "STATELIST'"};

        table[std::make_pair("STATELIST'", "ident")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "{")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "}")] = {""};
        table[std::make_pair("STATELIST'", "int")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "float")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "string")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", ";")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "break")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "print")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "read")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "return")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "if")] = {"STATELIST"};
        table[std::make_pair("STATELIST'", "for")] = {"STATELIST"};

        table[std::make_pair("ALLOCEXPRESSION", "new")] = {"new", "ALLOCEXPRESSION'"};

        table[std::make_pair("ALLOCEXPRESSION'", "int")] = {"int", "NUMEXPR1"};
        table[std::make_pair("ALLOCEXPRESSION'", "float")] = {"float", "NUMEXPR1"};
        table[std::make_pair("ALLOCEXPRESSION'", "string")] = {"string", "NUMEXPR1"};

        table[std::make_pair("NUMEXPR1", "int")] = {"[", "NUMEXPRESSION ", "]", "NUMEXPRLIST"};

        table[std::make_pair("NUMEXPRLIST", ")")] = {""};
        table[std::make_pair("NUMEXPRLIST", ";")] = {""};
        table[std::make_pair("NUMEXPRLIST", "[")] = {"[", "NUMEXPRESSION ", "]", "NUMEXPRLIST"};
        table[std::make_pair("NUMEXPRLIST", "]")] = {""};
        table[std::make_pair("NUMEXPRLIST", "=")] = {""};
        table[std::make_pair("NUMEXPRLIST", "<")] = {""};
        table[std::make_pair("NUMEXPRLIST", ">")] = {""};
        table[std::make_pair("NUMEXPRLIST", "-")] = {""};
        table[std::make_pair("NUMEXPRLIST", "+")] = {""};
        table[std::make_pair("NUMEXPRLIST", "*")] = {""};
        table[std::make_pair("NUMEXPRLIST", "/")] = {""};
        table[std::make_pair("NUMEXPRLIST", "%")] = {""};

        table[std::make_pair("EXPRESSION", "ident")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "(")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "int_constant")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "-")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "+")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "float_constant")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "string_constant")] = {"NUMEXPRESSION", "EXPRESSION'"};
        table[std::make_pair("EXPRESSION", "null")] = {"NUMEXPRESSION", "EXPRESSION'"};

        table[std::make_pair("EXPRESSION'", ")")] = {""};
        table[std::make_pair("EXPRESSION'", ";")] = {""};
        table[std::make_pair("EXPRESSION'", "=")] = {"=", "EXPRESSION''", "NUMEXPRESSION"};
        table[std::make_pair("EXPRESSION'", "<")] = {"<", "EXPRESSION''", "NUMEXPRESSION"};
        table[std::make_pair("EXPRESSION'", ">")] = {">", "EXPRESSION''", "NUMEXPRESSION"};

        table[std::make_pair("EXPRESSION''", "ident")] = {""};
        table[std::make_pair("EXPRESSION''", "(")] = {""};
        table[std::make_pair("EXPRESSION''", "int_constant")] = {""};
        table[std::make_pair("EXPRESSION''", "=")] = {""};
        table[std::make_pair("EXPRESSION''", "-")] = {""};
        table[std::make_pair("EXPRESSION''", "+")] = {""};
        table[std::make_pair("EXPRESSION''", "float_constant")] = {""};
        table[std::make_pair("EXPRESSION''", "string_constant")] = {""};
        table[std::make_pair("EXPRESSION''", "null")] = {""};

        table[std::make_pair("NUMEXPRESSION", "ident")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "(")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "int_constant")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "-")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "+")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "float_constant")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "string_constant")] = {"TERM", "OPTERM"};
        table[std::make_pair("NUMEXPRESSION", "null")] = {"TERM", "OPTERM"};

        table[std::make_pair("OPTERM", ")")] = {""};
        table[std::make_pair("OPTERM", ";")] = {""};
        table[std::make_pair("OPTERM", "]")] = {""};
        table[std::make_pair("OPTERM", "=")] = {""};
        table[std::make_pair("OPTERM", "<")] = {""};
        table[std::make_pair("OPTERM", ">")] = {""};
        table[std::make_pair("OPTERM", "-")] = {"-", "TERM", "OPTERM"};
        table[std::make_pair("OPTERM", "+")] = {"+", "TERM", "OPTERM"};

        table[std::make_pair("TERM", "ident")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "(")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "int_constant")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "-")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "+")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "float_constant")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "string_constant")] = {"UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("TERM", "null")] = {"UNARYEXPR", "UNARYTERM"};

        table[std::make_pair("UNARYTERM", ")")] = {""};
        table[std::make_pair("UNARYTERM", ";")] = {""};
        table[std::make_pair("UNARYTERM", "]")] = {""};
        table[std::make_pair("UNARYTERM", "=")] = {""};
        table[std::make_pair("UNARYTERM", "<")] = {""};
        table[std::make_pair("UNARYTERM", ">")] = {""};
        table[std::make_pair("UNARYTERM", "-")] = {""};
        table[std::make_pair("UNARYTERM", "+")] = {""};
        table[std::make_pair("UNARYTERM", "*")] = {"*", "UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("UNARYTERM", "/")] = {"/", "UNARYEXPR", "UNARYTERM"};
        table[std::make_pair("UNARYTERM", "%")] = {"%", "UNARYEXPR", "UNARYTERM"};

        table[std::make_pair("UNARYEXPR", "ident")] = {"FACTOR"};
        table[std::make_pair("UNARYEXPR", "(")] = {"FACTOR"};
        table[std::make_pair("UNARYEXPR", "int_constant")] = {"FACTOR"};
        table[std::make_pair("UNARYEXPR", "-")] = {"-", "FACTOR"};
        table[std::make_pair("UNARYEXPR", "+")] = {"+", "FACTOR"};
        table[std::make_pair("UNARYEXPR", "float_constant")] = {"FACTOR"};
        table[std::make_pair("UNARYEXPR", "string_constant")] = {"FACTOR"};
        table[std::make_pair("UNARYEXPR", "null")] = {"FACTOR"};

        table[std::make_pair("FACTOR", "ident")] = {"LVALUE"};
        table[std::make_pair("FACTOR", "(")] = {"(", "NUMEXPRESSION", ")"};
        table[std::make_pair("FACTOR", "int_constant")] = {"int_constant"};
        table[std::make_pair("FACTOR", "float_constant")] = {"float_constant"};
        table[std::make_pair("FACTOR", "string_constant")] = {"string_constant"};
        table[std::make_pair("FACTOR", "null")] = {"null"};

        table[std::make_pair("LVALUE", "ident")] = {"ident", "NUMEXPRLIST"};
    }

    bool SyntacticAnalyser::analyse(const std::string& input) {
        size_t i = 0;
        std::string x = stack.back();

        std::string input_with_end = input + " $";

        while (x != "$") {
            size_t j = (i <= input_with_end.size()) ? i : input_with_end.size();
            std::string buffer = input_with_end.substr(j, 1);

            if (buffer == " ") {
                j++;
                buffer = input_with_end.substr(j, 1);
            }

            bool found = false;
            while (true) {
                auto it = std::find(grammar.terminals.begin(), grammar.terminals.end(), buffer);
                bool found_n = (it != grammar.terminals.end()); 

                if (found && !found_n) {
                break;
                };

                if (found_n) {
                    found = true;
                }
                
                j++;
                if (j >= input_with_end.size()) break;
                buffer += input_with_end[j];
            }
            std::cout << "Buffer: " << buffer << std::endl;
            std::cout << "Stack: ";
            for (const auto& elem : stack) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
            
            std::string y = (j < input_with_end.size()) ? buffer.substr(0, buffer.size() - 1) : buffer;

            std::cout << "Y: " << y << std::endl;
            std::cout << "X: " << x << std::endl;

            if (x == y) {
                stack.pop_back();
                i += y.size()+1;
            } else if (std::find(grammar.terminals.begin(), grammar.terminals.end(), x) != grammar.terminals.end()) {
                std::cerr << "Erro: alguma coisa com terminais\n";
                return false;
            } else if (table.find({x, y}) == table.end()) {
                std::cerr << "Erro: não está na table\n";
                return false;
            } else {
                std::cout << x << " -> ";
                for (const auto& item : table.at({x, y})) {
                    std::cout << item << " ";
                }
                std::cout << "\n";

                stack.pop_back();
                auto producao = table.at({x, y});

                if (producao[0] != "") {
                    for (auto it = producao.rbegin(); it != producao.rend(); ++it) {
                        stack.push_back(*it);
                    }
                }
            }

            x = stack.back();
        }

        std::cout << "Analise sintatica concluida com sucesso!\n";
        return true;
    }

} // namespace syntactic analyser


