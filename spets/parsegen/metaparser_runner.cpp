#include <fstream>
#include <iostream>
#include <span>

#include <parsegen/metaparser.hpp>
#include <tokenizer/lexer.hpp>

int main(int argc, char** argv) {
    auto args = std::span(argv, size_t(argc));
    std::cout << R"(HEHEHEHEHEHELLOLOLOLOLOLOLOLOLOLOLOL???!?!?!?!?!?!!!!!??!!?)";
    std::fstream fin{args[1], std::fstream::in};
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    fin.close();

    std::cout << "File name: " << args[1] << "\n";
    std::cout << "Grammar file:\n" << content << "\n";
    Tokenizer t{content};

    MetaParser p{t};

    auto rules = p.start();
    if (rules) {
        std::cout << rules.value() << "\n";
        return 0;
    }
    std::cout << "Could not generated parser.\n";
    return 1;
}
