#pragma once

#include "lexer.hpp"
#include "parser.hpp"
#include "rule.hpp"

class GrammarParser : public Parser {
public:

    GrammarParser(const Tokenizer& t) : Parser{t} {};
    GrammarParser(const GrammarParser&) = default;
    GrammarParser(GrammarParser&&) noexcept = default;
    GrammarParser& operator=(const GrammarParser&) = default;
    GrammarParser& operator=(GrammarParser&&) noexcept = default;
    ~GrammarParser() = default;

    std::optional<std::vector<Rule>> grammar();
    std::optional<Rule> rule();
    std::optional<Alt> alternative();
    std::optional<std::string> item();
};

std::ostream& operator<<(std::ostream& stream, const std::vector<std::vector<std::string>>& alts);
