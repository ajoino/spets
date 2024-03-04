#include <optional>
#include <fstream>
#include <optional>

#include "node.hpp"
#include "lexer.hpp"
#include "parser.hpp"
    
class Toyparser : public Parser {
public:
    std::optional<Node> start() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> statement_0;
            std::cout << "\n### start: statement \n\n";
            if (true
                && (statement_0 = this->statement())
            ){
                std::cout << "statement_0.value(): " << statement_0.value() << "\n";
                return Node{"start", {statement_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        return memoize("start", inner_func, mark());
    }

    std::optional<Node> statement() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> expr_0;
            std::optional<Node> statement_end_0;
            std::cout << "\n### statement: expr statement_end \n\n";
            if (true
                && (expr_0 = this->expr())
                && (statement_end_0 = this->statement_end())
            ){
                std::cout << "expr_0.value(): " << expr_0.value() << "\n";
                std::cout << "statement_end_0.value(): " << statement_end_0.value() << "\n";
                return Node{"statement", {expr_0.value(), statement_end_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for statement\n";
            return {};
        };

        std::cout << "Parsing statement\n";
        return memoize("statement", inner_func, mark());
    }

    std::optional<Node> expr() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> expr_0;
            std::optional<Token> token_0;
            std::optional<Node> term_0;
            std::cout << "\n### expr: expr '+' term \n\n";
            if (true
                && (expr_0 = this->expr())
                && (token_0 = expect("+"))
                && (term_0 = this->term())
            ){
                std::cout << "expr_0.value(): " << expr_0.value() << "\n";
                std::cout << "Node(\"token\", token_0.value().value): " << Node("token", token_0.value().value) << "\n";
                std::cout << "term_0.value(): " << term_0.value() << "\n";
                return Node{"expr", {expr_0.value(), Node("token", token_0.value().value), term_0.value()}};
            }
            reset(pos);
            std::cout << "\n### expr: expr '-' term \n\n";
            if (true
                && (expr_0 = this->expr())
                && (token_0 = expect("-"))
                && (term_0 = this->term())
            ){
                std::cout << "expr_0.value(): " << expr_0.value() << "\n";
                std::cout << "Node(\"token\", token_0.value().value): " << Node("token", token_0.value().value) << "\n";
                std::cout << "term_0.value(): " << term_0.value() << "\n";
                return Node{"expr", {expr_0.value(), Node("token", token_0.value().value), term_0.value()}};
            }
            reset(pos);
            std::cout << "\n### expr: term \n\n";
            if (true
                && (term_0 = this->term())
            ){
                std::cout << "term_0.value(): " << term_0.value() << "\n";
                return Node{"expr", {term_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for expr\n";
            return {};
        };

        std::cout << "Parsing expr\n";
        return memoize("expr", inner_func, mark());
    }

    std::optional<Node> term() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> term_0;
            std::optional<Token> token_0;
            std::optional<Node> atom_0;
            std::cout << "\n### term: term '*' atom \n\n";
            if (true
                && (term_0 = this->term())
                && (token_0 = expect("*"))
                && (atom_0 = this->atom())
            ){
                std::cout << "term_0.value(): " << term_0.value() << "\n";
                std::cout << "Node(\"token\", token_0.value().value): " << Node("token", token_0.value().value) << "\n";
                std::cout << "atom_0.value(): " << atom_0.value() << "\n";
                return Node{"term", {term_0.value(), Node("token", token_0.value().value), atom_0.value()}};
            }
            reset(pos);
            std::cout << "\n### term: term '/' atom \n\n";
            if (true
                && (term_0 = this->term())
                && (token_0 = expect("/"))
                && (atom_0 = this->atom())
            ){
                std::cout << "term_0.value(): " << term_0.value() << "\n";
                std::cout << "Node(\"token\", token_0.value().value): " << Node("token", token_0.value().value) << "\n";
                std::cout << "atom_0.value(): " << atom_0.value() << "\n";
                return Node{"term", {term_0.value(), Node("token", token_0.value().value), atom_0.value()}};
            }
            reset(pos);
            std::cout << "\n### term: atom \n\n";
            if (true
                && (atom_0 = this->atom())
            ){
                std::cout << "atom_0.value(): " << atom_0.value() << "\n";
                return Node{"term", {atom_0.value()}};
            }
            reset(pos);
            std::cout << "No parse found for term\n";
            return {};
        };

        std::cout << "Parsing term\n";
        return memoize("term", inner_func, mark());
    }

    std::optional<Node> atom() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> token_0;
            std::optional<Node> expr_0;
            std::optional<Token> token_1;
            std::cout << "\n### atom: '(' expr ')' \n\n";
            if (true
                && (token_0 = expect("("))
                && (expr_0 = this->expr())
                && (token_1 = expect(")"))
            ){
                std::cout << "Node(\"token\", token_0.value().value): " << Node("token", token_0.value().value) << "\n";
                std::cout << "expr_0.value(): " << expr_0.value() << "\n";
                std::cout << "Node(\"token\", token_1.value().value): " << Node("token", token_1.value().value) << "\n";
                return Node("atom", {expr_0.value()}) ;
            }
            reset(pos);
            std::optional<Token> name_0;
            std::cout << "\n### atom: name \n\n";
            if (true
                && (name_0 = expect(TokenType::NAME))
            ){
                std::cout << "name_0.value().value: " << name_0.value().value << "\n";
                return Node{"atom", {name_0.value().value}};
            }
            reset(pos);
            std::optional<Token> number_0;
            std::cout << "\n### atom: number \n\n";
            if (true
                && (number_0 = expect(TokenType::NUMBER))
            ){
                std::cout << "number_0.value().value: " << number_0.value().value << "\n";
                return Node{"atom", {number_0.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for atom\n";
            return {};
        };

        std::cout << "Parsing atom\n";
        return memoize("atom", inner_func, mark());
    }

    std::optional<Node> statement_end() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> newline_0;
            std::cout << "\n### statement_end: newline \n\n";
            if (true
                && (newline_0 = expect(TokenType::NEWLINE))
            ){
                std::cout << "newline_0.value().value: " << newline_0.value().value << "\n";
                return Node{"statement_end", {newline_0.value().value}};
            }
            reset(pos);
            std::optional<Token> semicolon_0;
            std::cout << "\n### statement_end: semicolon \n\n";
            if (true
                && (semicolon_0 = expect(TokenType::SEMICOLON))
            ){
                std::cout << "semicolon_0.value().value: " << semicolon_0.value().value << "\n";
                return Node{"statement_end", {semicolon_0.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for statement_end\n";
            return {};
        };

        std::cout << "Parsing statement_end\n";
        return memoize("statement_end", inner_func, mark());
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
    std::cout << nodes.value() << "\n";
    std::cout << "number of children " << nodes.value().children.size() << "\n";
    for (const auto& child : nodes.value().children) {
        std::cout << child << "\n";
    }
  } else {
    std::cout << "Could not parse content.\n";
  }
}
    