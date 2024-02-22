#include <concepts>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

template <typename T>
using Parsed_t = std::optional<std::pair<T, std::string_view>>;

template <class P>
concept Parser =
    std::regular_invocable<P, std::string_view> &&
    requires(std::invoke_result_t<P, std::string_view> result) {
      std::same_as<decltype(result),
                   Parsed_t<typename decltype(result)::value_type::first_type>>;
    };

template <Parser P>
using Parser_result_t = std::invoke_result_t<P, std::string_view>;

template <Parser P>
using Parser_value_t = typename Parser_result_t<P>::value_type::first_type;

template <typename F, typename... Args>
concept Parser_combinator = std::regular_invocable<F, Args...> &&
                            Parser<std::invoke_result_t<F, Args...>>;

template <typename F, typename... Args>
  requires Parser_combinator<F, Args...>
using Parser_combinator_value_t = std::invoke_result_t<F, Args...>;

inline constexpr auto item = [](std::string_view input) -> Parsed_t<char> {
  if (input.empty()) {
    return {};
  } else {
    return {{input[0], input.substr(1)}};
  }
};

template <typename T>
inline constexpr auto empty =
    [](std::string_view) -> Parsed_t<T> { return {}; };

constexpr Parser auto str(std::string_view match) {
  return [match](std::string_view input) -> Parsed_t<std::string> {
    if (input.starts_with(match)) {
      return {
          {std::string{match}, {input.begin() + match.size(), input.end()}}};
    } else {
      return {};
    }
  };
}

template <typename T> constexpr Parser auto unit(T const &thing) {
  return [thing](std::string_view input) -> Parsed_t<T> {
    return {{thing, input}};
  };
}

template <Parser P, Parser Q>
  requires std::convertible_to<Parser_value_t<P>, Parser_value_t<Q>>
constexpr Parser auto operator|(P p, Q q) {
  return [=](std::string_view input) -> Parser_result_t<Q> {
    if (auto const &result = std::invoke(p, input)) {
      return result;
    } else {
      return std::invoke(q, input);
    }
  };
}

constexpr Parser auto choice(Parser auto parser, Parser auto... parsers) {
  if constexpr (std::is_pointer_v<decltype(parser)>) {
    return ([parser](auto input) { return std::invoke(parser, input); } | ... |
            parsers);
  } else {
    return (parser | ... | parsers);
  }
}

template <Parser P, Parser_combinator<Parser_value_t<P>> F>
constexpr Parser auto operator&(P parser, F func) {
  using Parser_t = Parser_combinator_value_t<F, Parser_value_t<P>>;
  return [=](std::string_view input) -> Parser_result_t<Parser_t> {
    if (auto const &result = std::invoke(parser, input)) {
      return std::invoke(std::invoke(func, result->first), result->second);
    } else {
      return {};
    }
  };
}

constexpr Parser auto chain(Parser auto parser, auto... funcs) {
  if constexpr (std::is_pointer_v<decltype(parser)>) {
    return ([parser](auto input) { return std::invoke(parser, input); } & ... &
            funcs);
  } else {
    return (parser & ... & funcs);
  }
}

constexpr Parser auto skip(Parser auto p, Parser auto q) {
  return [=](std::string_view input) {
    if (auto const &result = std::invoke(p, input)) {
      return std::invoke(q, result->second);
    } else {
      return std::invoke(p, input);
    }
  };
}

template <typename Pr, Parser P = decltype(item)>
  requires std::predicate<Pr, Parser_value_t<P>>
constexpr Parser auto satisfy(Pr pred, P parser = item) {
  return chain(parser, [pred](auto const &th) -> Parser auto {
    return [pred, th](std::string_view input) -> Parsed_t<Parser_value_t<P>> {
      if (std::invoke(pred, th))
        return {{th, input}};
      else
        return {};
    };
  });
}

Parser auto digit = satisfy(::isdigit);
Parser auto lower = satisfy(::islower);
Parser auto upper = satisfy(::isupper);
Parser auto letter = choice(lower, upper);
Parser auto alphanum = choice(letter, digit);

constexpr Parser auto symbol(char x) {
  return satisfy([x](char y) { return x == y; });
}

template <typename T, Parser P, std::regular_invocable<T, Parser_value_t<P>> F>
  requires std::convertible_to<std::invoke_result_t<F, T, Parser_value_t<P>>, T>
class reduce_many {
  T init;
  P parser;
  F func;

public:
  reduce_many(T const &thing, P const &p, F const &fn)
      : init{thing}, parser{p}, func{fn} {}

  constexpr auto operator()(std::string_view input) const -> Parsed_t<T> {
    return choice(
        chain(
            parser,
            [this](auto const &thing) {
              return reduce_many{std::invoke(func, init, thing), parser, func};
            }),
        unit(init))(input);
  }
};

template <Parser P>
  requires std::same_as<Parser_value_t<P>, char>
constexpr Parser auto many(P parser) {
  return reduce_many(std::string{}, parser,
                     [](std::string const &str, char ch) { return str + ch; });
}

