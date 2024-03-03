#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "rule.hpp"
#include "parser.hpp"

class GrammarParser : public Parser {
public:
  bool skip_ws() {
    expect(TokenType::WHITESPACE);
    return true;
  }
  std::optional<std::vector<Rule>> grammar() {
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

  std::optional<Rule> rule() {
    std::cout << "\"rule\" at pos: " << mark() << "\n";
    int pos = mark();
    if (auto name = expect(TokenType::NAME)) {
      if (expect(":") && skip_ws()) {
        auto alt = alternative();
        if (!alt.empty()) {
          auto alts = std::vector<std::vector<std::string>>{alt};
          skip_ws();
          int apos = mark();
          while (expect("|") && skip_ws() && !(alt = alternative()).empty()) {
            skip_ws();
            alts.push_back(alt);
            apos = mark();
          }
          reset(apos);
          if (expect(TokenType::NEWLINE)) {
            return Rule(name.value().value, alts);
          }
        }
      }
    }
    reset(pos);
    return {};
  }

  std::vector<std::string> alternative() {
    std::cout << "\"alternative\" at pos: " << mark() << "\n";
    std::vector<std::string> items{};
    while (auto item = this->item()) {
      std::cout << "new alternative at pos: " << mark() << ", " << item.value()
                << "\n";
      items.push_back(item.value());
      skip_ws();
    }
    return items;
  }

  std::optional<std::string> item() {
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
};

std::ostream &operator<<(std::ostream &stream,
                         const std::vector<std::vector<std::string>> &alts) {
  stream << "[";
  for (const auto &alt : alts) {
    stream << "[";
    for (const auto &s : alt) {
      stream << "\"" << s << "\", ";
    }
    stream << "]";
    stream << "\n        ";
  }
  stream << "]";
  return stream;
};

std::ostream &operator<<(std::ostream &stream, const Rule &rule) {
  return stream << "    Rule(\"" << rule.name << "\", " << rule.alts << ")";
}
std::ostream &operator<<(std::ostream &stream,
                         const std::vector<Rule> &rule_list) {
  stream << "[\n";
  for (const auto &rule : rule_list) {
    stream << rule << ",\n";
  }
  return stream << "]\n";
}
std::string str_tolower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); } // correct
  );
  return s;
}

template <class T> bool in_vector(const std::vector<T> &vt, const T &value) {
  for (const auto &t : vt) {
    if (t == value) {
      return true;
    }
  }
  return false;
}

bool all_upper(const std::string &s) {
  for (const auto &ch : s) {
    if (ch < 0x41 || ch > 0x5A) {
      return false;
    }
  }
  return true;
}


bool is_left_recursive(const Rule& rule) {
    for (const auto& alt : rule.alts) {
        if (alt[0] == rule.name) {
            return true;
        }
    }
    return false;
}

std::stringstream generate_parser_class(std::vector<Rule> rules) {
  std::stringstream stream;
  stream << R"preamble(#include <optional>
#include <fstream>
#include <optional>

#include "node.hpp"
#include "lexer.hpp"
#include "parser.hpp"
    
)preamble";
  stream << "class Toyparser : public Parser {\npublic:\n";
  for (const auto &rule : rules) {
    bool is_lr = is_left_recursive(rule);

    stream << "    std::optional<Node> " << rule.name << "() {\n\n";

    stream << "        // inner_func does the actual parsing but is called later by\n";
    stream << "        // to enable memoization\n";
    stream << "        auto inner_func = [&, this]() -> std::optional<Node> {\n";
    stream << "           int pos = mark();\n";
    std::vector<std::string> vars;
    for (const auto &alt : rule.alts) {
      std::vector<std::string> items;
      for (const auto &item : alt) {
        std::string var_name = str_tolower(item);
        if (!(item.starts_with("'") || item.starts_with("\"")) &&
            !in_vector(vars, var_name)) {
          if (all_upper(item)) {
            stream << "            std::optional<Token> " << var_name << ";\n";
          } else {
            stream << "            std::optional<Node> " << var_name << ";\n";
          }
          vars.push_back(item);
        }
      }
      stream << "                std::cout << \"\\n### " << rule.name << ": ";
      for (const auto &item : alt) {
         stream << str_tolower(item) << " ";
      }
      stream << "\\n\\n\";\n";
      stream << "            if (true\n";
      for (const auto &item : alt) {
        if (item.starts_with("'") || item.starts_with("\"")) {
          std::string string_item_inner{item.begin() + 1, item.end() - 1};
          stream << "                && expect(\"" << string_item_inner << "\")\n";
        } else {
          auto var = str_tolower(item);
          if (in_vector(items, var)) {
            var += std::to_string(items.size());
          }
          if (all_upper(item)) {
            items.push_back(var + ".value().value");
            stream << "                && (" << var
                   << " = expect(TokenType::" << item << "))\n";
          } else {
            items.push_back(var + ".value()");
            stream << "                && (" << var << " = this->" << item
                   << "())\n";
          }
        }
      }
      stream << "            ){\n";
      // stream << "                std::cout << \"";
      // for (const auto& item : items) {
      //   stream << item << " ";
      // }
      // stream << "\"\\n;\n";

      for (int i = 0; i < items.size(); i++) {
        stream << "                std::cout << \"" << items[i] << ": \" << " << items[i] << " << \"\\n\";\n";
      }
      // stream << "                std::cout << "
      stream << "                return Node{\"" << rule.name << "\", {";
      for (int i = 0; i < items.size(); i++) {
        stream << items[i];
        if (i != items.size() - 1) {
          stream << ", ";
        }
      }
      stream << "}};\n";
      stream << "            }\n";
    }
    stream << "            reset(pos);\n";
    stream << "            std::cout << \"No parse found for " << rule.name << "\\n\";\n";
    stream << "            return {};\n";

    stream << "        };\n\n";

    // if (is_lr) {
    //     stream << "        return memoize_left_rec(\"" << rule.name << "\", inner_func);\n";
    // } else {
        stream << "        std::cout << \"Parsing " << rule.name << "\\n\";\n";
        stream << "        return memoize(\"" << rule.name << "\", inner_func, mark());\n";
    // }

    stream << "    }\n\n";
  }
  stream << "};\n\n\n";

  stream << R"main_func(
int main(int argc, char**argv) {
  std::fstream fin{argv[1], std::fstream::in};
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
    )main_func";
  return stream;
}

int main(int argc, char **argv) {
  // std::string input{"<-\n\n"};
  std::fstream fin{argv[1], std::fstream::in};
  std::string content((std::istreambuf_iterator<char>(fin)),
                      (std::istreambuf_iterator<char>()));
  fin.close();

  std::cout << "Grammar file:\n" << content << "\n";
  Tokenizer t{content};

  GrammarParser p{t};

  auto rules = p.grammar();
  // if (rules) {
  //   std::cout << rules.value() << "\n\n\n";
  //   std::cout << generate_parser_class(rules.value()).str();
  // } else {
  //   std::cout << "Could not parse content.\n";
  // }

  std::fstream fout{"parsegen/generated_parser.cpp", std::fstream::out};
  fout << generate_parser_class(rules.value()).str();
}
