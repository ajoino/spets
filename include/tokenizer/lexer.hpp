#pragma once

#include <string>
#include <utility>
#include <vector>

enum class TokenType {
    NAME,
    STRING,
    NUMBER,
    WHITESPACE,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LCURL,
    RCURL,
    DOUBLEQUOTE,
    COMMA,
    SEMICOLON,
    DOT,
    BANG,
    PIPE,
    STAR,
    SLASH,
    INDENT,
    LEFTARROW,
    UNINDENT,
    NEWLINE,
    ENDOFFILE,
    ERROR,
};
const char* token_type_to_string(TokenType tok) noexcept;

struct Lexer {
    std::string::iterator start;
    std::string::iterator end;
    std::string::iterator current;
    int line{};
    int column{};
    std::vector<int> indent_depth_stack;

    Lexer(std::string& buffer) : start(buffer.begin()), end(buffer.end()), current(buffer.begin()){};

    ~Lexer() = default;

    Lexer(const Lexer& other) = default;
    Lexer& operator=(const Lexer& other) = default;

    Lexer(const Lexer&&) = delete;
    Lexer& operator=(const Lexer&&) = delete;

    [[nodiscard]] bool is_at_end() const;
    char advance();
    [[nodiscard]] char peek() const;

    void push_indent(int steps);
    [[nodiscard]] int get_indent() const;
    int pop_indent();
};

struct Token {
    TokenType type;
    std::string value;
    int line{};
    int column{};

    Token(const TokenType& token_type, std::string value, const int line, const int column) :
        type(token_type), value(std::move(value)), line(line), column(column){};
    Token(const Token& new_token) = default;
    Token& operator=(const Token& other) = default;
    Token(Token&&) = default;
    Token& operator=(Token&&) = default;
    Token() : type{TokenType::ERROR} {};
    ~Token() = default;

    static Token from_lexer(TokenType type, Lexer& lexer) noexcept;
    friend std::ostream& operator<<(std::ostream& os, Token const& t);
};

struct ScanResult {
    Token token;
    Lexer lexer;
};

ScanResult scan_token(Lexer& lexer);

class Tokenizer {
    Lexer lexer;
    std::vector<Token> tokens;
    int pos{};

public:

    Tokenizer(const Lexer& lexer) : lexer{lexer} {};

    [[nodiscard]] int mark() const { return pos; }
    [[nodiscard]] inline bool is_at_end() const { return lexer.is_at_end(); }

    void reset(int new_pos) { pos = new_pos; }

    Token get_token() {
        Token token = peek_token();
        pos += 1;
        return token;
    }

    Token peek_token() {
        if (pos == tokens.size()) {
            auto scan_result = scan_token(lexer);
            lexer = scan_result.lexer;
            tokens.push_back(scan_result.token);
        }
        return tokens[pos];
    }
};
