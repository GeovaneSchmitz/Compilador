#include "compiler.hpp"

namespace xpp_compiler {

    Compiler::Compiler(LexicalAnalyser* lexical_analyser, SyntacticAnalyser* syntactic_analyser, SemanticAnalyser* semantic_analyser) :
        lexical_analyser_(lexical_analyser), syntactic_analyser_(syntactic_analyser), semantic_analyser_(semantic_analyser),
        log_("log/compiler.log") {}


    int Compiler::compile(const char *source) {
        log_.write("Fazendo análise sintática...");
        this->syntactic_analyser_->analyse(*this->lexical_analyser_);
        log_.write("Análise sintática concluída!");

        std::string symbol_log_msg = "";
        for (auto symbol : this->lexical_analyser_->symbol_table()) {
            symbol_log_msg += symbol.first + ": ";
            for (auto coord : symbol.second) {
                symbol_log_msg += "(" + std::to_string(coord.first) + ", " + std::to_string(coord.second) + ") ";
            }
            log_.write(symbol_log_msg);
            symbol_log_msg.clear();
        }

        log_.write("Encerrando compilação.");
    }
            
}