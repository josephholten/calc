#ifndef LIBCALC_LEX_HPP
#define LIBCALC_LEX_HPP

#include <fmt/ranges.h>
#include <vector>

enum class TokenType {
    Number,
    Plus,
    Minus,
    Mult,
    Div,
    Exp,
    ParenL,
    ParenR,
    None,
};

template <>
struct fmt::formatter<TokenType> : formatter<string_view> {
	format_context::iterator format(TokenType t, format_context& ctx) const;
};

struct Token {
    std::string_view text;
    TokenType type;
};

Token lex_number(std::string_view input, size_t& start);
std::vector<Token> lex(std::string_view input);

#endif /* LIBCALC_LEX_HPP */
