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

    main_log.write("Fazendo análise léxica...");

    lexical_analyser::LexicalAnalyser lexical_analyser(&source_str);

    main_log.write("Análise léxica completa!");


    main_log.write("Fazendo análise sintática...");

    syntactic_analyser::SyntacticAnalyser syntactic_analyser;

    syntactic_analyser.analyse(lexical_analyser);
    main_log.write("Análise sintática concluída!");

    std::string symbol_log_msg = "";
    for (auto symbol : lexical_analyser.symbol_table()) {
        symbol_log_msg += symbol.first + ": ";
        for (auto coord : symbol.second) {
            symbol_log_msg += "(" + std::to_string(coord.first) + ", " + std::to_string(coord.second) + ") ";
        }
        main_log.write(symbol_log_msg);
        symbol_log_msg.clear();
    }

    main_log.write("Encerrando compilação.");
    return EXIT_SUCCESS;
}
