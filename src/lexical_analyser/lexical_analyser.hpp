#pragma once

#include "../log/log.hpp"
#include "token.hpp"
#include "token_type.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unordered_map>

namespace lexical_analyser {

class LexicalAnalyser {
    public:
    LexicalAnalyser(std::string *str);
    ~LexicalAnalyser();

    const std::vector<Token *> token_list() const;
    bool inTokenList(const std::string &value) const;

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

    Token nextToken();
    void toPreviousCharacter();
    void toNextCharacter();

    private:
    std::string *source_code;
    std::string::iterator current_position;
    cmp_log::Log log_;
    uint row = 1, col = 1;
    std::unordered_map<std::string, TokenType> reserved_words;
};

} // namespace lexical_analyser
