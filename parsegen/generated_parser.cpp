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
            std::optional<Node> assignment;
            std::optional<Node> statement_end;
                std::cout << "\n### statement: assignment statement_end \n\n";
            if (true
                && (assignment = this->assignment())
                && (statement_end = this->statement_end())
            ){
                std::cout << "assignment.value(): " << assignment.value() << "\n";
                std::cout << "statement_end.value(): " << statement_end.value() << "\n";
                return Node{"statement", {assignment.value(), statement_end.value()}};
            }
            std::optional<Node> expr;
                std::cout << "\n### statement: expr statement_end \n\n";
            if (true
                && (expr = this->expr())
                && (statement_end = this->statement_end())
            ){
                std::cout << "expr.value(): " << expr.value() << "\n";
                std::cout << "statement_end.value(): " << statement_end.value() << "\n";
                return Node{"statement", {expr.value(), statement_end.value()}};
            }
            std::optional<Node> if_statement;
                std::cout << "\n### statement: if_statement statement_end \n\n";
            if (true
                && (if_statement = this->if_statement())
                && (statement_end = this->statement_end())
            ){
                std::cout << "if_statement.value(): " << if_statement.value() << "\n";
                std::cout << "statement_end.value(): " << statement_end.value() << "\n";
                return Node{"statement", {if_statement.value(), statement_end.value()}};
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
            std::optional<Node> expr;
            std::optional<Node> term;
                std::cout << "\n### expr: expr '+' term \n\n";
            if (true
                && (expr = this->expr())
                && expect("+")
                && (term = this->term())
            ){
                std::cout << "expr.value(): " << expr.value() << "\n";
                std::cout << "term.value(): " << term.value() << "\n";
                return Node{"expr", {expr.value(), term.value()}};
            }
                std::cout << "\n### expr: expr '-' term \n\n";
            if (true
                && (expr = this->expr())
                && expect("-")
                && (term = this->term())
            ){
                std::cout << "expr.value(): " << expr.value() << "\n";
                std::cout << "term.value(): " << term.value() << "\n";
                return Node{"expr", {expr.value(), term.value()}};
            }
                std::cout << "\n### expr: term \n\n";
            if (true
                && (term = this->term())
            ){
                std::cout << "term.value(): " << term.value() << "\n";
                return Node{"expr", {term.value()}};
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
            std::optional<Node> term;
            std::optional<Node> atom;
                std::cout << "\n### term: term '*' atom \n\n";
            if (true
                && (term = this->term())
                && expect("*")
                && (atom = this->atom())
            ){
                std::cout << "term.value(): " << term.value() << "\n";
                std::cout << "atom.value(): " << atom.value() << "\n";
                return Node{"term", {term.value(), atom.value()}};
            }
                std::cout << "\n### term: term '/' atom \n\n";
            if (true
                && (term = this->term())
                && expect("/")
                && (atom = this->atom())
            ){
                std::cout << "term.value(): " << term.value() << "\n";
                std::cout << "atom.value(): " << atom.value() << "\n";
                return Node{"term", {term.value(), atom.value()}};
            }
                std::cout << "\n### term: atom \n\n";
            if (true
                && (atom = this->atom())
            ){
                std::cout << "atom.value(): " << atom.value() << "\n";
                return Node{"term", {atom.value()}};
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
            std::optional<Token> name;
                std::cout << "\n### atom: name \n\n";
            if (true
                && (name = expect(TokenType::NAME))
            ){
                std::cout << "name.value().value: " << name.value().value << "\n";
                return Node{"atom", {name.value().value}};
            }
            std::optional<Token> number;
                std::cout << "\n### atom: number \n\n";
            if (true
                && (number = expect(TokenType::NUMBER))
            ){
                std::cout << "number.value().value: " << number.value().value << "\n";
                return Node{"atom", {number.value().value}};
            }
            std::optional<Node> expr;
                std::cout << "\n### atom: '(' expr ')' \n\n";
            if (true
                && expect("(")
                && (expr = this->expr())
                && expect(")")
            ){
                std::cout << "expr.value(): " << expr.value() << "\n";
                return Node{"atom", {expr.value()}};
            }
            reset(pos);
            std::cout << "No parse found for atom\n";
            return {};
        };

        std::cout << "Parsing atom\n";
        return memoize("atom", inner_func, mark());
    }

    std::optional<Node> assignment() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Node> target;
            std::optional<Node> expr;
                std::cout << "\n### assignment: target '=' expr \n\n";
            if (true
                && (target = this->target())
                && expect("=")
                && (expr = this->expr())
            ){
                std::cout << "target.value(): " << target.value() << "\n";
                std::cout << "expr.value(): " << expr.value() << "\n";
                return Node{"assignment", {target.value(), expr.value()}};
            }
            reset(pos);
            std::cout << "No parse found for assignment\n";
            return {};
        };

        std::cout << "Parsing assignment\n";
        return memoize("assignment", inner_func, mark());
    }

    std::optional<Node> target() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Token> name;
                std::cout << "\n### target: name \n\n";
            if (true
                && (name = expect(TokenType::NAME))
            ){
                std::cout << "name.value().value: " << name.value().value << "\n";
                return Node{"target", {name.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for target\n";
            return {};
        };

        std::cout << "Parsing target\n";
        return memoize("target", inner_func, mark());
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

    std::optional<Node> if_statement() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
           int pos = mark();
            std::optional<Node> expr;
            std::optional<Node> statement;
                std::cout << "\n### if_statement: 'if' expr ':' statement \n\n";
            if (true
                && expect("if")
                && (expr = this->expr())
                && expect(":")
                && (statement = this->statement())
            ){
                std::cout << "expr.value(): " << expr.value() << "\n";
                std::cout << "statement.value(): " << statement.value() << "\n";
                return Node{"if_statement", {expr.value(), statement.value()}};
            }
            reset(pos);
            std::cout << "No parse found for if_statement\n";
            return {};
        };

        std::cout << "Parsing if_statement\n";
        return memoize("if_statement", inner_func, mark());
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
    