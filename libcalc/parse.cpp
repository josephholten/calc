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

std::optional<AstPtr> Parser::parse_exp() {
	std::optional<AstPtr> base = parse_base();
	if (!base) {
		return {};
	}

	while (next->type == TokenType::Exp) {
		// nextsym
		next++;

		// expect
		std::optional<AstPtr> exp = parse_exp(); // either number or exp expression
		if (!exp) {
			throw std::invalid_argument("expected exp-expression");
		}
		base = std::make_unique<OperatorNode>(std::move(*base), OperatorType::Exp, std::move(*exp));
	}

	return base;
}

std::optional<AstPtr> Parser::parse_paren() {
	if (next->type != TokenType::ParenL) {
		return {};
	}

	// nextsym
	next++;
	std::optional<AstPtr> paren_expr = parse_expr();

	if (next->type != TokenType::ParenR) {
		throw std::invalid_argument("expected right parenthesis");
	}

	// nextsym
	next++;

	return paren_expr;
}

std::optional<AstPtr> Parser::parse_base() {
	std::optional<AstPtr> number_expr = parse_number();
	if (number_expr) {
		return number_expr;
	}

	std::optional<AstPtr> paren_expr = parse_paren();
	if (paren_expr) {
		return paren_expr;
	}

	return {};
}

static OperatorType token_to_operator(TokenType t) {
	OperatorType op = OperatorType::None;
	switch (t) {
		case TokenType::Mult:
			op = OperatorType::Mult;
			break;
		case TokenType::Div:
			op = OperatorType::Div;
			break;
		case TokenType::Plus:
			op = OperatorType::Plus;
			break;
		case TokenType::Minus:
			op = OperatorType::Minus;
			break;
		default:
			throw std::logic_error(fmt::format("Error: unexpected token type {}", t));
	}
	return op;
}

std::optional<AstPtr> Parser::parse_product() {
	std::optional<AstPtr> lhs = parse_exp();
	if (!lhs) {
		return {};
	}

	while (next->type == TokenType::Mult || next->type == TokenType::Div) {
		OperatorType op = token_to_operator(next->type);
		// nextsym
		next++;

		// expect
		std::optional<AstPtr> rhs = parse_exp();
		if (!rhs) {
			throw std::invalid_argument(fmt::format("expected factor-expression"));
		}

		lhs = std::make_unique<OperatorNode>(std::move(*lhs), op, std::move(*rhs));
	}
	return lhs;
}

std::optional<AstPtr> Parser::parse_expr() {
	std::optional<AstPtr> lhs = parse_product();
	if (!lhs) {
		return {};
	}

	while (next->type == TokenType::Plus || next->type == TokenType::Minus) {
		OperatorType op = token_to_operator(next->type);
		// nextsym
		next++;

		// expect
		std::optional<AstPtr> rhs = parse_product();
		if (!rhs) {
			throw std::invalid_argument(fmt::format("expected product-expression"));
		}

		lhs = std::make_unique<OperatorNode>(std::move(*lhs), op, std::move(*rhs));
	}
	return lhs;
}

std::optional<AstPtr> Parser::parse() {
	auto expr = parse_expr();
	if (next != tokens.end()) {
		throw std::invalid_argument(fmt::format("unused token {}", *next));
	}
	return expr;
}
