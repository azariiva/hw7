#include "Parser.hpp"

#include <stdexcept>
#include <iomanip>
#include <iostream>

using namespace parse;

Parser::Parser(std::ifstream &in) : m_lexer(), m_in(in), m_last_token(NULL) {}

Parser::~Parser() {
    if (m_last_token)
        delete m_last_token;
}

void Parser::parse() {
    state_s();
}

void Parser::save_token(std::string meta) {
    std::cout << std::left << std::setw(20) << meta
                    << std::left << std::setw(20) << m_last_token->Token::text_repr() << " [" << m_last_token->text_repr() << "] "
                    << "at (" << m_last_token->row() << "; " << m_last_token->col() << ")\n";
    delete m_last_token;
    m_last_token = NULL;
}

void Parser::extract_token() {
    if (m_last_token == NULL)
        m_last_token = m_lexer.get_token(m_in);
}

void Parser::state_s() {
    state_ctype();
    state_vars();
    extract_token();
    if (m_last_token->type() == Token::SYMBOL) {
        Symbol *symbol = (Symbol *)m_last_token;
        if (symbol->type() == Symbol::SEMICOLON) {
            save_token("state_s");
            return;
        }
    }
    throw ParserError(*this);
}

void Parser::state_ctype() {
    state_const();
    state_type();
    state_const();
}

void Parser::state_type() {
    extract_token();
    if (m_last_token->type() == Token::KEYWORD) {
        Keyword *keyword = (Keyword *)m_last_token;
        switch (keyword->type()) {
        case Keyword::INT:
            save_token("state_type");
            return;
        case Keyword::LONG:
            save_token("state_type");
            state_type1();
            return;
        default:
            break;
        }
    }
    throw ParserError(*this);
}

void Parser::state_type1() {
    extract_token();
    if (m_last_token->type() == Token::KEYWORD) {
        Keyword *keyword = (Keyword *)m_last_token;
        if (keyword->type() == Keyword::INT) {
            save_token("state_type1");
        }
    }
}

void Parser::state_const() {
    extract_token();
    if (m_last_token->type() == Token::KEYWORD) {
        Keyword *keyword = (Keyword *)m_last_token;
        if (keyword->type() == Keyword::CONST) {
            save_token("state_const");
            state_const();
        }
    }
}

void Parser::state_vars() {
    state_var();
    extract_token();
    if (m_last_token->type() == Token::SYMBOL) {
        Symbol *symbol = (Symbol *)m_last_token;
        if (symbol->type() == Symbol::COMMA) {
            save_token("state_vars");
            state_vars();
        }
    }
}

void Parser::state_var() {
    extract_token();
    if (m_last_token->type() == Token::SYMBOL && \
        ((Symbol *)m_last_token)->type() == Symbol::STAR
    ) {
        save_token("state_var");
        state_const();
        state_var();
    } else {
        state_v();
    }
}

void Parser::state_v() {
    state_f();
    state_mas();
}

void Parser::state_f() {
    extract_token();
    if (m_last_token->type() == Token::ID) {
        save_token("state_f");
        return;
    } else if (m_last_token->type() == Token::SYMBOL) {
        Symbol *symbol = (Symbol *)m_last_token;
        if (symbol->type() == Symbol::OPENBR_1) {
            save_token("state_f");
            state_var();
            state_mas3(Symbol::CLOSEBR_1);
            return;
        }
    }
    throw ParserError(*this);
}

void Parser::state_mas() {
    extract_token();
    if (m_last_token->type() == Token::SYMBOL) {
        Symbol *symbol = (Symbol *)m_last_token;
        switch (symbol->type()) {
        case Symbol::OPENBR_2:
            save_token("state_mas");
            state_mas1();
            state_mas();
            return;
        case Symbol::OPENBR_1:
            save_token("state_mas");
            state_mas2();
            state_mas();
            return;
        default:
            break;
        }
    }
}

void Parser::state_mas1() {
    extract_token();
    if (m_last_token->type() == Token::NUMBER) {
        save_token("state_mas1");
        state_mas3(Symbol::CLOSEBR_2);
        return;
    }
    throw ParserError(*this);
}

void Parser::state_mas2() {
    extract_token();
    if (m_last_token->type() != Token::SYMBOL)
        state_args();
    state_mas3(Symbol::CLOSEBR_1);
}

void Parser::state_mas3(Symbol::t_type type) {
    extract_token();
    if (m_last_token->type() == Token::SYMBOL) {
        Symbol *symbol = (Symbol *)m_last_token;
        if (symbol->type() == type) {
            save_token("state_mas3");
            return;
        }
    }
    throw ParserError(*this);
}

void Parser::state_args() {
    state_ctype();
    state_var();
    extract_token();
    if (m_last_token->type() == Token::SYMBOL) {
        Symbol *symbol = (Symbol *)m_last_token;
        if (symbol->type() == Symbol::COMMA) {
            save_token("state_args");
            state_args();
        }
    }
}