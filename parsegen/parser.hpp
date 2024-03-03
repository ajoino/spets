#include <functional>
#include <iostream>
#include <map>
#include <optional>

#include "lexer.hpp"
#include "node.hpp"

class Parser {
  Tokenizer tokenizer;

protected:
  // std::map<int, std::map<std::string, std::pair<std::optional<Node>, int>>>
  //     memos;
  struct MemoKey {
    std::string func_name;
    int pos;

    inline constexpr bool operator<(const MemoKey &rhs) const {
      if (this->func_name == rhs.func_name) {
        return this->pos < rhs.pos;
      } else {
        return this->func_name < rhs.func_name;
      }
    }
  };

  struct MemoValue {
    std::optional<Node> res;
    int endpos;
  };

  std::map<MemoKey, MemoValue> memo;

  bool skip_ws() {
    if (expect(TokenType::WHITESPACE)) {
    }
    return true;
  }

  std::optional<Node> memoize(const std::string &func_name,
                              std::function<std::optional<Node>()> func,
                              const int pos) {
    auto key = MemoKey(func_name, pos);

    if (!memo.contains(key)) {
      memo[key] = MemoValue(std::nullopt, pos);
      auto res = func();
      memo[key].res = res;
      memo[key].endpos = mark();
      return res;
    } else {
      reset(memo[key].endpos);
      return memo[key].res;
    }
  }

  // std::optional<Node> memoize(const std::string &key,
  //                             std::function<std::optional<Node>()> func) {
  //   int pos = mark();
  //   int endpos;
  //   std::optional<Node> res{};
  //   if (!memos.contains(pos)) {
  //     memos[pos] = {};
  //   } else {
  //     std::cout << "pos: " << pos << ", key: " << key << "\n";
  //   }
  //   if (memos[pos].contains(key)) {
  //     std::cout << key << ": Using memoized value - \n";
  //     res = memos[pos][key].first;
  //     endpos = memos[pos][key].second;
  //     reset(endpos);
  //     if (res) {
  //       std::cout << res.value() << "\n";
  //     } else {
  //       std::cout << " std::nullopt\n";
  //     }
  //   } else {
  //     std::cout << key << ": Computing value - \n";
  //     res = func();
  //     endpos = mark();
  //     memos[pos][key] = {res, endpos};
  //     if (res) {
  //       std::cout << res.value() << "\n";
  //     } else {
  //       std::cout << " std::nullopt\n";
  //     }
  //   }
  //   return res;
  // }
  //
  // std::optional<Node>
  // memoize_left_rec(const std::string &key,
  //                  std::function<std::optional<Node>()> func) {
  //   int pos = mark();
  //   int endpos;
  //   std::optional<Node> res{};
  //   if (!memos.contains(pos)) {
  //     memos[pos] = {};
  //   } else {
  //     std::cout << "pos: " << pos << ", key: " << key << "\n";
  //   }
  //   if (memos[pos].contains(key)) {
  //     std::cout << key << ": Using memoized value - \n";
  //     res = memos[pos][key].first;
  //     endpos = memos[pos][key].second;
  //     reset(endpos);
  //     if (res) {
  //       std::cout << res.value() << "\n";
  //     } else {
  //       std::cout << " std::nullopt\n";
  //     }
  //   } else {
  //     std::cout << key << ": Computing value - \n";
  //     // Prime the cache with a failure
  //     auto [lastres, lastpos] = memos[pos][key] =
  //         std::pair<std::optional<Node>, int>{std::nullopt, pos};
  //     // Loop until parse is no longer attained
  //     while (true) {
  //       reset(pos);
  //       res = func();
  //       endpos = mark();
  //       std::cout << "endpos: " << endpos << ", lastpos: " << lastpos <<
  //       "\n"; if (res) {
  //         std::cout << res.value() << "\n";
  //       } else {
  //         std::cout << " std::nullopt\n";
  //       }
  //       if (endpos <= lastpos) {
  //         break;
  //       }
  //       lastres = res;
  //       lastpos = endpos;
  //       memos[pos][key] = {res, endpos};
  //     }
  //     res = lastres;
  //     reset(lastpos);
  //   }
  //
  //   return res;
  // }

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
      std::cout << "Peeked token: " << token
                << ", requested token type: " << token_type_to_string(t)
                << "\n";
      return {};
    }
  }

  std::optional<Token> expect(const std::string &s) {
    auto token = tokenizer.peek_token();
    if (token.value == s) {
      std::cout << "Parsed token: " << token << "\n";
      return tokenizer.get_token();
    } else {
      std::cout << "Peeked token: " << token << ", string to match \"" << s
                << "\"\n";
      return {};
    }
  }
};
