#pragma once

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
    DEF,
    BREAK,
    NEW,
    RETURN,
    READ,
    PRINT,
    ELSE,
    IF,
    FOR,
    INT,
    FLOAT,
    STRING,
    NULL_TYPE

};

} // namespace lexical_analyser