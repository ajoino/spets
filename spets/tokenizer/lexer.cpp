#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <format>

#include <tokenizer/ctre.hpp>
#include <tokenizer/lexer.hpp>

const char* token_type_to_string(TokenType tok) noexcept {
    switch (tok) {
    case TokenType::NAME:
        return "NAME";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::WHITESPACE:
        return "WS";
    case TokenType::LPAREN:
        return "LPAREN";
    case TokenType::RPAREN:
        return "RPAREN";
    case TokenType::LBRACK:
        return "LBRACK";
    case TokenType::RBRACK:
        return "RBRACK";
    case TokenType::LCURL:
        return "LCURL";
    case TokenType::RCURL:
        return "RCURL";
    case TokenType::DOUBLEQUOTE:
        return "DOUBLEQUOTE";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::DOT:
        return "DOT";
    case TokenType::BANG:
        return "BANG";
    case TokenType::PIPE:
        return "PIPE";
    case TokenType::STAR:
        return "STAR";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::LEFTARROW:
        return "LEFTARROW";
    case TokenType::INDENT:
        return "INDENT";
    case TokenType::UNINDENT:
        return "UNINDENT";
    case TokenType::NEWLINE:
        return "NEWLINE";
    case TokenType::NL:
        return "NL";
    case TokenType::COMMENT:
        return "COMMENT";
    case TokenType::ENDOFFILE:
        return "ENDOFFILE";
    case TokenType::ERROR:
        return "ERROR";
    };
}

/* *
 * Lexer member functions
 * */

bool Lexer::is_at_end() const { return current == end; }

char Lexer::advance() {
    ++current;
    return *(current - 1);
}

char Lexer::peek() const { return *current; }

void Lexer::push_indent(int steps) { indent_depth_stack.push_back(steps); }

int Lexer::get_indent() const {
    if (indent_depth_stack.empty()) {
        return 0;
    }
    return indent_depth_stack.back();
}

int Lexer::pop_indent() {
    const auto tos = indent_depth_stack.back();
    indent_depth_stack.pop_back();
    return tos;
}

/* *
 * Token member functions
 * */

Token Token::from_lexer(TokenType type, Lexer& lexer) noexcept {
    std::string token_string;
    std::string raw_lexer_string = std::string{lexer.start, lexer.current};
    if (raw_lexer_string.starts_with('\n')) {
        token_string = "\\n";
    } else {
        token_string = raw_lexer_string;
    }
    Token ret_token = Token{type, token_string, lexer.line, lexer.column};
    lexer.column += static_cast<int>(lexer.current - lexer.start);
    return ret_token;
}

std::ostream& operator<<(std::ostream& os, Token const& t) {
    return os << "Token " << token_type_to_string(t.type) << ", line: " << t.line << ", column: " << t.column
              << ", value: \"" << t.value << "\"";
}

/* *
 * Scan functions
 * */

const std::set OPERATOR_CHARS{'+', '-', '*', '/', '=', '!', '.', '<'};

bool is_whitespace_char(const char ch) { return ch == ' ' || ch == '\t' || ch == '\r'; }

bool is_operator_char(const char ch) { return OPERATOR_CHARS.contains(ch); }

ScanResult scan_whitespace(Lexer& lexer) {
    for (;;) {
        char c = lexer.peek();
        if (is_whitespace_char(c)) {
            lexer.advance();
        } else {
            return {Token::from_lexer(TokenType::WHITESPACE, lexer), lexer};
        }
    }
}

int skip_leading_spaces(Lexer& lexer) {
    int skipped_spaces = 0;
    for (;;) {
        char c = lexer.peek();
        if (c == ' ') {
            lexer.advance();
            skipped_spaces++;
        } else {
            break;
        }
    }
    return skipped_spaces;
}

