#include <optional>
#include <fstream>

#include <parser/node.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
#include <parser/parsing_helpers.hpp>

#include <parsegen/rule.hpp>
    
class Toyparser : public Parser {
public:
    std::optional<Node> start() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe__0;
            Node _0;
            if (true
                && (maybe__0 = this->statement())
            ){
                std::cout << "generating alt in rule: start\n";
                statement_0 = maybe_statement_0.value();
                return {Node{"start", {maybe__0.value()}}};
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        std::optional<std::any> return_value = memoize("start", inner_func, mark());
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

    std::optional<Node> statement() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe__0;
            Node _0;
            if (true
                && (maybe__0 = this->expr())
                && (maybe__0 = this->statement_end())
            ){
                std::cout << "generating alt in rule: statement\n";
                expr_0 = maybe_expr_0.value();
                statement_end_0 = maybe_statement_end_0.value();
                return {Node{"statement", {maybe__0.value(), maybe__0.value()}}};
            }
            reset(pos);
            std::cout << "No parse found for statement\n";
            return {};
        };

        std::cout << "Parsing statement\n";
        std::optional<std::any> return_value = memoize("statement", inner_func, mark());
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

    std::optional<Node> expr() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe__0;
            Node _0;
            if (true
                && (maybe__0 = this->expr())
                && (maybe_token_0 = expect("+"))
                && (maybe__0 = this->term())
            ){
                std::cout << "generating alt in rule: expr\n";
                expr_0 = maybe_expr_0.value();
                '+'_0 = maybe_'+'_0.value();
                term_0 = maybe_term_0.value();
                return {Node{"expr", {maybe__0.value(), Node("token", maybe_token_0.value().value), maybe__0.value()}}};
            }
            reset(pos);
            if (true
                && (maybe__0 = this->expr())
                && (maybe_token_0 = expect("-"))
                && (maybe__0 = this->term())
            ){
                std::cout << "generating alt in rule: expr\n";
                expr_0 = maybe_expr_0.value();
                '-'_0 = maybe_'-'_0.value();
                term_0 = maybe_term_0.value();
                return {Node{"expr", {maybe__0.value(), Node("token", maybe_token_0.value().value), maybe__0.value()}}};
            }
            reset(pos);
            if (true
                && (maybe__0 = this->term())
            ){
                std::cout << "generating alt in rule: expr\n";
                term_0 = maybe_term_0.value();
                return {Node{"expr", {maybe__0.value()}}};
            }
            reset(pos);
            std::cout << "No parse found for expr\n";
            return {};
        };

        std::cout << "Parsing expr\n";
        std::optional<std::any> return_value = memoize("expr", inner_func, mark());
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

    std::optional<Node> term() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe__0;
            Node _0;
            if (true
                && (maybe__0 = this->term())
                && (maybe_token_0 = expect("*"))
                && (maybe__0 = this->atom())
            ){
                std::cout << "generating alt in rule: term\n";
                term_0 = maybe_term_0.value();
                '*'_0 = maybe_'*'_0.value();
                atom_0 = maybe_atom_0.value();
                return {Node{"term", {maybe__0.value(), Node("token", maybe_token_0.value().value), maybe__0.value()}}};
            }
            reset(pos);
            if (true
                && (maybe__0 = this->term())
                && (maybe_token_0 = expect("/"))
                && (maybe__0 = this->atom())
            ){
                std::cout << "generating alt in rule: term\n";
                term_0 = maybe_term_0.value();
                '/'_0 = maybe_'/'_0.value();
                atom_0 = maybe_atom_0.value();
                return {Node{"term", {maybe__0.value(), Node("token", maybe_token_0.value().value), maybe__0.value()}}};
            }
            reset(pos);
            if (true
                && (maybe__0 = this->atom())
            ){
                std::cout << "generating alt in rule: term\n";
                atom_0 = maybe_atom_0.value();
                return {Node{"term", {maybe__0.value()}}};
            }
            reset(pos);
            std::cout << "No parse found for term\n";
            return {};
        };

        std::cout << "Parsing term\n";
        std::optional<std::any> return_value = memoize("term", inner_func, mark());
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

    std::optional<Node> atom() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> maybe_token_2;
            Token token_2;
            std::optional<Node> maybe__0;
            Node _0;
            if (true
                && (maybe_token_0 = expect("("))
                && (maybe__0 = this->expr())
                && (maybe_token_1 = expect(")"))
            ){
                std::cout << "generating alt in rule: atom\n";
                '('_0 = maybe_'('_0.value();
                expr_0 = maybe_expr_0.value();
                ')'_0 = maybe_')'_0.value();
                return Node("atom", {expr_0.value()}) ;
            }
            reset(pos);
            if (true
                && (maybe__0 = expect(TokenType::NAME))
            ){
                std::cout << "generating alt in rule: atom\n";
                NAME_0 = maybe_NAME_0.value();
                return {Node{"atom", {maybe__0.value().value}}};
            }
            reset(pos);
            if (true
                && (maybe__0 = expect(TokenType::NUMBER))
            ){
                std::cout << "generating alt in rule: atom\n";
                NUMBER_0 = maybe_NUMBER_0.value();
                return {Node{"atom", {maybe__0.value().value}}};
            }
            reset(pos);
            std::cout << "No parse found for atom\n";
            return {};
        };

        std::cout << "Parsing atom\n";
        std::optional<std::any> return_value = memoize("atom", inner_func, mark());
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

    std::optional<Node> statement_end() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> maybe__0;
            Token _0;
            if (true
                && (maybe__0 = expect(TokenType::NEWLINE))
            ){
                std::cout << "generating alt in rule: statement_end\n";
                NEWLINE_0 = maybe_NEWLINE_0.value();
                return {Node{"statement_end", {maybe__0.value().value}}};
            }
            reset(pos);
            if (true
                && (maybe__0 = expect(TokenType::SEMICOLON))
            ){
                std::cout << "generating alt in rule: statement_end\n";
                SEMICOLON_0 = maybe_SEMICOLON_0.value();
                return {Node{"statement_end", {maybe__0.value().value}}};
            }
            reset(pos);
            std::cout << "No parse found for statement_end\n";
            return {};
        };

        std::cout << "Parsing statement_end\n";
        std::optional<std::any> return_value = memoize("statement_end", inner_func, mark());
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
    