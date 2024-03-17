#include <optional>
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


    std::optional<Rules> MetaParser::start() {

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
                return  rules ;
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

    std::optional<Rules> MetaParser::rules() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Rules> {
            int pos = mark();
            std::optional<Rule> maybe_rule;
            Rule rule;
            std::optional<Rules> maybe_rules;
            Rules rules;
            if (true
                && (maybe_rule = this->rule())
                && (maybe_rules = this->rules())
            ){
                rule = maybe_rule.value();
                rules = maybe_rules.value();
                std::cout << "generating alt in rule: rules\n";
                return  prepend_vector(rules, rule) ;
            }
            reset(pos);
            if (true
                && (maybe_rule = this->rule())
            ){
                rule = maybe_rule.value();
                std::cout << "generating alt in rule: rules\n";
                return  create_vector(rule) ;
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

    std::optional<Rule> MetaParser::rule() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Rule> {
            int pos = mark();
            std::optional<Token> maybe_name;
            Token name;
            std::optional<Token> maybe_lbrack;
            Token lbrack;
            std::optional<String> maybe_type;
            String type;
            std::optional<Token> maybe_rbrack;
            Token rbrack;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<String> maybe_ws_1;
            String ws_1;
            std::optional<Alts> maybe_alts;
            Alts alts;
            std::optional<Token> maybe_newline;
            Token newline;
            std::optional<Token> maybe_indent;
            Token indent;
            std::optional<Alts> maybe_more_alts;
            Alts more_alts;
            std::optional<Token> maybe_unindent;
            Token unindent;
            if (true
                && (maybe_name = expect(TokenType::NAME))
                && (maybe_lbrack = expect(TokenType::LBRACK))
                && (maybe_type = this->type())
                && (maybe_rbrack = expect(TokenType::RBRACK))
                && (maybe_ws = this->ws())
                && (maybe_token = expect("<-"))
                && (maybe_ws_1 = this->ws())
                && (maybe_alts = this->alts())
                && (maybe_newline = expect(TokenType::NEWLINE))
                && (maybe_indent = expect(TokenType::INDENT))
                && (maybe_more_alts = this->more_alts())
                && (maybe_unindent = expect(TokenType::UNINDENT))
            ){
                name = maybe_name.value();
                lbrack = maybe_lbrack.value();
                type = maybe_type.value();
                rbrack = maybe_rbrack.value();
                ws = maybe_ws.value();
                token = maybe_token.value();
                ws_1 = maybe_ws_1.value();
                alts = maybe_alts.value();
                newline = maybe_newline.value();
                indent = maybe_indent.value();
                more_alts = maybe_more_alts.value();
                unindent = maybe_unindent.value();
                std::cout << "generating alt in rule: rule\n";
                return  Rule(name.value, concat(alts, more_alts), type) ;
            }
            reset(pos);
            if (true
                && (maybe_name = expect(TokenType::NAME))
                && (maybe_lbrack = expect(TokenType::LBRACK))
                && (maybe_type = this->type())
                && (maybe_rbrack = expect(TokenType::RBRACK))
                && (maybe_ws = this->ws())
                && (maybe_token = expect("<-"))
                && (maybe_ws_1 = this->ws())
                && (maybe_alts = this->alts())
                && (maybe_newline = expect(TokenType::NEWLINE))
            ){
                name = maybe_name.value();
                lbrack = maybe_lbrack.value();
                type = maybe_type.value();
                rbrack = maybe_rbrack.value();
                ws = maybe_ws.value();
                token = maybe_token.value();
                ws_1 = maybe_ws_1.value();
                alts = maybe_alts.value();
                newline = maybe_newline.value();
                std::cout << "generating alt in rule: rule\n";
                return  Rule(name.value, alts, type) ;
            }
            reset(pos);
            if (true
                && (maybe_name = expect(TokenType::NAME))
                && (maybe_lbrack = expect(TokenType::LBRACK))
                && (maybe_type = this->type())
                && (maybe_rbrack = expect(TokenType::RBRACK))
                && (maybe_ws = this->ws())
                && (maybe_token = expect("<-"))
                && (maybe_ws_1 = this->ws())
                && (maybe_newline = expect(TokenType::NEWLINE))
                && (maybe_indent = expect(TokenType::INDENT))
                && (maybe_more_alts = this->more_alts())
                && (maybe_unindent = expect(TokenType::UNINDENT))
            ){
                name = maybe_name.value();
                lbrack = maybe_lbrack.value();
                type = maybe_type.value();
                rbrack = maybe_rbrack.value();
                ws = maybe_ws.value();
                token = maybe_token.value();
                ws_1 = maybe_ws_1.value();
                newline = maybe_newline.value();
                indent = maybe_indent.value();
                more_alts = maybe_more_alts.value();
                unindent = maybe_unindent.value();
                std::cout << "generating alt in rule: rule\n";
                return  Rule(name.value, more_alts, type) ;
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

    std::optional<Alts> MetaParser::more_alts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Alts> {
            int pos = mark();
            std::optional<Token> maybe_token;
            Token token;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<Alts> maybe_alts;
            Alts alts;
            std::optional<Token> maybe_newline;
            Token newline;
            std::optional<Alts> maybe_more_alts;
            Alts more_alts;
            if (true
                && (maybe_token = expect("/"))
                && (maybe_ws = this->ws())
                && (maybe_alts = this->alts())
                && (maybe_newline = expect(TokenType::NEWLINE))
                && (maybe_more_alts = this->more_alts())
            ){
                token = maybe_token.value();
                ws = maybe_ws.value();
                alts = maybe_alts.value();
                newline = maybe_newline.value();
                more_alts = maybe_more_alts.value();
                std::cout << "generating alt in rule: more_alts\n";
                return  concat(alts, more_alts) ;
            }
            reset(pos);
            if (true
                && (maybe_token = expect("/"))
                && (maybe_ws = this->ws())
                && (maybe_alts = this->alts())
                && (maybe_newline = expect(TokenType::NEWLINE))
            ){
                token = maybe_token.value();
                ws = maybe_ws.value();
                alts = maybe_alts.value();
                newline = maybe_newline.value();
                std::cout << "generating alt in rule: more_alts\n";
                return  alts ;
            }
            reset(pos);
            std::cout << "No parse found for more_alts\n";
            return {};
        };

        std::cout << "Parsing more_alts\n";
        std::optional<std::any> return_value = memoize("more_alts", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<Alts>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Alts> MetaParser::alts() {

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
                return  append_vector(alts, alt) ;
            }
            reset(pos);
            if (true
                && (maybe_alt = this->alt())
            ){
                alt = maybe_alt.value();
                std::cout << "generating alt in rule: alts\n";
                return  create_vector(alt) ;
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

    std::optional<Alt> MetaParser::alt() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Alt> {
            int pos = mark();
            std::optional<Items> maybe_items;
            Items items;
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
                return  Alt(items, type) ;
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

    std::optional<Items> MetaParser::items() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Items> {
            int pos = mark();
            std::optional<Items> maybe_items;
            Items items;
            std::optional<String> maybe_ws;
            String ws;
            std::optional<Item> maybe_item;
            Item item;
            if (true
                && (maybe_items = this->items())
                && (maybe_ws = this->ws())
                && (maybe_item = this->item())
            ){
                items = maybe_items.value();
                ws = maybe_ws.value();
                item = maybe_item.value();
                std::cout << "generating alt in rule: items\n";
                return  append_vector(items, item) ;
            }
            reset(pos);
            if (true
                && (maybe_item = this->item())
            ){
                item = maybe_item.value();
                std::cout << "generating alt in rule: items\n";
                return  create_vector(item) ;
            }
            reset(pos);
            std::cout << "No parse found for items\n";
            return {};
        };

        std::cout << "Parsing items\n";
        std::optional<std::any> return_value = memoize("items", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<Items>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<Item> MetaParser::item() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Item> {
            int pos = mark();
            std::optional<Token> maybe_name;
            Token name;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<String> maybe_atom;
            String atom;
            if (true
                && (maybe_name = expect(TokenType::NAME))
                && (maybe_token = expect("="))
                && (maybe_atom = this->atom())
            ){
                name = maybe_name.value();
                token = maybe_token.value();
                atom = maybe_atom.value();
                std::cout << "generating alt in rule: item\n";
                return  Item(atom, name.value) ;
            }
            reset(pos);
            if (true
                && (maybe_atom = this->atom())
            ){
                atom = maybe_atom.value();
                std::cout << "generating alt in rule: item\n";
                return  Item(atom) ;
            }
            reset(pos);
            std::cout << "No parse found for item\n";
            return {};
        };

        std::cout << "Parsing item\n";
        std::optional<std::any> return_value = memoize("item", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<Item>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> MetaParser::atom() {

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
                std::cout << "generating alt in rule: atom\n";
                return  name.value ;
            }
            reset(pos);
            std::optional<Token> maybe_string;
            Token string;
            if (true
                && (maybe_string = expect(TokenType::STRING))
            ){
                string = maybe_string.value();
                std::cout << "generating alt in rule: atom\n";
                return  string.value ;
            }
            reset(pos);
            std::cout << "No parse found for atom\n";
            return {};
        };

        std::cout << "Parsing atom\n";
        std::optional<std::any> return_value = memoize("atom", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
            return std::any_cast<std::optional<String>>(return_value.value());
        } else {
            std::cout << " value is null\n";
            return std::nullopt;
        }
    }

    std::optional<String> MetaParser::type() {

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
                return  parts ;
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

    std::optional<String> MetaParser::parts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<String> {
            int pos = mark();
            std::optional<String> maybe_parts;
            String parts;
            std::optional<String> maybe_part;
            String part;
            if (true
                && (maybe_parts = this->parts())
                && (maybe_part = this->part())
            ){
                parts = maybe_parts.value();
                part = maybe_part.value();
                std::cout << "generating alt in rule: parts\n";
                return  parts.append(part) ;
            }
            reset(pos);
            if (true
                && (maybe_part = this->part())
            ){
                part = maybe_part.value();
                std::cout << "generating alt in rule: parts\n";
                return  part ;
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

    std::optional<String> MetaParser::part() {

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
                return  name.value ;
            }
            reset(pos);
            std::optional<Token> maybe_semicolon;
            Token semicolon;
            if (true
                && (maybe_semicolon = expect(TokenType::SEMICOLON))
            ){
                semicolon = maybe_semicolon.value();
                std::cout << "generating alt in rule: part\n";
                return  semicolon.value ;
            }
            reset(pos);
            std::optional<Token> maybe_token;
            Token token;
            if (true
                && (maybe_token = expect("<"))
            ){
                token = maybe_token.value();
                std::cout << "generating alt in rule: part\n";
                return  token.value ;
            }
            reset(pos);
            if (true
                && (maybe_token = expect(">"))
            ){
                token = maybe_token.value();
                std::cout << "generating alt in rule: part\n";
                return  token.value ;
            }
            reset(pos);
            std::optional<Token> maybe_lparen;
            Token lparen;
            if (true
                && (maybe_lparen = expect(TokenType::LPAREN))
            ){
                lparen = maybe_lparen.value();
                std::cout << "generating alt in rule: part\n";
                return  lparen.value ;
            }
            reset(pos);
            std::optional<Token> maybe_rparen;
            Token rparen;
            if (true
                && (maybe_rparen = expect(TokenType::RPAREN))
            ){
                rparen = maybe_rparen.value();
                std::cout << "generating alt in rule: part\n";
                return  rparen.value ;
            }
            reset(pos);
            std::optional<String> maybe_ws;
            String ws;
            if (true
                && (maybe_ws = this->ws())
            ){
                ws = maybe_ws.value();
                std::cout << "generating alt in rule: part\n";
                return  ws ;
            }
            reset(pos);
            std::optional<Token> maybe_comma;
            Token comma;
            if (true
                && (maybe_comma = expect(TokenType::COMMA))
            ){
                comma = maybe_comma.value();
                std::cout << "generating alt in rule: part\n";
                return  comma.value ;
            }
            reset(pos);
            std::optional<Token> maybe_dot;
            Token dot;
            if (true
                && (maybe_dot = expect(TokenType::DOT))
            ){
                dot = maybe_dot.value();
                std::cout << "generating alt in rule: part\n";
                return  dot.value ;
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

    std::optional<String> MetaParser::ws() {

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
                return  whitespace.value ;
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

