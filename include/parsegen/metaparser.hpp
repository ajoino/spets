
#pragma once

#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parsegen/rule.hpp>
class  MetaParser : public Parser {
int synthetic_rule_counter{};
Rules synthetic_rules;
public:

MetaParser(const Tokenizer& t) : Parser{t} {};
MetaParser(const MetaParser&) = default;
MetaParser(MetaParser&&) noexcept = default;
MetaParser& operator=(const MetaParser&) = default;
MetaParser& operator=(MetaParser&&) noexcept = default;
~MetaParser() = default;


std::string synthetic_rule(Alts alts) {
    std::string rule_name = std::string("_synthetic_rule_") + std::to_string(synthetic_rule_counter);
    synthetic_rule_counter++;
    synthetic_rules.push_back(Rule(rule_name, alts));
    return rule_name;
}
        std::optional<Grammar> start();
std::optional<Strings> metas();
std::optional<String> meta();
std::optional<Rules> rules();
std::optional<Rule> rule();
std::optional<Alts> more_alts();
std::optional<Alts> alts();
std::optional<Alt> alt();
std::optional<Items> items();
std::optional<Item> item();
std::optional<String> atom();
std::optional<String> type();
std::optional<String> parts();
std::optional<String> part();
std::optional<String> ws();
};
