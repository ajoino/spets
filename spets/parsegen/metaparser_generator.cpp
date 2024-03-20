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
#include <parsegen/metaparser.hpp>
#include <parsegen/rule.hpp>
#include <parser/parsing_helpers.hpp>
#include <tokenizer/lexer.hpp>

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

    void
    generate_alt(const Alt& alt, const Rule& rule, std::vector<std::string>& vars, std::vector<Item>& global_items) {
        std::vector<std::string> items;
        uint16_t token_counter = 0;
        std::map<std::string, uint16_t> name_counter;
        std::vector<Item> local_items;
        for (const auto& item : alt.items) {
            std::string generated_name;
            if (item.name) {
                generated_name = item.name.value();
            } else {
                generated_name = item.item;
            }
            std::cout << "alt item: " << item << "\n";
            std::string return_type = "Node";
            std::string expect_value = std::format("this->{}()", item.item);
            if ((generated_name.starts_with("'") || generated_name.starts_with("\""))) {
                return_type = "Token";
                generated_name = "token";
                expect_value = std::format("expect({})", item.item);
            } else if (all_upper(item.item)) {
                generated_name = str_tolower(generated_name);
                expect_value = std::format("expect(TokenType::{})", item.item);
                return_type = "Token";
            }
            int count{};
            for (const auto& r : rules) {
                if (r.name == item.item) {
                    return_type = r.return_type;
                    break;
                }
            }
            if (return_type == "Token" && generated_name == "token") {
                std::cout << "Increase token count\n";
                count = token_counter;
                token_counter++;
            } else {
                count = name_counter[generated_name];
                name_counter[generated_name]++;
            }
            local_items.emplace_back(item.item, generated_name, return_type, expect_value, count);
        }
        for (const auto& item : local_items) {
            if (in_vector(global_items, item)) {
                continue;
            }
            stream << "            std::optional<" << item.type << "> maybe_" << item.var_name() << ";\n";
            stream << "            " << item.type << " " << item.var_name() << ";\n";
        }

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
        std::cout << "items generated in rule " << rule.name << "\n";
        for (const auto& item : items_) {
            std::cout << "    " << item << "\n";
        }
        stream << "            std::cout << \"No parse found for " << rule.name << "\\n\";\n";
        stream << "            return {};\n";
        stream << "        };\n\n";
        stream << "        std::cout << \"Parsing " << rule.name << "\\n\";\n";
        stream << "        std::optional<std::any> return_value = memoize(\"" << rule.name
               << "\", inner_func, mark());\n";
        stream << "        if (return_value) {\n";
        stream << "            std::cout << \" value not null\\n\";\n";
        stream << "            return std::any_cast<std::optional<" << rule.return_type
               << ">>(return_value.value());\n";
        stream << "        } else {\n";
        stream << "            std::cout << \" value is null\\n\";\n";
        stream << "            return std::nullopt;\n";
        stream << "        }\n";
        stream << "    }\n\n";
    }

    std::stringstream& generate_parser(const std::optional<Grammar>& maybe_grammar) {
        // clear stringstream
        stream.str(std::string{});

        if (!maybe_grammar) {
            std::cout << "Cannot generate parser from no rules.\n";
            return stream;
        }
        std::cout << "THIS DOES HAPPEN RIGHT???";
        rules = maybe_grammar.value().rules;
        auto grammar = maybe_grammar.value();
        stream << R"preamble(#include <optional>
#include <fstream>

#include <parser/node.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parser/parsing_helpers.hpp>
)preamble"
               << "\n\n";
        if (!grammar.metas.empty()) {
            stream << "//subheader\n";
            std::string meta_content;
            if (grammar.metas.at(0).starts_with(R"(""")")) {
                meta_content = std::string(grammar.metas.at(0).begin() + 3, grammar.metas.at(0).end() - 3);
            } else {
                meta_content = std::string(grammar.metas.at(0).begin() + 1, grammar.metas.at(0).end() - 1);
            }
            stream << meta_content << "\n\n";
        }
        for (const auto& rule : rules) {
            generate_rule(rule);
        }

        return stream;
    }

    std::stringstream& generate_header(const std::optional<Grammar>& maybe_grammar) {
        // clear stringstream
        stream.str(std::string{});

        if (!maybe_grammar) {
            std::cout << "Cannot generate parser from no rules.\n";
            return stream;
        }

        rules = maybe_grammar.value().rules;
        std::cout << rules << "\n";

        stream << R"(
#pragma once

#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parsegen/rule.hpp>)"
               << "\n";
        stream << "class  " << name << "Parser : public Parser {\n";
        stream << "int synthetic_rule_counter{};\n";
        stream << "Rules synthetic_rules;\n";
        stream << "public:\n\n";
        stream << R"()";
        stream << name << "Parser(const Tokenizer& t) : Parser{t} {};\n";
        stream << name << "Parser(const " << name << "Parser&) = default;\n";
        stream << name << "Parser(" << name << "Parser&&) noexcept = default;\n";
        stream << name << "Parser& operator=(const " << name << "Parser&) = default;\n";
        stream << name << "Parser& operator=(" << name << "Parser&&) noexcept = default;\n";
        stream << "~" << name << "Parser() = default;\n\n";

        stream << R"(
std::string synthetic_rule(Alts alts) {
    std::string rule_name = std::string("_synthetic_rule_") + std::to_string(synthetic_rule_counter);
    synthetic_rule_counter++;
    synthetic_rules.push_back(Rule(rule_name, alts));
    return rule_name;
}
        )";

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

    std::optional<Grammar> grammar = p.start();
    std::cout << std::flush;
    std::cout << typeid(grammar).name() << "\n";
    std::cout << grammar.value().rules;
    std::cout << "DOES THIS PRINT???!?!\n";
    if (grammar) {
        std::cout << grammar.value().rules;
        std::fstream fout{args[3], std::fstream::out};
        fout << Generator(std::string(args[1])).generate_parser(grammar).rdbuf();
        fout.close();
        fout = std::fstream{args[4], std::fstream::out};
        fout << Generator(std::string(args[1])).generate_header(grammar).rdbuf();
        fout.close();
        return 0;
    }
    std::cout << "Could not generate parser.\n";
    return 1;
}
