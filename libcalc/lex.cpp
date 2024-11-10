#include <fmt/format.h>
#include <libcalc/lex.hpp>

auto fmt::formatter<TokenType>::format(TokenType t, format_context& ctx) const -> format_context::iterator {
    string_view name = "unknown";
    switch (t) {
        case TokenType::Number:
            name = "Number";
            break;
        case TokenType::Plus:
            name = "Plus";
            break;
        case TokenType::Minus:
            name = "Minus";
            break;
        case TokenType::Mult:
            name = "Mult";
            break;
        case TokenType::Div:
            name = "Div";
            break;
        case TokenType::Exp:
            name = "Exp";
            break;
        case TokenType::ParenL:
            name = "ParenL";
            break;
        case TokenType::ParenR:
            name = "ParenR";
            break;
        case TokenType::None:
            name = "None";
            break;
        default:
            throw std::logic_error(fmt::format("Error: invalid TokenType {}, shouldn't reach this", t));
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

Token lex_operator(std::string_view input, size_t& start) {
    TokenType type = TokenType::None;
    switch (input[start]) {
        case '+':
            type = TokenType::Plus;
            break;
        case '-':
            type = TokenType::Minus;
            break;
        case '*':
            type = TokenType::Mult;
            break;
        case '/':
            type = TokenType::Div;
            break;
        case '^':
            type = TokenType::Exp;
            break;
    }
    Token op = {.text = input.substr(start, 1), .type = type};
    start++;
    return op;
}

std::vector<Token> lex(std::string_view input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];
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
                tokens.push_back(lex_operator(input, i));
                break;
			default: 
				throw std::invalid_argument(fmt::format("invalid character {}", c));
        }
    }
    return tokens;
}


