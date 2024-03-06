#include <cstdint>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include <parsegen/grammar_parser.hpp>
#include <parsegen/rule.hpp>
#include <tokenizer/lexer.hpp>

std::ostream& operator<<(std::ostream& os, const std::vector<Alt>& alts) {
    for (const auto& alt : alts) {
        os << "Alt(" << std::format("{}", alt.items) << ")";
    }
    return os;
}

std::ostream& operator<<(std::ostream& stream, const Rule& rule) {
    return stream << "    Rule(\"" << rule.name << "\", " << rule.alts << ")";
}

std::ostream& operator<<(std::ostream& stream, const std::vector<Rule>& rule_list) {
    stream << "[\n";
    for (const auto& rule : rule_list) {
        stream << rule << ",\n";
    }
    return stream << "]\n";
}

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
    std::stringstream stream;
    uint indentation = 0;

public:

    void inc_indentation() noexcept { indentation++; }

    void dec_indentation() noexcept {
        if (indentation > 0) {
            indentation--;
        }
    }

    void put(const std::string& input) { stream << std::string(" ", indentation) << input; }

    void generate_item(
        const std::string& item, std::vector<std::string>& items, uint16_t& token_counter,
        std::map<std::string, uint16_t>& name_counter
    ) {
        if (item.starts_with("'") || item.starts_with("\"")) {
            std::string string_item_inner{item.begin() + 1, item.end() - 1};
            auto token_id = std::string{"token_"} + std::to_string(token_counter);
            stream << "                && (" << token_id << " = expect(\"" << string_item_inner << "\"))\n";
            token_counter++;
            auto node_string = std::format("Node(\"token\", {})", token_id + ".value().value");
            items.push_back(node_string);
        } else {
            auto var = str_tolower(item) + "_" + std::to_string(name_counter[str_tolower(item)]);
            if (in_vector(items, var)) {
                var += std::to_string(items.size());
            }
            if (all_upper(item)) {
                items.push_back(var + ".value().value");
                stream << "                && (" << var << " = expect(TokenType::" << item << "))\n";
            } else {
                items.push_back(var + ".value()");
                stream << "                && (" << var << " = this->" << item << "())\n";
            }
        }
    }

    void generate_alt(const Alt& alt, const Rule& rule, std::vector<std::string>& vars) {
        std::vector<std::string> items;
        uint16_t token_counter = 0;
        std::map<std::string, uint16_t> name_counter;
        for (const auto& item : alt.items) {
            std::string var_name = str_tolower(item) + "_" + std::to_string(name_counter[str_tolower(item)]);
            auto token_name = std::string{"token_"} + std::to_string(token_counter);
            if (!in_vector(vars, var_name)) {
                if ((item.starts_with("'") || item.starts_with("\""))) {
                    if (in_vector(vars, token_name)) {
                        continue;
                    }
                    stream << "            std::optional<Token> " << token_name << ";\n";
                    token_counter++;
                    vars.push_back(token_name);
                    continue;
                }
                if (all_upper(item)) {
                    stream << "            std::optional<Token> " << var_name << ";\n";
                } else {
                    stream << "            std::optional<Node> " << var_name << ";\n";
                }
                name_counter[var_name]++;
                vars.push_back(var_name);
            }
        }
        stream << "            std::cout << \"\\n### " << rule.name << ": ";
        for (const auto& item : alt.items) {
            stream << str_tolower(item) << " ";
        }
        stream << "\\n\\n\";\n";
        token_counter = 0;
        name_counter.clear();
        stream << "            if (true\n";
        for (const auto& item : alt.items) {
            generate_item(item, items, token_counter, name_counter);
        }
        stream << "            ){\n";

        if (alt.action && !alt.action.value().empty()) {
            stream << "                return " << alt.action.value() << ";\n";
        } else {
            stream << "                return Node{\"" << rule.name << "\", {";
            for (int i = 0; i < items.size(); i++) {
                stream << items[i];
                if (i != items.size() - 1) {
                    stream << ", ";
                }
            }
            stream << "}};\n";
        }
        stream << "            }\n";
        stream << "            reset(pos);\n";
    }

    void generate_rule(const Rule& rule) {
        stream << "    std::optional<Node> " << rule.name << "() {\n\n";
        stream << "        // inner_func does the actual parsing but is called later by\n";
        stream << "        // to enable memoization\n";
        stream << "        auto inner_func = [&, this]() -> std::optional<Node> {\n";
        stream << "            int pos = mark();\n";
        std::vector<std::string> vars;
        for (const auto& alt : rule.alts) {
            generate_alt(alt, rule, vars);
        }
        stream << "            std::cout << \"No parse found for " << rule.name << "\\n\";\n";
        stream << "            return {};\n";
        stream << "        };\n\n";
        stream << "        std::cout << \"Parsing " << rule.name << "\\n\";\n";
        stream << "        return memoize(\"" << rule.name << "\", inner_func, mark());\n";
        stream << "    }\n\n";
    }

    std::stringstream& generate_parser(const std::optional<std::vector<Rule>>& maybe_rules) {
        if (!maybe_rules) {
            std::cout << "Cannot generate parser from no rules.\n";
            return stream;
        }
        auto rules = maybe_rules.value();
        stream << R"preamble(#include <optional>
#include <fstream>
#include <optional>

#include "node.hpp"
#include "lexer.hpp"
#include "parser.hpp"
    
)preamble";
        stream << "class Toyparser : public Parser {\npublic:\n";
        for (const auto& rule : rules) {
            generate_rule(rule);
        }
        stream << "};\n\n\n";

        stream << R"c++(
int main(int argc, char**argv) {
  std::fstream fin{std::span(argv, size_t(argc))[1], std::fstream::in};
  std::string content((std::istreambuf_iterator<char>(fin)),
                      (std::istreambuf_iterator<char>()));
  fin.close();
    
  Tokenizer t{content};

  Toyparser p{t};

  auto nodes = p.start();
  if (nodes) {
    std::cout << nodes.value() << "\n";
    std::cout << "number of children " << nodes.value().children.size() << "\n";
    for (const auto& child : nodes.value().children) {
        std::cout << child << "\n";
    }
  } else {
    std::cout << "Could not parse content.\n";
  }
}
    )c++";
        return stream;
    }
};

int main(int argc, char** argv) {
    auto args = std::span(argv, size_t(argc));
    std::fstream fin{args[1], std::fstream::in};
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    fin.close();

    std::cout << "Grammar file:\n" << content << "\n";
    Tokenizer t{content};

    GrammarParser p{t};

    auto rules = p.grammar();
    if (rules) {
        std::cout << rules.value();
        std::fstream fout{"parsegen/generated_parser.cpp", std::fstream::out};
        fout << Generator().generate_parser(rules).rdbuf();
        return 0;
    }
    std::cout << "Could not generated parser.\n";
    return 1;
}
