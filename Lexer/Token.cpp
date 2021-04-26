#include "Token.hpp"
#include <cstring>

using namespace lex;

/*
    Token methods and functions below
*/
const std::map<Token::t_type,std::string> Token::m_names = {
    {NONE, "unknown"},
    {STAR, "star"},
    {ID, "identifier"},
    {NUMBER, "number"},
    {SYMBOL, "symbol"},
    {KEYWORD, "keyword"}
};

Token::Token(t_type type, size_t row, size_t col) : m_type(type), m_row(row), m_col(col) {}

Token::~Token() {}

size_t Token::row() const {
    return m_row;
}

size_t Token::col() const {
    return m_col;
}

Token::t_type Token::type() const {
    return m_type;
}

const std::string &Token::text_repr() const {
    return m_names.at(m_type);
}

/*
    Punctuation methods and functions below
*/
const std::map<Symbol::t_type,std::string > Symbol::m_text_repr = {
    {STAR, "*"},
    {OPENBR_1, "("},
    {CLOSEBR_1, ")"},
    {OPENBR_2, "["},
    {CLOSEBR_2, "]"},
    {COMMA, ","},
    {SEMICOLON, ";"}
};

Symbol::Symbol(t_type type, size_t row, size_t col) : Token(Token::SYMBOL, row, col), m_type(type) {}

const std::string &Symbol::text_repr() const {
    return m_text_repr.at(m_type);
}

bool Symbol::is_symbol(int c) {
    return (std::strchr("*()[],;", c) != NULL);
} 
/*
    Keyword methods and functions below
*/
const std::map<Keyword::t_type,std::string > Keyword::m_text_repr = {
    {INT, "int"},
    {LONG, "long"},
    {CONST, "const"}
};

Keyword::Keyword(t_type type, size_t row, size_t col) : Token(Token::KEYWORD, row, col), m_type(type) {}

const std::string &Keyword::text_repr() const {
    return m_text_repr.at(m_type);
}

const std::string &Keyword::text_repr(t_type type) {
    return m_text_repr.at(type);
}

/*
    Identifier methods and functions below
*/
Identifier::Identifier(size_t row, size_t col, const std::string &id_name) : Token(Token::ID, row, col), m_id_name(id_name) {}

const std::string &Identifier::text_repr() const {
    return id_name();
}

const std::string &Identifier::id_name() const {
    return m_id_name;
}

/*
    Number methods and functions below
*/
Number::Number(size_t row, size_t col, unsigned long val) : Token(Token::NUMBER, row, col), m_val(val), m_text_repr(std::to_string(val)) {}

const std::string &Number::text_repr() const {
    return m_text_repr;
}

unsigned long Number::val() const {
    return m_val;
}