#include <fstream>
#include <iostream>
#include <span>

#ifndef METAPARSER_CANDIDATE
#include <parsegen/metaparser.hpp>
#else
#include <parsegen/metaparser_candidate.hpp>
#endif
#include <tokenizer/lexer.hpp>

int main(int argc, char** argv) {
    auto args = std::span(argv, size_t(argc));
    std::fstream fin{args[1], std::fstream::in};
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    fin.close();

    std::cout << "File name: " << args[1] << "\n";
    std::cout << "Grammar file:\n" << content << "\n";
    Tokenizer t{content};

    MetaParser p{t};

    auto grammar = p.start();
    if (grammar) {
        std::cout << grammar.value().rules << "\n";
        return 0;
    }
    std::cout << "Could not generated parser.\n";
    return 1;
}
