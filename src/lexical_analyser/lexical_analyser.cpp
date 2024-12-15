#include "lexical_analyser.hpp"
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
      reserved_words{{"def", TokenType::RESERVED_WORD_DEF},       {"break", TokenType::RESERVED_WORD_BREAK},
                     {"new", TokenType::RESERVED_WORD_NEW},       {"call", TokenType::RESERVED_WORD_CALL},
                     {"null", TokenType::RESERVED_WORD_NULL},     {"return", TokenType::RESERVED_WORD_RETURN},
                     {"read", TokenType::RESERVED_WORD_READ},     {"print", TokenType::RESERVED_WORD_PRINT},
                     {"else", TokenType::RESERVED_WORD_ELSE},     {"if", TokenType::RESERVED_WORD_IF},
                     {"for", TokenType::RESERVED_WORD_FOR},       {"int", TokenType::RESERVED_WORD_INT},
                     {"float", TokenType::RESERVED_WORD_FLOAT},   {"string", TokenType::RESERVED_WORD_STRING},
                     {"null", TokenType::RESERVED_WORD_NULL_TYPE}} {}

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

Token *LexicalAnalyser::getNextToken() {
    int state = 0;
    std::string::iterator start = current_position;
    std::string new_token_msg;
    /**
     * O current_position é um ponteiro para um caractere no código-fonte atual, nesta função nós percorremos
     * caractere por caractere até conseguir montar um token válido.
     */

    while (current_position != source_code->end()) {
        this->col++;
        if (*current_position == '\n') {
            this->row++;
            this->col = 1;
        }
        switch (state) {
        case 0:
            switch (*current_position++) {
            case '*':
            case '/':
            case '%':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::HIGH_PRIORITY_OPERATOR) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::HIGH_PRIORITY_OPERATOR, start, current_position);
                break;
            case ',':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::COMMA) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::COMMA, start, current_position);
                break;
            case ')':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::CLOSE_PARENTHESIS) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::CLOSE_PARENTHESIS, start, current_position);
                break;
            case '(':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::OPEN_PARENTHESIS) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
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
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::OPEN_SQUARE_BRACKET) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::OPEN_SQUARE_BRACKET, start, current_position);
                break;
            case '"':
                state = 1;
                break;
            case ';':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::SEMICOLON) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::SEMICOLON, start, current_position);
                break;
            case ']':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::CLOSE_SQUARE_BRACKET) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
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
            case '_':
                state = 2;
                break;
            case '+':
            case '-':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::LOW_PRIORITY_OPERATOR) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::LOW_PRIORITY_OPERATOR, start, current_position);
                break;
            case '{':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::OPEN_CURLY_BRACE) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::OPEN_CURLY_BRACE, start, current_position);
                break;
            case '}':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::CLOSE_CURLY_BRACE) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
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
                state = 8;
                break;
            default:
                start = current_position;
                break;
            }
            break;
        case 1:
            switch (*current_position++) {
            case '"':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::STRING_CONSTANT) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::STRING_CONSTANT, start, current_position);
                break;
            default:
                break;
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
            case '_':
                break;
            default: {
                current_position--;
                std::string word(start, current_position);
                auto it = reserved_words.find(word);
                if (it != reserved_words.end()) {
                new_token_msg = "Token " + lexical_analyser::to_string(it->second) + " encontrado: " + std::string(start, current_position);
                    this->log_.write(new_token_msg);
                    return new Token(it->second, start, current_position);
                }
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::IDENT) + " encontrado: " + std::string(start, current_position);
                new_token_msg += "(" + std::to_string(row) + ", " + std::to_string(col) + ");";
                this->log_.write(new_token_msg);
                Token* new_token = new Token(TokenType::IDENT, start, current_position);
                this->symbol_table_[new_token->value()].push_back(std::make_pair(row, col));
                return new_token;
                break;
            }
            }
            break;
        case 3:
            switch (*current_position++) {
            case '=':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::COMPARATOR) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::COMPARATOR, start, current_position);
                break;
            default:
                current_position--;
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::COMPARATOR) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
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
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::INT_CONSTANT) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::INT_CONSTANT, start, current_position);
                break;
            }
            break;
        case 5:
            switch (*current_position++) {
            case '=':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::COMPARATOR) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
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
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::FLOAT_CONSTANT) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::FLOAT_CONSTANT, start, current_position);
                break;
            }
            break;
        case 8:
            switch (*current_position++) {
            case '=':
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::COMPARATOR) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::COMPARATOR, start, current_position);
                break;
            default:
                current_position--;
                new_token_msg = "Token " + lexical_analyser::to_string(TokenType::ASSIGNMENT) + " encontrado: " + std::string(start, current_position);
                this->log_.write(new_token_msg);
                return new Token(TokenType::ASSIGNMENT, start, current_position);
                break;
            }
            break;
        default:
            break;
        }
    }
    return nullptr;
}

const std::unordered_map<std::string, std::list<std::pair<uint, uint>>> LexicalAnalyser::symbol_table() const {
    return this->symbol_table_;
}

const std::vector<Token*> LexicalAnalyser::token_list() const {
    return this->token_list_;
}

// append_token_list não coloca duplicatas
void LexicalAnalyser::append_token_list(Token* token) {
    for (auto ptr : this->token_list_) {
        if (ptr == token) {
            this->log_.write("Tentou adicionar um token que já está na lista!!!");
            return;
        }
    }

    this->token_list_.push_back(token);
}


} // namespace lexical_analyser
