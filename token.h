#ifndef TOKEN_H
#define TOKEN_H
#include <cstring>



class Token {
public:
    enum class TokenType {
        kIdent = 1,
        kIntConstant,
        kFloatConstant,
        kStringConstant,
        kComparator,
        kHighPriorityOperator,
        kLowPriorityOperator,
        kSemicolon,
        kAssignment,
        kComma,
        kOpenCurlyBrace,
        kCloseCurlyBrace,
        kOpenSquareBracket,
        kCloseSquareBracket,
        kOpenParenthesis,
        kCloseParenthesis
    };

    Token(TokenType type, char *start, char *end) {
        std::size_t len = end - start;
        this->p = new char[len + 1];
        std::memcpy(this->p, start, len);
        this->p[len] = '\0';
        this->type_ = type;
    }

    ~Token() {
        delete[] p;
    }

    char *value() {
        return p;
    }

    TokenType type() {
        return type_;
    }

private:
    TokenType type_;
    char *p;
};


#endif // TOKEN_H
