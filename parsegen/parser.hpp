#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <variant>

#include "lexer.hpp"
#include "node.hpp"

std::ostream& operator<<(std::ostream& os, std::optional<Node> maybe_node) {
    if (maybe_node) {
        return os << maybe_node.value();
    }
    return os << std::string{"std::nullopt"};
}

class Parser {
    struct Head {
        std::string rule;
        std::set<std::string> involved_set;
        std::set<std::string> eval_set;

        auto operator<=>(const Head&) const = default;
    };

    struct LR {
        // bool detected;
        Node seed;
        std::string rule;
        std::optional<Head> head;
        std::unique_ptr<LR> next;
    };

    Tokenizer tokenizer;
    std::map<int, Head> heads;
    std::unique_ptr<LR> lr_stack;

protected:

    // std::map<int, std::map<std::string, std::pair<std::optional<Node>, int>>>
    //     memos;
    struct MemoKey {
        std::string func_name;
        int pos;

        inline constexpr bool operator<(const MemoKey& rhs) const {
            if (this->func_name == rhs.func_name) {
                return this->pos < rhs.pos;
            } else {
                return this->func_name < rhs.func_name;
            }
        }

        inline constexpr bool operator==(const MemoKey& rhs) const {
            return (this->func_name == rhs.func_name && this->pos == rhs.pos);
        }

        friend std::ostream& operator<<(std::ostream& os, const MemoKey& mk) {
            return os << "(" << mk.pos << ", " << mk.func_name << ")";
        }
    };

    struct MemoValue {
        std::variant<std::shared_ptr<LR>, std::optional<Node>> res;
        int endpos;

        friend std::ostream& operator<<(std::ostream& os, const MemoValue& mv) {
            os << "(";
            if (std::holds_alternative<std::shared_ptr<LR>>(mv.res)) {
                os << (std::get<std::shared_ptr<LR>>(mv.res)->detected ? "LR(true)" : "LR(false)");
            } else if (auto r = std::get<std::optional<Node>>(mv.res)) {
                os << r.value();
            } else {
                os << "std::nullopt";
            }
            os << ", " << mv.endpos << ")";
            return os;
        }
    };

    std::map<MemoKey, MemoValue> memo;

    bool skip_ws() {
        if (expect(TokenType::WHITESPACE)) {}
        return true;
    }

    void setup_lr(std::string& rule_name, LR& L) {
        if (!L.head) {
            L.head = Head(rule_name, {}, {});
        }
        auto& s = lr_stack;
        while (s->head != L.head) {
            s->head = L.head;
            L.head->involved_set.insert(s->rule);
            s = std::move(s->next);
        }
    }

    std::optional<Node>
    grow_lr(const std::string& func_name, std::function<std::optional<Node>()> func, const int pos, MemoKey K, int* H) {
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

    std::optional<Node>
    memoize(const std::string& func_name, std::function<std::optional<Node>()> func, const int pos) {
        std::cout << "Calling memoization routine for rule : " << func_name << " at pos: " << pos << "\n";
        auto key = MemoKey(func_name, pos);
        for (const auto& [k, m] : memo) {
            std::cout << "\t" << k << " -> " << m;
            if (k.pos == pos && k.func_name == func_name) {
                std::cout << " <- currently under consideration\n";
            } else {
                std::cout << "\n";
            }
        }

        if (!memo.contains(key)) {
            std::cout << "Memoization cache is empty.\n";
            auto lr = std::make_shared<LR>(false);
            memo[key] = MemoValue(lr, pos);
            std::cout << "Calling parsing function for rule: " << func_name << " at pos: " << pos << "\n";
            auto res = func();
            std::cout << "Result of calling parsing function for rule: " << func_name << " at pos: " << pos << "\n\t";
            if (res) {
                std::cout << res.value() << "\n";
            } else {
                std::cout << "std::nullopt"
                          << "\n";
            }
            memo[key].res = res;
            memo[key].endpos = mark();
            std::cout << "\tendpos: " << memo[key].endpos << "\n";
            if (lr->detected && res != std::nullopt) {
                std::cout << "Left recursion detected in rule: " << func_name << " at pos: " << pos << "\n";
                return grow_lr(func_name, func, pos, key, NULL);
            } else {
                std::cout << "No left recursion detected in rule: " << func_name << " at pos: " << pos << "\n";
                return res;
            }
        } else {
            reset(memo[key].endpos);
            if (std::holds_alternative<std::shared_ptr<LR>>(memo[key].res)) {
                std::cout << "LR before: " << std::get<std::shared_ptr<LR>>(memo[key].res)->detected << "\n";
                std::get<std::shared_ptr<LR>>(memo[key].res)->detected = true;
                std::cout << "LR after: " << std::get<std::shared_ptr<LR>>(memo[key].res)->detected << "\n";
                return std::nullopt;
            }
            return std::get<std::optional<Node>>(memo[key].res);
        }
    }

public:

    Parser(Tokenizer t) : tokenizer{t}, heads{} {};

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

    std::optional<Token> expect(const std::string& s) {
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
