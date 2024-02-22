#include <optional>
#include <fstream>
#include <optional>

#include "node.hpp"
#include "lexer.hpp"
#include "parser.hpp"
    
class Toyparser : public Parser {
public:
    std::optional<Node> statement() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "statement";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        }
        std::optional<Node> assignment;
        if (true
            && (assignment = this->assignment())
        ){
            Node res("statement", {assignment.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        std::optional<Node> expr;
        if (true
            && (expr = this->expr())
        ){
            Node res("statement", {expr.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        std::optional<Node> if_statement;
        if (true
            && (if_statement = this->if_statement())
        ){
            Node res("statement", {if_statement.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

    std::optional<Node> expr() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "expr";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        } else {
            auto [lastres, lastpos] = memo[key] = std::pair<std::optional<Node>, int>{std::nullopt, pos};
        }
        std::optional<Node> expr;
        std::optional<Node> term;
        if (true
            && (expr = this->expr())
            && expect("+")
            && (term = this->term())
        ){
            Node res("expr", {expr.value(), term.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        if (true
            && (expr = this->expr())
            && expect("-")
            && (term = this->term())
        ){
            Node res("expr", {expr.value(), term.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        if (true
            && (term = this->term())
        ){
            Node res("expr", {term.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

    std::optional<Node> term() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "term";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        } else {
            auto [lastres, lastpos] = memo[key] = std::pair<std::optional<Node>, int>{std::nullopt, pos};
        }
        std::optional<Node> term;
        std::optional<Node> atom;
        if (true
            && (term = this->term())
            && expect("*")
            && (atom = this->atom())
        ){
            Node res("term", {term.value(), atom.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        if (true
            && (term = this->term())
            && expect("/")
            && (atom = this->atom())
        ){
            Node res("term", {term.value(), atom.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        if (true
            && (atom = this->atom())
        ){
            Node res("term", {atom.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

    std::optional<Node> atom() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "atom";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        }
        std::optional<Token> name;
        if (true
            && (name = expect(TokenType::NAME))
        ){
            Node res("atom", {name.value().value});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        std::optional<Token> number;
        if (true
            && (number = expect(TokenType::NUMBER))
        ){
            Node res("atom", {number.value().value});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        std::optional<Node> expr;
        if (true
            && expect("(")
            && (expr = this->expr())
            && expect(")")
        ){
            Node res("atom", {expr.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

    std::optional<Node> assignment() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "assignment";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        }
        std::optional<Node> target;
        std::optional<Node> expr;
        if (true
            && (target = this->target())
            && expect("=")
            && (expr = this->expr())
        ){
            Node res("assignment", {target.value(), expr.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

    std::optional<Node> target() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "target";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        }
        std::optional<Token> name;
        if (true
            && (name = expect(TokenType::NAME))
        ){
            Node res("target", {name.value().value});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

    std::optional<Node> if_statement() {
        int pos = mark();
        std::map<std::string, std::pair<std::optional<Node>, int>> memo;
        if (!memos.contains(pos)) { memo = memos[pos] = {};} else { memo = memos[pos]; }
        std::string key = "if_statement";
        if (memo.contains(key)) {
             auto [res, endpos] = memo[key];
             reset(endpos);
             return res;
        }
        std::optional<Node> expr;
        std::optional<Node> statement;
        if (true
            && expect("if")
            && (expr = this->expr())
            && expect(":")
            && (statement = this->statement())
        ){
            Node res("if_statement", {expr.value(), statement.value()});
            int endpos = mark();
            memo[key] = {res, endpos};
            return res;
        }
        reset(pos);
        return {};
    }

};



int main(int argc, char**argv) {
  std::fstream fin{argv[1], std::fstream::in};
  std::string content((std::istreambuf_iterator<char>(fin)),
                      (std::istreambuf_iterator<char>()));
  fin.close();
    
  Tokenizer t{content};

  Toyparser p{t};

  auto nodes = p.statement();
  if (nodes) {
    std::cout << nodes.value() << "\n";
  } else {
    std::cout << "Could not parse content.\n";
  }
}
    
