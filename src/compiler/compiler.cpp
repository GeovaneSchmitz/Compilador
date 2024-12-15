#include "compiler.hpp"

using syntactic_analyser::ASTNode;
using syntactic_analyser::NonTerminal;

namespace xpp_compiler {

Compiler::Compiler(LexicalAnalyser *lexical_analyser, SyntacticAnalyser *syntactic_analyser,
                   SemanticAnalyser *semantic_analyser)
    : lexical_analyser_(lexical_analyser),
      syntactic_analyser_(syntactic_analyser),
      semantic_analyser_(semantic_analyser),
      log_("log/compiler.log") {}

Compiler::~Compiler() = default;

int Compiler::compile(const char *source) {

    log_.write("Inicializando tabela de derivação...");
    this->syntactic_analyser_->initialize_table();
    log_.write("Tabela de derivação inicializada.");

    log_.write("Fazendo análise sintática...");
    auto ast_tree = new ASTNode(NonTerminal::PROGRAM);

    this->syntactic_analyser_->analyse(*this->lexical_analyser_, ast_tree);
    log_.write("Análise sintática concluída!");

    this->semantic_analyser_->analyse(ast_tree);

    log_.write("Encerrando compilação.");

    // FIXME: Só pra IDE e pro clang pararem de reclamar.
    return (int)std::string(source).size();
}

} // namespace xpp_compiler