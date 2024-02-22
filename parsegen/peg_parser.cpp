#include <string>

#include <parsegen/peg_parser.hpp>

namespace peg {
const bool Scanner::is_at_end() const { return current == end; }
const char Scanner::advance() {
  ++current;
  return *(current - 1);
}
const char Scanner::peek(const int lookahead) const { return *(current + lookahead - 1); }
void Scanner::reset_scan() { start = current; }

const std::string Scanner::scanned_string() const {
    return std::string{start, current};
}
} // namespace peg
