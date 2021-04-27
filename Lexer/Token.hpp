#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <map>
#include <cstddef>
#include <string>

namespace lex {
    class Token {
    public:
        typedef enum
        {
            NONE,
            ID,
            NUMBER,
            SYMBOL,
            KEYWORD
        } t_type;

    public:
        Token(t_type type, size_t row, size_t col);
        virtual ~Token();
        size_t row() const;
        size_t col() const;
        t_type type() const;
        virtual const std::string &text_repr() const;

    private:
        static const std::map<Token::t_type,std::string> m_names;

        const t_type m_type;
        const size_t m_row;
        const size_t m_col;
    };

    class Symbol : public Token {
    public:
        typedef enum {
            NONE,
            STAR = '*',
            OPENBR_1 = '(',
            CLOSEBR_1 = ')',
            OPENBR_2 = '[',
            CLOSEBR_2 = ']',
            COMMA = ',',
            SEMICOLON = ';'
        } t_type;

    public:
        Symbol(t_type type, size_t row, size_t col);
        const std::string &text_repr() const;
        static bool is_symbol(int c);
        t_type type() const;

    private:
        static const std::map<Symbol::t_type,std::string > m_text_repr;
        const t_type m_type;
    };

    class Keyword : public Token {
    public:
        typedef enum {
            INT,
            LONG,
            CONST
        } t_type;
    
    public:
        Keyword(t_type type, size_t row, size_t col);
        const std::string &text_repr() const;
        static const std::string &text_repr(t_type type);
        t_type type() const;

    private:
        static const std::map<Keyword::t_type,std::string > m_text_repr;
        const t_type m_type;
    };

    class Identifier : public Token {
    public:
        Identifier(size_t row, size_t col, const std::string &name);
        const std::string &text_repr() const;
        const std::string &id_name() const;

    private:
        const std::string m_id_name;
    };

    class Number: public Token {
    public:
        Number(size_t row, size_t col, unsigned long val);
        const std::string &text_repr() const;
        unsigned long val() const;

    private:
        const unsigned long m_val;
        const std::string m_text_repr;
    };
}

#endif