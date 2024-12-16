#include "compiler.hpp"
#include <stdexcept>

namespace xpp_compiler {

    Compiler::Compiler(LexicalAnalyser* lexical_analyser, SyntacticAnalyser* syntactic_analyser, SemanticAnalyser* semantic_analyser) :
        lexical_analyser_(lexical_analyser), syntactic_analyser_(syntactic_analyser), semantic_analyser_(semantic_analyser),
        log_("log/compiler.log") {}

    Compiler::~Compiler() = default;

    int Compiler::compile(const char *source) {

        log_.write("Inicializando tabela de derivação...");
        this->syntactic_analyser_->initialize_table();
        log_.write("Tabela de derivação inicializada.");

        log_.write("Fazendo análise sintática...");

        bool status = false;

        try {
            status = this->syntactic_analyser_->analyse(*this->lexical_analyser_);
        } catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
            return -1;
        }

        log_.write("Análise sintática concluída!");
        if (status) {
            std::cout << "Analisador sintático calculou que o código pertence de fato à linguagem." << std::endl;
        }

        log_.write("Lista de tokens: ");
        std::string tk = "";
        for (auto token : this->lexical_analyser_->token_list()) {
            tk += "\t" + token->value() + " : " + lexical_analyser::to_string(token->type());
            log_.write(tk);
            tk.clear();
        }

        log_.write("Tabela de símbolos:");
        std::cout << "Tabela de símbolos (com coordenadas):" << std::endl;

        std::string symbol_log_msg = "\t";
        for (auto symbol : this->lexical_analyser_->symbol_table()) {
            symbol_log_msg += symbol.first + ": ";
            for (auto coord : symbol.second) {
                symbol_log_msg += "(" + std::to_string(coord.first) + ", " + std::to_string(coord.second) + ") ";
            }
            log_.write(symbol_log_msg);
            std::cout << symbol_log_msg << std::endl;
            symbol_log_msg.clear();
            symbol_log_msg += "\t";
        }

        log_.write("Encerrando compilação.");

        // FIXME: Só pra IDE e pro clang pararem de reclamar.
        return (int) std::string(source).size();
    }
            
}