template <Parser P>
  requires std::same_as<Parser_value_t<P>, std::string>
constexpr Parser auto many(P parser) {
  return reduce_many(
      std::string{}, parser,
      [](std::string const &str, std::string s) { return str + s; });
}

Parser auto whitespace = many(satisfy(::isspace));

constexpr Parser auto token(Parser auto parser) {
  return chain(skip(whitespace, parser),
               [](auto const &thing) { return skip(whitespace, unit(thing)); });
}

template <Parser P>
  requires std::same_as<Parser_value_t<P>, char>
constexpr Parser auto some(P parser) {
  return chain(parser, [=](char ch) {
    return chain(many(parser), [=](std::string const &str) {
      return unit(std::string(1, ch) + str);
    });
  });
}

Parser auto natural = chain(some(digit), [](std::string const &digits) {
  return unit(std::stoi(digits));
});

Parser auto integer =
    choice(natural, chain(
                        symbol('-'), [](auto) { return natural; },
                        [](int nat) { return unit(-nat); }));

inline constexpr decltype(auto) papply =
    []<typename F, typename... Args>(F &&f, Args &&...args) {
      if constexpr (std::invocable<F, Args...>) {
        return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
      } else { // Assuming too few arguments to invoke f
        return std::bind_front(std::forward<F>(f), std::forward<Args>(args)...);
      }
    };

template <Parser P, Parser Q> constexpr Parser auto operator^(P p, Q q) {
  using Result_t = std::invoke_result_t<decltype(papply), Parser_value_t<P>,
                                        Parser_value_t<Q>>;
  return [=](std::string_view input) -> Parsed_t<Result_t> {
    if (auto const &pr = std::invoke(p, input)) {
      if (auto const &qr = std::invoke(q, pr->second)) {
        return {{papply(pr->first, qr->first), qr->second}};
      } else {
        return {};
      }
    } else {
      return {};
    }
  };
}

template <typename F, Parser... Ps>
  requires std::regular_invocable<F, Parser_value_t<Ps>...>
constexpr Parser auto sequence(F func, Ps... parsers) {
  return (unit(func) ^ ... ^ parsers);
}

template <Parser P> constexpr Parser auto operator!(P p) {
  return [p](std::string_view input) {

  };
}

struct Node {
  const std::string node_type;
  const std::optional<std::string> text;
  std::vector<Node> children;

  const Node &add_child(Node child) {
    children.push_back(child);
    return *this;
  }
};

template <Parser P> Parser auto to_node_parser(P p) {
  return [p](std::string_view input) -> Parsed_t<Node> {
    auto parsed_content = p(input);
    if (!parsed_content) {
      return {};
    } else {
      return {{Node("", parsed_content->first), parsed_content->second}};
    }
  };
}

template <Parser P>
  requires std::same_as<Parser_value_t<P>, Node>
constexpr Parser auto many(P parser) {
  return reduce_many(
      Node("many", std::nullopt), parser,
      [](Node parent, Node child) { return parent.add_child(child); });
}

std::ostream &operator<<(std::ostream &os, const Node &n) {
  auto &ret = os << n.node_type << "_Node("
                 << n.text.transform([](auto t) { return "\"" + t + "\""; })
                        .value_or("None");
  if (!n.children.empty()) {
    for (const auto &child : n.children) {
      ret << ", " << child;
    }
  }
  return ret << ")";
}

// inline constexpr auto
// Grammar = sequence(
//     [](){ return Node<bool>("Grammar", std::nullopt, {}); },
//     skip(Spacing),
//     some(Definition),
//     skip(EndOfFile),
// )
//

inline constexpr Parser auto EndOfLine =
    sequence([](std::string s) { return Node("EndOfLine", s); },
             choice(str("\r\n"), str("\n"), str("\r")));

inline constexpr Parser auto Space =
    choice(sequence([](std::string s) { return Node("Space", s); },
                    choice(str(" "), str("\t"))),
           EndOfLine);

// inline Parser decltype(auto) Spacing = many(Space);
auto Spacing(std::string_view input) -> Parsed_t<Node> {
  return many(Space)(input);
}

// inline constexpr Parser auto DOT =
//     [](std::string_view input) -> Parsed_t<Node> {
//     std::string consumed_input{};
//   sequence([&consumed_input](Node n) { consumed_input += n.text.value_or(""); return Node("DOT", consumed_input); }, to_node_parser(str(".")),
//            Spacing);
// };
// inline Parser auto CLOSE = chain(str("."), Spacing);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::fstream fin{argv[1], std::fstream::in};
  const std::string content((std::istreambuf_iterator<char>(fin)),
                            (std::istreambuf_iterator<char>()));
  fin.close();

  std::cout << content << "\n";
  // auto test =  (empty<char> | item | unit('x'))("abc123");
  auto test = chain(to_node_parser(str(".")), to_node_parser(str(".")))("...");

  std::cout << test.value().first << "\n";
}
