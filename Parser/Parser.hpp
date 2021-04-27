#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "Lexer.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace parse {
    using lex::Lexer;
    using lex::Token;
    using lex::Identifier;
    using lex::Keyword;
    using lex::Number;
    using lex::Symbol;

    class ParserError;

    class Parser {
    public:
        Parser(std::ifstream &in);
        ~Parser();
        void parse();
        void clear();
    
    private:
        void state_s();
        void state_ctype();
        void state_type(); void state_type1();
        void state_const();
        void state_vars();
        void state_var();
        void state_v();
        void state_f();

        void state_mas(); 
        void state_mas1(); 
        void state_mas2(); 
        void state_mas3(Symbol::t_type type);

        void state_args();

        void save_token(std::string meta="");
        void extract_token();

    private:
        Lexer m_lexer;
        std::ifstream &m_in;
        Token *m_last_token;
        friend ParserError;
    };

    class ParserError {
    public:
        ParserError(Parser &parser) {
            parser.extract_token();
            m_last_token = parser.m_last_token;
            parser.m_last_token = NULL;
        }
        ~ParserError() {
            delete m_last_token;
        }
        void show() {
            std::cout << "ParserError: " << m_last_token->Token::text_repr() << " [" << m_last_token->text_repr() << "] "
                     << "at (" << m_last_token->row() << "; " << m_last_token->col() << ")\n";
        }
    private:
        Token *m_last_token;
    };
}

#endif