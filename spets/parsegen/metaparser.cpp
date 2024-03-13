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
            std::optional<std::vector<Rule>> maybe_rules.value();
            std::optional<std::vector<Rule>> rules.value();
            std::optional<Node> maybe_endoffile.value();
            std::optional<Node> endoffile.value();
            if (true
                && (maybe_rules_0 = this->rules())
                && (maybe_endoffile_0 = expect(TokenType::ENDOFFILE))
            ){
                std::cout << "generating alt in rule: start\n";
                rules_0 = maybe_rules_0.value();
                ENDOFFILE_0 = maybe_ENDOFFILE_0.value();
                return rules_0 ;
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
            std::optional<std::vector<Rule>> maybe_rules.value();
            std::optional<std::vector<Rule>> rules.value();
            std::optional<Rule> maybe_rule.value();
            std::optional<Rule> rule.value();
            if (true
                && (maybe_rules_0 = this->rules())
                && (maybe_rule_0 = this->rule())
            ){
                std::cout << "generating alt in rule: rules\n";
                rules_0 = maybe_rules_0.value();
                rule_0 = maybe_rule_0.value();
                return append_vector(rules_0, rule_0) ;
            }
            reset(pos);
            rules.value();
            rule.value();
            if (true
                && (maybe_rule_0 = this->rule())
            ){
                std::cout << "generating alt in rule: rules\n";
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
            std::optional<Node> maybe_name.value();
            std::optional<Node> name.value();
            std::optional<Token> maybe_token.value().value;
            std::optional<Token> token.value().value;
            std::optional<std::string> maybe_type.value();
            std::optional<std::string> type.value();
            std::optional<Token> maybe_token_1.value().value;
            std::optional<Token> token_1.value().value;
            std::optional<Node> maybe_ws.value();
            std::optional<Node> ws.value();
            std::optional<Token> maybe_token_2.value().value;
            std::optional<Token> token_2.value().value;
            std::optional<Node> maybe_ws_1.value();
            std::optional<Node> ws_1.value();
            std::optional<std::vector<Alt>> maybe_alts.value();
            std::optional<std::vector<Alt>> alts.value();
            std::optional<Node> maybe_newline.value();
            std::optional<Node> newline.value();
            if (true
                && (maybe_name_0 = expect(TokenType::NAME))
                && (maybe_token_0 = expect("["))
                && (maybe_type_0 = this->type())
                && (maybe_token_1 = expect("]"))
                && (maybe_ws_0 = this->ws())
                && (maybe_token_2 = expect("<-"))
                && (maybe_ws_0 = this->ws())
                && (maybe_alts_0 = this->alts())
                && (maybe_newline_0 = expect(TokenType::NEWLINE))
            ){
                std::cout << "generating alt in rule: rule\n";
                NAME_0 = maybe_NAME_0.value();
                "["_0 = maybe_"["_0.value();
                type_0 = maybe_type_0.value();
                "]"_0 = maybe_"]"_0.value();
                ws_0 = maybe_ws_0.value();
                "<-"_0 = maybe_"<-"_0.value();
                ws_0 = maybe_ws_0.value();
                alts_0 = maybe_alts_0.value();
                NEWLINE_0 = maybe_NEWLINE_0.value();
                return Rule{name_0.value, alts_0, {}} ;
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
            std::optional<std::vector<Alt>> maybe_alts.value();
            std::optional<std::vector<Alt>> alts.value();
            std::optional<Node> maybe_ws.value();
            std::optional<Node> ws.value();
            std::optional<Token> maybe_token.value().value;
            std::optional<Token> token.value().value;
            std::optional<Node> maybe_ws_1.value();
            std::optional<Node> ws_1.value();
            std::optional<Alt> maybe_alt.value();
            std::optional<Alt> alt.value();
            if (true
                && (maybe_alts_0 = this->alts())
                && (maybe_ws_0 = this->ws())
                && (maybe_token_0 = expect("/"))
                && (maybe_ws_0 = this->ws())
                && (maybe_alt_0 = this->alt())
            ){
                std::cout << "generating alt in rule: alts\n";
                alts_0 = maybe_alts_0.value();
                ws_0 = maybe_ws_0.value();
                "/"_0 = maybe_"/"_0.value();
                ws_0 = maybe_ws_0.value();
                alt_0 = maybe_alt_0.value();
                return {append_vector(alts_0, alt_0)} ;
            }
            reset(pos);
            alts.value();
            ws.value();
            token.value().value;
            ws_1.value();
            alt.value();
            if (true
                && (maybe_alt_0 = this->alt())
            ){
                std::cout << "generating alt in rule: alts\n";
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
            std::optional<std::vector<std::string>> maybe_items.value();
            std::optional<std::vector<std::string>> items.value();
            if (true
                && (maybe_items_0 = this->items())
            ){
                std::cout << "generating alt in rule: alt\n";
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
            std::optional<std::vector<std::string>> maybe_items.value();
            std::optional<std::vector<std::string>> items.value();
            std::optional<std::string> maybe_item.value();
            std::optional<std::string> item.value();
            if (true
                && (maybe_items_0 = this->items())
                && (maybe_item_0 = this->item())
            ){
                std::cout << "generating alt in rule: items\n";
                items_0 = maybe_items_0.value();
                item_0 = maybe_item_0.value();
                return {append_vector(items_0, item_0)} ;
            }
            reset(pos);
            items.value();
            item.value();
            if (true
                && (maybe_item_0 = this->item())
            ){
                std::cout << "generating alt in rule: items\n";
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
            std::optional<Node> maybe_name.value();
            std::optional<Node> name.value();
            if (true
                && (maybe_name_0 = expect(TokenType::NAME))
            ){
                std::cout << "generating alt in rule: item\n";
                NAME_0 = maybe_NAME_0.value();
                return {name_0.value} ;
            }
            reset(pos);
            name.value();
            std::optional<Node> maybe_string.value();
            std::optional<Node> string.value();
            if (true
                && (maybe_string_0 = expect(TokenType::STRING))
            ){
                std::cout << "generating alt in rule: item\n";
                STRING_0 = maybe_STRING_0.value();
                return {string_0.value } ;
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
            std::optional<std::string> maybe_parts.value();
            std::optional<std::string> parts.value();
            if (true
                && (maybe_parts_0 = this->parts())
            ){
                std::cout << "generating alt in rule: type\n";
                parts_0 = maybe_parts_0.value();
                return parts_0 ;
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
            std::optional<std::string> maybe_parts.value();
            std::optional<std::string> parts.value();
            std::optional<std::string> maybe_part.value();
            std::optional<std::string> part.value();
            if (true
                && (maybe_parts_0 = this->parts())
                && (maybe_part_0 = this->part())
            ){
                std::cout << "generating alt in rule: parts\n";
                parts_0 = maybe_parts_0.value();
                part_0 = maybe_part_0.value();
                return parts_0.append(part_0) ;
            }
            reset(pos);
            parts.value();
            part.value();
            if (true
                && (maybe_part_0 = this->part())
            ){
                std::cout << "generating alt in rule: parts\n";
                part_0 = maybe_part_0.value();
                return part_0 ;
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
            std::optional<Node> maybe_name.value();
            std::optional<Node> name.value();
            if (true
                && (maybe_name_0 = expect(TokenType::NAME))
            ){
                std::cout << "generating alt in rule: part\n";
                NAME_0 = maybe_NAME_0.value();
                return name_0.value ;
            }
            reset(pos);
            name.value();
            std::optional<Node> maybe_semicolon.value();
            std::optional<Node> semicolon.value();
            if (true
                && (maybe_semicolon_0 = expect(TokenType::SEMICOLON))
            ){
                std::cout << "generating alt in rule: part\n";
                SEMICOLON_0 = maybe_SEMICOLON_0.value();
                return semicolon_0.value ;
            }
            reset(pos);
            name.value();
            semicolon.value();
            std::optional<Token> maybe_token.value().value;
            std::optional<Token> token.value().value;
            if (true
                && (maybe_token_0 = expect("<"))
            ){
                std::cout << "generating alt in rule: part\n";
                "<"_0 = maybe_"<"_0.value();
                return token_0.value ;
            }
            reset(pos);
            name.value();
            semicolon.value();
            token.value().value;
            std::optional<Token> maybe_token.value().value;
            std::optional<Token> token.value().value;
            if (true
                && (maybe_token_0 = expect(">"))
            ){
                std::cout << "generating alt in rule: part\n";
                ">"_0 = maybe_">"_0.value();
                return token_0.value ;
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

    std::optional<Node> ws() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe_whitespace.value();
            std::optional<Node> whitespace.value();
            if (true
                && (maybe_whitespace_0 = expect(TokenType::WHITESPACE))
            ){
                std::cout << "generating alt in rule: ws\n";
                WHITESPACE_0 = maybe_WHITESPACE_0.value();
                return {Node{"ws", {maybe_whitespace_0.value().value}}};
            }
            reset(pos);
            std::cout << "No parse found for ws\n";
            return {};
        };

        std::cout << "Parsing ws\n";
        std::optional<std::any> return_value = memoize("ws", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            std::cout << "any holds type: " << return_value.value().type().name() << "should hold Node\n";
            std::cout << std::any_cast<std::optional<Node>>(return_value.value()) << std::endl;
            return std::any_cast<std::optional<Node>>(return_value.value());
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
    