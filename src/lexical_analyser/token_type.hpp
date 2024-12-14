#pragma once
#include <string>

namespace lexical_analyser {

enum class TokenType {
    IDENT,
    INT_CONSTANT,
    FLOAT_CONSTANT,
    STRING_CONSTANT,
    COMPARATOR,
    HIGHEST_PRIORITY_OPERATOR,
    HIGH_PRIORITY_OPERATOR,
    LOW_PRIORITY_OPERATOR,
    SEMICOLON,
    ASSIGNMENT,
    COMMA,
    OPEN_CURLY_BRACE,
    CLOSE_CURLY_BRACE,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    RESERVED_WORD_DEF,
    RESERVED_WORD_BREAK,
    RESERVED_WORD_NEW,
    RESERVED_WORD_CALL,
    RESERVED_WORD_NULL,
    RESERVED_WORD_RETURN,
    RESERVED_WORD_READ,
    RESERVED_WORD_PRINT,
    RESERVED_WORD_ELSE,
    RESERVED_WORD_IF,
    RESERVED_WORD_FOR,
    RESERVED_WORD_INT,
    RESERVED_WORD_FLOAT,
    RESERVED_WORD_STRING,
    RESERVED_WORD_NULL_TYPE,
    END_OF_FILE

};

std::string to_string(TokenType tt);

} // namespace lexical_analyser