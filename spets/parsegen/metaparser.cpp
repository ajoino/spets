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
            std::optional<std::vector<Rule>> maybe_rules;
            std::vector<Rule> rules;
            std::optional<Token> maybe_endoffile;
            Token endoffile;
            if (true
                && (maybe_rules = this->rules())
                && (maybe_endoffile = expect(TokenType::ENDOFFILE))
            ){
                rules = maybe_rules.value();
                endoffile = maybe_endoffile.value();
                std::cout << "generating alt in rule: start\n";
                return rules ;
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        std::optional<std::any> return_value = memoize("start", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::vector<Rule>\n";
            std::cout << std::any_cast<std::optional<std::vector<Rule>>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::vector<Rule>>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::vector<Rule>> rules() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<Rule>> {
            int pos = mark();
            std::optional<std::vector<Rule>> maybe_rules;
            std::vector<Rule> rules;
            std::optional<Rule> maybe_rule;
            Rule rule;
            if (true
                && (maybe_rules = this->rules())
                && (maybe_rule = this->rule())
            ){
                rules = maybe_rules.value();
                rule = maybe_rule.value();
                std::cout << "generating alt in rule: rules\n";
                return append_vector(rules, rule) ;
            }
            reset(pos);
            if (true
                && (maybe_rule = this->rule())
            ){
                rule = maybe_rule.value();
                std::cout << "generating alt in rule: rules\n";
                return {{rule}} ;
            }
            reset(pos);
            std::cout << "No parse found for rules\n";
            return {};
        };

        std::cout << "Parsing rules\n";
        std::optional<std::any> return_value = memoize("rules", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::vector<Rule>\n";
            std::cout << std::any_cast<std::optional<std::vector<Rule>>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::vector<Rule>>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Rule> rule() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Rule> {
            int pos = mark();
            std::optional<Token> maybe_name;
            Token name;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<std::string> maybe_type;
            std::string type;
            std::optional<Token> maybe_token_1;
            Token token_1;
            std::optional<std::string> maybe_ws;
            std::string ws;
            std::optional<Token> maybe_token_2;
            Token token_2;
            std::optional<std::string> maybe_ws_1;
            std::string ws_1;
            std::optional<std::vector<Alt>> maybe_alts;
            std::vector<Alt> alts;
            std::optional<Token> maybe_newline;
            Token newline;
            if (true
                && (maybe_name = expect(TokenType::NAME))
                && (maybe_token = expect("["))
                && (maybe_type = this->type())
                && (maybe_token_1 = expect("]"))
                && (maybe_ws = this->ws())
                && (maybe_token_2 = expect("<-"))
                && (maybe_ws_1 = this->ws())
                && (maybe_alts = this->alts())
                && (maybe_newline = expect(TokenType::NEWLINE))
            ){
                name = maybe_name.value();
                token = maybe_token.value();
                type = maybe_type.value();
                token_1 = maybe_token_1.value();
                ws = maybe_ws.value();
                token_2 = maybe_token_2.value();
                ws_1 = maybe_ws_1.value();
                alts = maybe_alts.value();
                newline = maybe_newline.value();
                std::cout << "generating alt in rule: rule\n";
                return Rule{name.value, alts, {}} ;
            }
            reset(pos);
            std::cout << "No parse found for rule\n";
            return {};
        };

        std::cout << "Parsing rule\n";
        std::optional<std::any> return_value = memoize("rule", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold Rule\n";
            std::cout << std::any_cast<std::optional<Rule>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<Rule>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::vector<Alt>> alts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<Alt>> {
            int pos = mark();
            std::optional<std::vector<Alt>> maybe_alts;
            std::vector<Alt> alts;
            std::optional<std::string> maybe_ws;
            std::string ws;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<std::string> maybe_ws_1;
            std::string ws_1;
            std::optional<Alt> maybe_alt;
            Alt alt;
            if (true
                && (maybe_alts = this->alts())
                && (maybe_ws = this->ws())
                && (maybe_token = expect("/"))
                && (maybe_ws_1 = this->ws())
                && (maybe_alt = this->alt())
            ){
                alts = maybe_alts.value();
                ws = maybe_ws.value();
                token = maybe_token.value();
                ws_1 = maybe_ws_1.value();
                alt = maybe_alt.value();
                std::cout << "generating alt in rule: alts\n";
                return {append_vector(alts, alt)} ;
            }
            reset(pos);
            if (true
                && (maybe_alt = this->alt())
            ){
                alt = maybe_alt.value();
                std::cout << "generating alt in rule: alts\n";
                return {{alt}} ;
            }
            reset(pos);
            std::cout << "No parse found for alts\n";
            return {};
        };

        std::cout << "Parsing alts\n";
        std::optional<std::any> return_value = memoize("alts", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::vector<Alt>\n";
            std::cout << std::any_cast<std::optional<std::vector<Alt>>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::vector<Alt>>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Alt> alt() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Alt> {
            int pos = mark();
            std::optional<std::vector<std::string>> maybe_items;
            std::vector<std::string> items;
            if (true
                && (maybe_items = this->items())
            ){
                items = maybe_items.value();
                std::cout << "generating alt in rule: alt\n";
                return Alt{items, {}} ;
            }
            reset(pos);
            std::cout << "No parse found for alt\n";
            return {};
        };

        std::cout << "Parsing alt\n";
        std::optional<std::any> return_value = memoize("alt", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold Alt\n";
            std::cout << std::any_cast<std::optional<Alt>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<Alt>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::vector<std::string>> items() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::vector<std::string>> {
            int pos = mark();
            std::optional<std::vector<std::string>> maybe_items;
            std::vector<std::string> items;
            std::optional<std::string> maybe_item;
            std::string item;
            if (true
                && (maybe_items = this->items())
                && (maybe_item = this->item())
            ){
                items = maybe_items.value();
                item = maybe_item.value();
                std::cout << "generating alt in rule: items\n";
                return {append_vector(items, item)} ;
            }
            reset(pos);
            if (true
                && (maybe_item = this->item())
            ){
                item = maybe_item.value();
                std::cout << "generating alt in rule: items\n";
                return {{item}} ;
            }
            reset(pos);
            std::cout << "No parse found for items\n";
            return {};
        };

        std::cout << "Parsing items\n";
        std::optional<std::any> return_value = memoize("items", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::vector<std::string>\n";
            std::cout << std::any_cast<std::optional<std::vector<std::string>>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::vector<std::string>>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::string> item() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::string> {
            int pos = mark();
            std::optional<Token> maybe_name;
            Token name;
            if (true
                && (maybe_name = expect(TokenType::NAME))
            ){
                name = maybe_name.value();
                std::cout << "generating alt in rule: item\n";
                return {name.value} ;
            }
            reset(pos);
            std::optional<Token> maybe_string;
            Token string;
            if (true
                && (maybe_string = expect(TokenType::STRING))
            ){
                string = maybe_string.value();
                std::cout << "generating alt in rule: item\n";
                return {string.value } ;
            }
            reset(pos);
            std::cout << "No parse found for item\n";
            return {};
        };

        std::cout << "Parsing item\n";
        std::optional<std::any> return_value = memoize("item", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::string\n";
            std::cout << std::any_cast<std::optional<std::string>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::string>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::string> type() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::string> {
            int pos = mark();
            std::optional<std::string> maybe_parts;
            std::string parts;
            if (true
                && (maybe_parts = this->parts())
            ){
                parts = maybe_parts.value();
                std::cout << "generating alt in rule: type\n";
                return parts ;
            }
            reset(pos);
            std::cout << "No parse found for type\n";
            return {};
        };

        std::cout << "Parsing type\n";
        std::optional<std::any> return_value = memoize("type", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::string\n";
            std::cout << std::any_cast<std::optional<std::string>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::string>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::string> parts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::string> {
            int pos = mark();
            std::optional<std::string> maybe_parts;
            std::string parts;
            std::optional<std::string> maybe_part;
            std::string part;
            if (true
                && (maybe_parts = this->parts())
                && (maybe_part = this->part())
            ){
                parts = maybe_parts.value();
                part = maybe_part.value();
                std::cout << "generating alt in rule: parts\n";
                return parts.append(part) ;
            }
            reset(pos);
            if (true
                && (maybe_part = this->part())
            ){
                part = maybe_part.value();
                std::cout << "generating alt in rule: parts\n";
                return part ;
            }
            reset(pos);
            std::cout << "No parse found for parts\n";
            return {};
        };

        std::cout << "Parsing parts\n";
        std::optional<std::any> return_value = memoize("parts", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::string\n";
            std::cout << std::any_cast<std::optional<std::string>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::string>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::string> part() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::string> {
            int pos = mark();
            std::optional<Token> maybe_name;
            Token name;
            if (true
                && (maybe_name = expect(TokenType::NAME))
            ){
                name = maybe_name.value();
                std::cout << "generating alt in rule: part\n";
                return name.value ;
            }
            reset(pos);
            std::optional<Token> maybe_semicolon;
            Token semicolon;
            if (true
                && (maybe_semicolon = expect(TokenType::SEMICOLON))
            ){
                semicolon = maybe_semicolon.value();
                std::cout << "generating alt in rule: part\n";
                return semicolon.value ;
            }
            reset(pos);
            std::optional<Token> maybe_token;
            Token token;
            if (true
                && (maybe_token = expect("<"))
            ){
                token = maybe_token.value();
                std::cout << "generating alt in rule: part\n";
                return token.value ;
            }
            reset(pos);
            if (true
                && (maybe_token = expect(">"))
            ){
                token = maybe_token.value();
                std::cout << "generating alt in rule: part\n";
                return token.value ;
            }
            reset(pos);
            std::cout << "No parse found for part\n";
            return {};
        };

        std::cout << "Parsing part\n";
        std::optional<std::any> return_value = memoize("part", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::string\n";
            std::cout << std::any_cast<std::optional<std::string>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::string>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<std::string> ws() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<std::string> {
            int pos = mark();
            std::optional<Token> maybe_whitespace;
            Token whitespace;
            if (true
                && (maybe_whitespace = expect(TokenType::WHITESPACE))
            ){
                whitespace = maybe_whitespace.value();
                std::cout << "generating alt in rule: ws\n";
                return whitespace.value ;
            }
            reset(pos);
            std::cout << "No parse found for ws\n";
            return {};
        };

        std::cout << "Parsing ws\n";
        std::optional<std::any> return_value = memoize("ws", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold std::string\n";
            std::cout << std::any_cast<std::optional<std::string>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<std::string>>(return_value.value());
        } else {
            std::cout << " value is null\n";
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
    
