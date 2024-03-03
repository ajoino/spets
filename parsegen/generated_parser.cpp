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
            std::optional<Node> atom;
            std::optional<Node> statement_end;
                std::cout << "\n### statement: atom statement_end \n\n";
            if (true
                && (atom = this->atom())
                && (statement_end = this->statement_end())
            ){
                std::cout << "atom.value(): " << atom.value() << "\n";
                std::cout << "statement_end.value(): " << statement_end.value() << "\n";
                return Node{"statement", {atom.value(), statement_end.value()}};
            }
            reset(pos);
            std::cout << "No parse found for statement\n";
            return {};
        };

        std::cout << "Parsing statement\n";
        return memoize("statement", inner_func, mark());
    }

    std::optional<Node> atom() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> name;
                std::cout << "\n### atom: name \n\n";
            if (true
                && (name = expect(TokenType::NAME))
            ){
                std::cout << "name.value().value: " << name.value().value << "\n";
                return Node{"atom", {name.value().value}};
            }
            reset(pos);
            std::optional<Token> number;
                std::cout << "\n### atom: number \n\n";
            if (true
                && (number = expect(TokenType::NUMBER))
            ){
                std::cout << "number.value().value: " << number.value().value << "\n";
                return Node{"atom", {number.value().value}};
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
            std::optional<Token> newline;
                std::cout << "\n### statement_end: newline \n\n";
            if (true
                && (newline = expect(TokenType::NEWLINE))
            ){
                std::cout << "newline.value().value: " << newline.value().value << "\n";
                return Node{"statement_end", {newline.value().value}};
            }
            reset(pos);
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
    