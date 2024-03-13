#include <optional>
#include <fstream>

#include <parser/node.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parser/parsing_helpers.hpp>

#include <parsegen/rule.hpp>
    
class Toyparser : public Parser {
public:
    std::optional<std::vector<Rule>> start() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<Rule>> {
            int pos = mark();
            std::optional<std::vector<Rule>> maybe_rules_0;
            std::vector<Rule> rules_0;
            std::optional<Token> maybe_endoffile_0;
            Token endoffile_0;
            if (true
                && (maybe_rules_0 = this->rules())
                && (maybe_endoffile_0 = expect(TokenType::ENDOFFILE))
            ){
                rules_0 = maybe_rules_0.value();
                endoffile_0 = maybe_endoffile_0.value();
                return rules_0 ;
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        std::optional<std::any> return_value = memoize("start", inner_func, mark());
        if (return_value) {
            return std::any_cast<std::vector<Rule>>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<std::vector<Rule>> rules() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<Rule>> {
            int pos = mark();
            std::optional<std::vector<Rule>> maybe_rules_0;
            std::vector<Rule> rules_0;
            std::optional<Rule> maybe_rule_0;
            Rule rule_0;
            if (true
                && (maybe_rules_0 = this->rules())
                && (maybe_rule_0 = this->rule())
            ){
                rules_0 = maybe_rules_0.value();
                rule_0 = maybe_rule_0.value();
                return append_vector(rules_0, rule_0) ;
            }
            reset(pos);
            if (true
                && (maybe_rule_0 = this->rule())
            ){
                rules_0 = maybe_rules_0.value();
                rule_0 = maybe_rule_0.value();
                return {{rule_0}} ;
            }
            reset(pos);
            std::cout << "No parse found for rules\n";
            return {};
        };

        std::cout << "Parsing rules\n";
        std::optional<std::any> return_value = memoize("rules", inner_func, mark());
        if (return_value) {
            return std::any_cast<std::vector<Rule>>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Rule> rule() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Rule> {
            int pos = mark();
            std::optional<Token> maybe_name_0;
            Token name_0;
            std::optional<Node> maybe_ws_0;
            Node ws_0;
            std::optional<Token> maybe_token_0;
            Token token_0;
            std::optional<std::vector<Alt>> maybe_alts_0;
            std::vector<Alt> alts_0;
            std::optional<Token> maybe_newline_0;
            Token newline_0;
            if (true
                && (maybe_name_0 = expect(TokenType::NAME))
                && (maybe_ws_0 = this->ws())
                && (maybe_token_0 = expect("<-"))
                && (maybe_ws_0 = this->ws())
                && (maybe_alts_0 = this->alts())
                && (maybe_newline_0 = expect(TokenType::NEWLINE))
            ){
                name_0 = maybe_name_0.value();
                ws_0 = maybe_ws_0.value();
                token_0 = maybe_token_0.value();
                alts_0 = maybe_alts_0.value();
                newline_0 = maybe_newline_0.value();
                return Rule{name_0.value, alts_0, {}} ;
            }
            reset(pos);
            std::cout << "No parse found for rule\n";
            return {};
        };

        std::cout << "Parsing rule\n";
        std::optional<std::any> return_value = memoize("rule", inner_func, mark());
        if (return_value) {
            return std::any_cast<Rule>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<std::vector<Alt>> alts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<Alt>> {
            int pos = mark();
            std::optional<std::vector<Alt>> maybe_alts_0;
            std::vector<Alt> alts_0;
            std::optional<Node> maybe_ws_0;
            Node ws_0;
            std::optional<Token> maybe_token_0;
            Token token_0;
            std::optional<Alt> maybe_alt_0;
            Alt alt_0;
            if (true
                && (maybe_alts_0 = this->alts())
                && (maybe_ws_0 = this->ws())
                && (maybe_token_0 = expect("/"))
                && (maybe_ws_0 = this->ws())
                && (maybe_alt_0 = this->alt())
            ){
                alts_0 = maybe_alts_0.value();
                ws_0 = maybe_ws_0.value();
                token_0 = maybe_token_0.value();
                alt_0 = maybe_alt_0.value();
                return {append_vector(alts_0, alt_0)} ;
            }
            reset(pos);
            if (true
                && (maybe_alt_0 = this->alt())
            ){
                alts_0 = maybe_alts_0.value();
                ws_0 = maybe_ws_0.value();
                token_0 = maybe_token_0.value();
                alt_0 = maybe_alt_0.value();
                return {{alt_0}} ;
            }
            reset(pos);
            std::cout << "No parse found for alts\n";
            return {};
        };

        std::cout << "Parsing alts\n";
        std::optional<std::any> return_value = memoize("alts", inner_func, mark());
        if (return_value) {
            return std::any_cast<std::vector<Alt>>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Alt> alt() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Alt> {
            int pos = mark();
            std::optional<std::vector<std::string>> maybe_items_0;
            std::vector<std::string> items_0;
            if (true
                && (maybe_items_0 = this->items())
            ){
                items_0 = maybe_items_0.value();
                return Alt{items_0, {}} ;
            }
            reset(pos);
            std::cout << "No parse found for alt\n";
            return {};
        };

        std::cout << "Parsing alt\n";
        std::optional<std::any> return_value = memoize("alt", inner_func, mark());
        if (return_value) {
            return std::any_cast<Alt>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<std::vector<std::string>> items() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<std::string>> {
            int pos = mark();
            std::optional<std::vector<std::string>> maybe_items_0;
            std::vector<std::string> items_0;
            std::optional<std::string> maybe_item_0;
            std::string item_0;
            if (true
                && (maybe_items_0 = this->items())
                && (maybe_item_0 = this->item())
            ){
                items_0 = maybe_items_0.value();
                item_0 = maybe_item_0.value();
                return {append_vector(items_0, item_0)} ;
            }
            reset(pos);
            if (true
                && (maybe_item_0 = this->item())
            ){
                items_0 = maybe_items_0.value();
                item_0 = maybe_item_0.value();
                return {{item_0}} ;
            }
            reset(pos);
            std::cout << "No parse found for items\n";
            return {};
        };

        std::cout << "Parsing items\n";
        std::optional<std::any> return_value = memoize("items", inner_func, mark());
        if (return_value) {
            return std::any_cast<std::vector<std::string>>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<std::string> item() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::string> {
            int pos = mark();
            std::optional<Token> maybe_name_0;
            Token name_0;
            if (true
                && (maybe_name_0 = expect(TokenType::NAME))
            ){
                name_0 = maybe_name_0.value();
                return {name_0.value} ;
            }
            reset(pos);
            std::optional<Token> maybe_string_0;
            Token string_0;
            if (true
                && (maybe_string_0 = expect(TokenType::STRING))
            ){
                name_0 = maybe_name_0.value();
                string_0 = maybe_string_0.value();
                return {string_0.value } ;
            }
            reset(pos);
            std::cout << "No parse found for item\n";
            return {};
        };

        std::cout << "Parsing item\n";
        std::optional<std::any> return_value = memoize("item", inner_func, mark());
        if (return_value) {
            return std::any_cast<std::string>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Node> ws() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> maybe_whitespace_0;
            Token whitespace_0;
            if (true
                && (maybe_whitespace_0 = expect(TokenType::WHITESPACE))
            ){
                whitespace_0 = maybe_whitespace_0.value();
                return Node{"ws", {maybe_whitespace_0.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for ws\n";
            return {};
        };

        std::cout << "Parsing ws\n";
        std::optional<std::any> return_value = memoize("ws", inner_func, mark());
        if (return_value) {
            return std::any_cast<Node>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

};



int main(int argc, char**argv) {
  std::fstream fin{std::span(argv, size_t(argc))[1], std::fstream::in};
  std::string content((std::istreambuf_iterator<char>(fin)),
                      (std::istreambuf_iterator<char>()));
  fin.close();
    
  Tokenizer t{content};

  Toyparser p{t};

  auto nodes = p.start();
  if (nodes) {
    for (const auto& child : nodes.value()) {
        std::cout << child << "\n";
    }
  } else {
    std::cout << "Could not parse content.\n";
  }
}
    