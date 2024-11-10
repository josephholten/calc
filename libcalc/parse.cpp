#include <libcalc/parse.hpp>
#include <charconv>
#include <optional>

auto fmt::formatter<OperatorType>::format(OperatorType op, format_context& ctx) const -> format_context::iterator {
    string_view name = "unknown";
    switch (op) {
        case OperatorType::Plus:
            name = "Plus";
            break;
        case OperatorType::Minus:
            name = "Minus";
            break;
        case OperatorType::Mult:
            name = "Mult";
            break;
        case OperatorType::Div:
            name = "Div";
            break;
        case OperatorType::Exp:
            name = "Exp";
            break;
        case OperatorType::None:
            name = "None";
            break;
    }
    return formatter<string_view>::format(name, ctx);
}

double OperatorNode::evaluate() {
	switch (op) {
		case OperatorType::Plus:
			return left->evaluate() + right->evaluate();
		case OperatorType::Minus:
			return left->evaluate() - right->evaluate();
		case OperatorType::Mult:
			return left->evaluate() * right->evaluate();
		case OperatorType::Div:
			return left->evaluate() / right->evaluate();
		case OperatorType::Exp:
			return std::pow(left->evaluate(), right->evaluate());
		default:
			throw std::logic_error(fmt::format("unsupported operator type {}", op));
	}
}

Parser::Parser(std::vector<Token> _tokens)
	: tokens{std::move(_tokens)}, next{tokens.begin()}
{ }

std::optional<AstPtr> Parser::parse() {
	return parse_product();
}

std::optional<AstPtr> Parser::parse_number() {
	if (next->type == TokenType::Number) {
		double x;
		std::string_view text = next->text;
		auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), x);
		if (ec != std::errc()) { // error
			throw std::invalid_argument(fmt::format("cannot parse '{}' as double", text));
		}
		auto node = std::make_unique<NumberNode>(x);
		next++;
		return node;
	} else {
		return {};
	}
}

std::optional<AstPtr> Parser::parse_product() {
	std::optional<AstPtr> lhs = parse_number();
	if (!lhs) {
		return {};
	}

	while (next->type == TokenType::Mult || next->type == TokenType::Div) {
		OperatorType op = OperatorType::None;
		switch (next->type) {
			case TokenType::Mult:
				op = OperatorType::Mult;
				break;
			case TokenType::Div:
				op = OperatorType::Div;
				break;
			default:
				throw std::logic_error(fmt::format("Error: unexpected token {}", *next));
		}

		// nextsym
		next++;

		// expect
		std::optional<AstPtr> rhs = parse_number();
		if (!rhs) {
			throw std::invalid_argument(fmt::format("expected number token, got {}", *next));
		}

		lhs = std::make_unique<OperatorNode>(std::move(*lhs), op, std::move(*rhs));
	}
	return lhs;
}

/*
std::unique_ptr<Ast> parse(std::vector<Token>::const_iterator start, std::vector<Token>::const_iterator end) {
	if (start == end) {
		throw std::invalid_argument("no input");
	}

	// first should be number
	if (start->type != TokenType::Number) {
		throw std::invalid_argument(fmt::format("start: unexpected token '{}'", start->text));
	}

	double x;
	auto [ptr, ec] = std::from_chars(start->text.data(), start->text.data() + start->text.size(), x);
	if (ec != std::errc()) { // error
		throw std::invalid_argument(fmt::format("cannot parse '{}' as double", start->text));	
	}
	// fmt::println("{}", x);

	std::unique_ptr<Ast> number = std::make_unique<NumberNode>(x);

    start++;
	// next either done
	if (start == end) {
		return number;
	}

	// or next should be operator
	if (start->type == TokenType::Number) {
		throw std::invalid_argument(fmt::format("next should be op: unexpected token '{}'", start->text));
	}
	if (start->text.size() != 1) {
		throw std::logic_error(fmt::format("encountered invalid operator '{}'", start->text));
	}

	OperatorType op = OperatorType::None;
	switch (start->type) {
		case TokenType::Plus:
			op = OperatorType::Plus;
			break;
		case TokenType::Minus:
			op = OperatorType::Minus;
			break;
		case TokenType::Mult:
			op = OperatorType::Mult;
			break;
		case TokenType::Div:
			op = OperatorType::Div;
			break;
		case TokenType::Exp:
			op = OperatorType::Exp;
			break;
		default:
			throw std::logic_error(fmt::format("encountered invalid operator '{}'", start->text));
	}

	start++;
	return std::make_unique<OperatorNode>(
		std::move(number), 
		op,
		parse(start, end)
	);
}
*/

