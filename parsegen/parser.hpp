#include <functional>
#include <optional>
#include <iostream>
#include <map>

#include "lexer.hpp"
#include "node.hpp"

class Parser {
  Tokenizer tokenizer;
protected:
  std::map<int, std::map<std::string, std::pair<std::optional<Node>, int>>>
      memos;

public:
  Parser(Tokenizer t) : tokenizer{t} {};

  using ParseMethod = std::function<std::optional<Node>()>;

  int mark() { return tokenizer.mark(); }
  void reset(int pos) { tokenizer.reset(pos); }

  std::optional<Token> expect(TokenType t) {
    auto token = tokenizer.peek_token();
    if (token.type == t) {
      std::cout << "Parsed token: " << token << "\n";
      return tokenizer.get_token();
    } else {
      std::cout << "Peeked token: " << token << ", requested token type: " << token_type_to_string(t) << "\n";
      return {};
    }
  }

  std::optional<Token> expect(const std::string &s) {
    auto token = tokenizer.peek_token();
    if (token.value == s) {
      std::cout << "Parsed token: " << token << "\n";
      return tokenizer.get_token();
    } else {
      std::cout << "Peeked token: " << token << ", string to match \"" << s << "\"\n";
      return {};
    }
  }
};
