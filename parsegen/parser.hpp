#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <set>
#include <variant>

#include "lexer.hpp"
#include "node.hpp"

namespace rr = std::ranges;

// namespace rv = rr::views;
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

        friend std::ostream& operator<<(std::ostream& os, const Head& h) {
            os << "Head(" << h.rule;
            for (const auto& is : h.involved_set) {
                os << ", " << is;
            }
            for (const auto& es : h.eval_set) {
                os << ", " << es;
            }
            return os << ")";
        }
    };

    struct LR {
        std::optional<Node> seed;
        std::string rule;
        std::optional<Head> head;

        friend std::ostream& operator<<(std::ostream& os, const LR& lr) {
            os << "LR(" << lr.seed << ", " << lr.rule << ", ";
            if (!lr.head) {
                os << "std::nullopt";
            } else {
                os << lr.head.value();
            }
            return os << ")";
        }
    };

    Tokenizer tokenizer;
    std::map<int, Head> heads;
    std::vector<std::shared_ptr<LR>> lr_stack;

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
                os << std::get<std::shared_ptr<LR>>(mv.res);
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

    void setup_lr(const std::string& rule_name, std::shared_ptr<LR>& L) {
        std::cout << "Calling setup_lr for rule " << rule_name << "\n";
        if (!L->head) {
            L->head = Head(rule_name, {}, {});
        }
        for (auto& s : std::ranges::reverse_view{lr_stack}) {
            if (s->head != L->head) {
                break;
            }
            s->head = L->head;
            L->head->involved_set.insert(s->rule);
        }
    }

    std::optional<Node> lr_answer(
        const std::string& rule_name, std::function<std::optional<Node>()> func, const int pos, const MemoKey& k
    ) {
        std::cout << "Calling lr_answer for rule: " << rule_name << " at pos: " << pos << "\n";
        auto get_lr_res = [&, this]() { return std::get<std::shared_ptr<LR>>(memo[k].res); };
        auto h = get_lr_res()->head.value();
        if (h.rule != rule_name) {
            return get_lr_res()->seed;
        } else {
            memo[k].res = get_lr_res()->seed;
            if (!std::get<std::optional<Node>>(memo[k].res)) {
                return {};
            } else {
                return grow_lr(rule_name, func, pos, k, h);
            }
        }
    }

    void recall(const std::string& rule_name, std::function<std::optional<Node>()> func, const int pos) {
        auto key = MemoKey(rule_name, pos);
        // If not growing a seed parse, just return what is stored
        // in the memo table.
        std::cout << "Calling recall for rule: " << rule_name << " at pos: " << pos << "\n";
        if (!heads.contains(pos)) {
            return;
        }

        // Do not evaluate any rule that is not involved in this
        // left recursion
        if (!memo.contains(key) && (rule_name == heads[pos].rule || heads[pos].involved_set.contains(rule_name))) {
            memo[key] = MemoValue{std::nullopt, pos};
            return;
        }

        // Allow involved rules to be evaluate but only once,
        // during a seed-growing iteration
        if (heads[pos].eval_set.contains(rule_name)) {
            std::vector<std::string> diff_vec{};
            std::ranges::set_difference(heads[pos].eval_set, std::set{rule_name}, std::back_inserter(diff_vec));
            heads[pos].eval_set
                = std::set(std::make_move_iterator(diff_vec.begin()), std::make_move_iterator(diff_vec.end()));

            auto res = func();
            memo[key].res = res;
            memo[key].endpos = mark();
        }

        return;
    }

    std::optional<Node> grow_lr(
        const std::string& func_name, std::function<std::optional<Node>()> func, const int pos, MemoKey K, const Head& H
    ) {
        std::cout << "Calling `grow_lr` for rule: " << func_name << " at pos: " << pos << "\n";
        heads[pos] = H;
        while (true) {
            reset(pos);
            heads[pos].eval_set = std::set<std::string>{heads[pos].involved_set.begin(), heads[pos].involved_set.end()};
            auto res = func();
            if (res == std::nullopt || mark() <= memo[K].endpos) {
                break;
            }
            memo[K].res = res;
            memo[K].endpos = mark();
        }
        heads.erase(pos);
        reset(memo[K].endpos);
        return std::get<std::optional<Node>>(memo[K].res);
    }

    std::optional<Node>
    memoize(const std::string& func_name, std::function<std::optional<Node>()> func, const int pos) {
        std::cout << "Calling memoization routine for rule: " << func_name << " at pos: " << pos << "\n";
        auto key = MemoKey(func_name, pos);
        // memo[key] = recall(func_name, func, pos);
        recall(func_name, func, pos);
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
            // Create a new LR and push it onto the rule invocation stack.
            auto lr = std::make_shared<LR>(std::nullopt, func_name, std::nullopt);
            lr_stack.push_back(lr);

            // Memoize lr and evaluate parsing function
            // m = MemoValue(std::make_shared<LR>(lr), pos);
            memo[key] = MemoValue(lr, pos);
            std::cout << "Calling parsing function for rule: " << func_name << " at pos: " << pos << "\n";
            auto res = func();
            std::cout << "Result of calling parsing function for rule: " << func_name << " at pos: " << pos << "\n\t";
            // Pop lr off the rule invocation stack
            lr_stack.pop_back();
            memo[key].endpos = mark();
            if (res) {
                std::cout << res.value() << "\n";
            } else {
                std::cout << "std::nullopt"
                          << "\n";
            }
            if (std::get<std::shared_ptr<LR>>(memo[key].res)->head) {
                std::get<std::shared_ptr<LR>>(memo[key].res)->seed = res.value();
                return lr_answer(func_name, func, pos, key);
            } else {
                memo[key].res = res;
                return res;
            }
            // memo[key].res = res;
            // memo[key].endpos = mark();
            // std::cout << "\tendpos: " << memo[key].endpos << "\n";
            // if (lr->detected && res != std::nullopt) {
            //     std::cout << "Left recursion detected in rule: " << func_name << " at pos: " << pos << "\n";
            //     return grow_lr(func_name, func, pos, key, NULL);
            // } else {
            //     std::cout << "No left recursion detected in rule: " << func_name << " at pos: " << pos << "\n";
            //     return res;
            // }
        } else {
            std::cout << "Resetting pos to " << memo[key].endpos << "\n";
            reset(memo[key].endpos);
            std::cout << "pos is now: " << mark() << "\n";
            if (std::holds_alternative<std::shared_ptr<LR>>(memo[key].res)) {
                // std::cout << "LR before: " << std::get<std::shared_ptr<LR>>(memo[key].res)->detected << "\n";
                // std::get<std::shared_ptr<LR>>(memo[key].res)->detected = true;
                // std::cout << "LR after: " << std::get<std::shared_ptr<LR>>(memo[key].res)->detected << "\n";
                // return std::nullopt;
                setup_lr(func_name, std::get<std::shared_ptr<LR>>(memo[key].res));
                return std::get<std::shared_ptr<LR>>(memo[key].res)->seed;
            }
            return std::get<std::optional<Node>>(memo[key].res);
        }
    }

public:

    Parser(Tokenizer t) : tokenizer{t}, heads{}, lr_stack{} {};

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
