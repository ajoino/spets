#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <set>
#include <utility>
#include <variant>
#include <any>

#include <tokenizer/lexer.hpp>
#include <parser/node.hpp>

namespace rr = std::ranges;

std::ostream& operator<<(std::ostream& os, std::optional<Node> maybe_node);

struct Head {
    std::string rule;
    std::set<std::string> involved_set;
    std::set<std::string> eval_set;

    auto operator<=>(const Head&) const = default;
};

struct LR {
    std::optional<std::any> seed;
    std::string rule;
    std::optional<Head> head;
};

struct MemoKey {
    std::string func_name;
    int pos;

    constexpr bool operator<(const MemoKey& rhs) const;

    constexpr bool operator==(const MemoKey& rhs) const;
};

struct MemoValue {
    std::variant<std::shared_ptr<LR>, std::optional<std::any>> res;
    int endpos;
};

class Parser {

    Tokenizer tokenizer;
    std::map<int, Head> heads;
    std::vector<std::shared_ptr<LR>> lr_stack;
    std::map<MemoKey, MemoValue> memo;

protected:

    inline Token get_token() { return tokenizer.get_token(); }
    inline bool is_at_end() { return tokenizer.is_at_end(); }

    inline bool skip_ws() {
        if (expect(TokenType::WHITESPACE)) {}
        return true;
    }

    inline bool skip_nl() {
        while (expect(TokenType::NEWLINE) || expect(TokenType::INDENT)) {}
        return true;
    }

    inline bool skip(TokenType tt) {
        expect(tt);
        return true;
    }

    inline bool skip_while(TokenType tt) {
        while (expect(tt)) {}
        return true;
    }

    inline bool skip_until(TokenType tt) {
        while (!expect(tt)) {}
        return true;
    }

    inline bool skip(const std::string& s) {
        expect(s);
        return true;
    }

    inline bool skip_while(const std::string& s) {
        while (expect(s)) {}
        return true;
    }

    inline bool skip_until(const std::string& s) {
        while (!expect(s)) {}
        return true;
    }

    void setup_lr(const std::string& rule_name, std::shared_ptr<LR>& L);

    std::optional<std::any> lr_answer(
        const std::string& rule_name, const std::function<std::optional<std::any>()>& func, const int pos, const MemoKey& k
    );

    void recall(const std::string& rule_name, const std::function<std::optional<std::any>()>& func, const int pos);

    std::optional<std::any> grow_lr(
        const std::string& func_name, const std::function<std::optional<std::any>()>& func, const int pos, const MemoKey& K,
        const Head& H
    );

    std::optional<std::any>
    memoize(const std::string& func_name, const std::function<std::optional<std::any>()>& func, const int pos);

public:

    Parser(const Tokenizer& t) : tokenizer{t} {};

    using ParseMethod = std::function<std::optional<Node>()>;

    int mark() { return tokenizer.mark(); }

    void reset(int pos) { tokenizer.reset(pos); }

    std::optional<Token> expect(TokenType t);

    std::optional<Token> expect(const std::string& s);

    template <typename R, typename ...Args>
    bool lookahead(bool positive, std::function<std::optional<R>(Args...)> func, Args... args);
};
