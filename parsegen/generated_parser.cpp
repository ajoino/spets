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
            std::optional<Node> statement;
                std::cout << "\n### start: statement \n\n";
            if (true
                && (statement = this->statement())
            ){
                std::cout << "statement.value(): " << statement.value() << "\n";
                return Node{"start", {statement.value()}};
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
            std::optional<Node> expr;
            std::optional<Node> statement_end;
                std::cout << "\n### statement: expr statement_end \n\n";
            if (true
                && (expr = this->expr())
                && (statement_end = this->statement_end())
            ){
                std::cout << "expr.value(): " << expr.value() << "\n";
                std::cout << "statement_end.value(): " << statement_end.value() << "\n";
                return Node{"statement", {expr.value(), statement_end.value()}};
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
            std::optional<Node> atom1;
            std::optional<Node> atom2;
                std::cout << "\n### expr: atom1 '+' atom2 \n\n";
            if (true
                && (atom1 = this->atom1())
                && expect("+")
                && (atom2 = this->atom2())
            ){
                std::cout << "atom1.value(): " << atom1.value() << "\n";
                std::cout << "atom2.value(): " << atom2.value() << "\n";
                return Node{"expr", {atom1.value(), atom2.value()}};
            }
            std::optional<Node> atom3;
                std::cout << "\n### expr: atom3 \n\n";
            if (true
                && (atom3 = this->atom3())
            ){
                std::cout << "atom3.value(): " << atom3.value() << "\n";
                return Node{"expr", {atom3.value()}};
            }
            reset(pos);
            std::cout << "No parse found for expr\n";
            return {};
        };

        std::cout << "Parsing expr\n";
        return memoize("expr", inner_func, mark());
    }

    std::optional<Node> atom1() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Token> name;
                std::cout << "\n### atom1: name \n\n";
            if (true
                && (name = expect(TokenType::NAME))
            ){
                std::cout << "name.value().value: " << name.value().value << "\n";
                return Node{"atom1", {name.value().value}};
            }
            std::optional<Token> number;
                std::cout << "\n### atom1: number \n\n";
            if (true
                && (number = expect(TokenType::NUMBER))
            ){
                std::cout << "number.value().value: " << number.value().value << "\n";
                return Node{"atom1", {number.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for atom1\n";
            return {};
        };

        std::cout << "Parsing atom1\n";
        return memoize("atom1", inner_func, mark());
    }

    std::optional<Node> atom2() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Token> name;
                std::cout << "\n### atom2: name \n\n";
            if (true
                && (name = expect(TokenType::NAME))
            ){
                std::cout << "name.value().value: " << name.value().value << "\n";
                return Node{"atom2", {name.value().value}};
            }
            std::optional<Token> number;
                std::cout << "\n### atom2: number \n\n";
            if (true
                && (number = expect(TokenType::NUMBER))
            ){
                std::cout << "number.value().value: " << number.value().value << "\n";
                return Node{"atom2", {number.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for atom2\n";
            return {};
        };

        std::cout << "Parsing atom2\n";
        return memoize("atom2", inner_func, mark());
    }

    std::optional<Node> atom3() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Token> name;
                std::cout << "\n### atom3: name \n\n";
            if (true
                && (name = expect(TokenType::NAME))
            ){
                std::cout << "name.value().value: " << name.value().value << "\n";
                return Node{"atom3", {name.value().value}};
            }
            std::optional<Token> number;
                std::cout << "\n### atom3: number \n\n";
            if (true
                && (number = expect(TokenType::NUMBER))
            ){
                std::cout << "number.value().value: " << number.value().value << "\n";
                return Node{"atom3", {number.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for atom3\n";
            return {};
        };

        std::cout << "Parsing atom3\n";
        return memoize("atom3", inner_func, mark());
    }

    std::optional<Node> statement_end() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Token> newline;
                std::cout << "\n### statement_end: newline \n\n";
            if (true
                && (newline = expect(TokenType::NEWLINE))
            ){
                std::cout << "newline.value().value: " << newline.value().value << "\n";
                return Node{"statement_end", {newline.value().value}};
            }
            std::optional<Token> semicolon;
                std::cout << "\n### statement_end: semicolon \n\n";
            if (true
                && (semicolon = expect(TokenType::SEMICOLON))
            ){
                std::cout << "semicolon.value().value: " << semicolon.value().value << "\n";
                return Node{"statement_end", {semicolon.value().value}};
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
    