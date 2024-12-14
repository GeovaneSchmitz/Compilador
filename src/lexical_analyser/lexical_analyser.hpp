#pragma once
#include "../log.hpp"

#include "token.hpp"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>
namespace lexical_analyser {

// ****************
// TOKEN PROPERTIES
// ****************

// *****
// TOKEN
// *****

// ****************
// LEXICAL ANALYSER
// ****************

class Symbol {
    public:
    Symbol(const Token &token, std::pair<ulong, ulong> occurrence)
        : token_(token),
          occurrences_() {
        this->occurrences_.push_back(occurrence);
    }
    Symbol(const Token &token)
        : token_(token),
          occurrences_() {}
    ~Symbol() = default;

    private:
    Token token_;
    std::vector<std::pair<ulong, ulong>> occurrences_;
};

class LexicalAnalyser {
    public:
    LexicalAnalyser(std::string str) {
        std::size_t len = str.length();
        this->current_position = new char[len + 1];
        strcpy(this->current_position, str.c_str());
        this->initial_state_ = current_position;
        this->last_state_ = current_position + len;
    }

    ~LexicalAnalyser();
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

    Token *getNextToken() {
        uint64_t state = 0;
        char *start = current_position;
        /**
         * O current_position é um ponteiro para um caractere no código-fonte atual, nesta função nós percorremos
         * caractere por caractere até conseguir montar um token válido.
         */

        while (current_position != last_state_) {
            switch (state) {
            case 0:
                switch (*current_position++) {
                case '*':
                case '/':
                case '%':
                    return new Token(TokenType::HIGH_PRIORITY_OPERATOR, start, current_position);
                    break;
                case ',':
                    return new Token(TokenType::COMMA, start, current_position);
                    break;
                case ')':
                    return new Token(TokenType::CLOSE_PARENTHESIS, start, current_position);
                    break;
                case '(':
                    return new Token(TokenType::OPEN_PARENTHESIS, start, current_position);
                    break;
                case '!':
                    state = 5;
                    break;
                case '<':
                case '>':
                    state = 3;
                    break;
                case '[':
                    return new Token(TokenType::OPEN_SQUARE_BRACKET, start, current_position);
                    break;
                case '"':
                    state = 1;
                    break;
                case ';':
                    return new Token(TokenType::SEMICOLON, start, current_position);
                    break;
                case ']':
                    return new Token(TokenType::CLOSE_SQUARE_BRACKET, start, current_position);
                    break;
                case 'a' ... 'z':
                case 'A' ... 'Z':
                    state = 2;
                    break;
                case '+':
                case '-':
                    return new Token(TokenType::LOW_PRIORITY_OPERATOR, start, current_position);
                    break;
                case '{':
                    return new Token(TokenType::OPEN_CURLY_BRACE, start, current_position);
                    break;
                case '}':
                    return new Token(TokenType::CLOSE_CURLY_BRACE, start, current_position);
                    break;
                case '0' ... '9':
                    state = 4;
                    break;
                case '=':
                    state = 11;
                    break;
                default:
                    start = current_position;
                    break;
                }
                break;
            case 1:
                switch (*current_position++) {
                case '"':
                    return new Token(TokenType::STRING_CONSTANT, start, current_position);
                    break;
                case 'a' ... 'z':
                case 'A' ... 'Z':
                case '0' ... '9':
                    break;
                default:
                    return nullptr;
                }
                break;
            case 2:
                switch (*current_position++) {
                case 'a' ... 'z':
                case 'A' ... 'Z':
                case '0' ... '9':
                    break;
                default:
                    current_position--;
                    return new Token(TokenType::IDENT, start, current_position);
                    break;
                }
                break;
            case 3:
                switch (*current_position++) {
                case '=':
                    return new Token(TokenType::COMPARATOR, start, current_position);
                    break;
                default:
                    current_position--;
                    return new Token(TokenType::COMPARATOR, start, current_position);
                    break;
                }
            case 4:
                switch (*current_position++) {
                case '.':
                    state = 6;
                    break;
                case '0' ... '9':
                    break;
                default:
                    current_position--;
                    return new Token(TokenType::INT_CONSTANT, start, current_position);
                    break;
                }
                break;
            case 5:
                switch (*current_position++) {
                case '=':
                    return new Token(TokenType::COMPARATOR, start, current_position);
                    break;
                default:
                    return nullptr;
                    break;
                }
                break;
            case 6:
                switch (*current_position++) {
                case '0' ... '9':
                    state = 7;
                    break;
                default:
                    return nullptr;
                    break;
                }
                break;
            case 7:
                switch (*current_position++) {
                case '0' ... '9':
                    break;
                default:
                    current_position--;
                    return new Token(TokenType::FLOAT_CONSTANT, start, current_position);
                    break;
                }
            default:
                break;
            }
        }
        return nullptr;
    }

    private:
    char *initial_state_;
    char *last_state_;
    char *current_position;
};

LexicalAnalyser::~LexicalAnalyser() { delete[] initial_state_; }

} // namespace lexical_analyser
