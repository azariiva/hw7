#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include "Token.hpp"

#include <vector>
#include <fstream>

namespace lex {
    class Lexer {
    public:
        Lexer();
        ~Lexer();
        void print_tokens();
        void tokenize(const std::string &fname);
        void tokenize(std::ifstream &in);
        void clear();

    private:
        void state_chooser();
        void state_s();
        void state_id();
        void state_number();
        void state_symbol();
        template <Keyword::t_type type>
        void state_keyword();

    private:
        typedef enum {
            S,
            ID,
            NUMBER,
            INT, LONG, CONST,
            SYMBOL
        } t_state;

        std::string m_fname;
        std::vector<Token *> m_tokens;
        t_state m_state;
        unsigned m_state_idx;
        size_t m_row;
        size_t m_col;
        size_t m_first_col;
        std::string m_str_lex;
        char m_c;
    };
}

#endif