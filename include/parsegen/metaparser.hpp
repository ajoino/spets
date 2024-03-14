
#pragma once

#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parsegen/rule.hpp>
class  MetaParser : public Parser {
public:

MetaParser(const Tokenizer& t) : Parser{t} {};
MetaParser(const MetaParser&) = default;
MetaParser(MetaParser&&) noexcept = default;
MetaParser& operator=(const MetaParser&) = default;
MetaParser& operator=(MetaParser&&) noexcept = default;
~MetaParser() = default;

};