ScanResult scan_token(Lexer& lexer) {
    lexer.start = lexer.current;
    if (lexer.is_at_end()) {
        auto token = Token::from_lexer(TokenType::ENDOFFILE, lexer);
        return ScanResult(token, lexer);
    }
    if (lexer.column == 0) {
        int current_indent = skip_leading_spaces(lexer);
        int last_indent = lexer.get_indent();
        if (lexer.peek() == '\n') {
            lexer.advance();
            auto token = Token::from_lexer(TokenType::NL, lexer);
            lexer.line++;
            lexer.column = 0;
            return ScanResult{token, lexer};
        }
        if (current_indent < last_indent) {
            lexer.pop_indent();
            auto token = Token::from_lexer(TokenType::UNINDENT, lexer);
            lexer.column = current_indent;
            return ScanResult{token, lexer};
        }
        if (current_indent > last_indent) {
            lexer.push_indent(current_indent);
            auto token = Token::from_lexer(TokenType::INDENT, lexer);
            lexer.column = current_indent;
            return ScanResult{token, lexer};
        }
        if (current_indent == last_indent && current_indent > 0) {
            lexer.start = lexer.current;
        }
    }

    char c = lexer.advance();

    // Single-character tokens
    switch (c) {
    case '\n': {
        // Whitespace significance means newlines needs complex handling.
        auto token = Token::from_lexer(TokenType::NEWLINE, lexer);
        lexer.column = 0;
        lexer.line++;
        return ScanResult(token, lexer);
        // return scan_newline(lexer);
    }
    case ' ': {
        return scan_whitespace(lexer);
    }
    case '(':
        return {Token::from_lexer(TokenType::LPAREN, lexer), lexer};
    case ')':
        return {Token::from_lexer(TokenType::RPAREN, lexer), lexer};
    case '[':
        return {Token::from_lexer(TokenType::LBRACK, lexer), lexer};
    case ']':
        return {Token::from_lexer(TokenType::RBRACK, lexer), lexer};
    case '{':
        return {Token::from_lexer(TokenType::LCURL, lexer), lexer};
    case '}':
        return {Token::from_lexer(TokenType::RCURL, lexer), lexer};
    case ',':
        return {Token::from_lexer(TokenType::COMMA, lexer), lexer};
    case ':':
        return {Token::from_lexer(TokenType::SEMICOLON, lexer), lexer};
    case '.':
        return {Token::from_lexer(TokenType::DOT, lexer), lexer};
    case '!':
        return {Token::from_lexer(TokenType::BANG, lexer), lexer};
    case '|':
        return {Token::from_lexer(TokenType::PIPE, lexer), lexer};
    case '*':
        return {Token::from_lexer(TokenType::STAR, lexer), lexer};
    case '/':
        return {Token::from_lexer(TokenType::SLASH, lexer), lexer};
    default:
        break;
    }

    if (auto res = ctre::starts_with<R"(#[^\n])">(lexer.start, lexer.end)) {
        for (int i = 1; i < res.size(); i++) {
            lexer.advance();
        }
        return ScanResult{Token::from_lexer(TokenType::COMMENT, lexer), lexer};
    }
    if (auto res = ctre::starts_with<R"(([_a-zA-Z][_a-zA-Z0-9]*)|([<>*+=/\-|]+))">(lexer.start, lexer.end)) {
        // Identifiers
        for (int i = 1; i < res.size(); i++) {
            lexer.advance();
        }
        return ScanResult{Token::from_lexer(TokenType::NAME, lexer), lexer};
    }
    if (auto res = ctre::starts_with<
            R"(("""[\w+\-*/()=:<>\[\]]*""")|('''[\w+\-*/()=:<>\[\]]*''')|("[\w+\-*/()=:<>\[\]]*")|('[\w+\-*/()=:<>\[\]]*'))">(
            lexer.start, lexer.end
        )) {
        // String literals
        std::cout << "res: " << res << "\n";
        for (int i = 1; i < res.size(); i++) {
            lexer.advance();
        }
        return ScanResult{Token::from_lexer(TokenType::STRING, lexer), lexer};
    }
    if (auto res = ctre::starts_with<"[0-9]+(\\.[0-9]+)?([eE]-?[0-9]+)?">(lexer.start, lexer.end)) {
        for (int i = 1; i < res.size(); i++) {
            lexer.advance();
        }
        return ScanResult{Token::from_lexer(TokenType::NUMBER, lexer), lexer};
    }
    // If the lexer matches nothing, return an error.
    std::string error_message = std::string("Unexpected string: \"") + std::string(lexer.start, lexer.current) + "\"";
    lexer.column++;
    return ScanResult{Token(TokenType::ERROR, error_message, lexer.line, lexer.column), lexer};
}
