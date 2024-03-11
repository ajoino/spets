#include <fstream>
#include <iostream>
#include <string>

#include <tokenizer/lexer.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }
  char ch;
  // Token token;
  auto token_list = std::vector<Token>{};
  int row = 0, column = 0;
  std::fstream fin{argv[1], std::fstream::in};
  std::string content((std::istreambuf_iterator<char>(fin)),
                      (std::istreambuf_iterator<char>()));
  fin.close();

  std::cout << content << "\n";
  Lexer lexer{content};

  std::cout << std::string{lexer.start, lexer.end} << "\n";
  int line = -1;

  for (;;) {
    auto scan_result = scan_token(lexer);
    auto [token, lexer] = scan_result;

    std::cout << token << "\n";

    if (token.type == TokenType::ENDOFFILE) {
      break;
    }
  }
  return 0;
}
