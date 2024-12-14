#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "lexical_analyser/lexical_analyser.hpp"
#include "log/log.hpp"
#include "syntactic_analyser/syntactic_analyser.hpp"

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cerr << "Uso: main arquivo_a_compilar.xpp" << std::endl;
        std::cout << "Segmentation fault (core dumped)." << std::endl;
        return EXIT_FAILURE;
    }

    log::Log main_log("log/main.log");
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

    main_log.write("Fazendo análise léxica...");

    lexical_analyser::LexicalAnalyser lexis(&source_str);

    main_log.write("Análise léxica completa.");

    main_log.write("Encerrando.");

    syntactic_analyser::SyntacticAnalyser analyser;

    analyser.analyse(lexis);

    return EXIT_SUCCESS;
}
