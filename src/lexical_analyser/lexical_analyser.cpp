#include "lexical_analyser.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unordered_map>

namespace lexical_analyser {

LexicalAnalyser::LexicalAnalyser(std::string *str)
    : source_code{str},
      current_position{str->begin()},
      reserved_words{{"def", TokenType::DEF},       {"break", TokenType::BREAK}, {"new", TokenType::NEW},
                     {"return", TokenType::RETURN}, {"read", TokenType::READ},   {"print", TokenType::PRINT},
                     {"else", TokenType::ELSE},     {"if", TokenType::IF},       {"for", TokenType::FOR},
                     {"int", TokenType::INT},       {"float", TokenType::FLOAT}, {"string", TokenType::STRING},
                     {"null", TokenType::NULL_TYPE}} {}

LexicalAnalyser::~LexicalAnalyser() {}

/**
 * @brief Recupera o próximo token do código-fonte.
 *
 * Esta função percorre o código-fonte caractere por caractere a partir da posição atual
 * para construir um token válido. Ela garante que o maior token possível seja retornado
 * utilizando uma máquina de estados.
 *
 * A função mantém uma variável de estado para rastrear o estado atual e a atualiza
 * com base no caractere atual sendo processado. Ela também usa um ponteiro de início para
 * marcar o início do token e atualiza o iterator current_position à medida que processa
 * cada caractere.
 *
 *
 * @return Um ponteiro para o próximo objeto Token se um token válido for encontrado, ou nullptr
 *         se nenhum token válido puder ser construído.
 */

Token *LexicalAnalyser::getNextToken() {
    int state = 0;
    std::string::iterator start = current_position;
    /**
     * O current_position é um ponteiro para um caractere no código-fonte atual, nesta função nós percorremos
     * caractere por caractere até conseguir montar um token válido.
     */

    while (current_position != source_code->end()) {
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
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
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
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
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
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            default:
                return nullptr;
            }
            break;
        case 2:
            switch (*current_position++) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
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
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
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
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 7;
                break;
            default:
                return nullptr;
                break;
            }
            break;
        case 7:
            switch (*current_position++) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
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

} // namespace lexical_analyser
