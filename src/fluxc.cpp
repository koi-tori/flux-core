#include "fcore.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifndef FLUXC_EXIT_SUCCESS
    #define FLUXC_EXIT_SUCCESS 0
#endif

#ifndef FLUXC_EXIT_FAILURE
    #define FLUXC_EXIT_FAILURE 1
#endif

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::printf("Usage: fluxc filename\n");
        return 1;
    }

    std::ifstream file(argv[1], std::ios::in | std::ios::binary);
    if (!file) {
        std::printf("No such file.\n");
        return 2;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    fluxc::TokenizedCode code(buffer.str());

    auto& tokens = code.get_all_tokens();
    
    for (auto& token : tokens) {
        std::cout << token.str_view();
        if (token.kind() != fluxc::TokenKind::NEWLINE) std::cout << '\t';
    }
    std::cout << '\n';

    return FLUXC_EXIT_SUCCESS;
}
