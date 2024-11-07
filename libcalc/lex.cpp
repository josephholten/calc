#include <fmt/format.h>
#include <libcalc/lex.hpp>

auto fmt::formatter<TokenType>::format(TokenType t, format_context& ctx) const -> format_context::iterator {
    string_view name = "unknown";
    switch (t) {
        case TokenType::Number:
            name = "Number";
            break;
        case TokenType::Operator:
            name = "Operator";
            break;
    }
    return formatter<string_view>::format(name, ctx);
}



Token lex_number(std::string_view input, size_t& start) {
    size_t end = start;
    while ((input[end] >= '0' && input[end] <= '9') || input[end] == '.') {
        end++;
    }
	Token token = {.text = input.substr(start, end-start), .type = TokenType::Number};
	start = end;
    return token;
}

std::vector<Token> lex(std::string_view input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];
		// fmt::println("lex: {} -> {}", i, c);
        switch (c) {
			case ' ':
			case '\r':
			case '\n':
				i++;
				break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                tokens.push_back(lex_number(input, i)); // substr starting from i
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
                tokens.push_back({.text = input.substr(i, 1), .type = TokenType::Operator});
				i++;
                break;
			default: 
				throw std::invalid_argument(fmt::format("invalid character {}", c));
        }
    }
    return tokens;
}


