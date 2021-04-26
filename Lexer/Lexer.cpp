#include "Lexer.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace lex;

Lexer::Lexer() : m_state(S), m_row(0), m_col(0), m_first_col(0) {}

Lexer::~Lexer() {
    clear();
}

void Lexer::print_tokens() {
    for (std::vector<Token *>::const_iterator it = m_tokens.begin(); it != m_tokens.end(); ++it)
        std::cout << std::left << std::setw(20) << (*it)->Token::text_repr() << " [" << (*it)->text_repr() << "] "
                  << "at (" << (*it)->row() << "; " << (*it)->col() << ")\n";
}

void Lexer::tokenize(std::ifstream &in) {
    m_str_lex.clear();
    m_state = S;
    while (in.get(m_c))
        state_chooser();
    m_c = '\n';
    state_chooser();
}

void Lexer::state_chooser() {
    switch (m_state) {
    case S:
        state_s();
        break;
    case ID:
        state_id();
        break;
    case NUMBER:
        state_number();
        break;
    case INT:
        state_keyword<Keyword::INT>();
        break;
    case LONG:
        state_keyword<Keyword::LONG>();
        break;
    case CONST:
        state_keyword<Keyword::CONST>();
        break;
    case SYMBOL:
        state_symbol();
        break;
    default:
        break;
    }
}

void Lexer::state_s() {
    if (m_c == '\n') {
        m_row++;
        m_col = 0;
        return;
    } else if (std::isspace(m_c)) {
        m_col++;
        return;
    } else if (m_c == Keyword::text_repr(Keyword::INT)[0]) {
        m_state = INT;
        m_state_idx = 1;
    } else if (m_c == Keyword::text_repr(Keyword::LONG)[0]) {
        m_state = LONG;
        m_state_idx = 1;
    } else if (m_c == Keyword::text_repr(Keyword::CONST)[0]) {
        m_state = CONST;
        m_state_idx = 1;
    } else if (m_c == '_' || isalpha(m_c)) {
        m_state = ID;
    } else if (isdigit(m_c)) {
        m_state = NUMBER;
    } else if (Symbol::is_symbol(m_c)) {
        m_state = SYMBOL;
    } else {
        throw std::runtime_error(std::string("Unexpected character: ") + m_c);
    }
    m_str_lex += m_c;
    m_first_col = m_col;
    m_col++;
}

void Lexer::state_symbol() {
    Symbol::t_type type;

    switch (m_str_lex.at(0)) {
    case '(':
        type = Symbol::OPENBR_1;
        break;
    case ')':
        type = Symbol::CLOSEBR_1;
        break;
    case '[':
        type = Symbol::OPENBR_2;
        break;
    case ']':
        type = Symbol::CLOSEBR_2;
        break;
    case '*':
        type = Symbol::STAR;
        break;
    case ',':
        type = Symbol::COMMA;
        break;
    case ';':
        type = Symbol::SEMICOLON;
        break;
    default:
        // unreachable
        break;
    }
    m_tokens.push_back(new Symbol(type, m_row, m_first_col));
    m_str_lex.clear();
    m_state = S;
    state_s();
}

void Lexer::state_id() {
    if (isalnum(m_c) || m_c == '_') {
        m_str_lex += m_c;
        m_col++;
    } else {
        m_tokens.push_back(new Identifier(m_row, m_first_col, m_str_lex));
        m_str_lex.clear();
        m_state = S;
        state_s();
    }
}

template <Keyword::t_type type>
void Lexer::state_keyword() {
    static const std::string &text_repr = Keyword::text_repr(type);
    static const unsigned text_repr_len = text_repr.length();

    if (m_state_idx == text_repr_len) {
        if (!(m_c == '_' || std::isalnum(m_c))) {
            m_tokens.push_back(new Keyword(type, m_row, m_first_col));
            m_str_lex.clear();
            m_state = S;
            state_s();
            return;
        }
    } else if (m_c == text_repr[m_state_idx]) {
        m_str_lex += m_c;
        m_state_idx++;
        m_col++;
        return;
    }
    m_state = ID;
    state_id();
}

void Lexer::state_number() {
    if (std::isdigit(m_c)) {
        m_str_lex += m_c;
        m_col++;
    } else {
        m_tokens.push_back(new Number(m_row, m_first_col, std::strtoul(m_str_lex.c_str(), NULL, 0)));
        m_str_lex.clear();
        m_state = S;
        state_s();
    }
}

void Lexer::clear() {
    m_state = S;
    m_row = m_col = m_first_col = 0;
    m_str_lex.clear();
    for (std::vector<Token *>::const_iterator it = m_tokens.begin(); it != m_tokens.end(); ++it) {
        delete *it;
    }
}