#ifndef LIBCALC_PARSE_HPP
#define LIBCALC_PARSE_HPP

#include <fmt/ranges.h>
#include <vector>
#include <libcalc/lex.hpp>
#include <memory>

enum class OperatorType {
    Plus,
    Minus,
    Mult,
    Div,
    Exp,
	None,
};

template <>
struct fmt::formatter<OperatorType> : formatter<string_view> {
	format_context::iterator format(OperatorType op, format_context& ctx) const;
};

class Ast {
public:
	virtual double evaluate() = 0;
	virtual void print(size_t indent = 0) = 0;
};

using AstPtr = std::unique_ptr<Ast>;

class OperatorNode : public Ast {
public:
	OperatorNode(std::unique_ptr<Ast> _left, OperatorType _op, std::unique_ptr<Ast> _right)
		: left{std::move(_left)}, op{_op}, right{std::move(_right)}
	{ }

	double evaluate() override;

	void print(size_t indent = 0) override {
		for (size_t i = 0; i < indent; i++)
			fmt::print(" ");
		fmt::println("(OperatorNode '{}')", op);
		left->print(indent + 2);
		right->print(indent + 2);
	}

private:
    std::unique_ptr<Ast> left;
	OperatorType op;
    std::unique_ptr<Ast> right;
};

class NumberNode : public Ast {
public:
	NumberNode(double _x)
		: x{_x}
	{ }

	double evaluate() override {
		return x;
	}

	void print(size_t indent = 0) override {
		for (size_t i = 0; i < indent; i++)
			fmt::print(" ");
		fmt::println("(NumberNode '{}')", x);
	}

private:
	double x;
};

class Parser {
public:
    Parser(std::vector<Token> _tokens);

	std::optional<AstPtr> parse();

private:
	std::optional<AstPtr> parse_number();
	std::optional<AstPtr> parse_exp();
	std::optional<AstPtr> parse_paren();
	std::optional<AstPtr> parse_base();
	std::optional<AstPtr> parse_product();
	std::optional<AstPtr> parse_expr();

	std::vector<Token> tokens;
    std::vector<Token>::const_iterator next;
};

#endif /* LIBCALC_PARSE_HPP */
