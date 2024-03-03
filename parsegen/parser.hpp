#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <variant>

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

  struct LR {
    bool detected;
  };

  struct MemoValue {
    std::variant<LR, std::optional<Node>> res;
    int endpos;
  };

  std::map<MemoKey, MemoValue> memo;

  bool skip_ws() {
    if (expect(TokenType::WHITESPACE)) {
    }
    return true;
  }

  std::optional<Node> grow_lr(const std::string &func_name,
                              std::function<std::optional<Node>()> func,
                              const int pos, MemoKey K, int *H) {
    std::cout << "Calling `grow_lr` for rule: " << func_name << " at pos: " << pos << "\n";
    // More to come
    while (true) {
      reset(pos);
      // More to come
      auto res = func();
      if (res == std::nullopt || mark() <= memo[K].endpos) {
        break;
      }
      memo[K].res = res;
      memo[K].endpos = mark();
    }
    // More to come
    reset(memo[K].endpos);
    return std::get<std::optional<Node>>(memo[K].res);
  }

  std::optional<Node> memoize(const std::string &func_name,
                              std::function<std::optional<Node>()> func,
                              const int pos) {
    std::cout << "Calling memoization routine for rule : " << func_name << " at pos: " << pos << "\n";
    auto key = MemoKey(func_name, pos);

    if (!memo.contains(key)) {
      std::cout << "Memoization cache is empty.\n";
      LR lr(false);
      memo[key] = MemoValue(lr, pos);
      std::cout << "Calling parsing function for rule: " << func_name << " at pos: " << pos << "\n";
      auto res = func();
      std::cout << "Result of calling parsing function for rule: " << func_name << " at pos: " << pos << "\n\t";
      if (res) {
        std::cout << res.value() << "\n";
      } else {
        std::cout << "std::nullopt" << "\n";
      }
      memo[key].res = res;
      memo[key].endpos = mark();
      std::cout << "\tendpos: " << memo[key].endpos << "\n";
      if (lr.detected && res != std::nullopt) {
        std::cout << "Left recursion detected in rule: " << func_name << " at pos: " << pos << "\n";
        return grow_lr(func_name, func, pos, key, NULL);
      } else {
        std::cout << "No left recursion detected in rule: " << func_name << " at pos: " << pos << "\n";
        return res;
      }
    } else {
      reset(memo[key].endpos);
      if (std::holds_alternative<LR>(memo[key].res)) {
        std::cout << "LR before: " << std::get<LR>(memo[key].res).detected
                  << "\n";
        std::get<LR>(memo[key].res).detected = true;
        std::cout << "LR after: " << std::get<LR>(memo[key].res).detected
                  << "\n";
        return std::nullopt;
      }
      return std::get<std::optional<Node>>(memo[key].res);
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
