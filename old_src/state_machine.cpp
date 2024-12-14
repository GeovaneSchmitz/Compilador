#include <cstdint>
#include <cstring>
#include <iostream>
#include <list>

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

class StateMachine {
   public:
    StateMachine(std::string str) {
        std::size_t len = str.length();
        this->p = new char[len + 1];
        strcpy(this->p, str.c_str());
        this->initial_state_ = p;
        this->last_state_ = p + len;
    }

    ~StateMachine();

    Token *getNextToken() {
        uint64_t state = 0;
        char *start = p;
        while (p != last_state_) {
            switch (state) {
                case 0:
                    switch (*p++) {
                        case '*':
                        case '/':
                        case '%':
                            return new Token(Token::TokenType::kHighPriorityOperator, start, p);
                            break;
                        case ',':
                            return new Token(Token::TokenType::kComma, start, p);
                            break;
                        case ')':
                            return new Token(Token::TokenType::kCloseParenthesis, start, p);
                            break;
                        case '(':
                            return new Token(Token::TokenType::kOpenParenthesis, start, p);
                            break;
                        case '!':
                            state = 5;
                            break;
                        case '<':
                        case '>':
                            state = 3;
                            break;
                        case '[':
                            return new Token(Token::TokenType::kOpenSquareBracket, start, p);
                            break;
                        case '"':
                            state = 1;
                            break;
                        case ';':
                            return new Token(Token::TokenType::kSemicolon, start, p);
                            break;
                        case ']':
                            return new Token(Token::TokenType::kCloseSquareBracket, start, p);
                            break;
                        case 'a' ... 'z':
                        case 'A' ... 'Z':
                            state = 2;
                            break;
                        case '+':
                        case '-':
                            return new Token(Token::TokenType::kLowPriorityOperator, start, p);
                            break;
                        case '{':
                            return new Token(Token::TokenType::kOpenCurlyBrace, start, p);
                            break;
                        case '}':
                            return new Token(Token::TokenType::kCloseCurlyBrace, start, p);
                            break;
                        case '0' ... '9':
                            state = 4;
                            break;
                        case '=':
                            state = 11;
                            break;
                        default:
                            start = p;
                            break;
                    }
                    break;
                case 1:
                    switch (*p++) {
                        case '"':
                            return new Token(Token::TokenType::kStringConstant, start, p);
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
                    switch (*p++) {
                        case 'a' ... 'z':
                        case 'A' ... 'Z':
                        case '0' ... '9':
                            break;
                        default:
                            p--;
                            return new Token(Token::TokenType::kIdent, start, p);
                            break;
                    }
                    break;
                case 3:
                    switch (*p++) {
                        case '=':
                            return new Token(Token::TokenType::kComparator, start, p);
                            break;
                        default:
                            p--;
                            return new Token(Token::TokenType::kComparator, start, p);
                            break;
                    }
                case 4:
                    switch (*p++) {
                        case '.':
                            state = 6;
                            break;
                        case '0' ... '9':
                            break;
                        default:
                            p--;
                            return new Token(Token::TokenType::kIntConstant, start, p);
                            break;
                    }
                    break;
                case 5:
                    switch (*p++) {
                        case '=':
                            return new Token(Token::TokenType::kComparator, start, p);
                            break;
                        default:
                            return nullptr;
                            break;
                    }
                    break;
                case 6:
                    switch (*p++) {
                        case '0' ... '9':
                            state = 7;
                            break;
                        default:
                            return nullptr;
                            break;
                    }
                    break;
                case 7:
                    switch (*p++) {
                        case '0' ... '9':
                            break;
                        default:
                            p--;
                            return new Token(Token::TokenType::kFloatConstant, start, p);
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
    char *p;
};

StateMachine::~StateMachine() {
    delete[] initial_state_;
}

int main(int argc, char *argv[]) {
    std::string str = "int main() {\n\treturn 0;\n}";
    StateMachine state_machine(str);
    
    std::list<std::string> reserved_words = {
        "def", "ident", "break", "new", "return", "read", "print",
        "else", "if", "for", "int", "float", "string", "null"
    };

    std::cout << "Input: " << str << std::endl << std::endl;

    Token *token;
    std::string *token_value;
    bool is_reserved = false;

    while ((token = state_machine.getNextToken()) != nullptr) {
        switch (token->type()) {
            case Token::TokenType::kIdent:
                is_reserved = false;
                token_value = new std::string(token->value());
                for (auto rw : reserved_words) {
                    if (rw == *token_value) {
                        std::cout << "Reserved Word: " << *token_value << std::endl;
                        is_reserved = true;
                    }
                }
                if (!is_reserved)
                    std::cout << "Ident: " << token->value() << std::endl;
                delete token_value;
                break;
            case Token::TokenType::kIntConstant:
                std::cout << "Int Constant: " << token->value() << std::endl;
                break;
            case Token::TokenType::kFloatConstant:
                std::cout << "Float Constant: " << token->value() << std::endl;
                break;
            case Token::TokenType::kStringConstant:
                std::cout << "String Constant: " << token->value() << std::endl;
                break;
            case Token::TokenType::kComparator:
                std::cout << "Comparator: " << token->value() << std::endl;
                break;
            case Token::TokenType::kHighPriorityOperator:
                std::cout << "High Priority Operator: " << token->value() << std::endl;
                break;
            case Token::TokenType::kLowPriorityOperator:
                std::cout << "Low Priority Operator: " << token->value() << std::endl;
                break;
            case Token::TokenType::kSemicolon:
                std::cout << "Semicolon: " << token->value() << std::endl;
                break;
            case Token::TokenType::kAssignment:
                std::cout << "Assignment: " << token->value() << std::endl;
                break;
            case Token::TokenType::kComma:
                std::cout << "Comma: " << token->value() << std::endl;
                break;
            case Token::TokenType::kOpenCurlyBrace:
                std::cout << "Open Curly Brace: " << token->value() << std::endl;
                break;
            case Token::TokenType::kCloseCurlyBrace:
                std::cout << "Close Curly Brace: " << token->value() << std::endl;
                break;
            case Token::TokenType::kOpenSquareBracket:
                std::cout << "Open Square Bracket: " << token->value() << std::endl;
                break;
            case Token::TokenType::kCloseSquareBracket:
                std::cout << "Close Square Bracket: " << token->value() << std::endl;
                break;
            case Token::TokenType::kOpenParenthesis:
                std::cout << "Open Parenthesis: " << token->value() << std::endl;
                break;
            case Token::TokenType::kCloseParenthesis:
                std::cout << "Close Parenthesis: " << token->value() << std::endl;
                break;
            default:
                break;
        }

        delete token;
    }
    return 0;
}