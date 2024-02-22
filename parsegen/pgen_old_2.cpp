#include <concepts>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "node.hpp"

using ParsedNode = std::optional<std::pair<Node, std::string_view>>;

template <class P>
concept Parser = std::regular_invocable<P, std::string_view> &&
                 requires(std::invoke_result_t<P, std::string_view> result) {
                   std::same_as<decltype(result), ParsedNode>;
                 };

template <typename F, typename... Args>
concept Parser_combinator = std::regular_invocable<F, Args...> &&
                            requires(std::invoke_result_t<F, Args...> result) {
                              Parser<std::invoke_result_t<F, Args...>>;
                            };

inline constexpr auto item(std::string_view input) -> ParsedNode {
  if (input.empty()) {
    return {};
  } else {
    return {{Node{"item", std::string{input[0]}}, input.substr(1)}};
  }
};

inline constexpr auto empty = [](std::string_view) -> ParsedNode { return {}; };

constexpr Parser auto str(std::string_view match) {
  return [match](std::string_view input) -> ParsedNode {
    if (input.starts_with(match)) {
      return {{Node{"str",
                    std::string{input.begin(), input.begin() + match.size()}},
               {input.begin() + match.size(), input.end()}}};
    } else {
      return {};
    }
  };
}

constexpr Parser auto unit(const Node &node) {
  return [node](std::string_view input) -> ParsedNode {
    return {{node, input}};
  };
}

template <Parser P, Parser Q> constexpr Parser auto operator|(P p, Q q) {
  return [=](std::string_view input) -> Node {
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

template <Parser P, Parser_combinator F>
constexpr Parser auto operator&(P parser, F func) {
  return [=](std::string_view input) -> ParsedNode {
    if (auto const &result = std::invoke(parser, input)) {
      return std::invoke(std::invoke(func, result->first.text), result->second);
    } else {
      return {};
    }
  };
}

constexpr Parser auto chain(Parser auto parser,
                            Parser_combinator auto... funcs) {
  if constexpr (std::is_pointer_v<decltype(parser)>) {
    return ([parser](std::string_view input) {
      return std::invoke(parser, input);
    } & ... &
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

constexpr Parser auto symbol(char x) {
  return [x](std::string_view input) -> ParsedNode {
    if (input.starts_with(x)) {
      return {{Node{"symbol", std::string{input[0]}}, input.substr(1)}};
    } else {
      return {};
    }
  };
}

template <Parser P, std::regular_invocable<Node, Node> F>
  requires std::convertible_to<std::invoke_result_t<F, Node, Node>, Node>
class reduce_many {
  Node init;
  P parser;
  F func;

public:
  reduce_many(Node const &n, P const &p, F const &fn)
      : init{n}, parser{p}, func{fn} {}

  constexpr auto operator()(std::string_view input) const -> ParsedNode {
    return choice(
        chain(
            parser,
            [this](const Node &thing) {
              return reduce_many{std::invoke(func, init, thing), parser, func};
            }),
        unit(init))(input);
  }
};

template <Parser P>
constexpr Parser auto many(P parser) {
  return reduce_many(Node("many", ""), parser, [](Node &parent, Node const& child) {
    return parent.add_child(child);
  });
}

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
  // auto test = chain(to_node_parser(str(".")),
  // to_node_parser(str(".")))("...");
  auto test = many(symbol('.'))(".");

  std::cout << test.value().first << "\n";
}
