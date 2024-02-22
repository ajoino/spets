#ifndef __PEG_PARSER_H__
#define __PEG_PARSER_H__

#include <string>
#include <string_view>
#include <vector>
#include <tuple>
#include <optional>
#include <functional>

namespace peg {

class Scanner {
  std::string_view::iterator start;
  const std::string_view::iterator end;
  std::string_view::iterator current;
  int line;

public:
  Scanner(std::string_view buffer)
      : start(buffer.begin()), end(buffer.end()), current(buffer.begin()),
        line(1){};

  void reset_scan();
  const bool is_at_end() const;
  const char advance();
  const char peek(const int lookahead) const;

  const std::string scanned_string() const;
};

struct Node {
    std::string kind;
    std::string value;
    std::vector<Node> children;

    Node(std::string kind) : kind(kind), value(), children() {};
    Node(std::string kind, std::string value, std::vector<Node> children) : kind(kind), value(value), children(children) {};
};

// auto literal(const std::string_view &pattern) -> std::function<std::optional<Node(std::string_view::iterator)>> {
//     return [](std::string_view::iterator cur_pos) {
//         
//     };
// }
} // namespace peg
#endif // !__PEG_PARSER_H__
