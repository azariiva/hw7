#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

// int main() {
//     lex::Lexer lexer;
//     std::ifstream in("in.txt", std::ios::in);
//     lex::Token *token;

//     while ((token = lexer.get_token(in))) {
//         std::cout << std::left << std::setw(20) << token->Token::text_repr() << " [" << token->text_repr() << "] "
//                     << "at (" << token->row() << "; " << token->col() << ")\n";
//         delete token;
//     }
//     in.close();
//     return 0;
// }

int main() {
    std::ifstream in("in2.txt", std::ios::in);
    parse::Parser parser(in);
    try {
        parser.parse();
    } catch (parse::ParserError &pe) {
        pe.show();
    }
    in.close();
    return 0;
}