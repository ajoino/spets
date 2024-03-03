#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <vector>

enum class TokenType {
  NAME,
  STRING,
  NUMBER,
  WHITESPACE,
  LPAREN,
  RPAREN,
  DOUBLEQUOTE,
  COMMA,
  SEMICOLON,
  DOT,
  BANG,
  PIPE,
  STAR,
  INDENT,
  UNINDENT,
  NEWLINE,
  ENDOFFILE,
  ERROR,
};
const char *token_type_to_string(TokenType t) noexcept;

struct Lexer {
  std::string::iterator start;
  std::string::iterator end;
  std::string::iterator current;
  int line;
  int column;
  std::vector<int> indent_depth_stack;

  Lexer(std::string &buffer)
      : start(buffer.begin()), end(buffer.end()), current(buffer.begin()),
        line(1), column(1){};

  ~Lexer() = default;

  Lexer(const Lexer &other)
      : start(other.start), end(other.end), current(other.current),
        line(other.line), column(other.column),
        indent_depth_stack(other.indent_depth_stack){};
  Lexer &operator=(const Lexer &other) {
    this->start = other.start;
    this->end = other.end;
    this->current = other.current;
    this->line = other.line;
    this->column = other.column;
    this->indent_depth_stack = other.indent_depth_stack;
    return *this;
  }

  Lexer(const Lexer &&) = delete;
  Lexer &operator=(const Lexer &&) = delete;

  const bool is_at_end(); // { return current == end; }
  const char advance();
  const char peek();
  const std::string match(const std::string_view s);

  void push_indent(int steps);
  const int get_indent();
  const int pop_indent();
};

struct Token {
  TokenType type;
  const std::string value;
  int line;
  int column;

  Token(const TokenType &token_type, const std::string &value, const int line,
        const int column)
      : type(token_type), value(value), line(line), column(column){};
  Token(const Token &new_token) = default;
  Token operator=(Token other) {
    return Token{other.type, other.value, other.line, other.column};
  }
  Token &operator=(Token &other) { return other; };

  static Token from_lexer(TokenType type, Lexer &lexer) noexcept;
  friend std::ostream &operator<<(std::ostream &os, Token const &t);
};

struct ScanResult {
  Token token;
  Lexer lexer;
};

ScanResult scan_token(Lexer &lexer);

class Tokenizer {
  Lexer lexer;
  std::vector<Token> tokens;
  int pos;

public:
  Tokenizer(Lexer lexer) : lexer{lexer}, tokens{}, pos{0} {};

  int mark() const { return pos; }
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

#endif // !__LEXER_H__
