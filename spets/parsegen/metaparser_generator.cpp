#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <parsegen/grammar_parser.hpp>
#include <parsegen/rule.hpp>
#include <parsegen/metaparser.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parsing_helpers.hpp>

std::string str_tolower(std::string s) {
    std::transform(
        s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); } // correct
    );
    return s;
}

template <class T> bool in_vector(const std::vector<T>& vt, const T& value) {
    return std::ranges::any_of(vt, [&](T t) { return t == value; });
}

bool all_upper(const std::string& s) {
    return !std::ranges::all_of(s, [](char ch) { return ch < 0x41 || ch > 0x5A; });
}

class Generator {
    std::string name;
    std::stringstream stream;
    uint indentation = 0;
    std::vector<Rule> rules;

public:
    Generator(std::string name) : name{std::move(name)} {};

    void inc_indentation() noexcept { indentation++; }

    void dec_indentation() noexcept {
        if (indentation > 0) {
            indentation--;
        }
    }

    void put(const std::string& input) { stream << std::string(" ", indentation) << input; }

    void generate_item(
        const Item& item, std::vector<Item>& items, uint16_t& token_counter,
        std::map<std::string, uint16_t>& name_counter
    ) {
        stream << "                && (maybe_" << item.var_name() << " = " << item.expect_value << ")\n";
    }

    void generate_alt(const Alt& alt, const Rule& rule, std::vector<std::string>& vars, std::vector<Item>& global_items) {
        std::vector<std::string> items;
        uint16_t token_counter = 0;
        std::map<std::string, uint16_t> name_counter;
        std::vector<Item> local_items;
        for (const auto& item : alt.items) {
            auto name = item.item;
            std::cout << "alt item: " << item << "\n";
            std::string return_type = "Node";
            std::string expect_value = std::format("this->{}()", item.item);
            if ((item.item.starts_with("'") || item.item.starts_with("\""))) {
                return_type = "Token";
                name = "token";
                expect_value = std::format("expect({})", item.item);
            } else if (all_upper(name)){
                name = str_tolower(name);
                expect_value = std::format("expect(TokenType::{})", item.item);
                return_type = "Token";
            }
            int count{};
            for (const auto& r : rules) {
                if (r.name == item) {
                    return_type = r.return_type;
                    break;
                }
            }
            if (return_type == "Token" && name == "token") {
                count = token_counter;
                token_counter++;
            } else {
                count = name_counter[name];
                name_counter[name]++;
            }
            local_items.emplace_back(name, return_type, expect_value, count);
        }
        token_counter = 0;
        name_counter.clear();

        for (const auto& item : local_items) {
            if (in_vector(global_items, item)) {
                continue;
            }
            stream << "            std::optional<" << item.type << "> maybe_" << item.var_name() << ";\n";
            stream << "            " << item.type << " " << item.var_name() << ";\n";
        }

        token_counter = 0;
        name_counter.clear();
        stream << "            if (true\n";
        for (const auto& item : local_items) {
            generate_item(item, local_items, token_counter, name_counter);
        }
        stream << "            ){\n";
        for (const auto& item : local_items) {
            stream << "                " << item.var_name() << " = maybe_" << item.eval_string() << ";\n";
        }
        stream << "                std::cout << \"generating alt in rule: " << rule.name << "\\n\";\n";
        if (alt.action && !alt.action.value().empty()) {
            stream << "                return " << alt.action.value() << ";\n";
        } else {
            stream << "                return {Node{\"" << rule.name << "\", {";
            for (int i = 0; i < items.size(); i++) {
                stream << items[i];
                if (i != items.size() - 1) {
                    stream << ", ";
                }
            }
            stream << "}}};\n";
        }
        stream << "            }\n";
        stream << "            reset(pos);\n";
        global_items.insert(std::end(global_items), std::begin(local_items), std::end(local_items));
    }

