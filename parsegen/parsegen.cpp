namespace std {
class type_info;
}
#include <cctype>
#include <concepts>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

#include <exception>
#include <stdexcept>
#include <typeinfo>

#include <parsegen/peg_parser.hpp>

typedef std::optional<peg::Node> OptNode;
// template<typename T>
// concept Parser = std::is_invocable<>;

// OptNode parse_Definition(const char c, peg::Scanner &scanner) {}
//
// std::optional<peg::Node> parse_Nothing(const char c, peg::Scanner &scanner) {
//   return std::nullopt;
// }
//
// OptNode parse_EndOfLine(peg::Scanner &scanner) {
//   if (scanner.peek(0) == '\r') {
//     if (scanner.peek(1) == '\n') {
//       scanner.advance();
//     }
//     return peg::Node("EndOfLine");
//   } else if (scanner.peek(0) == '\n') {
//     return peg::Node("EndOfLine");
//   } else {
//     return std::nullopt;
//   }
// }
//
//

typedef const std::string ParseString;

template <typename A> struct Parser {
  struct ParsePair {
    const A value;
    std::shared_ptr<ParseString> string;

    ParsePair(A value, ParseString string)
        : value(value), string(std::make_shared<ParseString>(string)){};
    ParsePair(A value, std::shared_ptr<ParseString> shared_string)
        : value(value), string(shared_string){};
  };
  static ParsePair make_pair(const A value, ParseString string) {
    return ParsePair(value, string);
  }
  static ParsePair make_pair(const A value,
                             std::shared_ptr<ParseString> string) {
    return ParsePair(value, string);
  }

  typedef const std::optional<ParsePair> ParseResult;
  typedef const std::function<ParseResult(ParseString)> ParseFunction;

  ParseFunction parse_function;
  Parser(ParseFunction p) : parse_function(p){};
  ParseResult parse(ParseString s) const { return parse_function(s); };

  template <class B> Parser<B> fmap(const std::function<B(A)> fn) {
    return Parser<B>([=, this](ParseString s) -> Parser<B>::ParseResult {
      auto out = parse(s);
      if (out == std::nullopt) {
        return std::nullopt;
      }

      auto &[value, rest_string] = out.value();
      auto transformed_value = fn(value);

      return Parser<B>::make_pair(transformed_value, rest_string);
    });
  }

  template <class B> Parser<B> bind(std::function<Parser<B>(A)> fn_a_b) {
    return Parser<B>([=, this](ParseString s) -> Parser<B>::ParseResult {
      auto out = parse(s);

      if (out == std::nullopt) {
        return std::nullopt;
      }

      auto &[value, rest_string] = out.value();

      return fn_a_b(value).parse(*rest_string);
    });
  }

  const Parser<A> operator|(const Parser<A> &p_a) const {
    return Parser<A>([=, this](ParseString s) -> Parser<A>::ParseResult {
      std::cout << "Parse string: " << s << "\n";
      std::cout << "Or started\n";
      std::cout << "Is parser somehow empty? "
                << (bool(this->parse_function) ? "no.\n" : "yes!\n");
      auto out = parse(s);

      std::cout << "Parsed first option\n";
      if (out != std::nullopt) {
        return out;
      }

      auto out2 = p_a.parse(s);

      if (out2 != std::nullopt) {
        return out2;
      }

      return std::nullopt;
    });
  }
};

template <typename A> Parser<A> pure(A val) {
  return Parser<A>([=](ParseString s) -> Parser<A>::ParseResult {
    return {val, s};
  });
}

template <class A> Parser<A> empty() { return std::nullopt; }

Parser<char> item() {
  return Parser<char>([=](ParseString s) -> Parser<char>::ParseResult {
    if (s.empty()) {
      return std::nullopt;
    } else {
      return Parser<char>::ParsePair{s[0], s.substr(1)};
    }
  });
}

Parser<char> satisfies(std::function<bool(char)> predicate) {
  return Parser<char>([=](ParseString s) -> Parser<char>::ParseResult {
    return item()
        .bind<char>([=](char c) -> Parser<char> {
          return Parser<char>([=](ParseString s) -> Parser<char>::ParseResult {
            if (predicate(c)) {
              return Parser<char>::ParsePair{c, s};
            } else {
              return std::nullopt;
            }
          });
        })
        .parse(s);
  });
}

Parser<char> character(const char c) {
  return satisfies([=](const char d) { return d == c; });
}

Parser<char> digit() {
  return satisfies([=](const char c) { return c >= '0' && c <= '9'; });
}

