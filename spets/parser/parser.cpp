#include <optional>
#include <ostream>
#include <ranges>

#include "parser/node.hpp"
#include "parser/parser.hpp"

std::ostream& operator<<(std::ostream& os, std::optional<Node> maybe_node) {
    if (maybe_node) {
        return os << maybe_node.value();
    }
    return os << std::string{"std::nullopt"};
}

std::ostream& operator<<(std::ostream& os, const Head& h) {
    os << "Head(" << h.rule;
    for (const auto& is : h.involved_set) {
        os << ", " << is;
    }
    for (const auto& es : h.eval_set) {
        os << ", " << es;
    }
    return os << ")";
}

// std::ostream& operator<<(std::ostream& os, const LR& lr) {
//     os << "LR(" << lr.seed << ", " << lr.rule << ", ";
//     if (!lr.head) {
//         os << "std::nullopt";
//     } else {
//         os << lr.head.value();
//     }
//     return os << ")";
// }

std::ostream& operator<<(std::ostream& os, const MemoKey& mk) {
    return os << "(" << mk.pos << ", " << mk.func_name << ")";
}

std::ostream& operator<<(std::ostream& os, const MemoValue& mv) {
    os << "(";
    if (std::holds_alternative<std::shared_ptr<LR>>(mv.res)) {
        os << std::get<std::shared_ptr<LR>>(mv.res);
    } else if (auto r = std::get<std::optional<std::any>>(mv.res)) {
        os << "value of type `" << r->type().name() << "`";
    } else {
        os << "std::nullopt";
    }
    os << ", " << mv.endpos << ")";
    return os;
}

constexpr bool MemoKey::operator<(const MemoKey& rhs) const {
    if (this->pos == rhs.pos) {
        return this->func_name < rhs.func_name;
    }
    return this->pos < rhs.pos;
}

constexpr bool MemoKey::operator==(const MemoKey& rhs) const {
    return (this->func_name == rhs.func_name && this->pos == rhs.pos);
}

void Parser::setup_lr(const std::string& rule_name, std::shared_ptr<LR>& L) {
    std::cout << "Calling setup_lr for rule " << rule_name << "\n";
    if (!L->head) {
        L->head = Head(rule_name, {}, {});
    }
    for (auto& s : std::ranges::views::reverse(lr_stack)) {
        if (s->head != L->head) {
            std::cout << "Finished setup_lr call\n";
            break;
        }
        s->head = L->head;
        L->head->involved_set.insert(s->rule);
    }
}

std::optional<std::any> Parser::lr_answer(
    const std::string& rule_name, const std::function<std::optional<std::any>()>& func, const int pos, const MemoKey& k
) {
    std::cout << "Calling lr_answer for rule: " << rule_name << " at pos: " << pos << "\n";
    auto get_lr_res = [&, this]() { return std::get<std::shared_ptr<LR>>(memo[k].res); };
    auto h = get_lr_res()->head.value();
    if (h.rule != rule_name) {
        return get_lr_res()->seed;
    }
    memo[k].res = get_lr_res()->seed;
    if (!std::get<std::optional<std::any>>(memo[k].res)) {
        return {};
    }
    return grow_lr(rule_name, func, pos, k, h);
}

void Parser::recall(const std::string& rule_name, const std::function<std::optional<std::any>()>& func, const int pos) {
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
}

std::optional<std::any> Parser::grow_lr(
    const std::string& func_name, const std::function<std::optional<std::any>()>& func, const int pos, const MemoKey& K,
    const Head& H
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
    return std::get<std::optional<std::any>>(memo[K].res);
}

std::optional<std::any>
Parser::memoize(const std::string& func_name, const std::function<std::optional<std::any>()>& func, const int pos) {
    std::cout << "Calling memoization routine for rule: " << func_name << " at pos: " << pos << "\n";
    auto key = MemoKey(func_name, pos);
    recall(func_name, func, pos);
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
        // Pop lr off the rule invocation stack
        lr_stack.pop_back();
        memo[key].endpos = mark();
        if (std::get<std::shared_ptr<LR>>(memo[key].res)->head) {
            std::get<std::shared_ptr<LR>>(memo[key].res)->seed = res.value();
            auto ans = lr_answer(func_name, func, pos, key);
            return ans;
        }
        memo[key].res = res;
        return res;
    }
    reset(memo[key].endpos);
    if (std::holds_alternative<std::shared_ptr<LR>>(memo[key].res)) {
        setup_lr(func_name, std::get<std::shared_ptr<LR>>(memo[key].res));
        std::cout << "Exiting memoize function\n";
        return std::get<std::shared_ptr<LR>>(memo[key].res)->seed;
    }
    return std::get<std::optional<std::any>>(memo[key].res);
}

std::optional<Token> Parser::expect(TokenType t) {
    auto token = tokenizer.peek_token();
    if (token.type == t) {
        std::cout << "Parsed token: " << token << "\n";
        return tokenizer.get_token();
    }
    return {};
}

std::optional<Token> Parser::expect(const std::string& s) {
    auto token = tokenizer.peek_token();
    if (token.value == s) {
        std::cout << "Parsed token: " << token << "\n";
        return tokenizer.get_token();
    }
    return {};
}
