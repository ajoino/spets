#include <fstream>
#include <span>
#include <ranges>
#include <print>

#include <tokenizer/lexer.hpp>

int main(int argc, char* argv[]) {
    auto args = std::span(argv, size_t(argc));

    std::fstream fin{args[1], std::fstream::in};
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    fin.close();

    Lexer lexer{content};

    std::vector<std::string> token_values;
    std::vector<std::string> padded_numbers;

    for (int i = 0;; i++) {
        auto scan_result = scan_token(lexer);
        auto [token, lexer] = scan_result;
        
        if (token.type == TokenType::WHITESPACE) {
            continue;
        }
        padded_numbers.push_back(std::to_string(i));
        token_values.push_back(token.value);
        if (token.type == TokenType::ENDOFFILE) {
            break;
        }
    }

    std::vector<std::string> line_tokens;
    size_t line_length = 0;
    for (auto [index, token] : std::views::zip(padded_numbers, token_values)) {
        size_t index_len = index.size();
        size_t token_len = token.size();


        if (index_len > token_len && token != "\\n") {
            size_t diff = index_len - token_len;
            for (size_t i = 0; i < diff; i++) {
                token.append(" ");
            }
        } else if (token_len > index_len) {
            size_t diff = token_len - index_len;
            for (size_t i = 0; i < diff; i++) {
                index.append(" ");
            }
        }
        line_length += index.size() + 1;
        if (token != "\\n" && line_length + index.size() + 1 <= 80) {
            std::print("{} ", index);
            line_tokens.push_back(token);
            continue;
        }
        std::print("\n");
        for (const auto& token : line_tokens) {
            std::print("{} ", token);
        }
        std::print("\n\n");
        line_tokens.clear();
        line_length = 0;
    }
}