Parser<char> lower() {
  return satisfies([=](const char c) {
    std::cout << "lower\n";
    return c >= 'a' && c <= 'z';
  });
}

const Parser<char> upper() {
  return satisfies([=](const char c) {
    std::cout << "upper\n";
    return c >= 'A' && c <= 'Z';
  });
}

Parser<char> letter() {
  std::cout << "letter\n";
  return upper() | lower();
}

// Parser<char> letter() {
//     return
// }
// Parser<char> item = [](const std::string_view &input) {
//   return Parser<char>([&](ParseString))
//   if (input.empty()) {
//     return std::nullopt;
//   } else {
//     return std::tuple{input[0], input.substr(1)};
//   }
// };

// auto sat = [](std::function<bool(char)> predicate)
//     -> std::function<std::optional<std::tuple<const char, ParseString>>(
//         const char)> {
//   return
//       [&](const char x) -> std::optional<std::tuple<const char, ParseString>>
//       {
//         if (predicate(x)) {
//           return std::tuple{x, ParseString{""}};
//         } else {
//           return std::nullopt;
//         }
//       };
// };

// auto character = [](const char c)
//     -> std::function<std::optional<std::tuple<const char, ParseString>>(
//         ParseString)> { return sat([&](const char d) { return c == d; }); };

// OptNode parse_Space(const char c, peg::Scanner &scanner) {
//     if (c == ' ' || c == '\t' || parse_EndOfLine(c, scanner) != std::nullopt)
//     {
//         return peg::Node("Space");
//     } else {
//         return std::nullopt;
//     }
//
// }
//
// OptNode parse_Spacing(const char c, peg::Scanner &scanner) {
//     while (parse_Space(c, scanner) != std::nullopt) {
//         scanner.advance();
//     }
//     return peg::Node("Spacing");
// }
//
// std::optional<peg::Node> parse_IdentStart(const char c, peg::Scanner
// &scanner) {
//   if ((c >= 'a' && c <= 'z') || c >= 'A' && c <= 'Z' || c == '_') {
//     return OptNode{peg::Node("IdentStart")};
//   } else {
//     return std::nullopt;
//   }
// }
//
// OptNode parse_IdentCont(const char c, peg::Scanner &scanner) {
//   return parse_IdentStart(c, scanner).or_else([&]() {
//       return c >= 0 && c <= 9 ? OptNode{peg::Node("IdentCont")} :
//       std::nullopt;
//   });
// }
//
// OptNode parse_Identifier(const char c, peg::Scanner &scanner) {
//     if (parse_IdentStart(c, scanner) != std::nullopt) {
//         while (parse_IdentCont(scanner.peek(1), scanner) != std::nullopt) {
//             scanner.advance();
//         }
//         scanner.advance();
//         parse_Spacing(c, scanner);
//         std::cout << scanner.peek(1) << "\n";
//         return peg::Node("Identifier", scanner.scanned_string(), {});
//     }
//     else {
//         return std::nullopt;
//     }
// }
//
// OptNode parse_LEFTARROW(const char c, peg::Scanner &scanner) {
//     if (c == '<' && scanner.peek(1) == '-') {
//         scanner.advance();
//         return peg::Node("LEFTARROW");
//     } else {
//         return std::nullopt;
//     }
// }
char plus_one(char c) { return c + 1; }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::fstream fin{argv[1], std::fstream::in};
  const std::string content((std::istreambuf_iterator<char>(fin)),
                            (std::istreambuf_iterator<char>()));
  fin.close();

  // std::cout << content << "\n";
  // peg::Scanner scanner{content};

  // auto [node, rest] =
  // peg::itemParser::parse(std::string_view{content.begin(), content.end()});
  // auto [node, rest] =
  //     peg::itemParser(std::string_view{content.begin(),
  //     content.end()}).value();
  // auto tree = parse_Identifier(c, scanner);
  // auto tree_2 = parse_LEFTARROW(c, scanner);

  std::cout << content << "\n";
  // auto test = fmap<char, char>([](char c) -> char { return c; }, item())
  //                 .parse(content)
  //                 .value();
  std::cout << "Is upper() emtpy? "
            << (upper().parse_function ? "No.\n" : "Yes!\n");
  auto test = letter().parse(content).value();
  // std::cout << "start: " << test.value << "\n";
  // std::cout << "rest: \n" << *test.string << "\n";

  std::cout << content << "\n";

  // if (tree.has_value()) {
  //   std::cout << tree.value().value;
  // }
  // if (tree_2.has_value()) {
  //   std::cout << tree_2.value().value;
  // }
  // std::cout << "Node: " << node.value << ", rest: " << rest << "\n";
}
