
#include "token_type.hpp"
#include <string>

namespace lexical_analyser {

std::string toString(TokenType tt) {
    switch (tt) {
    case TokenType::IDENT:
        return "IDENT";
    case TokenType::INT_CONSTANT:
        return "INT_CONSTANT";
    case TokenType::FLOAT_CONSTANT:
        return "FLOAT_CONSTANT";
    case TokenType::STRING_CONSTANT:
        return "STRING_CONSTANT";
    case TokenType::COMPARATOR:
        return "COMPARATOR";
    case TokenType::HIGHEST_PRIORITY_OPERATOR:
        return "HIGHEST_PRIORITY_OPERATOR";
    case TokenType::HIGH_PRIORITY_OPERATOR:
        return "HIGH_PRIORITY_OPERATOR";
    case TokenType::LOW_PRIORITY_OPERATOR:
        return "LOW_PRIORITY_OPERATOR";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::ASSIGNMENT:
        return "ASSIGNMENT";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::OPEN_CURLY_BRACE:
        return "OPEN_CURLY_BRACE";
    case TokenType::CLOSE_CURLY_BRACE:
        return "CLOSE_CURLY_BRACE";
    case TokenType::OPEN_SQUARE_BRACKET:
        return "OPEN_SQUARE_BRACKET";
    case TokenType::CLOSE_SQUARE_BRACKET:
        return "CLOSE_SQUARE_BRACKET";
    case TokenType::OPEN_PARENTHESIS:
        return "OPEN_PARENTHESIS";
    case TokenType::CLOSE_PARENTHESIS:
        return "CLOSE_PARENTHESIS";
    case TokenType::RESERVED_WORD_DEF:
        return "RESERVED_WORD_DEF";
    case TokenType::RESERVED_WORD_BREAK:
        return "RESERVED_WORD_BREAK";
    case TokenType::RESERVED_WORD_NEW:
        return "RESERVED_WORD_NEW";
    case TokenType::RESERVED_WORD_CALL:
        return "RESERVED_WORD_CALL";
    case TokenType::RESERVED_WORD_NULL:
        return "RESERVED_WORD_NULL";
    case TokenType::RESERVED_WORD_RETURN:
        return "RESERVED_WORD_RETURN";
    case TokenType::RESERVED_WORD_READ:
        return "RESERVED_WORD_READ";
    case TokenType::RESERVED_WORD_PRINT:
        return "RESERVED_WORD_PRINT";
    case TokenType::RESERVED_WORD_ELSE:
        return "RESERVED_WORD_ELSE";
    case TokenType::RESERVED_WORD_IF:
        return "RESERVED_WORD_IF";
    case TokenType::RESERVED_WORD_FOR:
        return "RESERVED_WORD_FOR";
    case TokenType::RESERVED_WORD_INT:
        return "RESERVED_WORD_INT";
    case TokenType::RESERVED_WORD_FLOAT:
        return "RESERVED_WORD_FLOAT";
    case TokenType::RESERVED_WORD_STRING:
        return "RESERVED_WORD_STRING";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    default:
        return "UNKNOWN";
    }
}

} // namespace lexical_analyser