    void generate_rule(const Rule& rule) {
        stream << "    std::optional<" << rule.return_type << "> " << name << "Parser::" << rule.name << "() {\n\n";
        stream << "        // inner_func does the actual parsing but is called later by\n";
        stream << "        // to enable memoization\n";
        stream << "        auto inner_func = [&, this]() -> std::optional<" << rule.return_type << "> {\n";
        stream << "            int pos = mark();\n";
        std::vector<std::string> vars;
        std::vector<Item> items_;
        for (const auto& alt : rule.alts) {
            generate_alt(alt, rule, vars, items_);
        }
        stream << "            std::cout << \"No parse found for " << rule.name << "\\n\";\n";
        stream << "            return {};\n";
        stream << "        };\n\n";
        stream << "        std::cout << \"Parsing " << rule.name << "\\n\";\n";
        stream << "        std::optional<std::any> return_value = memoize(\"" << rule.name << "\", inner_func, mark());\n";
        stream << "        if (return_value) {\n";
        stream << "            std::cout << \" value not null\\n\";\n";
        stream << "            return std::any_cast<std::optional<" << rule.return_type << ">>(return_value.value());\n";
        stream << "        } else {\n";
        stream << "            std::cout << \" value is null\\n\";\n";
        stream << "            return std::nullopt;\n";
        stream << "        }\n";
        stream << "    }\n\n";
    }

    std::stringstream& generate_parser(const std::optional<std::vector<Rule>>& maybe_rules) {
        // clear stringstream
        stream.str(std::string{});

        if (!maybe_rules) {
            std::cout << "Cannot generate parser from no rules.\n";
            return stream;
        }
        rules = maybe_rules.value();
        stream << R"preamble(#include <optional>
#include <fstream>

#include <parser/node.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parser/parsing_helpers.hpp>

#include <parsegen/rule.hpp>
    
#ifndef METAPARSER_CANDIDATE
#include <parsegen/metaparser.hpp>
#else
#include <parsegen/metaparser_candidate.hpp>
#endif
)preamble" << "\n\n";
        for (const auto& rule : rules) {
            generate_rule(rule);
        }

        return stream;
    }

    std::stringstream& generate_header(const std::optional<std::vector<Rule>>& maybe_rules) {
        // clear stringstream
        stream.str(std::string{});

        if (!maybe_rules) {
            std::cout << "Cannot generate parser from no rules.\n";
            return stream;
        }

        rules = maybe_rules.value();
        std::cout << rules << "\n";

        stream << R"(
#pragma once

#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parsegen/rule.hpp>)" << "\n";
        stream << "class  "<< name << "Parser : public Parser {\n";
        stream << "public:\n\n";
        stream << R"()";
        stream << name << "Parser(const Tokenizer& t) : Parser{t} {};\n";
    stream << name << "Parser(const " << name << "Parser&) = default;\n";
    stream << name << "Parser(" << name << "Parser&&) noexcept = default;\n";
    stream << name << "Parser& operator=(const " << name << "Parser&) = default;\n";
    stream << name << "Parser& operator=(" << name << "Parser&&) noexcept = default;\n";
    stream << "~" << name << "Parser() = default;\n\n";

        for (const auto& rule : rules) {
            stream << "std::optional<" << rule.return_type << "> " << rule.name << "();\n";
        }
        stream << "};\n";
        return stream;
    }
};

int main(int argc, char** argv) {
    auto args = std::span(argv, size_t(argc));
    std::fstream fin{args[2], std::fstream::in};
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    fin.close();

    std::cout << "Grammar file:\n" << content << "\n";
    Tokenizer t{content};

    MetaParser p{t};

    auto rules = p.start();
    if (rules) {
        std::cout << rules.value();
        std::fstream fout{args[3], std::fstream::out};
        fout << Generator(std::string(args[1])).generate_parser(rules).rdbuf();
        fout.close();
        fout = std::fstream{args[4], std::fstream::out};
        fout << Generator(std::string(args[1])).generate_header(rules).rdbuf();
        fout.close();
        return 0;
    }
    std::cout << "Could not generated parser.\n";
    return 1;
}
