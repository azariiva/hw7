#include "Lexer/Lexer.hpp"
#include <iostream>
#include <fstream>

int main() {
    lex::Lexer lexer;
    std::ifstream in("in.txt", std::ios::in);
    lexer.tokenize(in);
    in.close();
    lexer.print_tokens();
    return 0;
}