#include "lexical_analyser.hpp"
#include "token.hpp"
#include "token_type.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <utility>

namespace lexical_analyser {

LexicalAnalyser::LexicalAnalyser(std::string *str)
    : source_code{str},
      current_position{str->begin()},
      log_("log/lexical_analyser.log"),
      symbol_table_(),
      token_list_(),
      reserved_words{
          {"def", TokenType::RESERVED_WORD_DEF},     {"break", TokenType::RESERVED_WORD_BREAK},
          {"new", TokenType::RESERVED_WORD_NEW},     {"call", TokenType::RESERVED_WORD_CALL},
          {"null", TokenType::RESERVED_WORD_NULL},   {"return", TokenType::RESERVED_WORD_RETURN},
          {"read", TokenType::RESERVED_WORD_READ},   {"print", TokenType::RESERVED_WORD_PRINT},
          {"else", TokenType::RESERVED_WORD_ELSE},   {"if", TokenType::RESERVED_WORD_IF},
          {"for", TokenType::RESERVED_WORD_FOR},     {"int", TokenType::RESERVED_WORD_INT},
          {"float", TokenType::RESERVED_WORD_FLOAT}, {"string", TokenType::RESERVED_WORD_STRING},
      } {}

LexicalAnalyser::~LexicalAnalyser() {
    if (this->token_list_.size() > 0) {
        for (auto token : token_list_) {
            delete token;
        }
    }
}

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

Token LexicalAnalyser::nextToken() {
    int state = 0;
    std::string::iterator start = this->current_position;
    std::string new_token_msg;
    /**
     * O current_position é um ponteiro para um caractere no código-fonte atual, nesta função nós percorremos
     * caractere por caractere até conseguir montar um token válido.
     */

    while (this->current_position != source_code->end()) {
        this->col++;
        if (*this->current_position == '\n') {
            this->row++;
            this->col = 1;
        }
        switch (state) {
        case 0:
            switch (*this->current_position++) {
            case '*':
            case '/':
            case '%':
                // this->append_token_list(TokenType::HIGH_PRIORITY_OPERATOR, start);
                // return TokenType::HIGH_PRIORITY_OPERATOR;
                return Token(TokenType::HIGH_PRIORITY_OPERATOR, start, current_position);
                break;
            case ',':
                // this->appendTokenList(TokenType::COMMA, start);
                // return TokenType::COMMA;
                return Token(TokenType::COMMA, start, current_position);
                break;
            case ')':
                // this->appendTokenList(TokenType::CLOSE_PARENTHESIS, start);
                // return TokenType::CLOSE_PARENTHESIS;
                return Token(TokenType::CLOSE_PARENTHESIS, start, current_position);
                break;
            case '(':
                // this->appendTokenList(TokenType::OPEN_PARENTHESIS, start);
                // return TokenType::OPEN_PARENTHESIS;
                return Token(TokenType::OPEN_PARENTHESIS, start, current_position);
                break;
            case '!':
                state = 5;
                break;
            case '<':
            case '>':
                state = 3;
                break;
            case '[':
                // this->appendTokenList(TokenType::OPEN_SQUARE_BRACKET, start);
                // return TokenType::OPEN_SQUARE_BRACKET;
                return Token(TokenType::OPEN_SQUARE_BRACKET, start, current_position);
                break;
            case '"':
                state = 1;
                break;
            case ';':
                // this->appendTokenList(TokenType::SEMICOLON, start);
                // return TokenType::SEMICOLON;
                return Token(TokenType::SEMICOLON, start, current_position);
                break;
            case ']':
                // this->appendTokenList(TokenType::CLOSE_SQUARE_BRACKET, start);
                // return TokenType::CLOSE_SQUARE_BRACKET;
                return Token(TokenType::CLOSE_SQUARE_BRACKET, start, current_position);
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
            case '_':
                state = 2;
                break;
            case '+':
            case '-':
                // this->appendTokenList(TokenType::LOW_PRIORITY_OPERATOR, start);
                // return TokenType::LOW_PRIORITY_OPERATOR;
                return Token(TokenType::LOW_PRIORITY_OPERATOR, start, current_position);
                break;
            case '{':
                // this->appendTokenList(TokenType::OPEN_CURLY_BRACE, start);
                // return TokenType::OPEN_CURLY_BRACE;
                return Token(TokenType::OPEN_CURLY_BRACE, start, current_position);
                break;
            case '}':
                // this->appendTokenList(TokenType::CLOSE_CURLY_BRACE, start);
                // return TokenType::CLOSE_CURLY_BRACE;
                return Token(TokenType::CLOSE_CURLY_BRACE, start, current_position);
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
                state = 8;
                break;
            default:
                start = this->current_position;
                break;
            }
            break;
        case 1:
            switch (*this->current_position++) {
            case '"':
                // this->appendTokenList(TokenType::STRING_CONSTANT, start);
                //  return TokenType::STRING_CONSTANT;
                return Token(TokenType::STRING_CONSTANT, start, current_position);
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (*this->current_position++) {
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
            case '_':
                break;
            default: {
                this->current_position--;
                std::string word(start, current_position);
                auto it = reserved_words.find(word);
                if (it != reserved_words.end()) {

                    // this->appendTokenList(it->second, start);

                    // return it->second;
                    return Token(it->second, start, current_position);
                }
                // this->appendTokenList(TokenType::IDENT, start);
                // return TokenType::IDENT;
                return Token(TokenType::IDENT, start, current_position);
                break;
            }
            }
            break;
        case 3:
            switch (*this->current_position++) {
            case '=':
                // this->appendTokenList(TokenType::COMPARATOR, start);
                // return TokenType::COMPARATOR;
                return Token(TokenType::COMPARATOR, start, current_position);
                break;
            default:
                this->current_position--;
                // this->appendTokenList(TokenType::COMPARATOR, start);
                // return TokenType::COMPARATOR;
                return Token(TokenType::COMPARATOR, start, current_position);
                break;
            }
        case 4:
            switch (*this->current_position++) {
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
                this->current_position--;
                // this->appendTokenList(TokenType::INT_CONSTANT, start);
                // return TokenType::INT_CONSTANT;
                return Token(TokenType::INT_CONSTANT, start, current_position);
                break;
            }
            break;
        case 5:
            switch (*this->current_position++) {
            case '=':
                // this->appendTokenList(TokenType::COMPARATOR, start);
                // return TokenType::COMPARATOR;
                return Token(TokenType::COMPARATOR, start, current_position);
                break;
            default:
                return Token(TokenType::INVALID, start, current_position);
                break;
            }
            break;
        case 6:
            switch (*this->current_position++) {
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
                return Token(TokenType::INVALID, start, current_position);
                break;
            }
            break;
        case 7:
            switch (*this->current_position++) {
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
                this->current_position--;
                // this->appendTokenList(TokenType::FLOAT_CONSTANT, start);
                // return TokenType::FLOAT_CONSTANT;
                return Token(TokenType::FLOAT_CONSTANT, start, current_position);
                break;
            }
            break;
        case 8:
            switch (*this->current_position++) {
            case '=':
                // this->appendTokenList(TokenType::COMPARATOR, start);
                // return TokenType::COMPARATOR;
                // return Token(TokenType::COMPARATOR, start, current_position);
                break;
            default:
                this->current_position--;
                // this->appendTokenList(TokenType::ASSIGNMENT, start);
                // return TokenType::ASSIGNMENT;
                return Token(TokenType::ASSIGNMENT, start, current_position);
                break;
            }
            break;
        default:
            break;
        }
    }
    return Token(TokenType::END_OF_FILE, start, current_position);
    // return nullptr;
}

bool LexicalAnalyser::inTokenList(const std::string &value) const {
    if (this->token_list().size() <= 0) {
        return false;
    }

    for (auto token : this->token_list_) {
        if (token->value() == value) {
            return true;
        }
    }

    return false;
}

const std::unordered_map<std::string, std::list<std::pair<uint, uint>>> LexicalAnalyser::symbol_table() const {
    return this->symbol_table_;
}

const std::vector<Token *> LexicalAnalyser::token_list() const { return this->token_list_; }

// Não coloca duplicatas
void LexicalAnalyser::appendTokenList(TokenType type, std::string::iterator start) {
    std::string value = "";
    value.assign(start, this->current_position);
    std::string new_token_msg = "(" + std::to_string(this->row) + ", " + std::to_string(this->col) + ") " + " Token " +
                                lexical_analyser::to_string(type) + " encontrado: " + value;
    this->log_.write(new_token_msg);

    if (type == TokenType::IDENT) {
        this->symbol_table_[value].push_back(std::make_pair(row, col));
    }

    if (this->inTokenList(value)) {
        this->log_.write("Token já está na lista.");
        return;
    }

    this->token_list_.push_back(new Token(type, value));
    value.clear();
}

} // namespace lexical_analyser
