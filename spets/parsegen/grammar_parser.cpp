#include <parsegen/grammar_parser.hpp>
#include <format>

std::optional<std::vector<Rule>> GrammarParser::grammar() {
    std::cout << "\"grammar\" at pos: " << mark() << "\n";
    int pos = mark();
    if (auto rule = this->rule()) {
        std::vector<Rule> rules{rule.value()};
        while ((rule = this->rule())) {
            rules.push_back(rule.value());
        }
        if (expect(TokenType::ENDOFFILE)) {
            return rules;
        }
    }
    reset(pos);
    return {};
}

std::optional<Rule> GrammarParser::rule() {
    std::cout << "\"rule\" at pos: " << mark() << "\n";
    int pos = mark();
    std::optional<Token> name;
    if (skip_while(TokenType::UNINDENT) && (name = expect(TokenType::NAME))) {
        std::string return_type;
        if (expect("[")) {
            int level = 0;
            std::vector<std::string> return_type_tokens{};
            while(true) {
                std::string token = get_token().value;
                if (token == "[") {
                    level++;
                } else if (token == "]") {
                    level--;
                    if (level < 0) {
                        break;
                    }
                }
                return_type_tokens.push_back(token);
            }
            return_type = return_type_tokens[0];
            for (auto at = return_type_tokens.begin() + 1; at != return_type_tokens.end(); at++) {
                return_type.append(*at);
            }
        } else {
            return_type = "std::optional<Node>";
        }
        if (skip_ws() && expect("<-") && skip_ws() && skip("/") && skip_nl()) {
            auto maybe_alt = alternative();
            if (!maybe_alt) {
                throw "Empty alternative not allowed";
            }
            auto alt = maybe_alt.value();
            if (!alt.items.empty()) {
                auto alts = std::vector<Alt>{alt};
                skip_ws();
                int apos = mark();
                while (skip_nl() && skip_ws() && expect("/") && skip_ws() && (maybe_alt = alternative()) && !maybe_alt.value().items.empty()) {
                    skip_ws();
                    alts.push_back(maybe_alt.value());
                    apos = mark();
                }
                reset(apos);
                if (expect(TokenType::NEWLINE) || expect(TokenType::UNINDENT)) {
                    skip_nl();
                    return Rule(name.value().value, alts, return_type);
                }
            }
        }
    }
    reset(pos);
    return {};
}

std::optional<Alt> GrammarParser::alternative() {
    std::cout << "\"alternative\" at pos: " << mark() << "\n";
    std::vector<std::string> items{};
    while (auto item = this->item()) {
        std::cout << "new alternative at pos: " << mark() << ", " << item.value() << "\n";
        items.push_back(item.value());
        skip_ws();
    }
    if (items.empty()) {
        return {};
    }
    // return items;
    std::string action;
    int pos = mark();
    if (expect("{")) {
        skip_ws();
        int level = 0;
        std::vector<std::string> action_tokens{};
        while (true) {
            std::string token = get_token().value;
            if (token == "{") {
                level++;
            } else if (token == "}") {
                level--;
                if (level < 0) {
                    break;
                }
            }
            action_tokens.push_back(token);
        }
        action = action_tokens[0];
        for (auto at = action_tokens.begin() + 1; at != action_tokens.end(); at++) {
            action.append(*at);
        }
    }
    skip_ws();
    std::cout << "leaving alternative at pos: " << mark() << " having parsed: " << std::format("{}", items)
              << " and action: " << action << "\n";
    return Alt{items, action};
}

std::optional<std::string> GrammarParser::item() {
    std::cout << "\"item\" at pos: " << mark() << "\n";
    if (auto name = expect(TokenType::NAME)) {
        std::cout << "\"item\", ID branch: " << name.value() << "\n";
        return name.value().value;
    }
    if (auto string = expect(TokenType::STRING)) {
        std::cout << "\"item\", STR branch: " << string.value() << "\n";
        return string.value().value;
    }
    return {};
}

std::ostream& operator<<(std::ostream& stream, const std::vector<std::vector<std::string>>& alts) {
    stream << "[";
    for (const auto& alt : alts) {
        stream << "[";
        for (const auto& s : alt) {
            stream << "\"" << s << "\", ";
        }
        stream << "]";
        stream << "\n        ";
    }
    stream << "]";
    return stream;
};
