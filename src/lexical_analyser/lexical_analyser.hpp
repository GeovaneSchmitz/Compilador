#pragma once

#include "token.hpp"
#include "../log/log.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <list>

namespace lexical_analyser {

class LexicalAnalyser {
    public:
    LexicalAnalyser(std::string *str);
    ~LexicalAnalyser();

    const std::unordered_map<std::string, std::list<std::pair<uint, uint>>> symbol_table() const;
    const std::vector<Token*> token_list() const;
    void append_token_list(Token* token);

    /**
     * @brief Recupera o próximo token do código-fonte.
     *
     * Esta função percorre o código-fonte caractere por caractere a partir da posição atual
     * para construir um token válido. Ela garante que o maior token possível seja retornado
     * utilizando uma máquina de estados.
     *
     * A função mantém uma variável de estado para rastrear o estado atual e a atualiza
     * com base no caractere atual sendo processado. Ela também usa um ponteiro de início para
     * marcar o início do token e atualiza o ponteiro current_position à medida que processa
     * cada caractere.
     *
     *
     * @return Um ponteiro para o próximo objeto Token se um token válido for encontrado, ou nullptr
     *         se nenhum token válido puder ser construído.
     */

    Token *getNextToken();

    private:
    std::string *source_code;
    std::string::iterator current_position;
    cmp_log::Log log_;
    uint row = 1, col = 1;
    std::unordered_map<std::string, std::list<std::pair<uint, uint>>> symbol_table_;
    std::vector<Token*> token_list_;
    std::unordered_map<std::string, TokenType> reserved_words;
};

} // namespace lexical_analyser
