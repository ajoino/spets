#include <optional>
#include <fstream>

#include <parser/node.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parser/parsing_helpers.hpp>

#include <parsegen/rule.hpp>
    
class Toyparser : public Parser {
public:
    std::optional<Rules> start() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Rules> {
            int pos = mark();
            std::optional<Rules> maybe_rules;
            Rules rules;
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
            return std::any_cast<std::optional<Rules>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Rules> rules() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Rules> {
            int pos = mark();
            std::optional<Rules> maybe_rules;
            Rules rules;
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
                return Rules{rule} ;
            }
            reset(pos);
            std::cout << "No parse found for rules\n";
            return {};
        };

        std::cout << "Parsing rules\n";
        std::optional<std::any> return_value = memoize("rules", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<Rules>>(return_value.value());
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
            std::optional<String> maybe_type;
            String type;
            std::optional<Token> maybe_token_1;
            Token token_1;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<Token> maybe_token_2;
            Token token_2;
            std::optional<String> maybe_ws_1;
            String ws_1;
            std::optional<Alts> maybe_alts;
            Alts alts;
            std::optional<String> maybe_newlines;
            String newlines;
            if (true
                && (maybe_name = expect(TokenType::NAME))
                && (maybe_token = expect("["))
                && (maybe_type = this->type())
                && (maybe_token_1 = expect("]"))
                && (maybe_ws = this->ws())
                && (maybe_token_2 = expect("<-"))
                && (maybe_ws_1 = this->ws())
                && (maybe_alts = this->alts())
                && (maybe_newlines = this->newlines())
            ){
                name = maybe_name.value();
                token = maybe_token.value();
                type = maybe_type.value();
                token_1 = maybe_token_1.value();
                ws = maybe_ws.value();
                token_2 = maybe_token_2.value();
                ws_1 = maybe_ws_1.value();
                alts = maybe_alts.value();
                newlines = maybe_newlines.value();
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
            return std::any_cast<std::optional<Rule>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Alts> alts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Alts> {
            int pos = mark();
            std::optional<Alts> maybe_alts;
            Alts alts;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<String> maybe_ws_1;
            String ws_1;
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
            std::optional<Token> maybe_indent;
            Token indent;
            if (true
                && (maybe_alts = this->alts())
                && (maybe_indent = expect(TokenType::INDENT))
                && (maybe_token = expect("/"))
                && (maybe_ws = this->ws())
                && (maybe_alt = this->alt())
            ){
                alts = maybe_alts.value();
                indent = maybe_indent.value();
                token = maybe_token.value();
                ws = maybe_ws.value();
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
                return Alts{alt} ;
            }
            reset(pos);
            std::cout << "No parse found for alts\n";
            return {};
        };

        std::cout << "Parsing alts\n";
        std::optional<std::any> return_value = memoize("alts", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<Alts>>(return_value.value());
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
            std::optional<Strings> maybe_items;
            Strings items;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<Token> maybe_lcurl;
            Token lcurl;
            std::optional<String> maybe_type;
            String type;
            std::optional<Token> maybe_rcurl;
            Token rcurl;
            if (true
                && (maybe_items = this->items())
                && (maybe_ws = this->ws())
                && (maybe_lcurl = expect(TokenType::LCURL))
                && (maybe_type = this->type())
                && (maybe_rcurl = expect(TokenType::RCURL))
            ){
                items = maybe_items.value();
                ws = maybe_ws.value();
                lcurl = maybe_lcurl.value();
                type = maybe_type.value();
                rcurl = maybe_rcurl.value();
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
            return std::any_cast<std::optional<Alt>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Strings> items() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Strings> {
            int pos = mark();
            std::optional<Strings> maybe_items;
            Strings items;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<String> maybe_item;
            String item;
            if (true
                && (maybe_items = this->items())
                && (maybe_ws = this->ws())
                && (maybe_item = this->item())
            ){
                items = maybe_items.value();
                ws = maybe_ws.value();
                item = maybe_item.value();
                std::cout << "generating alt in rule: items\n";
                return append_vector(items, item) ;
            }
            reset(pos);
            if (true
                && (maybe_item = this->item())
            ){
                item = maybe_item.value();
                std::cout << "generating alt in rule: items\n";
                return Strings{item} ;
            }
            reset(pos);
            std::cout << "No parse found for items\n";
            return {};
        };

        std::cout << "Parsing items\n";
        std::optional<std::any> return_value = memoize("items", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<Strings>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> item() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
            int pos = mark();
            std::optional<Token> maybe_name;
            Token name;
            if (true
                && (maybe_name = expect(TokenType::NAME))
            ){
                name = maybe_name.value();
                std::cout << "generating alt in rule: item\n";
                return name.value ;
            }
            reset(pos);
            std::optional<Token> maybe_string;
            Token string;
            if (true
                && (maybe_string = expect(TokenType::STRING))
            ){
                string = maybe_string.value();
                std::cout << "generating alt in rule: item\n";
                return string.value ;
            }
            reset(pos);
            std::cout << "No parse found for item\n";
            return {};
        };

        std::cout << "Parsing item\n";
        std::optional<std::any> return_value = memoize("item", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> type() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
            int pos = mark();
            std::optional<String> maybe_parts;
            String parts;
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
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> parts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
            int pos = mark();
            std::optional<Token> maybe_lcurl;
            Token lcurl;
            std::optional<String> maybe_parts;
            String parts;
            std::optional<Token> maybe_rcurl;
            Token rcurl;
            if (true
                && (maybe_lcurl = expect(TokenType::LCURL))
                && (maybe_parts = this->parts())
                && (maybe_rcurl = expect(TokenType::RCURL))
            ){
                lcurl = maybe_lcurl.value();
                parts = maybe_parts.value();
                rcurl = maybe_rcurl.value();
                std::cout << "generating alt in rule: parts\n";
                return lcurl.value + parts + rcurl.value ;
            }
            reset(pos);
            std::optional<String> maybe_part;
            String part;
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
            if (true
                && (maybe_lcurl = expect(TokenType::LCURL))
                && (maybe_rcurl = expect(TokenType::RCURL))
            ){
                lcurl = maybe_lcurl.value();
                rcurl = maybe_rcurl.value();
                std::cout << "generating alt in rule: parts\n";
                return lcurl.value + rcurl.value ;
            }
            reset(pos);
            std::cout << "No parse found for parts\n";
            return {};
        };

        std::cout << "Parsing parts\n";
        std::optional<std::any> return_value = memoize("parts", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> part() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
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
            std::optional<Token> maybe_lparen;
            Token lparen;
            if (true
                && (maybe_lparen = expect(TokenType::LPAREN))
            ){
                lparen = maybe_lparen.value();
                std::cout << "generating alt in rule: part\n";
                return lparen.value ;
            }
            reset(pos);
            std::optional<Token> maybe_rparen;
            Token rparen;
            if (true
                && (maybe_rparen = expect(TokenType::RPAREN))
            ){
                rparen = maybe_rparen.value();
                std::cout << "generating alt in rule: part\n";
                return rparen.value ;
            }
            reset(pos);
            std::optional<String> maybe_ws;
            String ws;
            if (true
                && (maybe_ws = this->ws())
            ){
                ws = maybe_ws.value();
                std::cout << "generating alt in rule: part\n";
                return ws ;
            }
            reset(pos);
            std::optional<Token> maybe_comma;
            Token comma;
            if (true
                && (maybe_comma = expect(TokenType::COMMA))
            ){
                comma = maybe_comma.value();
                std::cout << "generating alt in rule: part\n";
                return comma.value ;
            }
            reset(pos);
            std::cout << "No parse found for part\n";
            return {};
        };

        std::cout << "Parsing part\n";
        std::optional<std::any> return_value = memoize("part", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> ws() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
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
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> newlines() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
            int pos = mark();
            std::optional<String> maybe_newlines;
            String newlines;
            std::optional<String> maybe_linebreak;
            String linebreak;
            if (true
                && (maybe_newlines = this->newlines())
                && (maybe_linebreak = this->linebreak())
            ){
                newlines = maybe_newlines.value();
                linebreak = maybe_linebreak.value();
                std::cout << "generating alt in rule: newlines\n";
                return newlines.append(linebreak) ;
            }
            reset(pos);
            if (true
                && (maybe_linebreak = this->linebreak())
            ){
                linebreak = maybe_linebreak.value();
                std::cout << "generating alt in rule: newlines\n";
                return linebreak ;
            }
            reset(pos);
            std::cout << "No parse found for newlines\n";
            return {};
        };

        std::cout << "Parsing newlines\n";
        std::optional<std::any> return_value = memoize("newlines", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> linebreak() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
            int pos = mark();
            std::optional<Token> maybe_newline;
            Token newline;
            if (true
                && (maybe_newline = expect(TokenType::NEWLINE))
            ){
                newline = maybe_newline.value();
                std::cout << "generating alt in rule: linebreak\n";
                return newline.value ;
            }
            reset(pos);
            std::optional<Token> maybe_indent;
            Token indent;
            if (true
                && (maybe_indent = expect(TokenType::INDENT))
            ){
                indent = maybe_indent.value();
                std::cout << "generating alt in rule: linebreak\n";
                return indent.value ;
            }
            reset(pos);
            std::optional<Token> maybe_unindent;
            Token unindent;
            if (true
                && (maybe_unindent = expect(TokenType::UNINDENT))
            ){
                unindent = maybe_unindent.value();
                std::cout << "generating alt in rule: linebreak\n";
                return unindent.value ;
            }
            reset(pos);
            std::cout << "No parse found for linebreak\n";
            return {};
        };

        std::cout << "Parsing linebreak\n";
        std::optional<std::any> return_value = memoize("linebreak", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<String>>(return_value.value());
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
    