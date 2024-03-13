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
            std::optional<Node> maybe_statement_0;
            Node statement_0;
            if (true
                && (maybe_statement_0 = this->statement())
            ){
                statement_0 = maybe_statement_0.value();
                return Node{"start", {maybe_statement_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        std::optional<std::any> return_value = memoize("start", inner_func, mark());
        if (return_value) {
            return std::any_cast<Node>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Node> statement() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe_expr_0;
            Node expr_0;
            std::optional<Node> maybe_statement_end_0;
            Node statement_end_0;
            if (true
                && (maybe_expr_0 = this->expr())
                && (maybe_statement_end_0 = this->statement_end())
            ){
                expr_0 = maybe_expr_0.value();
                statement_end_0 = maybe_statement_end_0.value();
                return Node{"statement", {maybe_expr_0.value(), maybe_statement_end_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for statement\n";
            return {};
        };

        std::cout << "Parsing statement\n";
        std::optional<std::any> return_value = memoize("statement", inner_func, mark());
        if (return_value) {
            return std::any_cast<Node>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Node> expr() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe_expr_0;
            Node expr_0;
            std::optional<Token> maybe_token_0;
            Token token_0;
            std::optional<Node> maybe_term_0;
            Node term_0;
            if (true
                && (maybe_expr_0 = this->expr())
                && (maybe_token_0 = expect("+"))
                && (maybe_term_0 = this->term())
            ){
                expr_0 = maybe_expr_0.value();
                token_0 = maybe_token_0.value();
                term_0 = maybe_term_0.value();
                return Node{"expr", {maybe_expr_0.value(), Node("token", maybe_token_0.value().value), maybe_term_0.value()}};
            }
            reset(pos);
            if (true
                && (maybe_expr_0 = this->expr())
                && (maybe_token_0 = expect("-"))
                && (maybe_term_0 = this->term())
            ){
                expr_0 = maybe_expr_0.value();
                token_0 = maybe_token_0.value();
                term_0 = maybe_term_0.value();
                return Node{"expr", {maybe_expr_0.value(), Node("token", maybe_token_0.value().value), maybe_term_0.value()}};
            }
            reset(pos);
            if (true
                && (maybe_term_0 = this->term())
            ){
                expr_0 = maybe_expr_0.value();
                token_0 = maybe_token_0.value();
                term_0 = maybe_term_0.value();
                return Node{"expr", {maybe_term_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for expr\n";
            return {};
        };

        std::cout << "Parsing expr\n";
        std::optional<std::any> return_value = memoize("expr", inner_func, mark());
        if (return_value) {
            return std::any_cast<Node>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Node> term() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> maybe_term_0;
            Node term_0;
            std::optional<Token> maybe_token_0;
            Token token_0;
            std::optional<Node> maybe_atom_0;
            Node atom_0;
            if (true
                && (maybe_term_0 = this->term())
                && (maybe_token_0 = expect("*"))
                && (maybe_atom_0 = this->atom())
            ){
                term_0 = maybe_term_0.value();
                token_0 = maybe_token_0.value();
                atom_0 = maybe_atom_0.value();
                return Node{"term", {maybe_term_0.value(), Node("token", maybe_token_0.value().value), maybe_atom_0.value()}};
            }
            reset(pos);
            if (true
                && (maybe_term_0 = this->term())
                && (maybe_token_0 = expect("/"))
                && (maybe_atom_0 = this->atom())
            ){
                term_0 = maybe_term_0.value();
                token_0 = maybe_token_0.value();
                atom_0 = maybe_atom_0.value();
                return Node{"term", {maybe_term_0.value(), Node("token", maybe_token_0.value().value), maybe_atom_0.value()}};
            }
            reset(pos);
            if (true
                && (maybe_atom_0 = this->atom())
            ){
                term_0 = maybe_term_0.value();
                token_0 = maybe_token_0.value();
                atom_0 = maybe_atom_0.value();
                return Node{"term", {maybe_atom_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for term\n";
            return {};
        };

        std::cout << "Parsing term\n";
        std::optional<std::any> return_value = memoize("term", inner_func, mark());
        if (return_value) {
            return std::any_cast<Node>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Node> atom() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> maybe_token_0;
            Token token_0;
            std::optional<Node> maybe_expr_0;
            Node expr_0;
            std::optional<Token> maybe_token_1;
            Token token_1;
            if (true
                && (maybe_token_0 = expect("("))
                && (maybe_expr_0 = this->expr())
                && (maybe_token_1 = expect(")"))
            ){
                token_0 = maybe_token_0.value();
                expr_0 = maybe_expr_0.value();
                token_1 = maybe_token_1.value();
                return Node("atom", {expr_0.value()}) ;
            }
            reset(pos);
            std::optional<Token> maybe_name_0;
            Token name_0;
            if (true
                && (maybe_name_0 = expect(TokenType::NAME))
            ){
                token_0 = maybe_token_0.value();
                expr_0 = maybe_expr_0.value();
                token_1 = maybe_token_1.value();
                name_0 = maybe_name_0.value();
                return Node{"atom", {maybe_name_0.value().value}};
            }
            reset(pos);
            std::optional<Token> maybe_number_0;
            Token number_0;
            if (true
                && (maybe_number_0 = expect(TokenType::NUMBER))
            ){
                token_0 = maybe_token_0.value();
                expr_0 = maybe_expr_0.value();
                token_1 = maybe_token_1.value();
                name_0 = maybe_name_0.value();
                number_0 = maybe_number_0.value();
                return Node{"atom", {maybe_number_0.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for atom\n";
            return {};
        };

        std::cout << "Parsing atom\n";
        std::optional<std::any> return_value = memoize("atom", inner_func, mark());
        if (return_value) {
            return std::any_cast<Node>(return_value.value());
        } else {
           return std::nullopt;
        }
    }

    std::optional<Node> statement_end() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> maybe_newline_0;
            Token newline_0;
            if (true
                && (maybe_newline_0 = expect(TokenType::NEWLINE))
            ){
                newline_0 = maybe_newline_0.value();
                return Node{"statement_end", {maybe_newline_0.value().value}};
            }
            reset(pos);
            std::optional<Token> maybe_semicolon_0;
            Token semicolon_0;
            if (true
                && (maybe_semicolon_0 = expect(TokenType::SEMICOLON))
            ){
                newline_0 = maybe_newline_0.value();
                semicolon_0 = maybe_semicolon_0.value();
                return Node{"statement_end", {maybe_semicolon_0.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for statement_end\n";
            return {};
        };

        std::cout << "Parsing statement_end\n";
        std::optional<std::any> return_value = memoize("statement_end", inner_func, mark());
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
    