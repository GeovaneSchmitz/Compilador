#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "compiler/compiler.hpp"
#include "lexical_analyser/lexical_analyser.hpp"
#include "log/log.hpp"
#include "semantic_analyser/semantic_analyser.hpp"
#include "syntactic_analyser/syntactic_analyser.hpp"

using lexical_analyser::LexicalAnalyser;
using semantic_analyser::SemanticAnalyser;
using syntactic_analyser::SyntacticAnalyser;
using xpp_compiler::Compiler;

/**
 * GRUPO:
 * Geovane Schmitz
 * Gian Carlo Ferrari
 * Guilherme Adenilson
 * Gustavo Konescki
 */

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cerr << "Uso: main arquivo_a_compilar.xpp" << std::endl;
        return EXIT_FAILURE;
    }

    cmp_log::Log main_log("log/main.log");
    main_log.write("Log da main() iniciado.");

    std::string reading_file("Lendo arquivo: ");
    reading_file += argv[1];
    main_log.write(reading_file);
    std::string source_str;

    {
        std::ifstream source(argv[1], std::ios::binary | std::ios::ate);
        if (!source.is_open()) {
            std::cerr << "Erro ao abrir arquivo: " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
        source_str.resize(static_cast<size_t>(source.tellg()));
        source.seekg(0, std::ios::beg);
        source.read(&source_str[0], static_cast<std::streamsize>(source_str.size()));
    }

    main_log.write("Arquivo lido.");
    main_log.write("Compilando...");

    LexicalAnalyser lexical = LexicalAnalyser(&source_str);
    SyntacticAnalyser syntactic = SyntacticAnalyser();

    SemanticAnalyser semantic = SemanticAnalyser();
    Compiler compiler(&lexical, &syntactic, &semantic);

    int status = compiler.compile(source_str.c_str());

    if (status < 0) {
        main_log.write("Compilação falhou!");
        std::cout << "Compilação falhou!" << std::endl;
    }

    main_log.write("Compilação encerrada.");

    return EXIT_SUCCESS;
}
