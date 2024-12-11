#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "lexical_analyser.hpp"
#include "log.hpp"

/**
 * NOTE: Aloca um char* do tamanho do arquivo! Lembrar de deletar.
 */
char* source_to_char_pointer(const char* file_path) {
    std::ifstream source(file_path, std::ios::binary | std::ios::ate);
    std::string file_str(file_path);
    if (!source) {
        throw std::runtime_error("Invalid source file: " + file_str );
    }

    std::streamsize size = source.tellg();
    source.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];
    if (source.read(buffer, size)) {
        buffer[size] = '\0';
        return buffer;
    } else {
        delete[] buffer;
        throw std::runtime_error("Error when reading file: " + file_str);
    }

}

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Uso: main arquivo_a_compilar.xpp" << std::endl;
        std::cout << "Segmentation fault (core dumped)." << std::endl;
        return EXIT_FAILURE;
    }

    logging::Log main_log("log/main.log");
    main_log.write("Log da main() iniciado.");

    std::string reading_file("Lendo arquivo: ");
    reading_file += argv[1];
    main_log.write(reading_file);
    char* source = source_to_char_pointer(argv[1]);
    main_log.write("Arquivo lido.");

    main_log.write("Fazendo análise léxica...");
    lexical_analyser::LexicalAnalyser lexis(source);
    lexis.parse();
    main_log.write("Análise léxica completa.");

    main_log.write("Encerrando.");

    delete source;
    return EXIT_SUCCESS;
}
