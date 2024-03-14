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
            std::optional<Node> maybe_statement;
            Node statement;
            if (true
                && (maybe_statement = this->statement())
            ){
                statement = maybe_statement.value();
                std::cout << "generating alt in rule: start\n";
                return {Node{"start", {}}};
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        std::optional<std::any> return_value = memoize("start", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
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
            std::optional<Node> maybe_expr;
            Node expr;
            std::optional<Node> maybe_statement_end;
            Node statement_end;
            if (true
                && (maybe_expr = this->expr())
                && (maybe_statement_end = this->statement_end())
            ){
                expr = maybe_expr.value();
                statement_end = maybe_statement_end.value();
                std::cout << "generating alt in rule: statement\n";
                return {Node{"statement", {}}};
            }
            reset(pos);
            std::cout << "No parse found for statement\n";
            return {};
        };

        std::cout << "Parsing statement\n";
        std::optional<std::any> return_value = memoize("statement", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
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
            std::optional<Node> maybe_expr;
            Node expr;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<Node> maybe_term;
            Node term;
            if (true
                && (maybe_expr = this->expr())
                && (maybe_token = expect('+'))
                && (maybe_term = this->term())
            ){
                expr = maybe_expr.value();
                token = maybe_token.value();
                term = maybe_term.value();
                std::cout << "generating alt in rule: expr\n";
                return {Node{"expr", {}}};
            }
            reset(pos);
            if (true
                && (maybe_expr = this->expr())
                && (maybe_token = expect('-'))
                && (maybe_term = this->term())
            ){
                expr = maybe_expr.value();
                token = maybe_token.value();
                term = maybe_term.value();
                std::cout << "generating alt in rule: expr\n";
                return {Node{"expr", {}}};
            }
            reset(pos);
            if (true
                && (maybe_term = this->term())
            ){
                term = maybe_term.value();
                std::cout << "generating alt in rule: expr\n";
                return {Node{"expr", {}}};
            }
            reset(pos);
            std::cout << "No parse found for expr\n";
            return {};
        };

        std::cout << "Parsing expr\n";
        std::optional<std::any> return_value = memoize("expr", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
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
            std::optional<Node> maybe_term;
            Node term;
            std::optional<Token> maybe_token;
            Token token;
            std::optional<Node> maybe_atom;
            Node atom;
            if (true
                && (maybe_term = this->term())
                && (maybe_token = expect('*'))
                && (maybe_atom = this->atom())
            ){
                term = maybe_term.value();
                token = maybe_token.value();
                atom = maybe_atom.value();
                std::cout << "generating alt in rule: term\n";
                return {Node{"term", {}}};
            }
            reset(pos);
            if (true
                && (maybe_term = this->term())
                && (maybe_token = expect('/'))
                && (maybe_atom = this->atom())
            ){
                term = maybe_term.value();
                token = maybe_token.value();
                atom = maybe_atom.value();
                std::cout << "generating alt in rule: term\n";
                return {Node{"term", {}}};
            }
            reset(pos);
            if (true
                && (maybe_atom = this->atom())
            ){
                atom = maybe_atom.value();
                std::cout << "generating alt in rule: term\n";
                return {Node{"term", {}}};
            }
            reset(pos);
            std::cout << "No parse found for term\n";
            return {};
        };

        std::cout << "Parsing term\n";
        std::optional<std::any> return_value = memoize("term", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
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
            std::optional<Token> maybe_token;
            Token token;
            std::optional<Node> maybe_expr;
            Node expr;
            std::optional<Token> maybe_token_1;
            Token token_1;
            if (true
                && (maybe_token = expect('('))
                && (maybe_expr = this->expr())
                && (maybe_token_1 = expect(')'))
            ){
                token = maybe_token.value();
                expr = maybe_expr.value();
                token_1 = maybe_token_1.value();
                std::cout << "generating alt in rule: atom\n";
                return Node("atom", {expr_0.value()}) ;
            }
            reset(pos);
            std::optional<Token> maybe_name;
            Token name;
            if (true
                && (maybe_name = expect(TokenType::NAME))
            ){
                name = maybe_name.value();
                std::cout << "generating alt in rule: atom\n";
                return {Node{"atom", {}}};
            }
            reset(pos);
            std::optional<Token> maybe_number;
            Token number;
            if (true
                && (maybe_number = expect(TokenType::NUMBER))
            ){
                number = maybe_number.value();
                std::cout << "generating alt in rule: atom\n";
                return {Node{"atom", {}}};
            }
            reset(pos);
            std::cout << "No parse found for atom\n";
            return {};
        };

        std::cout << "Parsing atom\n";
        std::optional<std::any> return_value = memoize("atom", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
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
            std::optional<Token> maybe_newline;
            Token newline;
            if (true
                && (maybe_newline = expect(TokenType::NEWLINE))
            ){
                newline = maybe_newline.value();
                std::cout << "generating alt in rule: statement_end\n";
                return {Node{"statement_end", {}}};
            }
            reset(pos);
            std::optional<Token> maybe_semicolon;
            Token semicolon;
            if (true
                && (maybe_semicolon = expect(TokenType::SEMICOLON))
            ){
                semicolon = maybe_semicolon.value();
                std::cout << "generating alt in rule: statement_end\n";
                return {Node{"statement_end", {}}};
            }
            reset(pos);
            std::cout << "No parse found for statement_end\n";
            return {};
        };

        std::cout << "Parsing statement_end\n";
        std::optional<std::any> return_value = memoize("statement_end", inner_func, mark());
        if (return_value) {
            std::cout << " value not null\n";
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
    