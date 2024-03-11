#include <optional>
#include <fstream>
#include <optional>

#include <parser/node.hpp>
#include <tokenizer/lexer.hpp>
#include <parser/parser.hpp>
    
class Toyparser : public Parser {
public:
    std::optional<Node> start() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> rules_0;
            std::optional<Token> endoffile_0;
            if (true
                && (rules_0 = this->rules())
                && (endoffile_0 = expect(TokenType::ENDOFFILE))
            ){
                return Node("start", {rules_0.value()}) ;
            }
            reset(pos);
            std::cout << "No parse found for start\n";
            return {};
        };

        std::cout << "Parsing start\n";
        return memoize("start", inner_func, mark());
    }

    std::optional<Node> rules() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> rule_0;
            std::optional<Node> rules_0;
            if (true
                && (rule_0 = this->rule())
                && (rules_0 = this->rules())
            ){
                return rules_0.value().prepend_child(rule_0.value()) ;
            }
            reset(pos);
            if (true
                && (rule_0 = this->rule())
            ){
                return Node("rules", {rule_0.value()}) ;
            }
            reset(pos);
            std::cout << "No parse found for rules\n";
            return {};
        };

        std::cout << "Parsing rules\n";
        return memoize("rules", inner_func, mark());
    }

    std::optional<Node> rule() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> name_0;
            std::optional<Node> ws_0;
            std::optional<Token> token_0;
            std::optional<Node> alts_0;
            std::optional<Token> newline_0;
            if (true
                && (name_0 = expect(TokenType::NAME))
                && (ws_0 = this->ws())
                && (token_0 = expect("<-"))
                && (ws_0 = this->ws())
                && (alts_0 = this->alts())
                && (newline_0 = expect(TokenType::NEWLINE))
            ){
                return Node("rule", { alts_0.value() });
            }
            reset(pos);
            std::cout << "No parse found for rule\n";
            return {};
        };

        std::cout << "Parsing rule\n";
        return memoize("rule", inner_func, mark());
    }

    std::optional<Node> alts() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> alt_0;
            std::optional<Node> ws_0;
            std::optional<Token> token_0;
            std::optional<Node> alts_0;
            if (true
                && (alt_0 = this->alt())
                && (ws_0 = this->ws())
                && (token_0 = expect("/"))
                && (ws_0 = this->ws())
                && (alts_0 = this->alts())
            ){
                return alts_0.value().prepend_child(alt_0.value()) ;
            }
            reset(pos);
            if (true
                && (alt_0 = this->alt())
            ){
                return Node("alts", { alt_0.value() });
            }
            reset(pos);
            std::cout << "No parse found for alts\n";
            return {};
        };

        std::cout << "Parsing alts\n";
        return memoize("alts", inner_func, mark());
    }

    std::optional<Node> alt() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> items_0;
            if (true
                && (items_0 = this->items())
            ){
                return Node("alt", {items_0.value()}) ;
            }
            reset(pos);
            std::cout << "No parse found for alt\n";
            return {};
        };

        std::cout << "Parsing alt\n";
        return memoize("alt", inner_func, mark());
    }

    std::optional<Node> items() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Node> item_0;
            std::optional<Node> items_0;
            if (true
                && (item_0 = this->item())
                && (items_0 = this->items())
            ){
                return items_0.value().prepend_child(item_0.value()) ;
            }
            reset(pos);
            if (true
                && (item_0 = this->item())
            ){
                return Node("items", {item_0.value()}) ;
            }
            reset(pos);
            std::cout << "No parse found for items\n";
            return {};
        };

        std::cout << "Parsing items\n";
        return memoize("items", inner_func, mark());
    }

    std::optional<Node> item() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> name_0;
            if (true
                && (name_0 = expect(TokenType::NAME))
            ){
                return Node("item", name_0.value().value) ;
            }
            reset(pos);
            std::optional<Token> string_0;
            if (true
                && (string_0 = expect(TokenType::STRING))
            ){
                return Node("item", name_0.value().value) ;
            }
            reset(pos);
            std::cout << "No parse found for item\n";
            return {};
        };

        std::cout << "Parsing item\n";
        return memoize("item", inner_func, mark());
    }

    std::optional<Node> ws() {

        // inner_func does the actual parsing but is called later by
        // to enable memoization
        auto inner_func = [&, this]() -> std::optional<Node> {
            int pos = mark();
            std::optional<Token> whitespace_0;
            if (true
                && (whitespace_0 = expect(TokenType::WHITESPACE))
            ){
                return Node{"ws", {whitespace_0.value().value}};
            }
            reset(pos);
            std::cout << "No parse found for ws\n";
            return {};
        };

        std::cout << "Parsing ws\n";
        return memoize("ws", inner_func, mark());
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
    